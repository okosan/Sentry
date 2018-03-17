#ifndef XOBJECTGHOST_H
#define XOBJECTGHOST_H

#include "render.h"
#include "xdynamicobject.h"

#include <vector>

const int GHOST_DAMAGE = 20;  // The amount of damage that the Ghost is inflicting on the player-Hero

class XObjectGhost : public XDynamicObject
{
private:
    int m_startLocationX;
    int m_startLocationY;

    int m_stepsToBecomeActive;
    bool m_isActive;

    void deactivate();

    bool m_isVisible;

public:
    XObjectGhost();

    void init(int startX, int startY);

    void stepAI();

    std::vector<OBJECT_RENDER_DESCRIPTOR> render();

    bool isCollided(int objectX, int objectY);

    void debug_receive_damage()
    {
        deactivate();
    }

    bool isActive()
    {
        return m_isActive;
    }

    void setVisibility(bool isVisible);
};

#endif // XOBJECTGHOST_H
