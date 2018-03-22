#include "xgamesettings.h"

#include <string>
#include <cstdio>

XGameSettings::XGameSettings()
{
    m_fDrawDiscoveryMap = false;    // true for debug
    m_fDrawFogOfViewMap = false;    // true for debug

    m_fDiscloseEntireMap = false;   // default: false
    m_fEnableFogOfView   = true;    // default: true

    m_discoveryRadiusX = 3;
    m_discoveryRadiusY = 3;

    m_fEnableDisplayLevelCompleteScreen = true;
    m_fEnableDisplayGameCompleteScreen = true;
}

/// return false if initialisation failed
bool XGameSettings::init(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        std::string param = std::string(argv[i]);
        if (param == "--help")
        {
            printf("Sentry v.0.0.1\n");
            printf("TODO: add more info on supported parameters here.\n");
            exit(1);
        }
        else if (param == "--debug")
        {
            m_fDiscloseEntireMap = true;
            m_fEnableFogOfView   = false;

            m_fEnableDisplayLevelCompleteScreen = false;
            m_fEnableDisplayGameCompleteScreen = false;
        }
    }

    return true;
}
