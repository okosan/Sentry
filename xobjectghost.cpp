#include "xobjectghost.h"

#include <cstdlib>

#include "world.h"
#include "sound.h"

#include <cstdio>
#include <cassert>

const char SIGN_GHOST  = 175;   // cyrillic П

XObjectGhost::XObjectGhost()
{

}


void XObjectGhost::init(int startX, int startY)
{
    m_x = startX;
    m_y = startY;

    m_startLocationX = startX;
    m_startLocationY = startY;

    m_stepsToBecomeActive = 0;
    m_isActive = true;

    m_isInitialized = true;

    m_isVisible = false;
}

/// defines the following random ghost coordinates
/// verifies that the ghost has not exceeded the level (only in the field, without leaving the walls of the control)
void XObjectGhost::stepAI()
{
    if (m_isInitialized)
    {
        if (m_isActive == true)
        {
            // determine the following random coordinates
            bool debug_allow_motion = true;

            if (debug_allow_motion)
            {
                int randomX = (rand() % 3) - 1;  // [-1 0 1]
                m_x += randomX;

                int randomY = (rand() % 3) - 1;  // [-1 0 1]
                m_y += randomY;

                // check that the ghost has not exceeded the level (only in the field, without leaving the wall)
                int worldW, worldH;
                getWorldDimensions(worldW, worldH);

                if (m_x > worldW - 2)
                {
                    m_x = worldW - 2;
                }

                if (m_x < 1)
                {
                    m_x = 1;
                }

                if (m_y > worldH - 2)
                {
                    m_y = worldH - 2;
                }

                if (m_y < 1)
                {
                    m_y = 1;
                }
            }

            ///  Checking the collision with a trap
            int rTrapX, rTrapY;
            bool trapExists; // true - if the trap exists, false - if the trap is not within the radius of the ghost (and the radius of the trap)

            // call function (whether the Ghost is in the Trap area (input: Ghost coordinates, output - Trap coordinates)
            trapExists = worldIsObjectInZoneOfTrap(m_x, m_y, rTrapX, rTrapY);

            if (trapExists == true)  // if the trap is actually located next (Ghost within the range of the trap)
            {
                setMapValue(rTrapX, rTrapY, OBJECT_EMPTY); //In place of the traps make a void
                deactivate();  // deactivate ghosts
                soundTrap();
            }
        }
        else
        {
            if (m_stepsToBecomeActive <= 0)
            {
                m_stepsToBecomeActive = 0;
                m_isActive = true;
            }
            else
            {
                m_stepsToBecomeActive --;
            }
        }
    }
}

std::vector<OBJECT_RENDER_DESCRIPTOR> XObjectGhost::render()
{
    std::vector<OBJECT_RENDER_DESCRIPTOR> vRenderDescriptors;

    if (m_isInitialized)
    {
        OBJECT_RENDER_DESCRIPTOR objRD;
        objRD.x = m_x;
        objRD.y = m_y;

        // NB: Ghost is visible when it is close to the user
        if (m_isActive)
        {
            if (m_isVisible)
            {
                objRD.color_red = 1.0;
                objRD.color_green = 1.0;
                objRD.color_blue = 1.0;
                objRD.isBold = true;

                objRD.str = SIGN_GHOST;

                goto descriptor_finish;
            }
            /*
            else
            {
                objRD.str = '8';
                goto descriptor_finish;
            }
            */
        }
        goto draw_skip;

descriptor_finish:
        vRenderDescriptors.push_back(objRD);

draw_skip:
        {
            /* do not add */
        }
    }

    return vRenderDescriptors;
}

void XObjectGhost::deactivate()
{
    assert(m_isInitialized == true);

    /// sets the check box that the ghost is not in active state
    m_isActive = false;

    // move the ghost in its original coordinates
    m_x = m_startLocationX;
    m_y = m_startLocationY;

    // Tell the ghost through how many steps it should become active
    m_stepsToBecomeActive = 5;
}


bool XObjectGhost::isCollided(int objectX, int objectY)
{
    // collision test
    if ((m_isInitialized == true) && (m_isActive == true) && (objectX == m_x) && (objectY == m_y))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void XObjectGhost::setVisibility(bool isVisible)
{
    m_isVisible = isVisible;
}


