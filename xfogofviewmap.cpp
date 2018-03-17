#include "xfogofviewmap.h"

#include "xgamesettings.h"

XFogOfViewMap::XFogOfViewMap() :
    XDiscoveryMap()
{

}

void XFogOfViewMap::update(int heroX, int heroY)
{
    // Discovery - no clear
    // Fog       - clear on every step
    clear();

    XDiscoveryMap::update(heroX, heroY);
}

void XFogOfViewMap::clear(int clearValue)
{
    if (!m_pGameSettings->isEnableFogOfView())
    {
        clearValue = DISCOVERY_MAP_CELL_OPEN;
    }

    for (int y = 0; y < m_H; y++)
    {
        for (int x = 0; x < m_W; x++)
        {
            setDiscoveryMapValue(x, y, clearValue);
        }
    }
}

bool XFogOfViewMap::isRenderingEnabled() const
{
    return m_pGameSettings->isEnableDrawFogOfViewMap();
}

