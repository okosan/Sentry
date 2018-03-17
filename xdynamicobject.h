#ifndef XDYNAMICOBJECT_H
#define XDYNAMICOBJECT_H


class XDynamicObject
{
public:
    XDynamicObject();

    void getXY(int &rX, int &rY) const;

    bool isInitialized()
    {
        return m_isInitialized;
    }

protected:
    int m_x;
    int m_y;

    bool m_isInitialized;
};

#endif // XDYNAMICOBJECT_H
