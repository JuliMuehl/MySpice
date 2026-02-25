#ifndef RESISTOR_H
#define RESISTOR_H

#include "component.h"
#include <cstring>
#include <iostream>

class Resistor : public Component
{
    double conductance;
    double current = 0;
    static inline std::vector<const char*> fieldNames =  {"Resistance"};
    static inline std::vector<const char*> fieldUnits =  {"Ohm"};
public:
    Resistor(double resistance) noexcept;
    virtual size_t getNumTerminals(void) noexcept override{
        return 2;
    }
    virtual void linmapStep(double time,
                            double dt,
                            std::vector<double>& currents,
                            const std::vector<double>& voltages) noexcept override;
    virtual void initializeCurrents(double time,
                                    double dt,
                                    std::vector<double>& currents,
                                    const std::vector<double>& voltages) noexcept override;

    virtual Component* copy() noexcept override{
        Resistor* copy = new Resistor(1.0 / conductance);
        return copy;
    }

    void draw(GraphicsAdapter &g, int x, int y, int size) noexcept override;

    void setField(int index, double value) override {
        if(index == 0){
            conductance = 1.0 / value;
        }
    }
    double getFieldValue(int index) override{
        if(index == 0){
            return 1.0 / conductance;
        }
        return 0;
    }
    const std::vector<const char *> & getFieldNames() override {
        return fieldNames;
    }
    const std::vector<const char *> &getFieldUnits() override{
        return fieldUnits;
    }
    double getCurrent() override{
        return current;
    }
};
#endif // RESISTOR_H
