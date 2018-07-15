#ifndef HEAD_H
#define HEAD_H

namespace spider {



class Head
{

    Head();
    public:
    static Head *GetInstance();
    void SetVerticalAngle(float a);
    void SetHorizontalAngle(float a);

private:
    static Head * m_singlton;
};

}
#endif // HEAD_H
