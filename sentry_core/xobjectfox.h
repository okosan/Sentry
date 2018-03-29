#ifndef XOBJECTFOX_H
#define XOBJECTFOX_H

#include "render.h"
#include "xdynamicobject.h"

#include <vector>

class XObjectFox : public XDynamicObject
{
public:
    XObjectFox();

    void init(int startX, int startY);

    void stepAI();

    std::vector<OBJECT_RENDER_DESCRIPTOR> render();
};

#endif // XOBJECTFOX_H
