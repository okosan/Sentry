#ifndef XOBJECTROBIK_H
#define XOBJECTROBIK_H

#include "render.h"
#include "xdynamicobject.h"

#include <vector>

class XObjectRobik : public XDynamicObject
{
public:
    XObjectRobik();

    void init(int startX, int startY);

    void stepAI();

    std::vector<OBJECT_RENDER_DESCRIPTOR> render();

    bool react(int actionCode);

private:
    enum RobotDirection
    {
        DIRECTION_UP = 0,
        DIRECTION_RIGHT = 1,
        DIRECTION_DOWN = 2,
        DIRECTION_LEFT = 3,
        DIRECTION_LAST = 4
    };

    bool m_isReadyForInteraction;
    bool m_isDebugDraw;
    RobotDirection m_direction;
    bool m_isTurnAllowed;

    void rotateClockWise();
    void rotateCounterClockWise();

    // continue to move in the main direction
    void makeOneStepForward();

    bool isWallAhead(); // Does the nose touch the wall?
    bool isWallTouchedByRightHand(); // Is the right arm touching the wall
    bool shouldBecomeInteractive();
};

#endif // XOBJECTROBIK_H
