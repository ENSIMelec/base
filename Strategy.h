//
// Created by Tom on 23/02/2022.
//

#ifndef KRABBS_STRATEGY_H
#define KRABBS_STRATEGY_H

#include <vector>
#include "Controller.h"
#include "Point.h"

using namespace std;

class Strategy {
public:
    Strategy(Point * startingPoint);

    Point *getCurrentPoint();

    bool isDone();
    void setNextPoint(Controller *pController);
    void addPoint(Point *point);
    void initController(Controller *controller);

    bool m_isDone = false;
private:
    int m_currentPointIndex = 0;


    vector<Point*> m_points;
    Point * m_currentPoint;
};


#endif //KRABBS_STRATEGY_H
