#include "xgamesettings.h"

XGameSettings::XGameSettings()
{
    m_fDrawDiscoveryMap = false;
    m_fDrawFogOfViewMap = true;

    m_fDiscloseEntireMap = false;   // default: false
    m_fEnableFogOfView   = true;   // default: true

    m_discoveryRadiusX = 3;
    m_discoveryRadiusY = 3;

    m_fEnableDisplayLevelCompleteScreen = true;
    m_fEnableDisplayGameCompleteScreen = true;
}

/// return false if initialisation failed
bool XGameSettings::init()
{
    return true;
}
