#ifndef COMPONENT_H
#define COMPONENT_H

#include "graphicsadapter.h"
#include <vector>
class Component
{
protected:
    std::vector<size_t> terminals;
    inline static void incrementCurrent(std::vector<double>& currents, size_t i, double curr){
        if(i > 0)
            currents[i-1] += curr;
    }
    inline static double getVoltage(const std::vector<double>& voltages, size_t i){
        if(i == 0){
            return 0;
        }
        return voltages[i-1];
    }
    inline static void companionResistorUpdate(size_t node1, size_t node2,
                                               double conductance,
                                               std::vector<double>& currents,
                                               const std::vector<double> voltages){
        double voltage = getVoltage(voltages,node2) - getVoltage(voltages,node1);
        double curr = conductance * voltage;
        incrementCurrent(currents,node1,-curr);
        incrementCurrent(currents,node2,curr);
    }
public:
    virtual void update(const std::vector<double>& currents, const std::vector<double>& voltages){}
    void setTerminalNode(size_t term,size_t node){
        while(terminals.size() <= getNumTerminals()){
            terminals.push_back(0);
        }
        terminals[term] = node;
    }
    size_t getTerminalNode(size_t term){
        if(term < terminals.size())
            return terminals[term];
        return 0;
    }
    virtual size_t getNumTerminals(void) noexcept = 0;
    virtual void initializeCurrents(double time,
                                    double dt,
                                    std::vector<double>& currents,
                                    const std::vector<double>& voltages) noexcept = 0;
    virtual void linmapStep(double time,
                            double dt,
                            std::vector<double>& currents,
                            const std::vector<double>& voltages) noexcept = 0;

    virtual Component* copy() noexcept = 0;
    virtual bool isVoltageSource() noexcept {
        return false;
    }
    virtual bool isNonlinear() noexcept{
        return false;
    }
    virtual void reset() noexcept {}
    virtual bool hasError() noexcept {return false;}
    virtual const char* getErrorMessage() noexcept {return "";};
    virtual void draw(GraphicsAdapter& g,int x,int y,int size) = 0;
    virtual void setField(int index,double value) = 0;
    virtual double getFieldValue(int index) = 0;
    virtual const std::vector<const char*>& getFieldNames() = 0;
    virtual const std::vector<const char*>& getFieldUnits() = 0;
    virtual double getCurrent() = 0;
    virtual ~Component() {};
};

#endif // COMPONENT_H
