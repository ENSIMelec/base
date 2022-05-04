#include "Controller.h"

#include "../strategy/PointType.h"

using namespace std;

Controller::Controller(MotorManager *moteurs, Odometry *codeurs, Config *conf) {
    //Récupération des différents coef de PID configuré dans le fichier config
    CoeffGLong =  conf->getCoeffGLong();
    CoeffDLong =  conf->getCoeffDLong();

    CoeffAnglD = conf->getCoeffAnglD();
    CoeffAnglG = conf->getCoeffAnglG();

    kpA = conf->getPIDkpA();
    kiA = conf->getPIDkiA();
    kdA = conf->getPIDkdA();

    kpDep = conf->getPIDkpDep();
    kiDep = conf->getPIDkiDep();
    kdDep = conf->getPIDkdDep();

    kpPos = conf->getPIDkpPos();
    kiPos = conf->getPIDkiPos();
    kdPos = conf->getPIDkdPos();

    config = conf;

    derapageG = false;
    derapageD = false;

    pathfindingInAction = false;
}

void Controller::initialiser(Point * pt) {

    codeurs->setPosition(nextPoint->getX(), nextPoint->getY(), nextPoint->getTheta());

    pointActuel = pt;

    x = pointActuel->getX();
    y = pointActuel->getY();

    angle = pointActuel->getTheta();
    consigne_angle = angle;
    cmdG = 0;
    cmdD = 0;

    moteurBloque = false;

    consigneG = 0;
    consigneD = 0;
    rotationDone = false;
    asservFini = true;
    asservFini = true;
    derapageG = false;
    derapageD = false;

    //Reset des integrals des PIDs
    somme_erreurAngle = 0;
    somme_erreurG = 0;
    somme_erreurD = 0;
    somme_erreurPosition = 0;
}

Point * Controller::getPointActuel() const{
    return pointActuel;
}

void Controller::pointSuivant(Point * point){
    //Changement du point cible

    //Init de la pente d'acceleration en fonction du point précédent
    if(pointActuel->getLissage() == false){//Pente activé
        coefAccel=0.01;

        //Reset des integrals des PIDs
        somme_erreurAngle=0;
        somme_erreurG=0;
        somme_erreurD=0;
        somme_erreurPosition=0;
        somme_erreurCorrection = 0;

        old_erreurPosition = 0;
        old_erreurG = 0;
        old_erreurD = 0;
        old_erreurAngle = 0;
        old_erreurCorrection = 0;

    } else {
        //Pente désactivé pour les transitions smooth
        coefAccel=1;
    }

    pointActuel = point;

    asservFini = false;
    rotationDone = false;

    derapageG = false;
    derapageD = false;

    if(pointActuel->getType() == PointType::DEPLACEMENT_X){
        yCible = y;
    }else{
        if(pointActuel->getType() == PointType::DEPLACEMENT_Y){
            xCible = x;
        }
    }

}

void Controller::calculConsigne(){

    switch(pointActuel->getType()){

        case PointType::POSITION: //Position
        case PointType::ANGLE : //Rotation
            xCible = pointActuel->getX();
            yCible = pointActuel->getY();
            consigne_angle = pointActuel->getTheta();
            break;
        case PointType::ANGLE_RELATIF: //Angle relatif
            xCible = pointActuel->getX();
            yCible = pointActuel->getY();
            consigne_angle = angle+pointActuel->getTheta();
            break;
        case PointType::DEPLACEMENT: //Translation
            xCible = pointActuel->getX();
            yCible = pointActuel->getY();
            break;
        case PointType::DEPLACEMENT_X: //Translation en X
            xCible = pointActuel->getX();
            break;
        case PointType::DEPLACEMENT_Y: //Translation en Y
            yCible = pointActuel->getY();
            break;
        case PointType::DEPLA_RELATIF: //translation relative
            xCible = x+pointActuel->getX();
            yCible = y+pointActuel->getY();
            break;
        default: break; //Recalage
    }

    //Calcul la distance entre le robot et le point cible
    consigne_distance = sqrt((xCible-x)*(xCible-x)+(yCible-y)*(yCible-y));

    if(pointActuel->getType() != PointType::ANGLE && pointActuel->getType() != PointType::ANGLE_RELATIF){// Point de Translation
        //Calcul de l'angle entre la position du robot et la cible
        if(pointActuel->getDirection() == "avant"){//Marche avant
            consigne_angle =atan2((xCible-x),(yCible-y))*180/M_PI;
        }else{//Marche arrière
            consigne_angle =atan2((xCible-x),(yCible-y))*180/M_PI-180;
        }

        //Si la consigne d'angle est supérieur à 180° on tourne dans l'autre sens
        if (consigne_angle > 180){
            consigne_angle = consigne_angle-360;
        }
        if(consigne_angle <= -180){
            consigne_angle = consigne_angle +360;
        }
    }
    //Si l'erreur d'angle est supérieur à 180° on tourne dans l'autre sens
    erreurAngle = consigne_angle-angle;
    if (erreurAngle > 180){
        erreurAngle = erreurAngle-360;
    }
    if(erreurAngle <= -180){
        erreurAngle = erreurAngle +360;
    }
    cout << "xCible : "<< xCible << " yCible : "<< yCible << endl;
}

void Controller::asservir(){
    cout << endl;
    //Mise à jour de la position et de la vitesse du robot
    cout << "Vitesse consigne :" << pointActuel->getSpeed()<<endl;
    odometry->update();
    x = odometry->getX();
    y = odometry->getY();
    angle = odometry->getTheta();

    //Calcul de la distance et de l'angle à fair pour aller au prochain point
    calculConsigne();

    switch(pointActuel->getType()){

        case PointType::ANGLE_RELATIF:
        case PointType::ANGLE: Angle(); break;

        case PointType::DEPLACEMENT_X:
        case PointType::DEPLACEMENT_Y:
        case PointType::DEPLA_RELATIF:
        case PointType::DEPLACEMENT: Deplacement(); break;

        case PointType::POSITION: Position(); break;

        case PointType::RECALAGE_X: RecalageX(); break;
        case PointType::RECALAGE_Y: RecalageY(); break;
        case PointType::RECALAGE_XY: RecalageXY(); break;
    }
}

void Controller::Deplacement(){
    cout << "### Point translation erreurAngle: "<<erreurAngle<<" distance : "<< consigne_distance<<endl;
    if(pointActuel->getCoefCourbe()!=0){//Courbe activé : désactive la rotation avant le déplacement
        rotationDone=true;
        if(pointActuel->getCoefCourbe()!=1){ //Coefficient p donné par le point
            kpDep=pointActuel->getCoefCourbe();
        }else{//Coefficient p par défaut
            kpDep = config->getPIDkpDep();
        }
    }else {
        kpDep = config->getPIDkpDep();
    }

    if(pathfindingInAction){
        kpDep = config->getPIDkpDepPathfinding();
    }else{
        kpDep = config->getPIDkpDep();
    }

    if(rotationDone == false){//Si la rotation n'est pas terminée
        //Rotation avant d'avancer
        if(abs(erreurAngle) > pointActuel->getDeltaAngle()){//Erreur d'angle toléré
            cout<<"### Rotation de "<<consigne_angle << " ###"<<endl;

            //Calcul des commandes moteurs pour la rotation (cmdG, cmdD);
            asservAngle(pointActuel->getSpeed());/////////////////////////////////////////////////////avant c'etait asservAngle(500)

            //Calcul de la pente d'acceleration
            if( coefAccel <1){
                coefAccel += 0.06;
                if(coefAccel>1){
                    coefAccel=1;
                }
            }else{
                coefAccel = 1;
            }
            //Envoie de la consigne a l'asservissement de vitesse
            asservVitesse(consigneG*coefAccel,consigneD*coefAccel);

        }else{//Angle atteint
            rotationDone = true;
            coefAccel=0.01;
        }
    }else{ //translation
        cout<<"### Translation de "<< consigne_distance <<" mm ###"<<endl;

        if(pointActuel->getDerapage()== false){
            //modification de consigneG et consigneD en fonction de l'erreur d'angle lors de la translation
            correctionAngle();
        }else{//On désactive la correction d'angle lors de la détection de dérapage
            consigneG=1;
            consigneD=1;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Chelou ? par rapport au if/else juste au dessus
        correctionAngle();
        cout <<"Correction d'angle : "<< consigneG <<", "<<consigneD<<endl;


        if(pointActuel->getLissage() == 0){//Ralenti à l'approche du point
            //Calcul de la consigneVitesse
            asservPosition();

        }else{//Transition smooth entre les points
            consigneVitesse = pointActuel->getSpeed();
        }

        //Calcul de la pente d'acceleration
        if( coefAccel <1){
            coefAccel += 0.05;
            if(coefAccel>1){
                coefAccel=1;
            }
        }else{
            coefAccel = 1;
        }

        cout << "CoefAccel: "<<coefAccel<<" consigneVitesse: "<<consigneVitesse <<endl;
        if(pointActuel->getDirection() == "avant" || rotationDone==false){
            //vitesse demandé+ consigneG(erreur angle)
            //Convertie la vitesse souhaité en commande moteur : cmdG et cmdD;
            asservVitesse(coefAccel*(consigneVitesse+consigneG), coefAccel*(consigneVitesse+consigneD));

        }else{//Marche arrière
            //Convertie la vitesse souhaité en commande moteur : cmdG et cmdD;
            asservVitesse(-coefAccel*(consigneVitesse+consigneG), -coefAccel*(consigneVitesse+consigneD));

        }


        if(consigne_distance < pointActuel->getDeltaDeplacement()){//Si le robot est arrivé au point cible
            if(pointActuel->getLissage()==0){//Freine à l'arrivé
                cout <<"STOP" << endl;
                cmdG = 0;
                cmdD = 0;
                coefAccel=0.01;
            }
            asservFini = true;
            somme_erreurCorrection=0;
        }
    }
    //Envoie des commandes aux moteurs
    cout <<"CMD :" << cmdG << " "<<cmdD << endl;
    moteurs->setOrder(cmdG, cmdD);
}

void Controller::Angle(){
    cout << "### Point rotation ###" <<endl;
    cout<<"### Rotation de "<<erreurAngle << " ###"<<endl;
    asservAngle(pointActuel->getSpeed());
    //Calcul de la pente d'acceleration
    if( coefAccel <1){
        coefAccel += 0.06;
        if(coefAccel>1){
            coefAccel=1;
        }
    }else{
        coefAccel = 1;
    }
    cout<<"CoeffAccel : "<<coefAccel<<endl;
    //Envoie de la consigne a l'asservissement de vitesse
    //cmdG = consigneG;
    //cmdD = consigneD;
    asservVitesse(consigneG*coefAccel,consigneD*coefAccel);

    if(abs(erreurAngle)>pointActuel->getDeltaAngle()){//Erreur d'angle toléré
        cout <<"CMD :" << cmdG << " "<<cmdD << endl;
        moteurs->setOrder(cmdG, cmdD);
    }else{//Angle atteint
        cout <<"STOP"<<endl;
        moteurs->stop();
        asservFini = true;
        coefAccel=0.01;
    }
}

void Controller::Position(){

    xCible = pointActuel->getX();
    yCible = pointActuel->getY();

    if(consigne_distance > pointActuel->getDeltaDeplacement()){ //Deplacement
        if(abs(consigne_angle)>90){
            if(pointActuel->getDirection() == "avant"){
                pointActuel->setDirection("arriere");
            }else{
                pointActuel->setDirection( "avant");
            }
        }
        if(pointActuel->getDirection()== "avant") {//Marche avant
            consigne_angle =atan2((xCible-x),(yCible-y))*180/M_PI;
        }else{//Marche arrière
            consigne_angle =atan2((xCible-x),(yCible-y))*180/M_PI-180;
        }
        Deplacement();
        asservFini = false;
    }else{
        consigne_angle = pointActuel->getTheta();
        erreurAngle = consigne_angle-angle;
        if (erreurAngle > 180){
            erreurAngle = erreurAngle-360;
        }
        if(erreurAngle <= -180){
            erreurAngle = erreurAngle +360;
        }
        Angle();
    }
}

//Asservissement en position
//Permet de ralentir le robot à l'approche du point cible
//En fonction de la distance à parcourir
void Controller::asservPosition(){
    //cout <<"ErreurAngle: "<<erreurAngle<<endl;
    double erreurPosition=consigne_distance;
    somme_erreurPosition += erreurPosition;
    double dErreurPosition = erreurPosition-old_erreurPosition;
    old_erreurPosition = erreurPosition;


    cout << "Position p: "<< kpPos << " i: "<< kiPos<<" d: "<< kdPos << endl;
    double PID = kpPos*erreurPosition + kiPos*somme_erreurPosition + kdPos*dErreurPosition;
    //Modification des variable consgineG et consigneD qui seront pris en compte dans la fonction Asservir
    cout << "PID pos : "<< PID <<endl;
    consigneVitesse = PID;


    //Limite la vitesse maximal à la vitesse souhaité
    if(consigneVitesse > pointActuel->getSpeed()){
        consigneVitesse = pointActuel->getSpeed();
    }
    if(consigneVitesse < -pointActuel->getSpeed()){
        consigneVitesse = -pointActuel->getSpeed();
    }

    cout <<"consigneVitesse : "<<consigneVitesse<<endl;
    cout << "Consigne vitesse angle :"<<consigneG<<endl;
    //asservVitesse(consigneG/2, consigneD/2);
}

//Asservissement en vitesse
//Transforme la vitesse de consigne en commande moteur
//Maintient les moteurs à la bonne vitesse
void Controller::asservVitesse(double vitConsigneG, double vitConsigneD){
    cout<<"VitG: "<<vitConsigneG<<" -> "<<vitG<<" VitD: "<<vitConsigneD<<" -> "<<vitD<<endl;
    double kpG, kiG, kdG;
    double kpD, kiD, kdD;

    //Modification du PID pour être dans la bonne gamme de vitesse
    if(vitConsigneG > -50 && vitConsigneG < 50){
        kpG = config->getPIDkpVLow();
        kiG = config->getPIDkiVLow();
        kdG = config->getPIDkdVLow();
    }else{
        if(vitConsigneG > -600 && vitConsigneG < 600){
            kpG = config->getPIDkpVMedium();
            kiG = config->getPIDkiVMedium();
            kdG = config->getPIDkdVMedium();
        }else{
            kpG = config->getPIDkpVHigh();
            kiG = config->getPIDkiVHigh();
            kdG = config->getPIDkdVHigh();
        }
    }

    if(vitConsigneD > -50 && vitConsigneD < 50){
        kpD = config->getPIDkpVLow();
        kiD = config->getPIDkiVLow();
        kdD = config->getPIDkdVLow();
    }else{
        if(vitConsigneD > -600 && vitConsigneD < 600){
            kpD = config->getPIDkpVMedium();
            kiD = config->getPIDkiVMedium();
            kdD = config->getPIDkdVMedium();
        }else{
            kpD = config->getPIDkpVHigh();
            kiD = config->getPIDkiVHigh();
            kdD = config->getPIDkdVHigh();
        }
    }

    //Différence entre la vitesse Souhaité et la vitesse réel
    double erreurG = vitConsigneG-vitG;
    double erreurD = vitConsigneD-vitD;

    somme_erreurG  += erreurG;
    somme_erreurD  += erreurD;

    dErreurG = erreurG-old_erreurG;
    dErreurD = erreurD-old_erreurD;

    old_erreurG = erreurG;
    old_erreurD = erreurD;
    //cout << "Vitesse p: "<< kpV << " i: "<< kiV<<" d: "<< kdV << endl;
    //cout << "ErreurG: "<<erreurG<<" sum " << somme_erreurG << " dErr: " << dErreurG << endl;
    //cout << "ErreurD: "<<erreurD<<" sum " << somme_erreurD << " dErr: " << dErreurD << endl;

    double PID_G = kpG*erreurG + kiG*somme_erreurG + kdG*dErreurG;
    double PID_D = kpD*erreurD + kiD*somme_erreurD + kdD*dErreurD;

    //cout <<"PID_G :"<< PID_G <<" PID_D :"<< PID_D << endl;
    cmdG = PID_G;
    cmdD = PID_D;
/*
	if(pointActuel.getDerapage()== true){//détection de dérapage
		cout <<"Wait Burn, Gauche : "<< abs(cmdG) <<" vitG: "<<abs(vitG)<<" ,Droite: "<< abs(cmdD)<<" vitD: "<<abs(vitD)<<endl;
		if(abs(cmdG)>=50 && abs(vitG)<5){
			cmdG=0;
			cout<<"Dérapage Gauche"<<endl;
		}

		if(abs(cmdD)>=50 && abs(vitD)<5){
			cmdD=0;
			cout<<"Dérapage Droite"<<endl;
		}

		if(cmdG==0 && cmdD==0){//Dérapage des deux cotés, on est contre la bordure, prêt pour le recalage
			asservFini = true;
		}
	}else{
		cout<< "PID vit "<< PID_G <<" "<< PID_D<<endl;
		if((abs(cmdG) >= 150 && abs(vitG)<=5) || (abs(cmdD)>= 150 && abs(vitD)<=5)){
			//Dérapage imprévu
			cout<< "\t ####### Robot bloqué #######"<<endl;
			//moteurBloque = true;
			//moteurs.stop();
			//exit(3);
		}else{
			moteurBloque = false;
		}
	}*/

    if(pointActuel->getDerapage()== true){//détection de dérapage
        cout <<"Wait Burn, Gauche : "<< abs(cmdG) <<" vitG: "<<abs(vitG)<<" ,Droite: "<< abs(cmdD)<<" vitD: "<<abs(vitD)<<endl;
        if(abs(cmdG)>=50 && abs(vitG)<5){
            cmdG = 0;
            derapageG = true;
            cout << "Dérapage Gauche" << endl;
        }

        if(abs(cmdD)>=50 && abs(vitD)<5){
            cmdD = 0;
            derapageD = true;
            cout << "Dérapage Droite" << endl;
        }

        if(derapageD && derapageG){//Dérapage des deux cotés, on est contre la bordure, prêt pour le recalage
            asservFini = true;
        }
    }else{
        cout<< "PID vit "<< PID_G <<" "<< PID_D<<endl;
        if((abs(cmdG) >= 85 && abs(vitG)<=5) || (abs(cmdD)>= 85 && abs(vitD)<=5) ){
            //Dérapage imprévu
            cout<< "\t ####### Robot bloqué #######"<<endl;
            moteurBloque = true;
            moteurs->stop();
            pointSuivant(pointActuel);
            //exit(3);
        }else{
            moteurBloque = false;
        }
    }

    //Vérification des limites de commandes moteurs
    if(cmdG > 255){
        cmdG = 255;
    }
    if(cmdG < -255){
        cmdG = -255;
    }

    if(cmdD > 255){
        cmdD = 255;
    }
    if(cmdD < -255){
        cmdD = -255;
    }
}
void Controller::asservAngle(double vit){
    //cout <<"ErreurAngle: "<<erreurAngle<<endl;
    somme_erreurAngle += erreurAngle;
    double dErreurAngle = erreurAngle-old_erreurAngle;
    old_erreurAngle = erreurAngle;

    double PID = kpA*erreurAngle + kiA*somme_erreurAngle + kdA*dErreurAngle;

    //cout << "Angle p: "<< kpA << " i: "<< kiA <<" d: "<< kdA << endl;
    //cout <<"PID :"<< PID << endl;

    //Modification des variable consigneG et consigneD qui seront pris en compte dans la fonction Asservire
    consigneG = PID/2;
    consigneD = -PID/2;

    //Vérification des limites de commandes moteurs
    if(consigneG > vit){
        consigneG = vit;
    }
    if(consigneG < -vit){
        consigneG = -vit;
    }

    if(consigneD > vit){
        consigneD = vit;
    }
    if(consigneD < -vit){
        consigneD = -vit;
    }
    //cout << "Consigne vitesse angle :"<<consigneG<<endl;
}

//Correction de l'angle du robot lors du déplacement pour etre sur d'atteindre le point cible
//Compense la déviation de cap en augmentent la vitesse d'un moteur
void Controller::correctionAngle(){
    //cout <<"ErreurAngle: "<<erreurAngle<<endl;
    erreurCorrection = erreurAngle;
    somme_erreurCorrection += erreurCorrection;
    double dErreurCorrection = erreurCorrection-old_erreurCorrection;
    old_erreurCorrection = erreurCorrection;
    double PID = kpDep*erreurCorrection + kiDep*somme_erreurCorrection + kdDep*dErreurCorrection;
    //cout<<"PID correction:"<< kpDep <<", " << kiDep << ", "<< kdDep <<endl;
    //Modification des variable consgineG et consigneD qui seront pris en compte dans la fonction Asservire
    //cout <<"PID correction: "<<PID<<endl;
    if(pointActuel->getDirection()== "avant"){
        consigneG = PID;
        consigneD = -PID;
        /*if(PID<0){
            consigneG = 1+PID/100;
            consigneD =1;
        }else{
            consigneG = 1;
            consigneD =1-PID/100;
        }*/
    }else{
        consigneG = -PID;
        consigneD = PID;
    }

    //cout << "Consigne vitesse angle :"<<consigneG<<endl;
}

//Stop les moteurs
void Controller::stop() {
    moteurs->stop();
}

//Recalage du robot en X, Y et en angle
void Controller::RecalageXY() {

    if(abs(x-pointActuel->getX()>50) || abs(y-pointActuel->getY()>50) ){
        cout<< "Recalage foireux !"<<endl;
        //exit(3);
    }
    x = pointActuel->getX();
    y = pointActuel->getY();
    angle = pointActuel->getTheta();

    consigne_angle = angle;
    cmdG = 0;
    cmdD = 0;

    consigneG = 0;
    consigneD = 0;
    rotationDone = false;
    asservFini = true;
}

//Recalage du robot en X et en angle
void Controller::RecalageX() {
    if(abs(x-pointActuel->getX()>50)){
        cout<< "Recalage foireux !"<<endl;
        //exit(3);
    }
    x = pointActuel->getX();
    angle = pointActuel->getTheta();

    consigne_angle = angle;
    cmdG = 0;
    cmdD = 0;

    consigneG = 0;
    consigneD = 0;
    rotationDone = false;
    asservFini = true;
}

//Recalage du robot en Y et en angle
void Controller::RecalageY() {

    if(abs(y-pointActuel->getY()>50)){
        cout<< "Recalage foireux !"<<endl;
        //exit(3);
    }
    y = pointActuel->getY();
    angle = pointActuel->getTheta();

    consigne_angle = angle;
    cmdG = 0;
    cmdD = 0;

    consigneG = 0;
    consigneD = 0;
    rotationDone = false;
    asservFini = true;
}

//Permet au programme principal de savoir quand le robot est arrivé à sa consigne
bool Controller::demandePointSuivant() {
    //Renvoie true si l'asservissement est fini (on est sur le point demandé, ou très près)
    //false sinon
    return asservFini;
}

bool Controller::blocageMoteur(){
    return moteurBloque;
}

//Permet à une autre classe de modifier la vitesse des moteurs
//Utilisé pour la manette :)
void Controller::setMoteur(double vitesseG, double vitesseD){
    moteurs->setOrder(vitesseG, vitesseD);
}

Point * Controller::getCoordonnees() {
    string sens = pointActuel->getDirection();
    return new Point(x, y, angle);
//    return new Point("Deplacement", x, y, angle, 0, 0.0, 0, sens, "aucun", 0.0, false, false, 0, "null", false);
}

void Controller::setVitessePointActuel(int newVitesse){
    pointActuel->setSpeed(newVitesse);
}

void Controller::PositionAbs(int xRelatif, int yRelatif, int *xAbso, int *yAbso)
{
    double teta;
    if(x >= 0){
        teta = (angle - 90) / 180 * M_PI;
    }
    else{
        teta = (angle) / 180 * M_PI;
    }
    double cosTeta = cos(teta);
    double sinTeta = sin(teta);
    *xAbso = (int)(cosTeta * xRelatif - sinTeta * yRelatif + x);
    *yAbso = (int)(sinTeta * xRelatif + cosTeta * yRelatif + y);
}

bool Controller::getPathfindingInAction(){
    return pathfindingInAction;
}

void Controller::setPathfindingInAction(bool newStatut){
    pathfindingInAction = newStatut;
}