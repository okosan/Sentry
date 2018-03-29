#include "xobjectfox.h"

#include "world.h"
#include "sound.h"

#include "xobjectplayerhero.h"
#include "utils.h"

#include <cstdio>
#include <vector>

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

    // calculate next move
    for (int attempt = 0; attempt < 10; attempt++)
    {
        int checkX = newX;
        int checkY = newY;
        bool isVerticalMove = (rand() % 2 == 0); // [0 .. 1]

        if (isVerticalMove)
        {
            int randomY = (rand() % 3) - 1; // [-1 0 1]
            checkY += randomY;
        }
        else
        {
            int randomX = (rand() % 3) - 1; // [-1 0 1]
            checkX += randomX;
        }

        int targetX, targetY;
        bool found = worldGetNearestObjectLocation(m_x, m_y, targetX, targetY, OBJECT_MUSHROOMS);
        if (!found)
            return;

        float distanceBefore = worldGetDistance(m_x, m_y, targetX, targetY);
        float distanceAfter = worldGetDistance(checkX, checkY, targetX, targetY);

        if (distanceAfter < distanceBefore)
        {
            // allow movement
            newX = checkX;
            newY = checkY;
            break;
        }
    } // for

    /// The movement is confirmed only if there is no wall-barrier at that point.
    /// Forbidden to walk through the door (any).
    int value = getMapValue(newX, newY);
    if ((value != OBJECT_WALL) && (value != OBJECT_WEAK_WALL) &&
        (value != OBJECT_DOOR) && (value != OBJECT_GREEN_DOOR) &&
        (value != OBJECT_BLUE_DOOR))
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

