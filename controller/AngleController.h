//
// Created by Tom on 08/05/2022.
//

#ifndef KRABBS_ANGLECONTROLLER_H
#define KRABBS_ANGLECONTROLLER_H


#include "../utility/Configuration.h"
#include "PID.h"

class AngleController {

public:
    explicit AngleController(Configuration * configuration);

    void calculateCommands(double theta);
    double getAngleCommand() const {return angleCommand;}

    void setTargetAngle(double theta) {targetAngle = theta;}

    bool isTargetReached() const;

private:
    double angleCommand;
    double targetAngle;
    double currentAngle;

    PID * pid;


    double accelerationFactor;
    double acceleration;

    double angleThreshold;

};


#endif //KRABBS_ANGLECONTROLLER_H
