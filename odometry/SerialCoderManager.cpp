#include "SerialCoderManager.h"

//#define DEBUG_SERIAL_CODER_MANAGER


using namespace std;

SerialCoderManager::SerialCoderManager()
{
//    cout << "Initializing coder manager ... ";

    bool initSuccess = false;
    for(int i=0 ; i <= 1; i++ ) {
        string port = "/dev/ttyUSB" + to_string(i);
        if((encoders_fd = serialOpen(port.c_str(), 115200)) > 0) {
            initSuccess = true;

            serialFlush(encoders_fd);
            cout << endl << "\tConnected to /dev/ttyUSB" << i << ", ";
            break;
        };
    }
    if(!initSuccess) {
        cout << "Serial Coder Manager Error :" << endl << "Unable to open serial device. " << strerror (errno) << endl;
        exit(3);
        //return 1 ;
    } else {
//        cout << "done" << endl;
    }
}

void SerialCoderManager::readAndReset() {

#ifdef DEBUG_SERIAL_CODER_MANAGER
    unsigned int startTime = millis();
#endif

    // Save last ticks
    oldRightTicks = rightTicks;
    oldLeftTicks = leftTicks;
    oldElapsedTime = elapsedTime;

    // Send command byte
    byte b = static_cast<byte>(COMMAND_GET);

    write(encoders_fd, &b, 1);
//    serialPutchar(encoders_fd, COMMAND_GET);

    int status = 0;
    unsigned int timeout = millis() + TIMEOUT;
    while ((status = serialDataAvail(encoders_fd)) <= 0) {
        if(millis() > timeout) {
            write(encoders_fd, &b, 1);
            timeout += TIMEOUT;
            break;
        }

        if(status == -1) {
            cout << "Error : " << strerror(errno) << endl;
        }
    };

    // Get data from Arduino

    // Datagram :
    // [StartByte]     [LSB][][][MSB]      [LSB][][][MSB]      [LSB][][][MSB]      [StopByte]
    //                    leftTicks           rightTicks         elapsedTime

    int buffer[14];

    int value;
    for(int byteIndex = 0; byteIndex < 14; byteIndex++) {
        if((value = serialGetchar(encoders_fd)) != -1) {
            buffer[byteIndex] = value;
        } else {
            cout << "[SerialCoderManager] Error while getting the data from the arduino" << endl;
            printBuffer(buffer);
        }
    }

    // Store the data in corresponding variables
    int startByte = buffer[0];
    int stopByte = buffer[13];
    if(startByte != START_BYTE || stopByte != STOP_BYTE) {
        // Incorrect data
        cout << "[SerialCoderManager] Warning : Incorrect data received from the arduino" << endl;
        printBuffer(buffer);
    } else {
        leftTicks = buffer[1] + (buffer[2] << 8) + (buffer[3] << 16) + (buffer[4] << 24);
        rightTicks = buffer[5] + (buffer[6] << 8) + (buffer[7] << 16) + (buffer[8] << 24);
        elapsedTime = buffer[9] + (buffer[10] << 8) + (buffer[11] << 16) + (buffer[12] << 24);
    }

#ifdef DEBUG_SERIAL_CODER_MANAGER
    unsigned int endTime = millis();
    cout << "[SerialCoderManager] Elapsed time : " << (endTime - startTime) << endl;
#endif
}

void SerialCoderManager::printBuffer(int *buffer) {
    cout << "Received : ";
    for (int i = 0; i < 14; i++) cout << "[" << buffer[i] << "]";
    cout << endl;
}

/*
void SerialCoderManager::readAndReset()
{
    char SerieData = ' ';

    char tickd [100];
    char tickg [100];
    char temps [100];
    int g = 0;
    int d = 0;
    int t = 0;

    memset(tickd, ' ', 100);
    memset(tickg, ' ', 100);
    memset(temps, ' ', 100);


    if (millis() > nextTime)
    {
        //Debuguer
        //printf ("\nOut: %3d: ", count) ;

        serialPutchar(encoders_fd, 'C');
        serialFlush(encoders_fd);
        nextTime += 10 ;
        ++count ;
    }

    //delay (10) ;

    while(serialDataAvail(encoders_fd) <= 0 || (SerieData = serialGetchar(encoders_fd)) != '?'){
//        serialPutchar(encoders_fd, 'C');
//        std::this_thread::sleep_for(std::chrono::milliseconds(5));
//        cout<<"Attente réception codeurs"<<endl;
    }

    //cout << "reception[";
    //SerieData = serialGetchar (fd);
    //cout << SerieData;
    if(SerieData=='?'){
        SerieData = serialGetchar (encoders_fd);
        //cout << SerieData;
        while(SerieData !=','){
            tickd[d]=SerieData;
            SerieData = serialGetchar (encoders_fd);
            //cout << SerieData;
            d++;
        }
        SerieData = serialGetchar (encoders_fd);
        //cout << SerieData;
        while(SerieData !=':'){
            tickg[g]=SerieData;
            SerieData = serialGetchar (encoders_fd);
            //cout << SerieData;
            g++;
        }
        SerieData = serialGetchar (encoders_fd);
        //cout << SerieData;
        while(SerieData !=';'){
            temps[t]=SerieData;
            SerieData = serialGetchar(encoders_fd);
            //cout << SerieData;
            t++;
        }
    }
    //cout << "]"<<endl;
    serialFlush(encoders_fd);

    oldLeftTicks = leftTicks;
    oldRightTicks = rightTicks;
    oldTempsLast = tempsLast;
    leftTicks = atoi(tickg);
    rightTicks = atoi(tickd);
    tempsLast = atoi(temps);
//    cout <<"leftTicks :"<<leftTicks <<endl;
//    cout <<"rightTicks :"<<rightTicks <<endl;
//    cout <<"tempsLast :"<<tempsLast <<endl;
    if(isnan(leftTicks)){
        leftTicks=0;
    }
    if(isnan(rightTicks)){
        rightTicks=0;
    }
    if(isnan(tempsLast)){
        tempsLast=0;
    }
}
*/

void SerialCoderManager::reset() const
{
	serialPutchar(encoders_fd, COMMAND_RESET);
}

int SerialCoderManager::getRightTicks() const
{
    return rightTicks - oldRightTicks;
}

int SerialCoderManager::getLeftTicks() const
{
    return leftTicks - oldLeftTicks;
}

int SerialCoderManager::getTime() const
{
    return elapsedTime - oldElapsedTime;
}
