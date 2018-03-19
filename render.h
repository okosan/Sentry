#ifndef RENDER_H
#define RENDER_H

#include <string>

const char SIGN_EMPTY  = 250;      // dot in a middle
const char SIGN_WALL   = 219;      // the icon of the Wall
const char SIGN_BLIN   = 253;      // a Bomb
const char SIGN_MEDKIT = 'A';      // a Medkit
const char SIGN_MAGIC_WELL = 'H';  // the Fountain of Health
const char SIGN_KEY    = 189;      // the icon of the Key
const char SIGN_DOOR   = 176;      // Doors made of grid
const char SIGN_FINISH = 'F';      // Finish sign
const char SIGN_TRAP   = '*';      // Ether Trap displayed as a small star *
const char SIGN_DINAMITE   = '=';  // the icon of the Dinamite
const char SIGN_WEAK_WALL  = 'Z';  // the icon of the "Week Wall"
const char SIGN_MUSHROOMS = 'M';   // the icon of the mushroom

class XDiscoveryMap;
class XFogOfViewMap;

struct OBJECT_RENDER_DESCRIPTOR
{
    std::string str;
    float color_red;   // 0.0 - 0.5 - 1.0
    float color_blue;
    float color_green;

    bool isBold;
    bool isBlinking;

    int x;
    int y;

    OBJECT_RENDER_DESCRIPTOR(): str(std::string("X"))
    {
        color_red = 0;
        color_blue = 0;
        color_green = 0;

        isBold = false;
        isBlinking = false;
    }
};

struct RENDER_HUD_INFOPANEL
{
    int frameNumber;

    int heroShield;
    int heroNumberOfTraps;
    int heroNumberOfChitin;
    int heroNumberOfDinamite;

    bool heroHasKey;
    bool heroHasGreenKey;
    bool heroHasBlueKey;
    bool heroIsInvincible;
};


void initRender(bool enable_pdcurses, bool enable_graphics,
                const XDiscoveryMap* pDiscoveryMap,
                const XFogOfViewMap* pFogOfViewMap);
void unloadRender();
void drawScene(const RENDER_HUD_INFOPANEL &hudInfo);

void printCongratulationMessage();
void printLevelCompleteStats(std::string prevLevelName, std::string nextLevelName, int score);

int getPdcColorSettingsForRGB(float r, float g, float b, bool isBold = false, bool isBlinking = false);
int getPdcColorSettingForRenderDescriptor(const OBJECT_RENDER_DESCRIPTOR &renderDesc);


#endif // RENDER_H
