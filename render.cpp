#include "render.h"

#include "world.h"

#include "xobjectplayerhero.h"

#include "xobjectpress.h"
#include "xobjectghost.h"
#include "xobjectcloud.h"
#include "xobjectbug.h"
#include "xobjectrobik.h"
#include "xobjectcarrier.h"
#include "xobjectinterceptors.h"

#include <cassert>
#include <string>
#include <vector>
#include <cmath>

#include "utils.h"

#define PDC_WIDE
#include <curses.h>


OBJECT_RENDER_DESCRIPTOR getRenderDescriptorForCellType(int objectType);

void renderMapTextMode();
void renderMapPdc();



extern XObjectPlayerHero hero;

extern XObjectPress press[WORLD_MAX_NUM_PRESS];
extern XObjectGhost ghost[WORLD_MAX_NUM_GHOST];
extern XObjectCloud cloud[WORLD_MAX_NUM_CLOUD];
extern XObjectBug bugs[WORLD_MAX_NUM_BUGS];
extern XObjectRobik robiks[WORLD_MAX_NUM_ROBIK];
extern XObjectCarrier carrier[WORLD_MAX_NUM_CARRIER];
extern XObjectInterceptors interceptors[WORLD_MAX_NUM_INTERCEPTORS];


bool g_renderIsPdCursesInitialized = false;  /// indicates if PDC library is initialised
bool g_renderIsGraphicsInitialized = false;  /// indicates if graphics subsystem is initialised

/// Discovery map pointer needed for overlay during visualisation of World Map
const XDiscoveryMap* g_pDiscoveryMapCurrent = NULL;

/// Fog Of View map pointer needed for overlay during visualisation of World Map
const XFogOfViewMap* g_pFogOfViewMapCurrent = NULL;

/// Color palette for rendering using PDCurses library
short g_pdcColorTable[] = {
    COLOR_RED, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN,
    COLOR_RED, COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE
};

/// Initialize render subsystem
void initRender(bool enable_pdcurses, bool enable_graphics,
                const XDiscoveryMap* pDiscoveryMap,
                const XFogOfViewMap* pFogOfViewMap)
{
    g_pDiscoveryMapCurrent = pDiscoveryMap;
    g_pFogOfViewMapCurrent = pFogOfViewMap;

    if (enable_pdcurses)
    {
        initscr();
        nodelay(stdscr, TRUE);
        noecho();
        if (has_colors())
            start_color();
        attrset(A_NORMAL);

        curs_set(0);

        for (int i = 0; i < 8; i++)
            //init_pair(i, g_pdcColorTable[i], COLOR_BLACK);
            init_pair(i, g_pdcColorTable[i], COLOR_WHITE );

        g_renderIsPdCursesInitialized = true;
    }

    if (enable_graphics)
    {
        assert(false);
    }
}

/// Generate Render Descriptor for specific type of the static Cell object type
OBJECT_RENDER_DESCRIPTOR getRenderDescriptorForCellType(int staticObjectType, int staticObjX, int staticObjY)
{
    OBJECT_RENDER_DESCRIPTOR renderDescriptor;

    renderDescriptor.x = staticObjX;
    renderDescriptor.y = staticObjY;

    char sign = 0;

    switch(staticObjectType)
    {
    case(OBJECT_EMPTY):
        sign = SIGN_EMPTY;
        renderDescriptor.color_blue = 1.0;
        break;

    case(OBJECT_WALL):
        sign = SIGN_WALL;
        renderDescriptor.color_blue = 1.0;
        break;
    case(OBJECT_WEAK_WALL):
        sign = SIGN_WEAK_WALL;
        renderDescriptor.color_blue = 0.5f;
        renderDescriptor.isBold = true;
        break;

    case(OBJECT_DOOR):
        sign = SIGN_DOOR;
        renderDescriptor.color_red = 1.0f;
        break;
    case(OBJECT_GREEN_DOOR):
        sign = SIGN_DOOR;
        renderDescriptor.color_green = 1.0f;
        break;
    case(OBJECT_BLUE_DOOR):
        sign = SIGN_DOOR;
        renderDescriptor.color_blue = 1.0f;
        break;

    case(OBJECT_BLIN):
        sign = SIGN_BLIN;
        renderDescriptor.color_blue = 1.0;
        renderDescriptor.isBold = true;
        break;

    case(OBJECT_MEDKIT):
        sign = SIGN_MEDKIT;
        renderDescriptor.color_red = 1.0f;
        break;

    case(OBJECT_MUSHROOMS):
        sign = SIGN_MUSHROOMS;
        renderDescriptor.color_red = 1.0f;
        renderDescriptor.color_green = 1.0f;
        renderDescriptor.isBold = true;
        break;

    case(OBJECT_MAGIC_WELL):
        sign = SIGN_MAGIC_WELL;
        renderDescriptor.color_green = 1.0f;
        break;

    case(OBJECT_KEY):
        sign = SIGN_KEY;
        // red key
        renderDescriptor.color_red = 1.0f;
        renderDescriptor.isBlinking = true;
        renderDescriptor.isBold = true;
        break;
    case(OBJECT_GREEN_KEY):
        sign = SIGN_KEY;
        renderDescriptor.color_green = 1.0f;
        renderDescriptor.isBlinking = true;
        renderDescriptor.isBold = true;
        break;
    case(OBJECT_BLUE_KEY):
        sign = SIGN_KEY;
        // green key
        renderDescriptor.color_blue = 1.0f;
        renderDescriptor.isBlinking = true;
        renderDescriptor.isBold = true;
        break;

    case(OBJECT_FINISH):
        sign = SIGN_FINISH;
        renderDescriptor.color_green = 1.0f;
        renderDescriptor.isBold = true;
        break;

    case(OBJECT_TRAP):
        sign = SIGN_TRAP;
        renderDescriptor.color_red = 1.0f;
        break;

    case(OBJECT_DINAMITE):
        sign = SIGN_DINAMITE;
        renderDescriptor.color_red = 0.5f;
        renderDescriptor.isBold = true;
        break;

    default:
        sign = 'X';
    }

    char szBuf[2]; // "a" - 'a', 0
    szBuf[0] = sign;
    szBuf[1] = 0;

    renderDescriptor.str = std::string(szBuf);

    return renderDescriptor;
}


/// generate PDC color attribs for RGB color
int getPdcColorSettingsForRGB(float r, float g, float b, bool isBold, bool isBlinking)
{
    OBJECT_RENDER_DESCRIPTOR renderDesc;
    renderDesc.color_blue = b;
    renderDesc.color_green = g;
    renderDesc.color_red = r;
    renderDesc.isBold = isBold;
    renderDesc.isBlinking = isBlinking;
    return getPdcColorSettingForRenderDescriptor(renderDesc);
}

/// find proper color attributes to draw using PDCurses library for specific Render Descriptor
int getPdcColorSettingForRenderDescriptor(const OBJECT_RENDER_DESCRIPTOR &renderDesc)
{
    int colorSetting = 0;

    //colorSetting = COLOR_PAIR(channelRed | channelGreen | channelBlue) | A_BOLD;
    int colorMix = COLOR_BLACK;

    if (renderDesc.color_red >= 0.5)
    {
        colorMix = colorMix | COLOR_RED;
    }

    if (renderDesc.color_green >= 0.5)
    {
        colorMix = colorMix | COLOR_GREEN;
    }

    if (renderDesc.color_blue >= 0.5)
    {
        colorMix = colorMix | COLOR_BLUE;
    }

    colorSetting = COLOR_PAIR(colorMix);
    if (renderDesc.isBold)
    {
        colorSetting = colorSetting | A_BOLD;
    }

    if (renderDesc.isBlinking)
    {
        // blinking does not work in PDCurses
        //colorSetting = colorSetting | A_BLINK;
    }

    return colorSetting;
}


std::vector<OBJECT_RENDER_DESCRIPTOR> selectCurrentCoordDescriptors(
        const std::vector<OBJECT_RENDER_DESCRIPTOR> &vRenderDescriptorList,
        int curX,
        int curY)
{
    std::vector<OBJECT_RENDER_DESCRIPTOR> vRenderDescriptorListCurrentCoord;

    for (const OBJECT_RENDER_DESCRIPTOR & descriptor : vRenderDescriptorList)
    {
        if (descriptor.x == curX && descriptor.y == curY)
        {
            vRenderDescriptorListCurrentCoord.push_back(descriptor);
        }
    }

    return vRenderDescriptorListCurrentCoord;
}

/// Returns true if cell is open and not in FogOfWar
bool isCellVisible(int x, int y)
{
    bool isDiscovered = g_pDiscoveryMapCurrent->isVisibleCoord(x, y);
    bool isVisible = (isDiscovered || (x >= 40));
    return isVisible;
}

bool isCellInFog(int x, int y)
{
    bool isFogged = !(g_pFogOfViewMapCurrent->isVisibleCoord(x, y));

    if (x >= 40)
    {
        isFogged = false; // for debug, do not shadow right (testing) half of the screen
    }
    return isFogged;
}

/// render map using PDCurses (color text output) library
void renderMapPdc(const RENDER_HUD_INFOPANEL &hudInfo)
{
    int W, H;
    getWorldDimensions(W, H);

    // clear screen
    clear();

    // ---------------------------
    std::vector<OBJECT_RENDER_DESCRIPTOR> vRenderDescriptorList;

    // Static objects render descriptors
    for (int y = 0; y < H; y++)
    {
        for (int x = 0; x < W; x++)
        {
            int staticObjectType = getMapValue(x, y);
            OBJECT_RENDER_DESCRIPTOR staticObjectRenderDesc = getRenderDescriptorForCellType(staticObjectType, x, y);

            vRenderDescriptorList.push_back(staticObjectRenderDesc);
        }
    }

    /// Ask the dynamic objects to state their graphic drawing descriptors
    /// descriptor = hero, tell your own descriptors in a given frame ()
    // vRenderDescriptorList += descriptors
    {
        std::vector<OBJECT_RENDER_DESCRIPTOR> vObjectRD = hero.render();
        vRenderDescriptorList.insert(vRenderDescriptorList.end(), vObjectRD.begin(), vObjectRD.end());
    }


    /// Ask the Bugs how they want to draw themselves.
    {
        for (int i = 0; i < WORLD_MAX_NUM_BUGS; i++)
        {
            //isDynamicObjectRendered = bugs[i].render(x, y);
            std::vector<OBJECT_RENDER_DESCRIPTOR> vObjectRD = bugs[i].render();
            vRenderDescriptorList.insert(vRenderDescriptorList.end(), vObjectRD.begin(), vObjectRD.end());
        }
    }

    /// Ask the Ghosts how they want to draw themselves.
    {
        for (int i = 0; i< WORLD_MAX_NUM_GHOST; i++)
        {
            //isDynamicObjectRendered = ghost[i].render(x, y);
            std::vector<OBJECT_RENDER_DESCRIPTOR> vObjectRD = ghost[i].render();
            vRenderDescriptorList.insert(vRenderDescriptorList.end(), vObjectRD.begin(), vObjectRD.end());
        }
    }

    /// Ask the Cloud how they want to draw themselves.
    {
        for (int i = 0; i< WORLD_MAX_NUM_CLOUD; i++)
        {
            std::vector<OBJECT_RENDER_DESCRIPTOR> vObjectRD = cloud[i].render();
            vRenderDescriptorList.insert(vRenderDescriptorList.end(), vObjectRD.begin(), vObjectRD.end());
        }
    }

    /// Press draws itself above all objects.
    {
        for (int i = 0; i < WORLD_MAX_NUM_PRESS; i++)
        {
            // isDynamicObjectRendered = press[i].render(x, y);
            std::vector<OBJECT_RENDER_DESCRIPTOR> vObjectRD = press[i].render();
            vRenderDescriptorList.insert(vRenderDescriptorList.end(), vObjectRD.begin(), vObjectRD.end());
        }
    }

    /// Ask the Robiks how they want to draw themselves.
    {
        for (int i = 0; i < WORLD_MAX_NUM_ROBIK; i++)
        {
            //isDynamicObjectRendered = robiks[i].render(x, y);
            std::vector<OBJECT_RENDER_DESCRIPTOR> vObjectRD = robiks[i].render();
            vRenderDescriptorList.insert(vRenderDescriptorList.end(), vObjectRD.begin(), vObjectRD.end());
        }
    }

    /// Ask the Carriers how they want to draw themselves.
    {
        for (int i = 0; i< WORLD_MAX_NUM_CARRIER; i++)
        {
            std::vector<OBJECT_RENDER_DESCRIPTOR> vObjectRD = carrier[i].render();
            vRenderDescriptorList.insert(vRenderDescriptorList.end(), vObjectRD.begin(), vObjectRD.end());
        }
    }

    /// Ask the Interceptors how they want to draw themselves.
    {
        for (int i = 0; i< WORLD_MAX_NUM_INTERCEPTORS; i++)
        {
            std::vector<OBJECT_RENDER_DESCRIPTOR> vObjectRD = interceptors[i].render();
            vRenderDescriptorList.insert(vRenderDescriptorList.end(), vObjectRD.begin(), vObjectRD.end());
        }
    }
    /// Offset of the map coordinate system for drawing.
    int drawOriginX = 0;
    int drawOriginY = 2;

    /// Sum up and draw everything.
    for (int y = 0; y < H; y++)
    {
        //for (int x = 0; x < W; x++)
        for (int x = 0; x < 80; x++)
        {
            /// 1)Find all descriptores that claim the x, y coordinates.
            std::vector<OBJECT_RENDER_DESCRIPTOR> vRenderDescriptorListCurrentCoord = selectCurrentCoordDescriptors(vRenderDescriptorList, x, y);

            if (!isCellVisible(x, y))
            {
                continue;
            }

            size_t numRenderDesc = vRenderDescriptorListCurrentCoord.size();

            /// 2) for each drawing coach at this point determine the order - where is the foreground who is in the background.
            /// 3) who is in the background - the first one is drawn.

            /// 4) who on the front - is drawn later from the top.
            if (numRenderDesc != 0)
            {
                size_t lastIndex = numRenderDesc - 1;
                OBJECT_RENDER_DESCRIPTOR &renderDesc = vRenderDescriptorListCurrentCoord[lastIndex];

                if (isCellInFog(x, y))
                {
                    renderDesc.color_blue = 0.5;
                    renderDesc.color_green = 0.5;
                    renderDesc.color_red = 0.0;
                    //renderDesc.color_blue = 0.5;
                    //renderDesc.color_green = 0.5;
                    //renderDesc.color_red = 0.5;
                }

                // Render last object
                int colorSetting = getPdcColorSettingForRenderDescriptor(renderDesc);
                attrset(colorSetting);

                mvaddstr(drawOriginY + y, drawOriginX + x, renderDesc.str.c_str());
            }
            else
            {
                // render emptyness!!!
                // cell will remain black
            }
        }
    }

    // ------------------------------------------------
    int hudDrawOriginX = 0;
    int hudDrawOriginY = 20;

    bool drawUnderMap = true;
    if (drawUnderMap)
    {
        hudDrawOriginY = drawOriginY + H + 2;
    }

    /// Displays the Health Indicator on the panel
    {
        //attrset(colorSetting);
        for (int i = 0; i < W; i++)
        {
            mvaddstr(hudDrawOriginY, hudDrawOriginX + i, " ");
        }

        float ratio = (float)hudInfo.heroShield / 100.0f;

        float stripLengthFloat = ratio * W;
        if (ratio > 1.0)
        {
            stripLengthFloat = W;
        }

        int stripLenInt = (int)(roundf(stripLengthFloat));

        OBJECT_RENDER_DESCRIPTOR renderDescDummy;

        if (ratio > 1.0)
        {
            renderDescDummy.color_blue = 1.0f;
            renderDescDummy.isBold = true;
        }

        if (ratio == 1.0)
        {
            renderDescDummy.color_green = 1.0f;
            renderDescDummy.isBold = true;
        }

        if (ratio < 1.0 && ratio >= 0.5)
        {
            renderDescDummy.color_green = 1.0f;
        }

        if (ratio >= 0.25 && ratio < 0.5)
        {
            renderDescDummy.color_green = 1.0f;
            renderDescDummy.color_red = 1.0f;
            renderDescDummy.isBold = true;
        }

        if (ratio < 0.25)
        {
            renderDescDummy.color_red = 1.0f;
            renderDescDummy.isBold = true;
        }

        int colorSetting = getPdcColorSettingForRenderDescriptor(renderDescDummy);
        attrset(colorSetting);

        for (int i = 0; i < stripLenInt; i++)
        {
            mvaddstr(hudDrawOriginY, hudDrawOriginX + i, "*");
        }

        // displays the level name in the top center
        {
            std::string l = getCurrentLevelName();
            int centerX = 80 / 2;
            int textX = centerX - (l.length() / 2);
            mvaddstr(0, textX, l.c_str());
        }

        // to see the number in the picture
        {
            int nomerKadra = hudInfo.frameNumber;

            std::string l = ssprintf("Frame: %d", nomerKadra);
            int centerX = 80 / 2;
            int textX = centerX - (l.length() / 2);
            int textY = 1;

            int colorSetting = getPdcColorSettingsForRGB(0.0, 0.0, 1.0);
            attrset(colorSetting);
            mvaddstr(textY, textX, l.c_str());
        }
    }

    {
        // render Hero shield bar
        char szBuf[255];
        sprintf(szBuf, "Shield   : %d", hudInfo.heroShield);

        OBJECT_RENDER_DESCRIPTOR renderDescDummy;
        int colorSetting = getPdcColorSettingForRenderDescriptor(renderDescDummy);
        attrset(colorSetting);

        mvaddstr(hudDrawOriginY + 1, hudDrawOriginX, szBuf);
    }

    // Draw Red Key indicator
    if (hudInfo.heroHasKey)
    {
        OBJECT_RENDER_DESCRIPTOR renderDescDummy;
        renderDescDummy.color_red = 1.0f;
        renderDescDummy.isBold = true;
        int colorSetting = getPdcColorSettingForRenderDescriptor(renderDescDummy);
        attrset(colorSetting);
        mvaddstr(hudDrawOriginY + 1, hudDrawOriginX + W - 1, "K");
    }

    // Draw Green Key indicator
    if (hudInfo.heroHasGreenKey)
    {
        OBJECT_RENDER_DESCRIPTOR renderDescDummy;
        renderDescDummy.color_green = 1.0f;
        renderDescDummy.isBold = true;
        int colorSetting = getPdcColorSettingForRenderDescriptor(renderDescDummy);
        attrset(colorSetting);
        mvaddstr(hudDrawOriginY + 1, hudDrawOriginX + W - 2, "Q");
    }

    // Draw Blue Key indicator
    if (hudInfo.heroHasBlueKey)
    {
        OBJECT_RENDER_DESCRIPTOR renderDescDummy;
        renderDescDummy.color_blue = 1.0f;
        renderDescDummy.isBold = true;
        int colorSetting = getPdcColorSettingForRenderDescriptor(renderDescDummy);
        attrset(colorSetting);
        mvaddstr(hudDrawOriginY + 1, hudDrawOriginX + W - 3, "T");
    }

    /// Displays the number of traps, chitin, dynamite, invulnerability in the bar of the row.
    {
        char szBuf[255];
        sprintf(szBuf, "Traps    : %d", hudInfo.heroNumberOfTraps);

        OBJECT_RENDER_DESCRIPTOR renderDescDummy;
        int colorSetting = getPdcColorSettingForRenderDescriptor(renderDescDummy);
        attrset(colorSetting);
        mvaddstr(hudDrawOriginY+2, hudDrawOriginX, szBuf);
    }

    {
        char szBuf[255];
        sprintf(szBuf, "Chitin   : %d", hudInfo.heroNumberOfChitin);

        OBJECT_RENDER_DESCRIPTOR renderDescDummy;
        int colorSetting = getPdcColorSettingForRenderDescriptor(renderDescDummy);
        attrset(colorSetting);
        mvaddstr(hudDrawOriginY+3, hudDrawOriginX, szBuf);
    }

    {
        char szBuf[255];
        sprintf(szBuf, "Dinamite : %d", hudInfo.heroNumberOfDinamite);

        OBJECT_RENDER_DESCRIPTOR renderDescDummy;
        int colorSetting = getPdcColorSettingForRenderDescriptor(renderDescDummy);
        attrset(colorSetting);
        mvaddstr(hudDrawOriginY+4, hudDrawOriginX, szBuf);
    }

    /// ACTION_TOGGLE_INVINCIBLE - Invincibility
    {
        char szBuf[255];
        sprintf(szBuf, "GodMode  : %d", hudInfo.heroIsInvincible);

        OBJECT_RENDER_DESCRIPTOR renderDescDummy;
        int colorSetting = getPdcColorSettingForRenderDescriptor(renderDescDummy);
        attrset(colorSetting);
        mvaddstr(hudDrawOriginY+5, hudDrawOriginX, szBuf);
    }

    refresh();
}


// TODO: fix broken funciton
void printCongratulationMessage()
{
    clear();
    {
        std::string l = "My gratitude to you for winning the game!";
        int centerX = 80 / 2;
        int textX = centerX - (l.length() / 2);
        int textY = 25/2;

        int colorSetting = getPdcColorSettingsForRGB(0.0, 1.0, 0.0);
        attrset(colorSetting);
        mvaddstr(textY, textX, l.c_str());
    }

    refresh();
}

// TODO: Stopper - need to be implemented with PDC
void printLevelCompleteStats(std::string prevLevelName, std::string nextLevelName, int score)
{

#if 0
    printf("==========================================================================");
    printf("                       Riven <%s> zakincheno! \n", prevLevelName.c_str());
    printf("                       Kilkist Ochok: %d  \n", score);
    printf("==========================================================================");
    printf("*** Press key to continue to next level <%s> ... \n", nextLevelName.c_str());
#endif
    clear();
    {
        std::string l = prevLevelName + " level complete!";
        int centerX = 80 / 2;
        int textX = centerX - (l.length() / 2);
        int textY = 25/2;

        int colorSetting = getPdcColorSettingsForRGB(0.0, 1.0, 0.0);
        attrset(colorSetting);
        mvaddstr(textY, textX, l.c_str());
    }
    refresh();
}

void drawScene(const RENDER_HUD_INFOPANEL &hudInfo)
{
    if (!g_renderIsPdCursesInitialized && !g_renderIsGraphicsInitialized)
    {
        // Plain text mode (without PDC), not supported anymore
        assert(false);
    }
    else
    {
        // draw map using PDC library
        renderMapPdc(hudInfo);
    }
}
