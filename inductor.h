#ifndef INDUCTOR_H
#define INDUCTOR_H

#include "component.h"

class Inductor : public Component
{
    double inductance;
    double current;
    static inline std::vector<const char*> fieldNames  = {"inductance"};
    static inline std::vector<const char*> fieldUnits  = {"Henry"};
public:
    Inductor(double inductance) noexcept;
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
        Inductor* copy = new Inductor(inductance);
        copy->current = current;
        return copy;
    }

    void draw(GraphicsAdapter &g, int x, int y, int size) noexcept override;

    void setField(int index, double value) override {
        if(index == 0){
            inductance = value;
        }
    }
    double getFieldValue(int index) override{
        if(index == 0){
            return inductance;
        }
        return 0;
    }
    const std::vector<const char *> & getFieldNames() override {
        return fieldNames;
    }
    const std::vector<const char *> & getFieldUnits() override {
        return fieldUnits;
    }
    double getCurrent() override {
        return current;
    }
    void reset() noexcept override{
        current = 0;
    }
};

#endif // INDUCTOR_H
