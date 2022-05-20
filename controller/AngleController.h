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
    [[nodiscard]] double getAngleCommand() const {return angleCommand;}
    [[nodiscard]] double getAngleError() const {return angleError;}
    [[nodiscard]] bool isTargetReached() const;

    void setAbsoluteTargetAngle(double theta);
    void setTargetAngle(double currentAngle, double destAngle);

private:
    double angleCommand{};
    double targetAngle{};
    double currentAngle{};
    double angleError = 0;

    double accelerationFactor{};
    double acceleration;

    double angleThreshold;

    PID * pid;
    double Pk_small;
    double Pi_small;
    double Pd_small;
    double Pk_big;
    double Pi_big;
    double Pd_big;

    void resetPid();
};


#endif //KRABBS_ANGLECONTROLLER_H
