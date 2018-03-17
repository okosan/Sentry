#include "sound.h"

#include <windows.h>

/// Sounds of a collision with Wall
void soundCollision()
{
    // TODO: Find better sound
    Beep(300, 100);
}

/// Sound of healing
void soundHealing()
{
    // TODO: Find better sound
    Beep(100, 100);
    Beep(500, 100);
    Beep(1000, 100);
    Beep(500, 100);
}

/// Sound of an explosion on a Bomb (Blin)
void soundBlin()
{
    // TODO: Find better sound
    Beep(1000, 50);
    Beep(300, 500);
    Beep(100, 100);
    Beep(5000, 400);
}

/// Sound of the destruction of the ghost
void soundTrap()
{
    // TODO: Find better sound
    Beep(1000, 50);
    Beep(300, 500);
    Beep(100, 100);
    Beep(5000, 400);
}

void soundDinamite()
{
    // TODO: Find better sound
    Beep(100, 100);
    Beep(500, 100);
    Beep(1000, 100);
    Beep(500, 100);
}
