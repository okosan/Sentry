#ifndef XFOGOFVIEWMAP_H
#define XFOGOFVIEWMAP_H

#include "xdiscoverymap.h"

class XFogOfViewMap : public XDiscoveryMap
{
public:
    XFogOfViewMap();

    void update(int heroX, int heroY);


protected:
    virtual bool isRenderingEnabled() const;

    void clear(int clearValue = DISCOVERY_MAP_CELL_CLOSED);
};

#endif // XFOGOFVIEWMAP_H
