//
// Created by Tom on 08/05/2022.
//

#include "AngleController.h"
#include "../utility/MathUtils.h"

AngleController::AngleController(Configuration *configuration) {
    Pk_small = configuration->getDouble("controller.angle.small.Pk");
    Pi_small = configuration->getDouble("controller.angle.small.Pi");
    Pd_small = configuration->getDouble("controller.angle.small.Pd");
    Pk_big = configuration->getDouble("controller.angle.big.Pk");
    Pi_big = configuration->getDouble("controller.angle.big.Pi");
    Pd_big = configuration->getDouble("controller.angle.big.Pd");

    pid = new PID(Pk_small, Pi_small, 0, -70, 70);

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

    if(abs(angleCommand) < 10) angleCommand = 10;
    angleCommand *= accelerationFactor;
}

void AngleController::setTargetAngle(double angle, double destAngle) {

    currentAngle = angle;
    const double base = M_PI;;
    double theta = MathUtils::normalizeAngle(currentAngle);

    double alpha_r = 0, alpha_d = 0;
    if (theta < 0) {
        alpha_r = -base - theta;
    } else {
        alpha_r = base - theta;
    }

    if (destAngle < 0) {
        alpha_d = base + destAngle;
    } else {
        alpha_d = -base + destAngle;
    }

    double s1 = destAngle - theta;
    double s2 = alpha_r + alpha_d;

    targetAngle = (abs(s1) < abs(s2)) ? s1 : s2;
}

void AngleController::setAbsoluteTargetAngle(double theta) {
    targetAngle = theta;
    resetPid();
}

void AngleController::resetPid() {
    pid->reset();

    // Different settings for small and big angles
    if(abs(targetAngle) < M_PI_4) {
        pid->setTunings(Pk_small, Pi_small, Pd_small);
    } else {
        pid->setTunings(Pk_big, Pi_big, Pd_big);
    }
}

bool AngleController::isTargetReached() const {
    return abs(targetAngle - currentAngle) < angleThreshold;
}

