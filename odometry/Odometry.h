#ifndef ROBOT_ODOMETRY_H
#define ROBOT_ODOMETRY_H

#include "SerialCoderManager.h"
#include "cmath"
#include "iostream"
#include "../utility/MathUtils.h"
#include "../utility/Configuration.h"
#include <wiringPi.h>

/**
 * @brief Structure de position.
 * \author Taoufik Tribki
 * Location est une structure de Odometry.h qui permet en une variable d'obtenir la totalité des informations à propos du positionnement du robot.
 */
struct Location
{
    /**
     * @brief Constructeur de Location.
     * Constructeur de Location qui initialise la position au coordonnées (0,0) et à l'angle 0.
     */
    Location() : x(0), y(0), theta(0){}

    /**
     * @brief  Constructeur de Location.
     *
     * Constructeur de Location qui initialise la position au coordonnées indiqués.
     *
     * @param x coordoonée en x initial.
     * @param y coordoonée en y initial.
     * @param theta angle initial.
     */
    Location(double x, double y, double theta) : x(x), y(y), theta(theta){}
    // Coordonnée en x
    double x;
    // Cordonnée en y
    double y;
    // Angle
    double theta;
};
/**
 * Calcul la position en temps réél du robot
 */
class Odometry {

public:

    explicit Odometry(Configuration * config);
    void debug();
    /*void calcul_position_segment(float distance, float angle);
    void calcul_position_arc(float distance, float angle);*/
    void distance_total_update(int long ticksLeft, int long ticksRight);


    
    bool isStuck(int motorLSpeed, int motorRSpeed);
    void unStuck(int graceTime = UNSTUCK_GRACE_TIME);

    
    /**
     * @brief Retourne la position
     * @return
     */
    const Location &getLocation() const { return m_pos; }
    void setPosition(float x, float y, float theta){ m_pos.x = x; m_pos.y = y; m_pos.theta = theta;}

    /**
    * @brief Calcule la nouvelle position et la nouvelle vitesse.
    * détermine la nouvelle vitesse instantanée et la nouvelle position.
    */
    void update();

    // ---------- Getters & Setters ----------
    float getDeltaOrientation() const { return m_dAvgTheta; }
    float getDeltaTheta() const { return m_dTheta; }
    float getTotalTheta() const { return totalAngle; }
    float getDeltaDistance() const { return m_dDistance; }
    float getTotalDistance() const { return totalDistance; }
    float getLinVel() const {return m_linVel;} /* Vitesse linéaire */
    float getAngVel() const {return m_angVel;} /* Vitesse angulaire */

    void setX(float x) {m_pos.x = x;}
    double getX() const {return m_pos.x;}

    void setY(float y) {m_pos.y = y;}
    double getY() const {return m_pos.y;}

    void setTheta(float theta) {m_pos.theta = theta;}
    double getTheta() const {return m_pos.theta;}

    SerialCoderManager *getSerialCoderManager() {return m_codeurs;}

protected:
    Location m_pos; /*!< Structure de position de Odometry. */

    float m_linVel = 0; /*!< Vitesse lineaire en mm/s. */
    float m_angVel = 0; /*!< Vitesse angulaire en rad/s.*/

    int m_totalTicksL = 0; /* Totat des tics gauche */
    int m_totalTicksR = 0;/* Totat des tics droite */
    float totalDistance = 0; /* Total distance parcouru (odometry test purpose) */
    float totalAngle = 0;

    float m_dDistance = 0; /*  Distance en mm parcouru en dt */
    float m_dAvgTheta = 0; /* moyenne angle(t-1) et t en rad en dt */
    float m_dTheta = 0; /* angle rad en dt */
    float m_lastTime = 0; /* dt */

    // constantes
    float PERIM_ROUE = 0; /* Perimetre roue en mm  Diametre * PI */
    float RESOLUTION = 0; /* nombre tics par 1 tour du roue */
    float COEF_CORRECTEUR = 0;
    float ENTRAXE = 0; // entraxe entre les deux roues en mm

    unsigned int initTime = 0;

    SerialCoderManager * m_codeurs;


    // Constants for stuck calcul
    inline static int TIME_BEFORE_STUCK_MS = 200;
    inline static int MOTOR_SPEED_FOR_STUCK = 40;
    inline static float MIN_VEL_FOR_STUCK = 0.05;
    inline static float MIN_ANG_FOR_STUCK = 0.4;   
    inline static int UNSTUCK_GRACE_TIME = 2000;

    // Timer for stuck calcul
    int stuckTimer = 0;
    int unStuckTimer = 0;

};


#endif //ROBOT_ODOMETRY_H
