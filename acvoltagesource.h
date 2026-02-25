#ifndef ACVOLTAGESOURCE_H
#define ACVOLTAGESOURCE_H

#include "voltagesource.h"
#include <cmath>
#include <iostream>

class ACVoltageSource : public VoltageSource
{
    double maxvoltage, freq;
    static inline std::vector<const char*> fieldNames  = {"Voltage","Frequency"};
    static inline std::vector<const char*> fieldUnits  = {"Volt","Herz"};
protected:
    double getVoltage(double time) noexcept override{
        return sin(time*freq*2*M_PI) * maxvoltage;
    }
public:
    ACVoltageSource(double maxvoltage, double freq) noexcept : maxvoltage(maxvoltage), freq(freq) {}
    virtual Component* copy() noexcept override{
        ACVoltageSource* copy = new ACVoltageSource(maxvoltage,freq);
        return copy;
    }
    void draw(GraphicsAdapter &g, int x, int y, int size) noexcept override;
public:
    void setField(int index, double value) override {
        if(index == 0){
            maxvoltage = value;
        }else if(index == 1){
            freq = value;
        }
    }
    const std::vector<const char *> &getFieldNames() override {
        return fieldNames;
    }
    const std::vector<const char *> &getFieldUnits() override {
        return fieldUnits;
    }
    double getFieldValue(int index) override{
        if(index == 0){
            return maxvoltage;
        }else if(index == 1){
            return freq;
        }
        return 0;
    }
};


#endif // ACVOLTAGESOURCE_H
