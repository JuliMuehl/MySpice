#include "capacitor.h"

#include "grapheditor.h"

size_t Capacitor::getNumTerminals() noexcept{
    return 2;
}

void Capacitor::linmapStep(double time,
                           double dt,
                           std::vector<double>& currents,
                           const std::vector<double>& voltages) noexcept {
    size_t node1 = terminals[0],node2 = terminals[1];
    double conductance = capacitance / dt;
    companionResistorUpdate(node1,node2,conductance,currents,voltages);
}

void Capacitor::initializeCurrents(double time,
                                   double dt,
                                   std::vector<double>& currents,
                                   const std::vector<double>& voltages) noexcept{
    size_t node1 = terminals[0],node2 = terminals[1];
    double conductance = capacitance / dt;
    double voltage = (getVoltage(voltages,node2) - getVoltage(voltages,node1));
    current =  -conductance * (voltage - lastVoltage);
    lastVoltage = voltage;
    companionResistorUpdate(node1,node2,conductance,currents,voltages);
}

void Capacitor::draw(GraphicsAdapter &g, int x, int y, int size) noexcept{
    GraphEditor::drawCapacitor(g,x,y,size);
}
