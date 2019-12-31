#ifndef XGAMESETTINGS_H
#define XGAMESETTINGS_H


class XGameSettings
{
public:
    XGameSettings();

    bool init(int argc, char *argv[]);

    bool isEnableDrawDiscoveryMap() const
    {
        return m_fDrawDiscoveryMap;
    }

    bool isEnableDrawFogOfViewMap() const
    {
        return m_fDrawFogOfViewMap;
    }

    bool isEnableDiscloseEntireMap() const
    {
        return m_fDiscloseEntireMap;
    }

    bool isEnableFogOfView() const
    {
        return m_fEnableFogOfView;
    }

    void getDiscoveryRadius(int &rX, int &rY) const
    {
        rX = m_discoveryRadiusX;
        rY = m_discoveryRadiusY;
    }

    bool isEnableDisplayLevelCompleteScreen() const
    {
        return m_fEnableDisplayLevelCompleteScreen;
    }

    bool isEnableDisplayGameCompleteScreen() const
    {
        return m_fEnableDisplayGameCompleteScreen;
    }

    bool isGenerateMap() const
    {
        return m_fGenerateMap;
    }

    unsigned getNumGeneratedMaps() const
    {
        return m_numGenerateLevelCount;
    }

private:
    bool m_fDrawDiscoveryMap;   /// Show Map of Intelligence
    bool m_fDrawFogOfViewMap;   /// Display Map of the Fog

    bool m_fDiscloseEntireMap;  /// open the whole map
    bool m_fEnableFogOfView;    /// Enable Fog

    int m_discoveryRadiusX;
    int m_discoveryRadiusY;

    bool m_fEnableDisplayLevelCompleteScreen;
    bool m_fEnableDisplayGameCompleteScreen;

    bool m_fGenerateMap;
    unsigned m_numGenerateLevelCount;
};

#endif // XGAMESETTINGS_H
