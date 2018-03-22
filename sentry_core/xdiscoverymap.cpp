#include "xdiscoverymap.h"

#include "world.h"

#include "xgamesettings.h"

XDiscoveryMap::XDiscoveryMap()
{
    m_pGameSettings = NULL;
}

void XDiscoveryMap::init(const XGameSettings *pGameSettings)
{
    m_pGameSettings = pGameSettings;

    int w, h;
    getWorldDimensions(w, h);

    m_W = w;
    m_H = h;

    clear();
}

void XDiscoveryMap::update(int heroX, int heroY)
{
    int discoveryRadiusX, discoveryRadiusY;

    m_pGameSettings->getDiscoveryRadius(discoveryRadiusX, discoveryRadiusY);

    for (int dy = -discoveryRadiusY; dy <= discoveryRadiusY; dy++)
    {
        for (int dx = -discoveryRadiusX; dx <= discoveryRadiusX; dx++)
        {
            int testX = heroX + dx;
            int testY = heroY + dy;
            int numberOfWallsOnLine = worldGetNumberOfWallsOnLine(heroX, heroY, testX, testY, 1.0f);

            if (numberOfWallsOnLine == 0)
            {
                if (testX == heroX && testY == heroY)
                {
                    setDiscoveryMapValue(testX, testY, DISCOVERY_MAP_CELL_OPEN);
                }
                else
                {
                    setDiscoveryMapValue(testX, testY, DISCOVERY_MAP_CELL_OPEN);
                }
            }
        }
    }
}

/// Render the world discovery map (for debugging) on the right
/// side of the screen
std::vector<OBJECT_RENDER_DESCRIPTOR> XDiscoveryMap::render()
{
    std::vector<OBJECT_RENDER_DESCRIPTOR> vRenderDescriptors;

    bool isVisible = isRenderingEnabled();
    if (!isVisible)
    {
        return vRenderDescriptors;  // return empty render list
    }

    int x0 = 40;  // TODO: calculate runtime, center of the screen
    int y0 = 0;
    for (int y = 0; y < m_H; y++)
    {
        for (int x = 0; x < m_W; x++)
        {
            int value = getDiscoveryMapValue(x, y);

            OBJECT_RENDER_DESCRIPTOR descr;
            descr.x = x0 + x;
            descr.y = y0 + y;
            descr.color_green = 0.7;
            descr.color_red = 0.7;

            switch(value)
            {
            case(DISCOVERY_MAP_CELL_CLOSED):
                descr.str = SIGN_EMPTY;
                break;
            case(DISCOVERY_MAP_CELL_OPEN):
                descr.str = SIGN_BLIN;
                break;
            default:
                descr.str = 'X';
            }

            vRenderDescriptors.push_back(descr);
        }
    }

    return vRenderDescriptors;
}

bool XDiscoveryMap::isVisibleCoord(int x, int y) const
{
    if (getDiscoveryMapValue(x, y) == DISCOVERY_MAP_CELL_OPEN)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void XDiscoveryMap::clear(int clearValue)
{
    if (m_pGameSettings->isEnableDiscloseEntireMap())
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


bool XDiscoveryMap::isRenderingEnabled() const
{
    return m_pGameSettings->isEnableDrawDiscoveryMap();
}
