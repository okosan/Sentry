#ifndef XOBJECTPLAYERHERO_H
#define XOBJECTPLAYERHERO_H

#include "xdiscoverymap.h"
#include "xfogofviewmap.h"
#include "render.h"
#include "xdynamicobject.h"

#include <vector>

class XObjectPlayerHero : public XDynamicObject
{
private:
    int  m_shield;

    int  m_numberOfTraps;
    int  m_numberOfChitin;
    int  m_numberOfDinamite;

    bool m_hasKey;
    bool m_hasGreenKey;
    bool m_hasBlueKey;

    bool m_isInvincible; /// invincibility mode

    XDiscoveryMap m_discoveryMap;
    XFogOfViewMap m_fogOfViewMap;

    const XGameSettings *m_pGameSettings;

public:
    XObjectPlayerHero();

    void init(const XGameSettings *pGameSettings, int startX, int startY);

    std::vector<OBJECT_RENDER_DESCRIPTOR> render();

    void notifyAboutDamage(int damagePoints);

    void notifyAboutHealing(int healPoints);

    bool react(int actionCode);
    void update();

    bool isCollidedWith(int objectX, int objectY);

    int tradeChitin();

    void giveChitin(int gift);

    void reinitForNextLevel(int x, int y);

    int getShield() const
    {
        return m_shield;
    }

    bool getHasKey() const
    {
        return m_hasKey;
    }

    bool getHasGreenKey() const
    {
        return m_hasGreenKey;
    }

    bool getHasBlueKey() const
    {
        return m_hasBlueKey;
    }

    int getNumberOfChitin() const
    {
        return m_numberOfChitin;
    }

    int getNumberOfTraps() const
    {
        return m_numberOfTraps;
    }

    int getNumberOfDinamite() const
    {
        return m_numberOfDinamite;
    }

    /// invincibility flag
    int getIsInvincible() const
    {
        return m_isInvincible;
    }

    const XDiscoveryMap* getDiscoveryMap() const
    {
        return &m_discoveryMap;
    }

    const XFogOfViewMap* getFogOfViewMap() const
    {
        return &m_fogOfViewMap;
    }
};

#endif // XOBJECTPLAYERHERO_H
