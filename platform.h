#ifndef PLATFORM_H
#define PLATFORM_H

#include "body/Leg.h"
#include <math.h>
#include <memory>
#include <thread>
#include <vector>
#include "servomanager.h"

//This class manage all movements of robot
//It contain it`s own thread, and really autonomous
namespace spider {
struct vec2f
{
    constexpr static const double PI=3.141592654;
    vec2f()
        :x(0)
        ,y(0){};
    vec2f(double x1,double y1)
        :x(x1)
        ,y(y1){};

    double x;
    double y;

    void rotate(double angle)
    {
        double tmpAngle=angle*PI/180.0;
        double tmpx=(cos(tmpAngle)*x)-(sin(tmpAngle)*y);
        double tmpy=(sin(tmpAngle)*x)-(cos(tmpAngle)*y);
        x=tmpx;
        y=tmpy;
    }
};

enum MovementState{
    Sleeping=0,
    Idle,
    Going,
    Rotating
};

struct LegStatus{
  vec2f position;
  float height;
  static vec2f maxOffset;
  static float stepHeight;

};

class Platform
{
public:
    Platform();
    //Immidiate stop
    void Stop();
    //Sit and power-off legs
    void Sleep();
    //Power-on legs
    void Wake();
    //coordinates
    vec2f GetCurrentPosition();
    void GoToPosition(vec2f);
    //just movement
    void MoveForward(float distance);
    void Turn(float degrees);

    void SetBodyHeight(float height);
    float GetBodyHeight();

private:

    void procedureGo();
    void procedureTurn();
    void prepareToGo();
    void prepareToTurn();
    void MovementThread();
    void MovementDelay();
    std::unique_ptr<std::thread> moving_thread_;
    MovementState state_;
    vec2f currentCoordinates;
    int moveSpeed;
    vec2f normalizedMovementVector;
    float currentMovementSpeed;
    std::vector <Leg> legs_;
    double bodyHeight_;
    double distanceLeft_;
    double directionAngle;
    double desiredAngle_;
    ServoManager servoManager_;
    float LegAngles_tmp[6];
};
};
#endif // PLATFORM_H
