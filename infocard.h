#ifndef INFOCARD_H
#define INFOCARD_H

#include "graphicsadapter.h"
#include "mouseeventobserver.h"

class InfoCard : public MouseEventObserver
{
protected:
    inline double roundingRadius(int width){
        return width/10;
    }
public:
    virtual int getHeight(int width,int fontheight) = 0;
    virtual void draw(GraphicsAdapter& g,int width,int x,int y) = 0;


    // MouseEventObserver interface
public:
    void mouseMoveEvent(int x, int y){}
    void mousePressEvent(int x, int y, int button){}
    void mouseReleaseEvent(int x, int y, int button){}
    virtual ~InfoCard(){}
};

#endif // INFOCARD_H
