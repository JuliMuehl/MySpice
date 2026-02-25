#ifndef VOLTAGESOURCE_H
#define VOLTAGESOURCE_H

#include "component.h"

class VoltageSource : public Component
{
protected:
    virtual double getVoltage(double time) noexcept = 0;
    size_t sourceidx;
    double current = 0;
public:
    //Current is equal to voltage at companion current source node
    void update(const std::vector<double>& currents, const std::vector<double>& voltages){
        current = voltages[sourceidx-1];
    }
    virtual size_t getNumTerminals(void) noexcept override;
    virtual void linmapStep(double time,
                            double dt,
                            std::vector<double>& currents,
                            const std::vector<double>& voltages) noexcept override;
    virtual void initializeCurrents(double time,
                                    double dt,
                                    std::vector<double>& currents,
                                    const std::vector<double>& voltages) noexcept override;
    inline void setSourceIndex(size_t idx){
        sourceidx = idx;
    }
    virtual bool isVoltageSource() noexcept override{
        return true;
    }
    double getCurrent() override {
        return current;
    }
    virtual ~VoltageSource(){}
};

#endif // VOLTAGESOURCE_H
