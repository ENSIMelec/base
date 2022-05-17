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

    void setTargetAngle(double theta);
private:
    double angleCommand{};
    double targetAngle{};
    double currentAngle{};
    double angleError = 0;

    PID * pid;

    double accelerationFactor{};
    double acceleration;

    double angleThreshold;
};


#endif //KRABBS_ANGLECONTROLLER_H
