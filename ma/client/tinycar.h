#ifndef Tinycar_H
#define Tinycar_H

#define Tinycar_MAX_SPEED 0xFF
#define Tinycar_MIN_SPEED 0
#define Tinycar_ORIENTATION_FORWARD 0
#define Tinycar_ORIENTATION_BACKWARD 1
/**
 * @brief The Tinycar class
 * tiny car control order
 */
#include<QObject>
class Tinycar{
public:

    /**
     * @brief leftOrientation
     * left wheel orientation range 0 means forward ,1 means backward
     * 左轮转的方向 01表示向后，00表示向前
     */
    QString leftOrientation;
    /**
     * @brief rightOrientation
     * right wheel orientation range 0 means forward ,1 means backward
     * 左轮转的方向 01表示向后，00表示向前
     */
    QString rightOrientation;


    int left_c_speed;
    int right_c_speed;
    QString left_speed_str;
    QString right_speed_str;
public:
    void init();
    Tinycar();

    QString getLeftOrientation(){return leftOrientation;}
    QString getRightOrientation(){return rightOrientation;}
    void printInfo();

    Tinycar& setLeftOrientation(QString);
    Tinycar& setRightOrientation(QString);

    //high type order
    Tinycar& goSpeedUp(quint8);
    Tinycar& backSpeedUp(quint8);
    Tinycar& turnleft(quint8);
    Tinycar& turnright(quint8);
    Tinycar& start(quint8);
    Tinycar& stop(quint8);



};
#endif // Tinycar_H
