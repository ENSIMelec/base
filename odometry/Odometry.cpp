#include "Odometry.h"
#include "../ui/UI.h"

using namespace std;

//#define DEBUG_ODOMETRY
#define INIT_TIME 1500

/**
 * Odométrie
 * @param codeurs
 */

Odometry::Odometry(Configuration * config) {
    m_codeurs = new SerialCoderManager();

    initTime = millis();

    // nouvelle approche odométrie
    //this->PERIM_ROUE = 31.5*M_PI;
    //this->PERIM_ROUE = 34.8*M_PI; //Diametre * PI
//
//    this->PERIM_ROUE = 31.5*M_PI; //Diametre * PI
//    this->RESOLUTION = 516;
//    this->COEF_CORRECTEUR = m_config.getCoeffCorrecteur();
//    this->ENTRAXE = 290;

    //this->ENTRAXE = 280;

    float diametre = config->getDouble("odometry.wheel_diameter");
    this->PERIM_ROUE = diametre * M_PI; //Diametre * PI
    this->RESOLUTION = config->getDouble("odometry.resolution");
    this->COEF_CORRECTEUR = config->getDouble("odometry.coeff_correcteur");
    this->ENTRAXE = config->getDouble("odometry.entraxe");
    
    // start
    this->m_pos.theta = 0;
}

/**
* @brief Calcule la nouvelle position et la nouvelle vitesse.
* détermine la nouvelle vitesse instantanée et la nouvelle position par approximation de segment de droite
 * Approximation linéaire
 */
void Odometry::update() {

    /* Important :
     * If you send data too early, the arduino will not work normally.
     * That's why there is an INIT_TIME to let the arduino boot correctly.
     */
    while(millis() < initTime + INIT_TIME) {};

#ifdef DEBUG_ODOMETRY
    unsigned int startTime = millis();
#endif

    // récupérer les tics des codeurs + réinitialisation
    m_codeurs->readAndReset();

    // Récupéreration des tics codeurs
    long int ticksLeft = m_codeurs->getLeftTicks();
    long int ticksRight = m_codeurs->getRightTicks();

    // calculer la distance de effectué par chaque roue
    float distanceLeft = ticksLeft * (PERIM_ROUE / RESOLUTION);
    float distanceRight = ticksRight * ((COEF_CORRECTEUR * PERIM_ROUE) / RESOLUTION);

    // Calculer les variations de position en distance et en angle

    // distance parcourue depuis la position de départ jusqu’à l’instant présent.
    float dDistance = (distanceLeft + distanceRight) / 2;
    // Calcul de la différence du nombre de tic entre chaque roue (appx. gauss)
    float diffCount = distanceRight - distanceLeft;
    float dAngle = diffCount/(ENTRAXE);

    // nouvelle méthode:

//    if (diffCount==0)    // On considère le mouvement comme une ligne droite
//    {
//        // Mise à jour de la position
//        this->m_pos.x    += dDistance * cos(this->m_pos.theta);
//        this->m_pos.y    += dDistance * sin(this->m_pos.theta);
//    }
//    else
//    {
//        //On approxime en considérant que le robot suit un arc de cercle
//        // On calcule le rayon de courbure du cercle
//        double R = dDistance / dAngle;
//
//        //Mise à jour de la position
//        this->m_pos.x    += R * (-sin(this->m_pos.theta) + sin(this->m_pos.theta + dAngle));
//        this->m_pos.y    += R * (cos(this->m_pos.theta) - cos(this->m_pos.theta + dAngle));
//        // Mise à jour du cap
//        this->m_pos.theta += dAngle;
//
//        // On limite le cap à +/- PI afin de ne pouvoir tourner dans les deux sens et pas dans un seul
//        if (this->m_pos.theta > M_PI)
//            this->m_pos.theta -= 2 * M_PI ;
//        else if (this->m_pos.theta <= -M_PI)
//            this->m_pos.theta += 2 * M_PI ;
//    }


    // <!> m_pos.theta l'angle initiale
    // Moyenne des angles pour connaître le cap exact
    float avgTheta = m_pos.theta + dAngle/2;

    //Mise à jour de la position du robot en xy et en angle

    this->m_pos.x       += dDistance * cosf(avgTheta); // dAngle?
    this->m_pos.y       += dDistance * sinf(avgTheta);
    this->m_pos.theta   += dAngle;

    // Calcul de la vitesse angulaire et linéaire
    // Actualisation du temps
    this->m_lastTime = m_codeurs->getTime();

    float timestep      = MathUtils::millis2sec(m_lastTime); // micros -> s
    //float timestep      = 0.01; // micros -> s

    float linVel = 0; // mm / s
    float angVel = 0; // rad / s

    if(timestep > 0) {
        linVel = dDistance / timestep;
        angVel = dAngle / timestep;
    }

    // Actualisation de la vitesse linéaire et angulaire du robot
    this->m_linVel = linVel;
    this->m_angVel = angVel;

    // Sauvegarde distance et angle actuelle
    m_dDistance = dDistance;
    m_dTheta = dAngle;

    // Actualisation du total distance parcouru
    distance_total_update(ticksLeft, ticksRight);

#ifdef DEBUG_ODOMETRY
    unsigned int endTime = millis();
    cout << "[Odometry] Elapsed time : " << (endTime - startTime) << endl;
#endif
}

/**
 * @brief Debug purpose
 */
void Odometry::debug() {
    double angleDeg = MathUtils::rad2deg(m_pos.theta);

    cout << "===========DEBUG ODOMETRY============" << endl;
//    cout << "[DATA CODEUR][TICS] : Gauche:" << m_codeurs.getLeftTicks() << " Droit: " << m_codeurs.getRightTicks() << endl;
//    cout << "[DATA CODEUR][TOTAL TICS] : Gauche:" << getTotalTicksL() << " Droit: " << getTotalTicksR() << endl;
//    cout << "[DATA CODEUR][LAST TIME] : " << getLastTime() << " (ms)" << endl;
    cout << "[ODOMETRY][POSITION] X:" << m_pos.x << "\tY: " << m_pos.y;
    cout << "\tTheta: " << angleDeg << "° (" << MathUtils::normalizeAngle(angleDeg, false) << "°)" << endl;
//    cout << "[ODOMETRY][DISTANCE PARCOURU EN LASTTIME (mm)] : " << getDeltaDistance() << endl;
//    cout << "[ODOMETRY][ROTATION EFFECTUE EN LASTTIME (rad)] : " << getDeltaOrientation() << endl;
    cout << "[ODOMETRY][VITESSE]: Vitesse angulaire (rad/s) : " << getAngVel() << " Vitesse Linéaire (mm/s) : " << getLinVel() << endl;
//    cout << "[ODOMETRY][TOTAL DISTANCE] (cm): " << getTotalDistance() / 10 << endl;
}

void Odometry::distance_total_update(int long ticksLeft, int long ticksRight) {

    m_totalTicksL += ticksLeft;
    m_totalTicksR += ticksRight;

    totalDistance += m_dDistance;
    totalAngle += m_dTheta;
}

bool Odometry::isStuck(int motorLSpeed, int motorRSpeed){
//    cout << " [STUCK DEBUG] : motorSpeeds(R/L) : " << motorRSpeed << " " << motorLSpeed << " | linVel : " << m_linVel << " | Timer : " << millis() << " | Stop timer : " << stuckTimer << endl;
    if(millis() < unStuckTimer){
        stuckTimer = millis() + TIME_BEFORE_STUCK_MS;
//        cout << " ======== Grace time, so not stuck ^^ ========= " << endl;
        return false;
    }
    if(motorRSpeed == 0 && motorLSpeed == 0 && millis() >= stuckTimer){
//        cout << " ======== Motor stopped, so I'm stuck ========= " << endl;
        return false;
    }

    if(motorRSpeed >= MOTOR_SPEED_FOR_STUCK || motorLSpeed >= MOTOR_SPEED_FOR_STUCK){
        if(m_linVel < MIN_VEL_FOR_STUCK && m_angVel < MIN_ANG_FOR_STUCK){
            if(millis() >= stuckTimer){
                UI::log(" ======== Stuck !!! =========== ");
                return true;
            }
        }else{
            stuckTimer = millis() + TIME_BEFORE_STUCK_MS;
        }
    }else{
        stuckTimer = millis() + TIME_BEFORE_STUCK_MS;
    }
    UI::log(" ======== Not stuck =========== ");
    return false;
}

void Odometry::unStuck(int graceTime){
    unStuckTimer = millis() + graceTime;
}