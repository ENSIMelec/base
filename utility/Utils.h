//
// Created by Tom on 02/12/2021.
//

#ifndef KRABBS_UTILS_H
#define KRABBS_UTILS_H

namespace Utils {
    /// @brief Function used to sleep a given amount of milliseconds
    void sleepMillis(int millis);

    /// @brief Clamp a value between a min and a max
    double clamp(double value, double min, double max);
}
#endif //KRABBS_UTILS_H
