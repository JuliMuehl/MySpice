#ifndef PLOTCARD_H
#define PLOTCARD_H

#include "infocard.h"
#include <cmath>
#include <vector>

class PlotCard : public InfoCard
{
    std::vector<double> timeData;
    std::vector<double> currentData;
    std::vector<double> voltageData;
    std::vector<int> xdata;
    std::vector<int> yCurrentData;
    std::vector<int> yVoltageData;
    char headerText[512];
    double maxAbsCurrent = 0.0;
    double maxAbsVoltage = 0.0;
public:
    inline void registerValues(double time,double current,double voltage){
        timeData.push_back(time);
        currentData.push_back(current);
        voltageData.push_back(voltage);
        maxAbsCurrent = fmax(maxAbsCurrent,fabs(current));
        maxAbsVoltage = fmax(maxAbsVoltage,fabs(voltage));
    }
    int getHeight(int width,int fontheight) override;
    void draw(GraphicsAdapter &g, int width, int x, int y) override;
    void reset(){
        timeData.clear();
        currentData.clear();
        voltageData.clear();
        maxAbsCurrent = 0.0;
        maxAbsVoltage = 0.0;
    }
};

#endif // PLOTCARD_H
