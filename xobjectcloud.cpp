#include "xobjectcloud.h"
#include <cstdlib>

#include "world.h"
#include "sound.h"

#include <cstdio>
#include <cassert>

const char SIGN_CLOUD  = '8';

XObjectCloud::XObjectCloud()
{
    m_cloudRadiusX = 0;
    m_cloudRadiusY = 0;
}

void XObjectCloud::stepAI()
{
    if (m_isInitialized)
    {
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
    }
}


void XObjectCloud::init(int startX, int startY, int cloudW, int cloudH)
{
    m_x = startX;
    m_y = startY;

    m_cloudRadiusX = cloudW;
    m_cloudRadiusY = cloudH;

    m_isInitialized = true;
}

/// Cloud mapping.
/// Standard drawing interface between dynamic object and Render.
/// The function returns an array of RenderDescripts (Questionnaire with information about what to draw).
std::vector<OBJECT_RENDER_DESCRIPTOR> XObjectCloud::render()
{
    std::vector<OBJECT_RENDER_DESCRIPTOR> vRenderDescriptors;

    if (m_isInitialized)
    {
        for (int dy = 0; dy < m_cloudRadiusY; dy++)
        {
            for (int dx = 0; dx < m_cloudRadiusX; dx++)
            {
                OBJECT_RENDER_DESCRIPTOR objRD;
                objRD.x = m_x + dx;
                objRD.y = m_y + dy;

                //objRD.color_red = 1.0;
                //objR D.color_green = 1.0;
                objRD.color_blue = 1.0;
                objRD.isBold = true;

                objRD.str = SIGN_CLOUD;

                vRenderDescriptors.push_back(objRD);
            }
        }
    }

    return vRenderDescriptors;
}

#if 0
// objectX, objectY - hero coordinates
bool XObjectCloud::isCollided(int objectX, int objectY)
{
    // collision test
    if ((m_isInitialized == true))
    {
        bool isIntersecting = false;

        if ((objectX == m_x) && (objectY == m_y))
        {
            isIntersecting = true;
        }

        if ((objectX == m_x+1) && (objectY == m_y))
        {
            isIntersecting = true;
        }

        if ((objectX == m_x+2) && (objectY == m_y))
        {
            isIntersecting = true;
        }

        if ((objectX == m_x) && (objectY == m_y+1))
        {
            isIntersecting = true;
        }

        if ((objectX == m_x+1) && (objectY == m_y+1))
        {
            isIntersecting = true;
        }

        if ((objectX == m_x+2) && (objectY == m_y+1))
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
#endif

#if 1
bool XObjectCloud::isCollided(int objectX, int objectY)
{
    // collision test
    if ((m_isInitialized == true))
    {
        for (int y = m_y; y <= m_y + m_cloudRadiusY; y++)
        {
            for (int x = m_x; x <= m_x + m_cloudRadiusX; x++)
            {
                if ((objectX == x) && (objectY == y))
                {
                    return true;
                }
            }
        }
    }

    return false;
}
#endif





