#include "xobjectrobik.h"

#include <cstdio>

#include "world.h"
#include "input.h"

#include "xobjectplayerhero.h"
extern XObjectPlayerHero hero;

#include "windows.h"
#include <cassert>

const char SIGN_ROBIK_NORMAL      = 'r'; // 0x01; // 'r'; //
const char SIGN_ROBIK_INTERACTIVE = 'R'; // 0x02; // 'R'; // TODO: smiley char in PDCurses

const char SIGN_ROBIK_DEBUG_UP    = '^'; // 0x18;
const char SIGN_ROBIK_DEBUG_DOWN  = 'V'; //0x19;
const char SIGN_ROBIK_DEBUG_LEFT  = '<'; //0x1B;
const char SIGN_ROBIK_DEBUG_RIGHT = '>' ; //0x1A;


/// distance to the Hero where Robik goes into interactive mode
const float ROBIK_RADIUS_BECOMES_INTERACTIVE = 2.0f;


XObjectRobik::XObjectRobik()
{
    m_isDebugDraw = false; /// In non-debug mode Robik will "smile" when nearby
}

void XObjectRobik::init(int startX, int startY)
{
    m_x = startX;
    m_y = startY;

    m_isReadyForInteraction = false;

    m_isInitialized = true;

    m_direction = DIRECTION_RIGHT;
    m_isTurnAllowed = true;
}


/// iteration of artificial intelligence
void XObjectRobik::stepAI()
{
    /// do nothing.
    if (m_isInitialized)
    {
        if (!m_isReadyForInteraction)
        {
            bool shall_move_forward = false;

            if (!isWallTouchedByRightHand())
            {
                if (m_isTurnAllowed)
                {
                    rotateClockWise();
                    m_isTurnAllowed = false;
                }
                else
                {
                    shall_move_forward = true;
                }
            }
            else
            {
                if (true)
                {
                    m_isTurnAllowed = true;
                }
                shall_move_forward = true;
            }

            if (shall_move_forward)
            {
                if (isWallAhead())
                {
                    m_isTurnAllowed = true;
                    rotateCounterClockWise();
                }
                else
                {
                    makeOneStepForward();
                }
            }

            /*
            if (isWallAhead())
            {
                rotateCounterClockWise();
            }
            else
            {
                makeOneStepForward(); // Continue movement in set direction
            }*/

        }
        m_isReadyForInteraction = shouldBecomeInteractive();
    }
}


std::vector<OBJECT_RENDER_DESCRIPTOR> XObjectRobik::render()
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

        bool f_drawSmiley = false;
        bool f_drawArrows = false;

        /// Allows you to choose a smiley depending on Robic's condition.
        if (!m_isDebugDraw)
        {
            if (m_isReadyForInteraction)
            {
                f_drawSmiley = true;
            }
            else
            {
                f_drawArrows = true;
            }
        }
        else
        {
            f_drawArrows = true;
        }

        assert(f_drawSmiley != f_drawArrows);

        /// Draws emoticons.
        if (f_drawSmiley)
        {
            if (m_isReadyForInteraction)
            {
                objRD.color_green = 0.7;
                objRD.isBold = true;
                objRD.str = SIGN_ROBIK_INTERACTIVE;

                goto descriptor_finish;
            }
            else
            {
                objRD.color_green = 0.7;
                objRD.str = SIGN_ROBIK_NORMAL;

                goto descriptor_finish;
            }
        }

        /// Draws arrows.
        if (f_drawArrows)
        {
            char sign = 'X';
            switch(m_direction)
            {
            case(DIRECTION_UP):
                sign = SIGN_ROBIK_DEBUG_UP;
                break;
            case(DIRECTION_DOWN):
                sign = SIGN_ROBIK_DEBUG_DOWN;
                break;
            case(DIRECTION_LEFT):
                sign = SIGN_ROBIK_DEBUG_LEFT;
                break;
            case(DIRECTION_RIGHT):
                sign = SIGN_ROBIK_DEBUG_RIGHT;
                break;
            default:
                assert(false);
            }
            objRD.color_green = 0.7;
            objRD.str = sign;

            goto descriptor_finish;
        }

descriptor_finish:

        vRenderDescriptors.push_back(objRD);
    }

    return vRenderDescriptors;
}

bool XObjectRobik::react(int actionCode)
{
    bool f_reacted = false;

    switch(actionCode)
    {
    case(ACTION_TRADE):
        {
            if (m_isReadyForInteraction)
            {
                hero.tradeChitin();
                Beep(7000, 150); // It's a deal!
                f_reacted = true;
            }
            break;
        }
    default:
        f_reacted = false;
        break;
    }

    return f_reacted;
}

bool XObjectRobik::shouldBecomeInteractive()
{
    if (m_isInitialized)
    {
        int currentHeroX, currentHeroY;
        hero.getXY(currentHeroX, currentHeroY);

        float distance = worldGetDistance(m_x, m_y, currentHeroX, currentHeroY);
        return (distance < ROBIK_RADIUS_BECOMES_INTERACTIVE);
    }
    return false;
}

void XObjectRobik::rotateClockWise()
{
    m_direction = (RobotDirection)( ((int)(m_direction) + 1) % DIRECTION_LAST);
}

void XObjectRobik::rotateCounterClockWise()
{
    int dir = (int)m_direction;
    dir--;
    if (dir < 0)
    {
        dir = (int)DIRECTION_LAST - 1;
    }

    m_direction = (RobotDirection)(dir);
}

/// Continues to move in the predominant direction.
void XObjectRobik::makeOneStepForward()
{
    switch(m_direction)
    {
    case(DIRECTION_UP):
        m_y--;
        break;
    case(DIRECTION_DOWN):
        m_y++;
        break;
    case(DIRECTION_LEFT):
        m_x--;
        break;
    case(DIRECTION_RIGHT):
        m_x++;
        break;
    default:
        assert(false);
    }
}

/// tells whether Robik rests to the wall with his nose.
bool XObjectRobik::isWallAhead()
{
    int x_test = 0;
    int y_test = 0;

    switch(m_direction)
    {
    case(DIRECTION_RIGHT):
        x_test = +1;
        break;
    case(DIRECTION_UP):
        y_test = -1;
        break;
    case(DIRECTION_LEFT):
        x_test = -1;
        break;
    case(DIRECTION_DOWN):
        y_test = +1;
        break;
    default:
        assert(false);
    }

    int x_world_test = m_x + x_test;
    int y_world_test = m_y + y_test;

    int worldValue = getMapValue(x_world_test, y_world_test);

    return (worldValue == OBJECT_WALL);
}

/// Does the right hand touch the wall?
bool XObjectRobik::isWallTouchedByRightHand()
{
    int x_test = 0;
    int y_test = 0;

    switch(m_direction)
    {
    case(DIRECTION_RIGHT):
        y_test = +1;
        break;
    case(DIRECTION_UP):
        x_test = +1;
        break;
    case(DIRECTION_LEFT):
        y_test = -1;
        break;
    case(DIRECTION_DOWN):
        x_test = -1;
        break;
    default:
        assert(false);
    }

    int x_world_test = m_x + x_test;
    int y_world_test = m_y + y_test;

    int worldValue = getMapValue(x_world_test, y_world_test);

    return (worldValue == OBJECT_WALL);
}
