#ifndef XOBJECTCLOUD_H
#define XOBJECTCLOUD_H

#include "render.h"
#include "xdynamicobject.h"

#include <vector>

const int CLOUD_DAMAGE = 1;  //The amount of damage that the Cloud is inflicting on the player-Hero

class XObjectCloud : public XDynamicObject
{
public:
    XObjectCloud();

    void init(int startX, int startY, int cloudW, int cloudH);

    void stepAI();

    std::vector<OBJECT_RENDER_DESCRIPTOR> render();

    bool isCollided(int objectX, int objectY);

private:
    int m_cloudRadiusX;
    int m_cloudRadiusY;
};

#endif // XOBJECTCLOUD_H
