//
// Created by Tom on 03/05/2022.
//

#ifndef KRABBS_POINT_TYPE_H
#define KRABBS_POINT_TYPE_H

enum PointType {
    STATIC_POSITION,
    ABSOLUTE_ANGLE,
    RELATIVE_ANGLE,
    SET_X,
    SET_Y,
    SET_XY_THETA,
    MOVE_TO_POSITION,
    MOVE_TO_POSITION_RELATIVE_TO_ANGLE,
    WAIT,
    ACTION,
    ANGLE,
    SET_THETA
};

#endif //KRABBS_POINT_TYPE_H
