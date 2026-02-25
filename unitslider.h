#ifndef UNITSLIDER_H
#define UNITSLIDER_H

#include <array>
#include "graphicsadapter.h"
#include "mouseeventobserver.h"

class UnitSlider : public MouseEventObserver
{
    constexpr static std::array<const char*,5> unitStrings = {
        "10^-9",
        "10^-6",
        "10^-3",
        "10^0",
        "10^3"
    };
    constexpr static const std::array<double,5> unitValues = {
        1e-9,
        1e-6,
        1e-3,
        1e0,
        1e3
    };
    std::array<int,unitValues.size()> unitBoxWidths = {0};
    constexpr static int minValue=1,maxValue = 1000;
    constexpr static int sliderHeight = 10;
    constexpr static int marginX = sliderHeight;
    constexpr static int unitBoxPadding = 5;
    int sliderWidth;
    int value = 500;
    int unitIndex = 2;
    int fontheight,txtWidth;
    int width,height;
    bool sliding = false;
    char infoString[256];
    const char* fieldName;
    const char* unitName;
public:
    UnitSlider(const char* fieldName,const char* unitName):fieldName(fieldName),unitName(unitName){}
    int getHeight(int width,int fontheight) const ;
    double getValue(){
        return unitValues[unitIndex] * value;
    }
    void setValue(double v);
    void draw(GraphicsAdapter& g,int width, int x,int y);
    void mouseMoveEvent(int x, int y);
    void mousePressEvent(int x, int y, int button);
    void mouseReleaseEvent(int x, int y, int button);
};

#endif // UNITSLIDER_H
