#ifndef XOBJECTINTERCEPTORS_H
#define XOBJECTINTERCEPTORS_H

#include "render.h"
#include "xdynamicobject.h"

#include <vector>

const int INTERCEPTORS_DAMAGE = 1; // The amount of damage that the Interceptors is inflicting on the player-Hero

class XObjectInterceptors : public XDynamicObject
{
private:
    int m_ox;
    int m_oy;     // reference point of the orbit in the world map coordinates
    int m_index;  // current state index of the position of the airplane

    XDynamicObject *m_pCarrierOwner;

public:
    XObjectInterceptors();

    void init(int startX, int startY, XDynamicObject *pCarrierOwner);

    void stepAI();

    std::vector<OBJECT_RENDER_DESCRIPTOR> render();

    bool isCollided(int objectX, int objectY);

};

#endif // XOBJECTINTERCEPTORS_H







