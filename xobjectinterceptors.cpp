#include "xobjectinterceptors.h"

#include <cstdlib>

#include "world.h"
#include "sound.h"

#include "xobjectplayerhero.h"
#include "xobjectcarrier.h"

#include <cstdio>
#include <cassert>

extern XObjectPlayerHero hero;
extern XObjectCarrier carrier[WORLD_MAX_NUM_CARRIER];

const char SIGN_INTERCEPTORS  = 'I';

XObjectInterceptors::XObjectInterceptors()
{
    m_pCarrierOwner = 0;
}

void XObjectInterceptors::stepAI()
{
    if (m_isInitialized)
    {
        //                0    1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17
        const int dx[] = {0,   1,  2,  3,  4,  5,  5,  5,  4,  3,  2,  1,  0, -1, -2, -2, -2, -1};
        const int dy[] = {-2, -2, -2, -2, -2, -1,  0,  1,  2,  2,  2,  2,  2,  2,  1,  0, -1, -2};

        m_pCarrierOwner->getXY(m_ox, m_oy);

        m_x = m_ox + dx[m_index];
        m_y = m_oy + dy[m_index];

        int worldW, worldH;
        getWorldDimensions(worldW, worldH);
        if (m_x >= worldW)
        {
            m_x = 0;
        }

        if (m_y >= worldH)
        {
            m_y = 0;
        }

        m_index++;
        if (m_index > 17)
        {
            m_index = 0;
        }
    }
}

void XObjectInterceptors::init(int startX, int startY, XDynamicObject *pCarrierOwner)
{
    m_x = startX;
    m_y = startY;

    m_ox = startX;
    m_oy = startY;

    m_index = 0;  // to initiate one of the 17 locations of the airplane (first index)

    m_pCarrierOwner = pCarrierOwner;

    m_isInitialized = true;
}

/// Interceptor icon display.
/// A standard drawing interface between a dynamic object and a renderer.
/// The function returns an array of RenderDescripts (Questionnaire with information about what to draw).
std::vector<OBJECT_RENDER_DESCRIPTOR> XObjectInterceptors::render()
{
    std::vector<OBJECT_RENDER_DESCRIPTOR> vRenderDescriptors;
    int interceptorsRadiusX, interceptorsRadiusY;

    interceptorsRadiusX = 1;
    interceptorsRadiusY = 1;

    if (m_isInitialized)
    {
        for (int dy = 0; dy < interceptorsRadiusY; dy++)
        {
            for (int dx = 0; dx < interceptorsRadiusX; dx++)
            {
                OBJECT_RENDER_DESCRIPTOR objRD;
                objRD.x = m_x + dx;
                objRD.y = m_y + dy;

                objRD.color_red = 1.0;
                //objR D.color_green = 1.0;
                //objRD.color_blue = 1.0;
                objRD.isBold = true;

                objRD.str = SIGN_INTERCEPTORS;

                if (!(hero.getFogOfViewMap()->isVisibleCoord(objRD.x, objRD.y)))
                {
                    continue;
                }

                vRenderDescriptors.push_back(objRD);
            }
        }
    }

    return vRenderDescriptors;
}

// objectX, objectY - hero coordinates
bool XObjectInterceptors::isCollided(int objectX, int objectY)
{
    // collision test
    if (m_isInitialized)
    {
        bool isIntersecting = false;

        if ((objectX == m_x) && (objectY == m_y))
        {
            isIntersecting = true;
        }

        return isIntersecting;
    }
    else
    {
        return false;
    }
}



