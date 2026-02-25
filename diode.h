#ifndef DIODE_H
#define DIODE_H

#include "component.h"

class Diode : public Component
{
    double itv;
    double saturationCurrent;
    double current;
    double conductance;
    double lastVoltage = 0;
    bool clamped = false;
    double parallelConductance = 0;
    static inline std::vector<const char *> fieldNames = {};
    static inline std::vector<const char *> fieldUnits = {};
    double limit(double v1,double v0);
public:
    Diode(double saturationCurrent,double itv) noexcept;
    Diode():saturationCurrent(1e-14),itv(1.0/(25.856e-3)){}
    virtual size_t getNumTerminals(void) noexcept override {
        return 2;
    };
    virtual void initializeCurrents(double time,
                                    double dt,
                                    std::vector<double>& currents,
                                    const std::vector<double>& voltages) noexcept override;
    virtual void linmapStep(double time,
                            double dt,
                            std::vector<double>& currents,
                            const std::vector<double>& voltages) noexcept override;
    virtual bool isNonlinear() noexcept override{
        return true;
    }

    virtual Component* copy() noexcept override{
        Diode* copy = new Diode(saturationCurrent,itv);
        copy->current = current;
        copy->conductance = conductance;
        return copy;
    }
    void draw(GraphicsAdapter &g, int x, int y, int size) noexcept override;

    void setField(int index, double value) override {}
    const std::vector<const char *> &getFieldNames() override{
        return fieldNames;
    };
    double getFieldValue(int index) override{
        return 0;
    }
    const std::vector<const char *> &getFieldUnits() override{
        return fieldUnits;
    }
    double getCurrent() override {
        return current;
    }

    void reset() noexcept override{
        current = 0;
        conductance = 0;
    }

    virtual void update(const std::vector<double>& currents, const std::vector<double>& voltages) override;

};

#endif // DIODE_H
