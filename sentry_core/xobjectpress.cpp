#include "xobjectpress.h"

#include "world.h"
#include <cstdio>

const char SIGN_PRESS  = 254;   // small rectangle

const int PRESS_NUMBER_OF_STEPS_BEFORE_DIRECTION_CHANGE = 4;

#include "xobjectplayerhero.h"
extern XObjectPlayerHero hero;


XObjectPress::XObjectPress()
{
    m_x = 0;
    m_y = 0;
    m_stepCounter = 0;
    m_isMovingInPositiveDirection = true;
}


void XObjectPress::init(int startX, int startY, int phaseNumber)
{
    m_x = startX;
    m_y = startY;
    m_stepCounter = 0;
    m_isMovingInPositiveDirection = true;

    m_isInitialized = true;

    setPhase(phaseNumber);
}


void XObjectPress::stepAI()
{
    if (m_isInitialized)
    {
        if (m_stepCounter == PRESS_NUMBER_OF_STEPS_BEFORE_DIRECTION_CHANGE)
        {
            m_isMovingInPositiveDirection = !m_isMovingInPositiveDirection;
            m_stepCounter = 0;
        }

        if (m_isMovingInPositiveDirection)
        {
            m_y ++;
            m_stepCounter ++;
        }
        else
        {
            m_y --;
            m_stepCounter ++;
        }
    }
}

//bool XObjectPress::render(int drawX, int drawY)
std::vector<OBJECT_RENDER_DESCRIPTOR> XObjectPress::render()
{
    std::vector<OBJECT_RENDER_DESCRIPTOR> vRenderDescriptors;
    if (!(hero.getFogOfViewMap()->isVisibleCoord(m_x, m_y)))
    {
        return vRenderDescriptors;
    }


    if (m_isInitialized)
    {
        OBJECT_RENDER_DESCRIPTOR objRD;
        objRD.x = m_x;
        objRD.y = m_y;

        objRD.color_blue = 1.0f;
        objRD.isBold = true;

        objRD.str = SIGN_PRESS;

        vRenderDescriptors.push_back(objRD);
    }

    return vRenderDescriptors;
}

bool XObjectPress::isCollidedWithPress(int objectX, int objectY)
{
    if ((m_isInitialized) && (objectX == m_x) && (objectY == m_y))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void XObjectPress::setPhase(int phaseNumber)
{
    phaseNumber = phaseNumber % (2 * PRESS_NUMBER_OF_STEPS_BEFORE_DIRECTION_CHANGE);

    // phaseNumber - 0, 1, 2, 3,    4, 5, 6, 7,    0
    int currentStep = phaseNumber % 4;
    bool isPositiveDirection = (phaseNumber < PRESS_NUMBER_OF_STEPS_BEFORE_DIRECTION_CHANGE);

    int dy = -1;

    if (phaseNumber < PRESS_NUMBER_OF_STEPS_BEFORE_DIRECTION_CHANGE)
    {
        dy = phaseNumber;
    }
    else
    {
        dy = 2 * PRESS_NUMBER_OF_STEPS_BEFORE_DIRECTION_CHANGE - phaseNumber;
    }

    m_stepCounter = currentStep;
    m_isMovingInPositiveDirection = isPositiveDirection;

    m_y += dy;
}
