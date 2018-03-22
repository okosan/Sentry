#include "xdynamicobject.h"

XDynamicObject::XDynamicObject()
{
    m_x = -1;
    m_y = -1;

    m_isInitialized = false;
}

void XDynamicObject::getXY(int &rX, int &rY) const
{
    rX = m_x;
    rY = m_y;
}
