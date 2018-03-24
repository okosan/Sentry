/*
 * Application name: Sentry (codename Bloha)
 * Authors: Oleh Kosan, Tetiana Popovich
 * Copyright (c) 2016-18
 * License: public domain or MIT or GPL
 */
#include <QCoreApplication>

#include "xgamesettings.h"
#include "sound.h"
#include "input.h"
#include "render.h"
#include "world.h"

#include "xobjectplayerhero.h"
#include "xobjectrobik.h"

#include "stdlib.h"

#include <locale.h>


extern XObjectPlayerHero hero;

extern XObjectRobik robiks[WORLD_MAX_NUM_ROBIK];

extern bool isGameFinished;
extern bool m_displayGameStatistics;

extern std::string m_currentLevelName;


int g_frameNumber = -1;

XGameSettings g_gameSettings;


void processActions()
{
    bool reacted = false;

    int inputAction = inputGetLastAction();

    reacted = hero.react(inputAction);

    if (reacted == false)
    {
        for (int i = 0; i < WORLD_MAX_NUM_ROBIK; i++)
        {
            reacted = robiks[i].react(inputAction);
            if (reacted)
            {
                break;
            }
        }
    }

    // Ask all other objects
    // ...

    if (!reacted)
    {
        //printf("Warning: Unable to process action code %d\n", inputAction);
    }
}

void updateHudInfoPanelData(RENDER_HUD_INFOPANEL &hudInfo)
{
    hudInfo.frameNumber = g_frameNumber;

    hudInfo.heroShield = hero.getShield();
    hudInfo.heroHasKey = hero.getHasKey();
    hudInfo.heroHasGreenKey = hero.getHasGreenKey();
    hudInfo.heroHasBlueKey = hero.getHasBlueKey();
    hudInfo.heroNumberOfChitin = hero.getNumberOfChitin();
    hudInfo.heroNumberOfTraps = hero.getNumberOfTraps();
    hudInfo.heroNumberOfDinamite = hero.getNumberOfDinamite();
    hudInfo.heroIsInvincible = hero.getIsInvincible();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    g_gameSettings.init(argc, argv);

    //setlocale(LC_ALL, "");

    g_frameNumber = 0;
    bool isInitialized = initWorld(&g_gameSettings);
    if (!isInitialized)
    {
        return EXIT_FAILURE;
    }
    initInput();

    bool enable_pdcurses = true;   /// Adds a color text mode.

    bool enable_graphics = false;  /// Three-dimensional graphics are not supported yet.

    const XDiscoveryMap* pDiscoveryMap = hero.getDiscoveryMap();
    const XFogOfViewMap* pFogOfViewMap = hero.getFogOfViewMap();
    initRender(enable_pdcurses, enable_graphics, pDiscoveryMap, pFogOfViewMap);


    RENDER_HUD_INFOPANEL hudInfo;

    std::string prevLevel = m_currentLevelName;

    isGameFinished = false;

    // First level starts
    hero.update();  // called to update fog of view and discovery map
    updateHudInfoPanelData(hudInfo);
    drawScene(hudInfo);

    while(1)
    {
        if (m_displayGameStatistics && g_gameSettings.isEnableDisplayLevelCompleteScreen())
        {
            if (isGameFinished == true)
            {
                m_currentLevelName = "KOSMOS!";
            }

            printLevelCompleteStats(prevLevel, m_currentLevelName, 99999);
            prevLevel = m_currentLevelName;
            readInput();  // TODO: modify whith simpler funciton that waits for any key pressed
            m_displayGameStatistics = false;

            // New (non-first) level starts
            // TODO: unify with first level
            hero.update();  // called to update fog of view and discovery map
            updateHudInfoPanelData(hudInfo);
            drawScene(hudInfo);
        }
        else
        {
            if (isGameFinished)
            {
                if (g_gameSettings.isEnableDisplayGameCompleteScreen())
                {
                    printCongratulationMessage();
                    readInput();
                }
                break;
            }
            else
            {
                // We continue the logic
                g_frameNumber++;

                readInput();

                if (inputGetLastAction() == ACTION_TERMINATE_GAME)
                {
                    break;
                }

                processActions();

                bool stepResult = worldStep();
                if (!stepResult)
                {
                    break;
                }

                // TODO: make characters more fast/vivid
                updateHudInfoPanelData(hudInfo);
                drawScene(hudInfo);
            }
        }
    }

    unloadRender();
    //return a.exec();
}
