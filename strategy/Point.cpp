//
// Created by Taoufik on 20/12/2019.
//

#include <iostream>
using namespace std;

#include "Point.h"

Point::Point(float x, float y, float theta) :
    m_x(x),
    m_y(y),
    m_theta(theta) {
}

void Point::logTargetInformation() {
    // Display some information according to the type of the point

    // For now let's just print the location
    cout << "Target : (" << m_x << ", " << m_y << ", " << m_theta << "°)" << endl;
    cout << "Type : " << m_type << endl;
    cout << "Action : " << action << "\n" << endl;
}

