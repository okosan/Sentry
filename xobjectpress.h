#ifndef XOBJECTPRESS_H
#define XOBJECTPRESS_H

#include "render.h"
#include "xdynamicobject.h"

#include <vector>

/// Force of damage to the Hero at the arrival of the Press (in units of the shield)
const int PRESS_DAMAGE = 10;

class XObjectPress : public XDynamicObject
{
private:
    int m_stepCounter;
    bool m_isMovingInPositiveDirection;

public:
    XObjectPress();

    void init(int startX, int startY, int phaseNumber);

    void stepAI();

    std::vector<OBJECT_RENDER_DESCRIPTOR> render();


    bool isCollidedWithPress(int objectX, int objectY);

    void setPhase(int phaseNumber);
};

#endif // XOBJECTPRESS_H
