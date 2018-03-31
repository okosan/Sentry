#include "xobjectfox.h"

#include "world.h"
#include "sound.h"

#include "xobjectplayerhero.h"
#include "utils.h"

#include <cstdio>
#include <vector>
#include <cassert>

extern XObjectPlayerHero hero;

const char SIGN_FOX = 245;

XObjectFox::XObjectFox()
{

}

void XObjectFox::init(int startX, int startY)
{
    m_x = startX;
    m_y = startY;

    m_isInitialized = true;
}

void XObjectFox::stepAI()
{
    if (!m_isInitialized)
        return;

    int worldW, worldH;
    getWorldDimensions(worldW, worldH);

    int newX = m_x;
    int newY = m_y;

    // calculate next move - use A* pathfinding
    {
        // identify target mushroom location (nearest) on map
        int targetX, targetY;
        bool found = worldGetNearestObjectLocation(m_x, m_y, targetX, targetY, OBJECT_MUSHROOMS);
        if (!found)
            return;

        // next movement point
        std::list<CoordXY> vPath;
        float distanceMeasured;
        bool allow_diagonal_moves = true;
        bool path_found = worldGetAStarPath(m_x, m_y, targetX, targetY,
                                            vPath, distanceMeasured,
                                            allow_diagonal_moves);
        if (!path_found)
            return;

        if (vPath.begin() == vPath.end())
        {
            // path is found, but no movement is required to reach target
        }
        else
        {
            const CoordXY &coord_next = *vPath.begin();

            newX = coord_next.x;
            newY = coord_next.y;
        }
    }

    /// The movement is confirmed only if there is no wall-barrier at that point.
    /// Forbidden to walk through the door (any).
    int value = getMapValue(newX, newY);
    if (!worldIsSolidObstacle(value))
    {
        m_x = newX;
        m_y = newY;
    }

    if (value == OBJECT_MUSHROOMS)
    {
        // collect mushroom
        setMapValue(m_x, m_y, OBJECT_EMPTY);
    }
}

std::vector<OBJECT_RENDER_DESCRIPTOR> XObjectFox::render()
{
    std::vector<OBJECT_RENDER_DESCRIPTOR> vRenderDescriptors;
    if (m_isInitialized)
    {
        OBJECT_RENDER_DESCRIPTOR objRD;
        objRD.x = m_x;
        objRD.y = m_y;

        objRD.color_red = 0.5;
        objRD.color_green = 0.0;
        objRD.color_blue = 0.0;
        objRD.isBold = true;

        objRD.str = SIGN_FOX;
        vRenderDescriptors.push_back(objRD);
    }

    return vRenderDescriptors;
}

