#ifndef XDISCOVERYMAP_H
#define XDISCOVERYMAP_H

#include "render.h"
#include "world.h"

#include <vector>

const int DISCOVERY_MAP_CELL_CLOSED = 0;  /// area not discovered before
const int DISCOVERY_MAP_CELL_OPEN   = 1;  /// discovered area

/// Map object that tracks which parts of the World map were already
/// revealed by the Player. Revealed areas might be still covered by
/// the FogOfViewMap (aka "fog of war").
class XDiscoveryMap
{
public:
    XDiscoveryMap();

    void init(const XGameSettings *pGameSettings);
    void update(int heroX, int heroY);
    virtual std::vector<OBJECT_RENDER_DESCRIPTOR> render();

    bool isVisibleCoord(int x, int y) const;

protected:
    int m_matrixDiscovery[WORLD_MAX_W][WORLD_MAX_H];
    int m_W;
    int m_H;

    const XGameSettings *m_pGameSettings;

    int getDiscoveryMapValue(int x, int y) const
    {
        int value = (m_matrixDiscovery[x][y]);
        return value;
    }

    void setDiscoveryMapValue(int x, int y, int value)
    {
        (m_matrixDiscovery[x][y]) = value;
    }

    void clear(int clearValue = DISCOVERY_MAP_CELL_CLOSED);

    virtual bool isRenderingEnabled() const;
};

#endif // XDISCOVERYMAP_H
