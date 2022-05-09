#include "ActionManager.h"

//Constructeur par défaut : utilise la liste d'initialisation pour init les classes utilisées dans la classe
ActionManager::ActionManager(int i2c_Servos, int nbAX12, const string& actionsPath) :
        servoManager(i2c_Servos), ax12Manager(nbAX12)
{
    path = actionsPath;

	servoManager.servoInitialisation();
	printf("Constructeur action");
}


void ActionManager::close() {
	cout << "Arrêt du lanceur" << endl;
	sleepMillis(250);
	ax12Manager.close();
	return;
}


void ActionManager::action(const string& filename) {
	
	unsigned int indexAction = 0;
	vector<ActionServo> actions;
	actions = FichierAction::readPoints(path + filename);
	char typeActionneur;
	int numActionneur, angleAction, forceAction, tempsAction;

	while(indexAction < actions.size()) {

		typeActionneur = actions.at(indexAction).gettype();
		numActionneur = actions.at(indexAction).getn();
		angleAction = actions.at(indexAction).geta();
		forceAction = actions.at(indexAction).getf();
		tempsAction = actions.at(indexAction).gett();

		switch(typeActionneur) {
			case 'S':
				servoManager.servoAction(numActionneur, angleAction);
				//client.sendMessage("I Servo n°"+to_string(numActionneur)+" angle : "+to_string(angleAction));
				break;
			case 'N':
				// Ici angleAction represente la hauteur en mm
				//stepper.setSpeed(forceAction);
				//stepper.setPosition(angleAction);

				//client.sendMessage("I Stepper hauteur" + to_string(angleAction));
				break;
			case 'P':
				//client.sendMessage("I Ajout de "+to_string(numActionneur)+" points");
				//client.addPoints(numActionneur, angleAction);
				break;
			case 'A':
				tempsAction -= ax12Manager.AX12Action(numActionneur, angleAction, forceAction); //On effectue l'action ax12Manager, on met à jour le temps qu'il reste avant la fin de l'action
				//client.sendMessage("I ax12Manager n°"+to_string(numActionneur)+" angle : "+to_string(angleAction)+" force : "+to_string(forceAction));
				break;//Attention aux delays
			default: //On ne lance pas d'action, mais on attend le temps demandé
				break;
		}
		if(tempsAction > 0)
		{
			cout << "Attente " << tempsAction << " ms" << endl;
			sleepMillis(tempsAction);
		}
		indexAction++;
	}
	
	return;
}

void ActionManager::sleepMillis(int millis) {
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}
