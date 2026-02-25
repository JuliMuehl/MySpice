#include "resistor.h"

#include "grapheditor.h"

Resistor::Resistor(double resistance) noexcept {
    conductance = 1.0 / resistance;
}

void Resistor::linmapStep(double time,
                         double dt,
                         std::vector<double>& currents,
                         const std::vector<double>& voltages) noexcept {
    size_t node1 = terminals[0],node2 = terminals[1];
    companionResistorUpdate(node1,node2,conductance,currents,voltages);
}

void Resistor::initializeCurrents(double time,
                                  double dt,
                                  std::vector<double>& currents,
                                  const std::vector<double>& voltages) noexcept{
    size_t node1 = terminals[0],node2 = terminals[1];
    double voltage = (getVoltage(voltages,node2) - getVoltage(voltages,node1));
    this->current = conductance * voltage;
}

void Resistor::draw(GraphicsAdapter &g, int x, int y, int size) noexcept{
    GraphEditor::drawResistor(g,x,y,size);
}
