//
// Created by Tom on 08/05/2022.
//

#include "AngleController.h"
#include "../utility/MathUtils.h"

AngleController::AngleController(Configuration *configuration) {
    double Pk = configuration->getDouble("controller.angle.Pk");
    double Pi = configuration->getDouble("controller.angle.Pi");

    pid = new PID(Pk, Pi, 0, -70, 70);

    acceleration = configuration->getDouble("controller.angle.acceleration");
    angleThreshold = MathUtils::deg2rad(configuration->getDouble("controller.angle.angle_threshold"));
}

void AngleController::calculateCommands(double theta) {

    currentAngle = theta;

    // Set acceleration
    if(accelerationFactor < 1) {
        accelerationFactor += acceleration;
    } else {
        accelerationFactor = 1;
    }

    angleError = targetAngle - theta;

    angleCommand = pid->compute(angleError);
    angleCommand *= accelerationFactor;
}

void AngleController::setTargetAngle(double theta) {
    targetAngle = theta;
    pid->reset();
}

bool AngleController::isTargetReached() const {
    return abs(targetAngle - currentAngle) < angleThreshold;
}
