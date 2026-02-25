#ifndef DUMMYGRAPHICS_H
#define DUMMYGRAPHICS_H

#include "graphicsadapter.h"

//Dummy GraphicsAdapter for disabling UI

class DummyGraphics : public GraphicsAdapter
{
public:
    DummyGraphics();


    // GraphicsAdapter interface
public:
    void setFillColorRGBf(double r, double g, double b){}
    void setDrawColorRGBf(double r, double g, double b){}
    void setLineWidth(int){}
    void setFontSize(int){}
    void drawLine(int x0, int y0, int x1, int y1){}
    void drawText(const char *txt, int x, int y){}
    void drawArc(int xcenter, int ycenter, double radius, double angle0, double angle1){}
    void drawRect(int x, int y, int width, int height){}
    void fillRect(int x, int y, int width, int height){}
    void fillCircle(int xcenter, int ycenter, double radius){}
    int getCanvasWidth(){return 0;}
    int getCanvasHeight(){return 0;}
    void getTextSize(int *txtw, int *txth, const char *txt){
        *txtw = 0;
        *txth = 0;
    }
    void pushClippingRect(int x, int y, int width, int height){}
    void popClippingRect(){}
    int getFontHeight(){
        return 0;
    }
};

#endif // DUMMYGRAPHICS_H
