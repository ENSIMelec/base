#include "SerialCodeurManager.h"

using namespace std;

int arduino;
unsigned int nextTime ;
int count = 1;
int encoders_fd ;


SerialCodeurManager::SerialCodeurManager()
{
//    cout << "Initializing coder manager ... ";

    bool initSuccess = false;
    for(int i=0 ; i <= 1; i++ ) {
        string port = "/dev/ttyUSB" + to_string(i);
        if((encoders_fd = serialOpen(port.c_str(), 115200)) > 0) {
            initSuccess = true;
            break;
        };
    }
    if(!initSuccess) //A REMETTRE A 1 PLUS TARD
    {
        cout << "Serial Coder Manager Error :" << endl << "Unable to open serial device. " << strerror (errno) << endl;
        exit(3);
        //return 1 ;
    } else {
//        cout << "done" << endl;
    }

    nextTime = millis () + 10 ;
}

//SerialCodeurManager::~SerialCodeurManager() {
//    serialClose(encoders_fd);
//}

void SerialCodeurManager::readAndReset()
{
    char SerieData = ' ';

    char tickd [10000];
    char tickg [10000];
    char temps [100000];
    int g = 0;
    int d = 0;
    int t = 0;

    memset(tickd, ' ', 10000);
    memset(tickg, ' ', 10000);
    memset(temps, ' ', 100000);


    if (millis() > nextTime)
    {
        //Debuguer
        //printf ("\nOut: %3d: ", count) ;
        fflush (stdout);
        serialPutchar (encoders_fd, 'C');
        nextTime += 10 ;
        ++count ;
    }

    //delay (10) ;

    while(serialDataAvail(encoders_fd)<=0 || (SerieData = serialGetchar(encoders_fd))!='?'){
        serialPutchar (encoders_fd, 'C');
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
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
    //cout <<"leftTicks :"<<leftTicks <<endl;
    //cout <<"rightTicks :"<<rightTicks <<endl;
    //cout <<"tempsLast :"<<tempsLast <<endl;
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

void SerialCodeurManager::reset()
{
	serialPutchar (encoders_fd, 'R');
}

int SerialCodeurManager::getRightTicks()
{
    return rightTicks - oldRightTicks;
}

int SerialCodeurManager::getLeftTicks()
{
    return leftTicks - oldLeftTicks;
}

int SerialCodeurManager::getTime()
{
    return tempsLast - oldTempsLast;
}
