#include "sound.h"

#ifdef __linux__
    // TODO: linux-specific beep library
#elif _WIN32
    #include <windows.h>
#else
    // Other OS
#endif


void simple_beep(unsigned _Frequency, unsigned _Duration)
{
#ifdef __linux__
    // TODO: linux-specific beep function
#elif _WIN32
    Beep(_Frequency, _Duration);
#else
    // Other OS
#endif
}

/// Sounds of a collision with Wall
void soundCollision()
{
    // TODO: Find better sound
    simple_beep(300, 100);
}

/// Sound of healing
void soundHealing()
{
    // TODO: Find better sound
    simple_beep(100, 100);
    simple_beep(500, 100);
    simple_beep(1000, 100);
    simple_beep(500, 100);
}

/// Sound of an explosion on a Bomb (Blin)
void soundBlin()
{
    // TODO: Find better sound
    simple_beep(1000, 50);
    simple_beep(300, 200);
    simple_beep(100, 100);
    simple_beep(5000, 150);
}

/// Sound of the destruction of the ghost
void soundTrap()
{
    // TODO: Find better sound
    simple_beep(1000, 50);
    simple_beep(300, 250);
    simple_beep(100, 100);
    simple_beep(5000, 150);
}

void soundDinamite()
{
    // TODO: Find better sound
    simple_beep(100, 100);
    simple_beep(500, 100);
    simple_beep(1000, 100);
    simple_beep(500, 100);
}

void soundRobikTrade()
{
    simple_beep(7000, 150);
}

void soundBugDamage()
{
    simple_beep(100, 100);
}

