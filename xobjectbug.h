#ifndef XOBJECTBUG_H
#define XOBJECTBUG_H

#include "render.h"
#include "xdynamicobject.h"

#include <vector>

class XObjectBug : public XDynamicObject
{
private:
    int m_state;
    int m_numberOfStepsLeftToRecover;
    int m_numberOfStepsInAgressiveState;

    bool m_isKilled;

public:
    XObjectBug();

    void init(int startX, int startY);

    void stepAI();

    std::vector<OBJECT_RENDER_DESCRIPTOR> render();

    void soundDamaged();

    void kill();

    bool isKilled()
    {
        return m_isKilled;
    }

    // we ask the World whether the Beetle should become aggressive - true - if it should become aggressive
    bool shouldBecomeAgressive();
};

#endif // XOBJECTBUG_H
