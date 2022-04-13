////
//// Created by Christophe on 01/03/2022.
////
//
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <signal.h>
//#include <string.h>
//#include <math.h>
//
//#include "../../sdk/lib/sl_lidar.h"
//#include "../../sdk/lib/sl_lidar_driver.h"
//#include "../../sdk/lib/sl_types.h"
//
//#ifndef _countof
//#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
//#endif
//
//#ifdef _WIN32
//#include <Windows.h>
//#define delay(x)   ::Sleep(x)
//#else
//#include <unistd.h>
//
//
//#ifndef LIDAR_H
//#define LIDAR_H
//
//static inline void delay(sl_word_size_t ms){
//    while (ms>=1000){
//        usleep(1000*1000);
//        ms-=1000;
//    };
//    if (ms!=0)
//        usleep(ms*1000);
//}
//#endif
//
//using namespace sl;
//
//class Lidar {
//public:
//    Lidar();
//    ~Lidar();
//    void printaround();
//
//    bool obstacleInRange(float distmesure, float anglemin, float anglemax);
//    int détecterGammeDeDistange(float anglemin, float anglemax, float dist1, float dist2, float dist3);
//
//
//private:
//
//
//    void ctrlc(int);
//
//    bool ctrl_c_pressed;
//    bool checkSLAMTECLIDARHealth(ILidarDriver *drv);
//
//    const char * opt_is_channel = NULL;
//    const char * opt_channel = NULL;
//    sl_u32         opt_channel_param_second = 0;
//    sl_u32         baudrateArray[2] = {115200, 256000};
//    sl_result     op_result;
//    int          opt_channel_type = CHANNEL_TYPE_SERIALPORT;
//
//    IChannel* _channel = (*createSerialPortChannel("/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0", 115200));
//    ILidarDriver * drv = *createLidarDriver();
//
//    sl_lidar_response_device_info_t devinfo;
//    bool connectSuccess = false;
//
//    sl_lidar_response_measurement_node_hq_t nodes[8192];
//    size_t   count = _countof(nodes);
//
//
//
//
//
//
//};
//
//
//#endif //LIDAR_H
