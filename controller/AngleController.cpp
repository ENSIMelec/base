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

    double angleError = targetAngle - theta;
//    angleCommand = angleError * Pk_angle * accelerationFactor;

    angleCommand = pid->compute(theta, targetAngle);
    angleCommand *= accelerationFactor;


//    cout << "[ANGLE CONTROLLER] Angle error : " << angleError << endl;
//    cout << "[ANGLE CONTROLLER] Angle command : " << angleCommand << endl;

}

bool AngleController::isTargetReached() const {
    return abs(targetAngle - currentAngle) < angleThreshold;
}
