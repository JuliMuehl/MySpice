#include "inductor.h"

#include "grapheditor.h"

Inductor::Inductor(double inductance) noexcept : inductance(inductance), current(0) {
}

size_t Inductor::getNumTerminals(void) noexcept {
    return 2;
}

void Inductor::linmapStep(double time,
                          double dt,
                          std::vector<double>& currents,
                          const std::vector<double>& voltages) noexcept {
    size_t node1 = terminals[0], node2 = terminals[1];
    double conductance = dt/this->inductance;
    companionResistorUpdate(node1,node2,conductance,currents,voltages);
}
void Inductor::initializeCurrents(double time,
                                  double dt,
                                  std::vector<double>& currents,
                                  const std::vector<double>& voltages) noexcept {

    size_t node1 = terminals[0], node2 = terminals[1];
    Component::incrementCurrent(currents,node1,-this->current);
    Component::incrementCurrent(currents,node2,this->current);
    double conductance = dt/this->inductance;
    double curr = conductance * (Component::getVoltage(voltages,node2) - Component::getVoltage(voltages,node1));
    this->current -= curr;
}

void Inductor::draw(GraphicsAdapter &g, int x, int y, int size) noexcept{
    GraphEditor::drawInductor(g,x,y,size);
}

void reset() noexcept{

}
