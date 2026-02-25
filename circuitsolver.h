#ifndef CIRCUITSOLVER_H
#define CIRCUITSOLVER_H

#include "gmressolver.h"
#include "component.h"
#include "voltagesource.h"

#include <vector>
#include <tuple>

class CircuitSolver : public GMResSolver
{
public:
    using Terminal = std::tuple<size_t,size_t>;
    using Connection = std::tuple<Terminal,Terminal>;
    CircuitSolver(double timeStep = 20e-6, size_t numNewtonIters = 10):
        timeStep(timeStep),
        numNewtonIters(numNewtonIters),
        isNonlinear(false),
        simTime(0.0),
        numUniqueNodes(0){}
    size_t addComponent(Component* component);
    void addConnection(Terminal,Terminal);
    void setGroundTerminal(Terminal);
    Terminal getGroundTerminal();
    double getVoltage(Terminal);
    inline double getSimulationTime(){
        return simTime;
    }
    bool computeNodes();
    void clear();
    size_t getNode(Terminal);
    inline size_t getNumNodes(){
        return numUniqueNodes;
    }
    bool newtonStep();
    bool implicitEulerStep();
    const char* getErrorMessage(){
        return errorMessage;
    }
    void setTime(double time){
        simTime = time;
    }
    virtual void linmap(std::vector<double>&, const std::vector<double>&) noexcept override;
private:
    const char* errorMessage = "";
    std::vector<Component*> components;
    std::vector<VoltageSource*> voltageSources;
    std::vector<Connection> connections;
    std::vector<size_t> parent,rank,cc;
    std::vector<double> currents,voltages;
    Terminal groundTerminal = {0,0};
    double timeStep;
    double simTime;
    bool isNonlinear;
    size_t numNewtonIters;
    size_t numUniqueNodes;
    void resetComponents();
    size_t termToIndex(CircuitSolver::Terminal term){
        return std::get<0>(term) + std::get<1>(term) * components.size();
    }
};

#endif // CIRCUITSOLVER_H
