#ifndef CAPACITOR_H
#define CAPACITOR_H

#include "component.h"

class Capacitor : public Component
{
    double capacitance;
    double lastVoltage = 0,current;
    static inline std::vector<const char*> fieldNames = {"Capacitance"};
    static inline std::vector<const char*> fieldUnits = {"Farrad"};
public:
    Capacitor(double capacitance) noexcept :capacitance(capacitance) {}
    virtual size_t getNumTerminals(void) noexcept override;
    virtual void linmapStep(double time,
                            double dt,
                            std::vector<double>& currents,
                            const std::vector<double>& voltages) noexcept override;
    virtual void initializeCurrents(double time,
                                    double dt,
                                    std::vector<double>& currents,
                                    const std::vector<double>& voltages) noexcept override;
    virtual Component* copy() noexcept override{
        Capacitor* copy = new Capacitor(capacitance);
        return copy;
    }
    void draw(GraphicsAdapter &g, int x, int y, int size) noexcept override;

    void setField(int index, double value) override {
        if(index == 0){
            capacitance = value;
        }
    }
    const std::vector<const char *> & getFieldNames() override {
        return fieldNames;
    }
    const std::vector<const char *> &getFieldUnits() override {
        return fieldUnits;
    }
    double getFieldValue(int index) override{
        if(index == 0){
            return capacitance;
        }
        return 0;
    }
    double getCurrent() override{
        return current;
    }
};

#endif // CAPACITOR_H
