#include "input.h"

int lastAction;


#include <conio.h>


const int KEY_UP     = 72;   // Arrow key up
const int KEY_DOWN   = 80;   // Arrow key down
const int KEY_LEFT   = 75;   // Arrow key left
const int KEY_RIGHT  = 77;   // Arrow key right
const int KEY_F12    = 134;  // F12 key - instantly terminate game

void initInput()
{
    lastAction = ACTION_DO_NOTHING;
}


void readKeyboardInput()
{
    /*
    http://www.asciitable.com/
    a = (key1 = 97,  key2 = 0)
    w = (key1 = 119, key2 = 0)
    ^ - (key1 = -32, key2 = 122)
    */
    int key1 = 0;
    int key2 = 0;

    key1 = getch();
    if (key1 == 224 || key1 == 0)
    {
        key2 = getch();  // not just a symbol, but an arrow!
    }

    /// Perform mapping of keys on actions - keyboard shortcuts
    lastAction = ACTION_DO_NOTHING;

    if (key2 != 0)
    {
        switch(key2)
        {
        case(KEY_UP):
            lastAction = ACTION_UP;
            break;
        case(KEY_DOWN):
            lastAction = ACTION_DOWN;
            break;
        case(KEY_LEFT):
            lastAction = ACTION_LEFT;
            break;
        case(KEY_RIGHT):
            lastAction = ACTION_RIGHT;
            break;
        case(KEY_F12):
            lastAction = ACTION_TERMINATE_GAME;
            break;
        }
    }
    else
    {
        /// the action of throwing the Red key
        if (key1 == 'z' || key1 == 'Z')
        {
            lastAction = ACTION_DROP_KEY;
        }

        /// the action of throwing the Trap
        if (key1 == 'x' || key1 == 'X')
        {
            lastAction = ACTION_DROP_TRAP;
        }

        /// the action of throwing the Dinamite
        if (key1 == 'd' || key1 == 'D')
        {
            lastAction = ACTION_DROP_DINAMITE;
        }

        /// the action of explode the Dinamite
        if (key1 == 'e' || key1 == 'E')
        {
            lastAction = ACTION_EXPLODE;
        }

        /// action "Trade (Robik)"
        if (key1 == 'c' || key1 == 'C')
        {
            lastAction = ACTION_TRADE;
        }

        /// the action of switching the invulnerability mode
        if ((key1 == 'i') || (key1 == 'I'))
        {
            lastAction = ACTION_TOGGLE_INVINCIBLE;
        }

        /// an instantaneous transition to the next level
        if ((key1 == 'f') || (key1 == 'F'))
        {
            lastAction = ACTION_FORCE_LEVEL_COMPLETE;
        }

        /// the action of the instant transfer of the Hero to the state of the Ghost
        if ((key1 == 'r') || (key1 == 'R'))
        {
            lastAction = ACTION_REMOVE_SHIELD;
        }

        /// the healing action adds 10 Shield units
        if ((key1 == 'a') || (key1 == 'A'))
        {
            lastAction = ACTION_HEAL_SHIELD;
        }
    }
}

void readInput()
{
    if (1)
    {
        readKeyboardInput();
    }
    else
    {
        //readAI_input();
    }
}

int inputGetLastAction()
{
    return lastAction;
}
