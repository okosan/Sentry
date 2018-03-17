#ifndef XOBJECTCARRIER_H
#define XOBJECTCARRIER_H

#include "render.h"
#include "xdynamicobject.h"

#include <vector>

const int CARRIER_DAMAGE = 1;  // The amount of damage that Baton inflicts on the player Hero

class XObjectCarrier : public XDynamicObject
{
private:
    int m_index;
    int m_numInterceptors;
    int m_stepInterc;

public:
    XObjectCarrier();

    void init(int startX, int startY);

    void stepAI();

    std::vector<OBJECT_RENDER_DESCRIPTOR> render();

    bool isCollided(int objectX, int objectY);
};

#endif // XOBJECTCARRIER_H


