#include "unitslider.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
int UnitSlider::getHeight(int width, int fontheight) const
{
    return 2*fontheight + sliderHeight;
}

void UnitSlider::draw(GraphicsAdapter &g, int width, int x, int y)
{
    fontheight = g.getFontHeight();
    double sliderPercentage = (double)(value-minValue)/(maxValue - minValue);
    this->width = width;
    this->height = getHeight(width,fontheight);
    this->txtWidth = 0;
    for(const char* txt : unitStrings){
        int txtw, txth;
        g.getTextSize(&txtw,&txth,txt);
        if(txtw > txtWidth){
            txtWidth = txtw;
        }
    }
    int xslider = x + marginX;
    sliderWidth = width - 2*marginX;
    g.setFillColorRGBf(0.5,0.5,0.5);
    g.fillRect(xslider,y,sliderWidth,sliderHeight);
    g.setFillColorRGBf(0.8,0.8,0.8);
    g.fillRect(xslider,y,sliderPercentage*sliderWidth,sliderHeight);
    int xi = xslider;
    unitBoxWidths[0] = 0;
    for(int i = 0;i<unitStrings.size();i++){
        const char* unittxt = this->unitStrings[i];
        g.setDrawColorRGBf(0.8,0.8,0.8);
        g.setFillColorRGBf(0.2,0.2,0.2);
        if(i == unitIndex){
            g.setFillColorRGBf(0.8,0.2,0.2);
        }
        int txtw,txth;
        g.getTextSize(&txtw,&txth,unittxt);
        g.fillRect(xi,y+sliderHeight,txtw+2*unitBoxPadding,txth);
        g.drawText(unittxt,xi+unitBoxPadding,y+sliderHeight+txth);
        unitBoxWidths[i] = txtw + 2*unitBoxPadding;
        xi += unitBoxWidths[i];
    }
    memset(infoString,0,sizeof(infoString));
    snprintf(infoString,sizeof(infoString),"%s = %.12g %s",fieldName,getValue(),unitName);
    g.drawText(infoString,xslider,y + sliderHeight + 2*fontheight);
}

void UnitSlider::mousePressEvent(int x, int y, int button)
{
    int xslider = marginX;
    sliding = button == MOUSE_BUTTON_LEFT && xslider <= x && x <= xslider + sliderWidth && 0 <= y && y <= sliderHeight;
    if(sliderHeight < y && y <= sliderHeight + fontheight){
        int xi = xslider;
        for(int i = 0;i<unitBoxWidths.size();i++){
            int boxwidth = unitBoxWidths[i];
            if(xi <= x && x <= xi + boxwidth){
                unitIndex = i;
                break;
            }
            xi += boxwidth;
        }
    }
}

void UnitSlider::mouseMoveEvent(int x, int y)
{
    if(sliding){;
        int xslider = marginX;
        double dpercentage = (double) (x-xslider) / sliderWidth;
        value = dpercentage * (maxValue - minValue);
        value = std::min(std::max(value,minValue),maxValue);
    }
}

void UnitSlider::mouseReleaseEvent(int x, int y, int button)
{
    if(sliding) sliding = false;
}

void UnitSlider::setValue(double v) {
    for (int i = 0; i < unitValues.size(); i++) {
        double unit = unitValues[i];
        if (minValue * unit <= v && v <= maxValue * unit) {
            unitIndex = i;
            value = (int)(v / unit);
            break;
        }
    }
}
