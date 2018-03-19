#include "xobjectbug.h"

#include "world.h"

#include <cstdio>
#include <windows.h>

#include "xobjectpress.h"
#include "xobjectplayerhero.h"
#include "utils.h"

extern XObjectPlayerHero hero;
extern XObjectPress press[WORLD_MAX_NUM_PRESS];


const char SIGN_BUG            = 134;  // cyrillic Ж
const char SIGN_BUG_KILLED     = '~';  // displayed when bug is killed
const char SIGN_BUG_PASSIVE    = 166;  // small cyrillic ж
const char SIGN_BUG_AGRESSIVE  = 134;  // cyrillic Ж
const char SIGN_BUG_SCARED     = 234;  // cyrillic ъ


const int BUG_STATE_PASSIVE   = 0;  /// Bug condition is passive
const int BUG_STATE_AGRESSIVE = 1;  /// aggressive
const int BUG_STATE_SCARED    = 2;  /// scared

const float BUG_RADIUS_BECOME_AGRESSIVE = 4.0f;

const int BUG_DAMAGE = 30;
const int BUG_NUMBER_OF_STEPS_TO_RECOVER_AFTER_ATTACK = 4;  // cooldown - number of steps for a beetle to recover from the attack

const int BUG_MAX_NUMBER_OF_STEPS_IN_AGRESSIVE_STATE = 10;

XObjectBug::XObjectBug()
{
    m_isKilled = true;
}


void XObjectBug::init(int startX, int startY)
{
    m_x = startX;
    m_y = startY;
    m_state = BUG_STATE_PASSIVE;
    m_numberOfStepsLeftToRecover = 0;

    m_isKilled = false;
    m_isInitialized = true;
}

///
void XObjectBug::stepAI()
{
    if ((m_isInitialized) && (!m_isKilled))
    {
        if (m_numberOfStepsLeftToRecover > 0)
        {
            m_numberOfStepsLeftToRecover --;
        }

        int newX = m_x;
        int newY = m_y;

        switch(m_state)
        {
        case(BUG_STATE_PASSIVE):
        {
            /// Bug is passive and makes random-free movements.

            // determine whether the movement will be vertically
            bool isVerticalMove = (rand() % 2 == 0); // [0 .. 1]

            /// Bug makes its move.
            if (isVerticalMove)
            {
                int randomY = (rand() % 3) - 1; // [-1 0 1]
                newY += randomY;
            }
            else
            {
                int randomX = (rand() % 3) - 1; // [-1 0 1]
                newX += randomX;
            }

            break;
        }
        case(BUG_STATE_AGRESSIVE):
        {
            for (int attempt = 0; attempt < 10; attempt++)
            {
                int checkX = newX;
                int checkY = newY;
                /// Bug must move in the direction of the hero.
                /// Determine whether the movement will be vertically.
                bool isVerticalMove = (rand() % 2 == 0); // [0 .. 1]

                /// the bug makes its move.
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

                int currentHeroX, currentHeroY;
                hero.getXY(currentHeroX, currentHeroY);

                float distanceBefore = worldGetDistance(m_x, m_y, currentHeroX, currentHeroY);
                float distanceAfter = worldGetDistance(checkX, checkY, currentHeroX, currentHeroY);

                if (distanceAfter < distanceBefore)
                {
                    // allow movement
                    newX = checkX;
                    newY = checkY;
                    break;
                }
            }

            break;
        }
        case(BUG_STATE_SCARED):
        {
            break;
        }
        } // switch(m_state)


        // check the condition of the bug
        if (shouldBecomeAgressive())
        {
            m_state = BUG_STATE_AGRESSIVE;
            m_numberOfStepsInAgressiveState = 0;
        }

        /// The movement is confirmed only if there is no wall-barrier at that point.
        /// Bug is forbidden to walk through the door (any).
        int value = getMapValue(newX, newY);
        if ((value != OBJECT_WALL) && (value != OBJECT_WEAK_WALL) &&
            (value != OBJECT_DOOR) && (value != OBJECT_GREEN_DOOR) && (value != OBJECT_BLUE_DOOR))
        {
            m_x = newX;
            m_y = newY;
        }

        // if the beetle is aggressive - only then he bites
        if (m_state == BUG_STATE_AGRESSIVE)
        {
            m_numberOfStepsInAgressiveState++;

            if  (m_numberOfStepsInAgressiveState > BUG_MAX_NUMBER_OF_STEPS_IN_AGRESSIVE_STATE)
            {
                m_state = BUG_STATE_PASSIVE;
            }

            if (hero.isCollidedWith(m_x, m_y))
            {
                notifyHeroAboutDamage(BUG_DAMAGE);

                m_numberOfStepsLeftToRecover = BUG_NUMBER_OF_STEPS_TO_RECOVER_AFTER_ATTACK;
                m_state = BUG_STATE_PASSIVE;
            }
        }

        for (int i = 0; i < WORLD_MAX_NUM_PRESS; i++)
        {
            if (press[i].isCollidedWithPress(m_x, m_y))
            {
                kill();
            }
        }
    }

    if ((m_isInitialized) && (m_isKilled))
    {
        if (hero.isCollidedWith(m_x, m_y))
        {
            m_isInitialized = false;
            hero.giveChitin(1);
        }
    }
}

std::vector<OBJECT_RENDER_DESCRIPTOR> XObjectBug::render()
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

        if (m_isKilled)
        {
            if (true)
            {
                objRD.color_green = 0.7;
                objRD.str = SIGN_BUG_KILLED;

                goto descriptor_finish;
            }
        }
        else
        {
            objRD.color_green = 0.7;

            switch(m_state)
            {
            case(BUG_STATE_PASSIVE):
                objRD.str = SIGN_BUG_PASSIVE;
                break;
            case(BUG_STATE_AGRESSIVE):
                objRD.str = SIGN_BUG_AGRESSIVE;
                break;
            case(BUG_STATE_SCARED):
                objRD.str = SIGN_BUG_SCARED;
                break;
            default:
                objRD.str = "!";
            }

            goto descriptor_finish;
        }
descriptor_finish:
        vRenderDescriptors.push_back(objRD);
    }

    return vRenderDescriptors;
}

void XObjectBug::soundDamaged()
{
    Beep(100, 100);
}

///Return return true if the ghost has to become aggressive, otherwise return false.
bool XObjectBug::shouldBecomeAgressive()
{
    if (likely((m_state != BUG_STATE_AGRESSIVE) && (m_numberOfStepsLeftToRecover <= 0)))
    {
        int s;
        s = hero.getShield();
        if (s <= 0)
        {
            return false;
        }

        int currentHeroX, currentHeroY;
        hero.getXY(currentHeroX, currentHeroY);

        float distance = worldGetDistance(m_x, m_y, currentHeroX, currentHeroY);

        return (distance <= BUG_RADIUS_BECOME_AGRESSIVE);
    }
    else
    {
        // Do not allow the bug to become aggressive
        return false;
    }
}

void XObjectBug::kill()
{
    soundDamaged();
    m_isKilled = true;
}
