#include "Lidar.h"

const char Lidar::SCAN[2] = {0xA5,0x20};
const char Lidar::STOP[2] = {0xA5,0x25};
const char Lidar::RESET[2] = {0xA5,0x40};

const uint8_t Lidar::SCAN_HEADER[7] = {0xA5, 0x5A, 0x05, 0x00, 0x00, 0x40, 0x81};

void Lidar::init(){
	#ifdef LIDAR_DEBUG
	printf("Initialisation du lidar...");
	#endif
	isRunning = false;

	pinMode(PWM_PIN_LIDAR, PWM_OUTPUT);
	setMotorSpeed(0);
	
	if((serialID = serialOpen(SERIAL_PORT, 115200)) <= 0){
		printf("Erreur d'ouverture du port série! Erreur No%d\n", errno);
		exit(-1);
	}
	
	stop();
	#ifdef LIDAR_DEBUG
	printf("ID port série : %d\n", serialID);
	printf("=== Initialisation du lidar terminée ===\n");
	#endif
}


void Lidar::println(const char* msg){
	printf("[Lidar] : %s\n", msg);
}


void Lidar::setMotorSpeed(int speed){
	if(speed > 1024)
		speed = 1023;
	if(speed < 0)
		speed = 0;
	pwmWrite(PWM_PIN_LIDAR, speed);
}

void Lidar::sendCommand(const char command[]){
	serialPuts(serialID, command);
	#ifdef LIDAR_DEBUG
	printf("[Lidar] : Commande envoyée au lidar : %s\n", command == SCAN ? "SCAN" : command == STOP ? "STOP" : command == RESET ? "RESET" : "Autre");
	#endif
}

void Lidar::start(int speed){
	sendCommand(SCAN);
	setMotorSpeed(speed);
	isRunning = true;
	#ifdef LIDAR_DEBUG
	println("Lidar démarré");
	#endif
}

void Lidar::stop(){
	sendCommand(STOP);
	setMotorSpeed(0);
	serialState = 0;
	isRunning = false;
	#ifdef LIDAR_DEBUG
	println("Lidar arrêté");
	#endif
}


bool Lidar::test(){
//	println("Test du lidar en cours...");
	serialFlush(serialID);
	start();
	delay(2000);
	bool ret = serialDataAvail(serialID) >= 4000;
	stop();
	delay(1000);
	printf("[Lidar] : %s\n", ret ? "Le lidar semble fonctionnel" : "Le lidar ne semble pas fonctionnel...");
	serialFlush(serialID);
//	println("Test du lidar terminé");
	return ret;
}

bool Lidar::isActive(){
	return isRunning;
}

bool Lidar::isWarning(){
	return warningCounter > 0;
}

bool Lidar::isDanger(){
	return dangerCounter > 0;
}


void Lidar::run(int runtime, int motorSpeed){
	#ifdef LIDAR_DEBUG
	printf("[Lidar] : Lidar activé pour %d secondes\n", runtime);
	#endif
	
	int i;
	
	time_t endTime = time(NULL) + runtime;
	time_t mil = time(NULL)*1000, lastMillis = mil, diffMillis;
	
	char data[5];
	uint8_t index = 0;
	uint8_t val;
	
	uint16_t angle, distance, qualite;
	
	uint16_t dangerCounterTmp, warningCounterTmp;
	
	int nbPoints = ANGLE_MAX >> ANGULAR_PRECISION;
	uint16_t relativeBuffer[nbPoints][2];
	
	for(i=0;i<nbPoints;i++){
		relativeBuffer[i][0] = 0;
		relativeBuffer[i][1] = 0;
	}
	
	// init
	dangerCounter = 0;
	warningCounter = 0;

	start(motorSpeed);
	
	while((time(NULL) < endTime) && isRunning){
		
		if((mil = millis()) != lastMillis){ //Update des TTL toutes les ms
			diffMillis = mil - lastMillis;
			lastMillis = mil;
			
			// Update des timings de danger
			if(dangerCounter != 0){
				if(dangerCounter <= diffMillis){
					dangerCounter = 0;
				}else{
					dangerCounter -= diffMillis;
				}
			}
			
			if(warningCounter != 0){
				if(warningCounter <= diffMillis){
					warningCounter = 0;
				}else{
					warningCounter -= diffMillis;
				}
			}
			
			dangerCounterTmp = 0;
			warningCounterTmp = 0;
			for(i=0;i<nbPoints;i++){
				if(relativeBuffer[i][1] > 0){ // Update des TTL des points
					if((relativeBuffer[i][1] <= diffMillis)){
						relativeBuffer[i][1] = 0;
					}else{
						relativeBuffer[i][1] -= diffMillis;
					}
				}
				
				if(relativeBuffer[i][1] != 0 && relativeBuffer[i][0] > DISTANCE_MIN){ // Si le point est toujours valide et à jour
					if(relativeBuffer[i][0] < DISTANCE_DANGER){	// Si le point est dans la zone de danger
						//printf("Danger pour angle=%d et distance=%d\n", i, relativeBuffer[i][0]);
						dangerCounterTmp++;
					}else if(relativeBuffer[i][0] < DISTANCE_WARNING){ //Si le point est dans la zone de warning
						//printf("Warning pour angle=%d et distance=%d\n", i, relativeBuffer[i][0]);
						warningCounterTmp++;
					}
				}
			} // End for
			
			//Update des niveau de danger
			if(dangerCounterTmp >= DANGER_QUOTA){
				dangerCounter = DANGER_TIME;
			}
			if(warningCounterTmp >= WARNING_QUOTA){
				warningCounter = DANGER_TIME;
			}
		} // End if millis update
		
		
		// Check de la liaison Serie
		if((val = serialGetchar(serialID)) != -1){
			if(serialState < 7){ // Reception du header scan
				if(SCAN_HEADER[serialState] == val){
					++serialState;
				} else {
					serialState = 0;
					#ifdef LIDAR_DEBUG
					println("Erreur liaison série sur la reception du scan header");
					#endif
				}
			} else { // Reception des datas
				data[index++] = val;
				if(index >= 5){	// Data reçues
					index = 0;
					angle = ((data[1] >> 1) | (data[2] << 7)) >> ANGULAR_PRECISION;
					distance = (data[3] | (data[4] << 8)) >> 2;
					qualite = data[0] >> 2;
					if(qualite > 9 && distance > DISTANCE_MIN){ // Si la qualité du rayon est >= à 10, ie la mesure est fiable, et si la mesure est au dela du minimum
						//printf("Angle : %d, Distance : %d\n", angle, distance);
						relativeBuffer[angle][0] = distance;
						relativeBuffer[angle][1] = TTL_LOCAL;
					}
				}
			}
			
			
		} // end data received
	} // end while
	stop();
	
}




//int main(int argc, char** args){
//	printf("Start\n");
//
//	Lidar::init();
//
//	if(!Lidar::test()){
//		exit(-1);
//	}
//	printf("Go\n");
//
//
//	while(!Lidar::isActive())
//		delay(100);
//	while(Lidar::isActive()){
//	//for(int i=0;i<100;i++){
//		printf("Warning : %d | Danger : %d\n", Lidar::isWarning(), Lidar::isDanger());
//		delay(100);
//	}
//
//	//Lidar::stop();
//
//	th.join();
//
//	printf("Done\n");
//
//	return 0;
//}
