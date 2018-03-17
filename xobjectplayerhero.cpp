#include "xobjectplayerhero.h"

#include "sound.h"
#include "input.h"

#include "world.h"

#include <cstdio>
#include <cassert>

const int HERO_INITIAL_NUMBER_OF_TRAPS = 3;
const int HERO_INITIAL_NUMBER_OF_CHITIN = 2;
const int HERO_INITIAL_NUMBER_OF_DINAMITE = 3;


XObjectPlayerHero::XObjectPlayerHero()
{
    m_pGameSettings = NULL;
}

void XObjectPlayerHero::init(const XGameSettings *pGameSettings, int startX, int startY)
{
    m_pGameSettings = pGameSettings;
    m_x = startX;
    m_y = startY;
    m_isInitialized = true;

    m_shield = 100;

    m_numberOfTraps = HERO_INITIAL_NUMBER_OF_TRAPS;
    m_numberOfDinamite = HERO_INITIAL_NUMBER_OF_DINAMITE;

    m_hasKey = false;
    m_hasGreenKey = false;
    m_hasBlueKey = false;

    m_numberOfChitin = HERO_INITIAL_NUMBER_OF_CHITIN;

    m_isInvincible = false;

    m_discoveryMap.init(pGameSettings);
    m_fogOfViewMap.init(pGameSettings);
}

void XObjectPlayerHero::reinitForNextLevel(int startX, int startY)
{
    assert(m_pGameSettings != NULL);
    m_x = startX;
    m_y = startY;

    // m_shield - not modfied

    if (m_numberOfTraps > 3)
    {
        m_numberOfTraps = 3;
    }

    if (m_numberOfChitin > 5)
    {
        m_numberOfChitin = 5;
    }
    if (m_numberOfDinamite > 5)
    {
        m_numberOfDinamite = 5;
    }

    m_hasKey = false;
    m_hasGreenKey = false;
    m_hasBlueKey = false;

    m_discoveryMap.init(m_pGameSettings);
}

void XObjectPlayerHero::notifyAboutDamage(int damagePoints)
{
    if (m_isInvincible)
        return;

    m_shield -= damagePoints;
    if (m_shield < 0)
    {
        m_shield = 0;
    }
}

void XObjectPlayerHero::notifyAboutHealing(int healPoints)
{
    m_shield += healPoints;
    if (m_shield > 100)
    {
        m_shield = 100;
    }
    soundHealing();
}

bool isUnderSolidCover(int epicenter_x, int epicenter_y, int test_x, int test_y)
{
    bool isDiagonal = false;

    int dx = test_x - epicenter_x;
    int dy = test_y - epicenter_y;

    int distX = abs(dx);
    int distY = abs(dy);

    if (distX == 1 && distY == 1)
    {
        isDiagonal = true;
    }

    if (isDiagonal)
    {
        int testA_x = epicenter_x + dx;
        int testA_y = epicenter_y;

        int testB_x = epicenter_x;
        int testB_y = epicenter_y + dy;

        int valueA = getMapValue(testA_x, testA_y);
        int valueB = getMapValue(testB_x, testB_y);

        if ((valueA == OBJECT_WALL) && (valueB == OBJECT_WALL))
        {
            return true;
        }
    }

    return false;
}

// If the character "kills" - he turns into a ghost, and we must take away
// to the starting point so that it "resumes" (the keys remain there)

//0 - did not react (not my team)
// 1 - responded
// 2 - responded with a mistake
bool XObjectPlayerHero::react(int actionCode)
{
    bool f_reacted = false;

    // copy coordinates
    int copyCoordsX = m_x;
    int copyCoordsY = m_y;

    // The checkbox determines whether we are trying to make any movement
    // (if so, then check if we are dealing with walls)
    bool triesToMove = false;


    switch(actionCode)
    {
    /// processing of the "movement left" action
    case(ACTION_LEFT):
        copyCoordsX --;
        triesToMove = true;

        f_reacted = true;
        break;

    /// proceccing of the "movement right" action
    case(ACTION_RIGHT):
        copyCoordsX ++;
        triesToMove = true;

        f_reacted = true;
        break;

    /// processing of the "movement up" action
    case(ACTION_UP):
        copyCoordsY --;
        triesToMove = true;

        f_reacted = true;
        break;

    /// processing of the "movement down " action
    case(ACTION_DOWN):
        copyCoordsY ++;
        triesToMove = true;

        f_reacted = true;
        break;

    /// processing of the "throwing out the Key" action (red Key)
    case(ACTION_DROP_KEY):
    {
        int value = getMapValue(m_x, m_y);
        if ((m_hasKey == true) && (m_shield > 0) && (value == OBJECT_EMPTY))
        {
            m_hasKey = false;
            setMapValue(m_x, m_y, OBJECT_KEY);
        }

        f_reacted = true;
        break;
    }

    /// processing of the action of "throwing aut the Trap"
    case(ACTION_DROP_TRAP):
    {
        int value = getMapValue(m_x, m_y);
        if ((m_numberOfTraps > 0) && (m_shield > 0) && (value == OBJECT_EMPTY))
        {
            setMapValue(m_x, m_y, OBJECT_TRAP);
            m_numberOfTraps --;
        }

        f_reacted = true;
        break;
    }

    /// processing of the action of "throwing aut the Dinamite"
    case(ACTION_DROP_DINAMITE):
    {
        int value = getMapValue(m_x, m_y);
        if ((m_numberOfDinamite > 0) && (m_shield > 0) && (value == OBJECT_EMPTY))
        {
            setMapValue(m_x, m_y, OBJECT_DINAMITE);
            m_numberOfDinamite --;
        }

        f_reacted = true;
        break;
    }

    /// processing of the action of detonation of Dynamite and surrounding objects.
    case(ACTION_EXPLODE):
    {
        if (m_shield > 0)
        {
            int dinx;
            int diny;

            const int explode_diameter = 3;
            const int explode_radius =  explode_diameter / 2;

            bool f_foundDinamite = worldGetFirstObjectLocation(dinx, diny, OBJECT_DINAMITE);

            if (f_foundDinamite == true)
            {
                for (int y = diny - explode_radius; y <= diny + explode_radius; y++)
                {
                    for (int x = dinx - explode_radius; x <= dinx + explode_radius; x++)
                    {
                        int value = getMapValue(x, y);

                        if ( (value != OBJECT_WALL) &&
                             (value != OBJECT_DOOR) && (value != OBJECT_GREEN_DOOR) && (value != OBJECT_BLUE_DOOR) &&
                             (!isUnderSolidCover(dinx, diny, x, y)) )
                        {
                            setMapValue(x, y, OBJECT_EMPTY);
                            // faktichno vidbuvsya vibuh v (x, y)
                            worldNotifyExplosionInArea(x, y);
                        }
                    }
                }

                soundDinamite();
            }
        }

        f_reacted = true;
        break;
    }

    /// processing of the action "Do nothing"
    case(ACTION_DO_NOTHING):
        f_reacted = true;
        break;

    /// processing action that instantly translates the Hero into the state of the Ghost
    case(ACTION_REMOVE_SHIELD):
        m_shield = 0;
        f_reacted = true;
        break;

    /// processing action that heals 10 Shield units
    case(ACTION_HEAL_SHIELD):
    {
        notifyAboutHealing(10);
        f_reacted = true;
        break;
    }

    /// processing action "Invincible"
    case(ACTION_TOGGLE_INVINCIBLE):
        if (m_isInvincible == true)
        {
            m_isInvincible = false;
        }
        else
        {
            m_isInvincible = true;
            if (m_shield > 0)
            {
                m_shield = 100;
            }
        }

        f_reacted = true;
        break;

    /// processing action "Go to another level"
    case(ACTION_FORCE_LEVEL_COMPLETE):
    {
        worldNotifyLevelComplete();
        break;
    }

    // unknown action
    default:
        f_reacted = false;
        break;
    }

    /// collision check with the World and with static objects while in motion
    bool heroMoved = false;
    if (triesToMove)
    {
        int value = getMapValue(copyCoordsX, copyCoordsY);
        if ((value == OBJECT_WALL) || (value == OBJECT_WEAK_WALL))
        {
            // clung to the wall
            soundCollision();
        }
        else
        {
            bool hero_can_move = true;

            if ((value == OBJECT_BLIN) && (m_shield > 0))
            {
                notifyAboutDamage(BLIN_DAMAGE);  // KABOOM! bomb just exploded

                soundBlin();

                // to destroy the Blin
                setMapValue(copyCoordsX, copyCoordsY, OBJECT_EMPTY);
            }

            if (m_shield < 100)
            {
                if (value == OBJECT_MEDKIT)
                {
                    notifyAboutHealing(MEDKIT_HEALS); // First-aid kit is used

                    // destroy the first aid kit
                    setMapValue(copyCoordsX, copyCoordsY, OBJECT_EMPTY);
                }
            }

            if (m_shield > 0)
            {
                if (value == OBJECT_KEY)
                {
                    m_hasKey = true;

                    // remove the Red Key from the card
                    setMapValue(copyCoordsX, copyCoordsY, OBJECT_EMPTY);
                }

                if (value == OBJECT_GREEN_KEY)
                {
                    m_hasGreenKey = true;

                    // remove the Green Key from the card
                    setMapValue(copyCoordsX, copyCoordsY, OBJECT_EMPTY);
                }

                if (value == OBJECT_BLUE_KEY)
                {
                    m_hasBlueKey = true;

                    // remove the Blue Key from the card
                    setMapValue(copyCoordsX, copyCoordsY, OBJECT_EMPTY);
                }

                if (value == OBJECT_FINISH)
                {
                    // Finish - Tell the world that we are ready to go to the next level
                    worldNotifyLevelComplete();
                }
            }

            if (value == OBJECT_DOOR)
            {
                if (m_hasKey == false)
                {
                    hero_can_move = false;
                }
            }

            if (value == OBJECT_GREEN_DOOR)
            {
                if (m_hasGreenKey == false)
                {
                    hero_can_move = false;
                }
            }

            if (value == OBJECT_BLUE_DOOR)
            {
                if (m_hasBlueKey == false)
                {
                    hero_can_move = false;
                }
            }

            if (m_shield < 100)
            {
                if (value == OBJECT_MUSHROOMS)
                {
                    notifyAboutHealing(MUSHROOMS_HEALS); // First-aid kit is used

                    // destroy the first aid kit
                    setMapValue(copyCoordsX, copyCoordsY, OBJECT_EMPTY);
                }
            }

            // confirm move to the new player's coordinates
            if (hero_can_move)
            {
                if (m_x != copyCoordsX || m_y != copyCoordsY)
                {
                    heroMoved = true;
                }

                m_x = copyCoordsX;
                m_y = copyCoordsY;
            }
        }
    }

    // Do not heal if you run quickly
    if ( (!triesToMove) ||
         ( (triesToMove == true) && (!heroMoved) ) )
    {
        int value = getMapValue(m_x, m_y);

        if (m_shield < 100)
        {
            if (value == OBJECT_MAGIC_WELL)
            {
                notifyAboutHealing(MAGIC_WELL_HEALS); // +1
            }
        }
    }

    m_discoveryMap.update(m_x, m_y);
    m_fogOfViewMap.update(m_x, m_y);

    return f_reacted;
}


std::vector<OBJECT_RENDER_DESCRIPTOR> XObjectPlayerHero::render()
{
    std::vector<OBJECT_RENDER_DESCRIPTOR> vDescriptors;

    OBJECT_RENDER_DESCRIPTOR heroDescriptor;
    heroDescriptor.x = m_x;
    heroDescriptor.y = m_y;

    if (m_shield <= 0)
    {
        //printf("#");
        heroDescriptor.color_red = 0.7;
        heroDescriptor.str = "#";
    }
    else
    {
        //printf("@");
        heroDescriptor.color_red = 0.7;
        heroDescriptor.str = "@";
        heroDescriptor.isBold = true;
    }

    vDescriptors.push_back(heroDescriptor);

    // Ask DiscoveryMap how it wants to be drawn on screen.
    std::vector<OBJECT_RENDER_DESCRIPTOR> vDescriptorsDiscoveryMap;
    vDescriptorsDiscoveryMap = m_discoveryMap.render();
    // Add discoveryMap's descriptors to hero descriptors
    vDescriptors.insert(vDescriptors.end(), vDescriptorsDiscoveryMap.begin(), vDescriptorsDiscoveryMap.end());


    // Ask FogOfViewMap how it wants to be drawn on screen.
    std::vector<OBJECT_RENDER_DESCRIPTOR> vDescriptorsFogOfViewMap;
    vDescriptorsFogOfViewMap = m_fogOfViewMap.render();
    // Add discoveryMap's descriptors to hero descriptors
    vDescriptors.insert(vDescriptors.end(), vDescriptorsFogOfViewMap.begin(), vDescriptorsFogOfViewMap.end());

    return vDescriptors;
}

bool XObjectPlayerHero::isCollidedWith(int objectX, int objectY)
{
    if (objectX == m_x && objectY == m_y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int XObjectPlayerHero::tradeChitin()
{
    int hadChitin = m_numberOfChitin;

    m_numberOfChitin = 0;
    m_numberOfTraps += hadChitin;

    return hadChitin;
}

void XObjectPlayerHero::giveChitin(int gift)
{
    m_numberOfChitin += gift;
}
