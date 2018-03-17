#include "xobjectcarrier.h"
#include <cstdlib>

#include "world.h"
#include "sound.h"

#include "xobjectplayerhero.h"

#include <cstdio>
#include <cassert>

const char SIGN_CARRIER  = 'C';

extern XObjectPlayerHero hero;

XObjectCarrier::XObjectCarrier() :
    XDynamicObject()
{
    m_index = 0;
    m_stepInterc = 0;
}

void XObjectCarrier::init(int startX, int  startY)
{
    m_x = startX;
    m_y = startY;

    m_index = 0;

    m_numInterceptors = 0;
    m_stepInterc = 0;

    m_isInitialized = true;
}

void XObjectCarrier::stepAI()
{
    if (m_isInitialized)
    {
        m_index++;
        if (m_index > 2)
        {
            m_index = 0;
            m_x ++;
            m_y ++;
        }

        if (m_numInterceptors < 3)
        {
            m_stepInterc++;
            if (m_stepInterc >= 5)
            {
                m_stepInterc = 0;
                worldAddOneObjectInterceptors(m_x, m_y, this);
                m_numInterceptors++;
            }
        }

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


/// Reflection on Carrier's concept.
/// Standard drawing interface between dynamic object and Render.
/// The function returns an array of RenderDescripts (Questionnaire with information about what to draw).
std::vector<OBJECT_RENDER_DESCRIPTOR> XObjectCarrier::render()
{
    std::vector<OBJECT_RENDER_DESCRIPTOR> vRenderDescriptors;
    int carrierRadiusX, carrierRadiusY;

    carrierRadiusX = 4;
    carrierRadiusY = 1;

    if (m_isInitialized)
    {
        for (int dy = 0; dy < carrierRadiusY; dy++)
        {
            for (int dx = 0; dx < carrierRadiusX; dx++)
            {
                OBJECT_RENDER_DESCRIPTOR objRD;
                objRD.x = m_x + dx;
                objRD.y = m_y + dy;

                objRD.color_red = 1.0;
                //objR D.color_green = 1.0;
                //objRD.color_blue = 1.0;
                objRD.isBold = true;

                objRD.str = SIGN_CARRIER;

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

// testX, testY - hero coordinates
bool XObjectCarrier::isCollided(int testX, int testY)
{
    // collision test
    if ((m_isInitialized == true))
    {
        bool isIntersecting = false;

        if ((testX == m_x) && (testY == m_y))
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

