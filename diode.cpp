#include "diode.h"

#include "grapheditor.h"

#include <cmath>
#include <iostream>

Diode::Diode(double saturationCurrent,double itv) noexcept
    : saturationCurrent(saturationCurrent),itv(itv)  {}

void Diode::linmapStep(double time,
                          double dt,
                          std::vector<double>& currents,
                          const std::vector<double>& voltages) noexcept {
    size_t node1 = terminals[0],node2 = terminals[1];
    companionResistorUpdate(node1,node2,conductance,currents,voltages);
}

void Diode::draw(GraphicsAdapter &g, int x, int y, int size) noexcept{
    GraphEditor::drawDiode(g,x,y,size);
}

void Diode::update(const std::vector<double> &currents, const std::vector<double> &voltages){
    size_t node1 = terminals[0], node2 = terminals[1];
    lastVoltage = getVoltage(voltages,node2) - getVoltage(voltages,node1);
    parallelConductance = saturationCurrent * 0.01;
}

double Diode::limit(double v1,double v0){
    double criticalVoltage = log(1.0/(itv*sqrt(2)*saturationCurrent));
    clamped = false;
    if (v1 > criticalVoltage && fabs(v1 - v0) > (2.0 / itv)) {
        clamped = true;
        if(v0 > 0) {
            double lin = 1 + (v1 - v0) * itv;
            if(lin > 0) {
                v1 = v0 + log(lin)/itv;
            } else {
                v1 = criticalVoltage;
            }
        } else {
            v1 = log(v1*itv)/itv;
        }
    }
    return v1;
}

void Diode::initializeCurrents(double time,
                                  double dt,
                                  std::vector<double>& currents,
                                  const std::vector<double>& voltages) noexcept{
    size_t node1 = terminals[0], node2 = terminals[1];
    double voltage = Component::getVoltage(voltages,node2) - Component::getVoltage(voltages,node1);
    voltage = limit(voltage,lastVoltage);
    this->current = saturationCurrent * (exp(itv*voltage) - 1);
    parallelConductance = 0 ;// 1.0/500.0;
    this->conductance = itv * (current + saturationCurrent) + parallelConductance;
    double curr = this->current - conductance*voltage;
    Component::incrementCurrent(currents,node1,curr);
    Component::incrementCurrent(currents,node2,-curr);
    //Check for NaN/Inf
    if(this->current != this->current){
        std::cerr << "Error: Maximum Diode current exceeded!" << std::endl;
    }
}
