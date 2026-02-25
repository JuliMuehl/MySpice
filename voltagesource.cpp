#include "voltagesource.h"

size_t VoltageSource::getNumTerminals() noexcept{
    return 2;
}

void VoltageSource::linmapStep(double time,
                          double dt,
                          std::vector<double>& currents,
                          const std::vector<double>& voltages) noexcept {

    size_t node1 = terminals[0], node2 = terminals[1];
    double curr = Component::getVoltage(voltages,node2) - Component::getVoltage(voltages,node1);
    incrementCurrent(currents,sourceidx,curr);
    incrementCurrent(currents,node1,voltages[sourceidx-1]);
    incrementCurrent(currents,node2,-voltages[sourceidx-1]);
}

void VoltageSource::initializeCurrents(double time,
                                  double dt,
                                  std::vector<double>& currents,
                                  const std::vector<double>& voltages) noexcept{
    currents[sourceidx-1] = getVoltage(time);
}
