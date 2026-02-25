#ifndef DCVOLTAGESOURCE_H
#define DCVOLTAGESOURCE_H

#include "voltagesource.h"

class DCVoltageSource : public VoltageSource
{
    double voltage;
    static inline std::vector<const char*> fieldNames = {"Voltage"};
    static inline std::vector<const char*> fieldUnits = {"Volt"};
protected:
    virtual double getVoltage(double time) noexcept override {
        return voltage;
    }
public:
    inline DCVoltageSource(double voltage) noexcept : voltage(voltage){}
    virtual Component* copy() noexcept override{
        DCVoltageSource* copy = new DCVoltageSource(voltage);
        return copy;
    }

    void draw(GraphicsAdapter &g, int x, int y, int size) noexcept override;
    void setField(int index, double value) override {
        if(index == 0){
            voltage = value;
        }
    }
    double getFieldValue(int index) override{
        if(index == 0){
            return voltage;
        }
        return 0;
    }
    const std::vector<const char *> &getFieldNames() override {
        return fieldNames;
    }
    const std::vector<const char *> &getFieldUnits() override {
        return fieldUnits;
    }

};
#endif // DCVOLTAGESOURCE_H
