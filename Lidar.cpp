////
//// Created by Christophe on 01/03/2022.
////
//
//
//#include "Lidar.h"
//
//
//
/////Constructeur : permet d'initier la connexion au Lidar
//Lidar::Lidar() {
//
//    if (SL_IS_OK((drv)->connect(_channel))) {
//        op_result = drv->getDeviceInfo(devinfo);
//
//        if (SL_IS_OK(op_result))
//        {
//            connectSuccess = true;
//        }
//        else{
//            delete drv;
//            drv = NULL;
//        }
//    }
//
//
//    if (!checkSLAMTECLIDARHealth(drv)) {
//        //goto on_finished;
//    }
//    //signal(SIGINT, ctrlc);
//    drv->setMotorSpeed();
//    drv->startScan(0,1);
//}
//
//
//
/////Destructeur : permet d'arrêter le Lidar et de s'en déconnecter
//Lidar::~Lidar(){
//    drv->stop();
//    drv->disconnect();
//}
//
//
//
///*void Lidar::print_usage(int argc, const char * argv[])
//{
//    printf("Simple LIDAR data grabber for SLAMTEC LIDAR.\n"
//           "Version: %s \n"
//           "Usage:\n"
//           " For serial channel %s --channel --serial <com port> [baudrate]\n"
//           "The baudrate is 115200(for A2) or 256000(for A3).\n"
//           " For udp channel %s --channel --udp <ipaddr> [port NO.]\n"
//           "The LPX default ipaddr is 192.168.11.2,and the port NO.is 8089. Please refer to the datasheet for details.\n"
//            , "SL_LIDAR_SDK_VERSION", argv[0], argv[0]);
//}
//*/
//
//
//
//
//
//bool Lidar::checkSLAMTECLIDARHealth(ILidarDriver * drv)
//{
//    sl_result     op_result;
//    sl_lidar_response_device_health_t healthinfo;
//
//    op_result = drv->getHealth(healthinfo);
//    if (SL_IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
//        printf("SLAMTEC Lidar health status : %d\n", healthinfo.status);
//        if (healthinfo.status == SL_LIDAR_STATUS_ERROR) {
//            fprintf(stderr, "Error, slamtec Lidar internal error detected. Please reboot the device to retry.\n");
//            // enable the following code if you want slamtec Lidar to be reboot by software
//            // drv->reset();
//            return false;
//        } else {
//            return true;
//        }
//
//    } else {
//        fprintf(stderr, "Error, cannot retrieve the Lidar health code: %x\n", op_result);
//        return false;
//    }
//
//
//
//}
//
//
//
//
//
//
//
//
//
//
//void Lidar::printaround(){
//    op_result = drv->grabScanDataHq(nodes, count);
//
//    if (SL_IS_OK(op_result)) {
//        drv->ascendScanData(nodes, count);
//        for (int pos = 0; pos < (int)count ; ++pos) {
//            float d = nodes[pos].dist_mm_q2/4.0f;
//            float theta = (nodes[pos].angle_z_q14 * 90.f) / 16384.f;
//            float x = d*cos(3.14*theta/360);
//            float y = d*sin(3.14*theta/360);
//            printf("%s theta: %03.2f Dist: %08.2f Q: %d X : %08.2f Y : %08.2f\n",
//                   (nodes[pos].flag & SL_LIDAR_RESP_HQ_FLAG_SYNCBIT) ?"S ":"  ",
//                   theta,
//                   d,
//                   nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT,
//                   x,
//                   y);
//            if (ctrl_c_pressed){
//                break;
//            }
//        }
//    }
//}
//
//
//
///**
// *
// * @param distmesure : the distance you want to verify there is an obstacle in
// * @param anglemin : the beginning of the angle range where you want to watch
// * @param anglemax : the end of the angle range tou want to watch
// * @return true if there is an obstacle,and false otherwise
// */
//
//bool Lidar::obstacleInRange(float distmesure, float anglemin, float anglemax) {
//    printf("Looking for obstacle in the first %f millimeters between angles %f degrees and %f degrees\n",distmesure,anglemin,anglemax);
//
//    op_result = drv->grabScanDataHq(nodes, count);
//
//    if (SL_IS_OK(op_result)) {
//        drv->ascendScanData(nodes, count);
//
//        for (int pos = 0; pos < (int)count ; ++pos) {
//            float d = nodes[pos].dist_mm_q2 / 4.0f;
//            float theta = (nodes[pos].angle_z_q14 * 90.f) / 16384.f;
//            unsigned int flag = nodes[pos].flag;
//            float quality = nodes[pos].quality;
//
//            printf("angle : %f\td : %f\tflag : %uhh\n", theta, d, flag);
//
//            if(theta > anglemax) return false;
//
//           /* if(flag != 2 && theta>anglemin && theta<anglemax && d < distmesure && quality != 0) {
//////                printf("%f\n",d);
//                printf("Found something at %f° and %f mm\n", theta, d);
//                printf("Flag : %hhu\n", nodes[pos].flag);
//                return true;
//            }*/
//
//            if(theta>anglemin && theta<anglemax && d < distmesure && quality != 0) {
//////                printf("%f\n",d);
//                printf("Found something at %f° and %f mm\n", theta, d);
//                printf("Flag : %hhu\n", nodes[pos].flag);
//                return true;
//            }
//
//
//            if (ctrl_c_pressed){
//                return false;
//            }
//        }
//    }
//    return false;
//}
//
//
///**
// *
// * @param anglemin
// * @param anglemax
// * @param dist1
// * @param dist2
// * @param dist3
// * @return 0 if there is no obstacle in range, 1 if the obstacle is in the first distance range, 2 if it is in the 2nd and 3 if it is in the 3rd
// */
//int Lidar::détecterGammeDeDistange(float anglemin, float anglemax, float dist1, float dist2, float dist3) {
//    if(obstacleInRange(dist1,anglemin,anglemax)){
//        return 1;
//    }
//    else if(obstacleInRange(dist2,anglemin,anglemax)){
//        return 2;
//    }
//    else if(obstacleInRange(dist3,anglemin,anglemax)){
//        return 3;
//    }
//    else{
//        return 0;
//    }
//}
