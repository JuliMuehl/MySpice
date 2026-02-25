#include "plotcard.h"
#include <cstdio>

int PlotCard::getHeight(int width,int fontheight)
{
    int r = roundingRadius(width);
    return 2*r + 3*fontheight + width;
}

void PlotCard::draw(GraphicsAdapter &g, int width, int x, int y)
{
    int fontheight = g.getFontHeight();
    int height = getHeight(width,fontheight);
    int r = roundingRadius(width);
    int hdrx = x + r;
    int hdry = y + r;
    int plotx = x;
    int ploty = y+(height - r - width);
    int plotwidth = width;
    int plotheight = width;
    g.setFillColorRGBf(0.0,0.0,0.0);
    g.fillRoundedRect(x,y,width,height,r);
    double time = timeData.empty() ? 0:timeData.back();
    snprintf(headerText,sizeof(headerText),"Voltage Scale = %f",maxAbsVoltage);
    g.setDrawColorRGBf(0.8,0.8,0.8);
    g.drawText(headerText,hdrx,hdry);
    snprintf(headerText,sizeof(headerText),"Current Scale= %f",maxAbsCurrent);
    g.drawText(headerText,hdrx,hdry+fontheight);
    snprintf(headerText,sizeof(headerText),"Time = %f",time);
    g.drawText(headerText,hdrx,hdry+2*fontheight);
    size_t n = timeData.size();
    if(n >= 2){
        int xorig = plotx;
        int yorig = ploty + plotheight/2;
        double T = timeData.back() - timeData[0];
        xdata.clear();
        yVoltageData.clear();
        yCurrentData.clear();
        double maxValue = fmax(maxAbsVoltage,maxAbsCurrent);
        for(size_t i = 0;i<n;i++){
            double x = xorig + plotwidth * (timeData[i] - timeData[0]) / T;
            double v = yorig - 0.9*plotheight/2 * (maxAbsVoltage == 0.0 ? 0:voltageData[i] / maxAbsVoltage);
            double j = yorig - 0.9*plotheight/2 * (maxAbsCurrent == 0.0 ? 0:currentData[i] / maxAbsCurrent);
            if(xdata.empty() || xdata.back() != x){
                xdata.push_back(x);
                yVoltageData.push_back(v);
                yCurrentData.push_back(j);
            }
            g.setDrawColorRGBf(0.8,0.8,0.3);
        }
        g.setDrawColorRGBf(0.8,0.3,0.3);
        g.drawLines(xdata.data(),yVoltageData.data(),xdata.size());
        g.setDrawColorRGBf(0.8,0.8,0.3);
        g.drawLines(xdata.data(),yCurrentData.data(),xdata.size());
    }
}
