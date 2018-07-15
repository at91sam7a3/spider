#ifndef MOVECOMMANDS_H
#define MOVECOMMANDS_H

#include "body/Hull.h"

namespace spider {

enum Actions
{
    Stop=0,
    Forward=1,
    TurnLeft=2
};

typedef std::vector<LegCoodinates> LegFramesList;

struct PrecalculatedAnimation
{
    //frame list for each leg
    LegFramesList frames[6];
    int totalFrames;
    int frameDelay;
};

class MoveCommands
{
public:
    static MoveCommands* GetInstance();

    /// @brief move forward
    /// @param distance [in] Distance in mm. if value  is negative then move back
    void MoveForward(float distance);

    /// @brief rotate
    /// @param angle[in] - angle for rotation. positive angle for clockwise
    void Rotate(float angle);

    /// @brief immediate stop current movement
    /// and switch to Steady position
    void Halt();

    /// @brief sit down
    void SitDown();

    void SetLegPos(int legIdx, float xpos, float ypos, float zpos);
    LegCoodinates GetLegPos(int legIdx);

    void StandUp(int height);

private:
    MoveCommands();
    static MoveCommands* m_singlton;
    Hull m_hull;

    float stepSize;
    float stepWidth;
    float stepSize2;
    float stepHeight;

    float rotateRadius;
    float stepAngleSize;
    float stepAngleSize2;


    std::vector <PrecalculatedAnimation> m_animations;
    // This function is called before any action to place legs in first frame position
    void PrepareToMove(spider::Actions action,bool direct=false);

    //void PrepareToMove3L(spider::Actions action);

    void GenerateAnimationStop();
    void GenerateAnimationLookUp();
    void GenerateAnimationMoveForward();
    void GenerateAnimationTurnLeft();
    void ApplyOffsetsToWalkAnimations();

    void PlayAnimation(spider::Actions action, int count);
    void PlayAnimationReverse(spider::Actions action, int count);
};
}
#endif // MOVECOMMANDS_H
