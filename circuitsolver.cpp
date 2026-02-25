#include "circuitsolver.h"

#include <iostream>
#include <cassert>

size_t CircuitSolver::addComponent(Component* component){
    if(component->isVoltageSource()){
        auto source = (VoltageSource*) component;
        source->setSourceIndex(voltageSources.size());
        voltageSources.push_back(source);
    }
    if(component->isNonlinear())
        isNonlinear = true;
    components.push_back(component);
    return components.size();
}

void CircuitSolver::setGroundTerminal(CircuitSolver::Terminal terminal){
    groundTerminal = terminal;
}

void CircuitSolver::addConnection(Terminal term1,Terminal term2){
    Connection conn = {term1,term2};
    connections.push_back(conn);
}

double CircuitSolver::getVoltage(Terminal term){
    size_t node = getNode(term);
    if(node > 1){
        return voltages[node-2];
    }else{
        return 0.0;
    }
}

void CircuitSolver::resetComponents(){
    for(auto& comp : components){
        comp->reset();
    }
    for(size_t i = 0;i<voltages.size();i++){
        voltages[i] = 0.0;
        currents[i] = 0.0;
    }
}

static size_t unionFindSearch(std::vector<size_t>& parent,size_t x){
    if(parent[x] != x){
        parent[x] = unionFindSearch(parent,parent[x]);
    }
    return parent[x];
}

static void unionFindMakeSet(std::vector<size_t>& parent,
                             std::vector<size_t>& rank,
                             size_t x){
    if(!parent[x]){
        parent[x] = x;
        rank[x] = 0;
    }
}

static void unionFindUnion(std::vector<size_t>& parent,
                           std::vector<size_t>& rank,
                           size_t x,size_t y){
    unionFindMakeSet(parent,rank,x);
    unionFindMakeSet(parent,rank,y);
    size_t rootx = unionFindSearch(parent,x);
    size_t rooty = unionFindSearch(parent,y);
    if(rank[rootx] < rank[rooty]){
        parent[rootx] = rooty;
    }
    else if(rank[rootx] > rank[rooty]){
        parent[rooty] = rootx;
    }else{
        parent[rooty] = rootx;
        rank[rootx]++;
    }
}

bool CircuitSolver::computeNodes(){
    size_t groundIndex = termToIndex(groundTerminal);
    if(groundIndex == 0){
        std::cerr << "Error: Ground Terminal must be set before computing nodes!" << std::endl;
        return false;
    }
    //Maximum number of terminals of a component is 3 (for transistors)
    parent.clear();
    parent.resize(3*components.size(),0);
    rank.clear();
    rank.resize(3*components.size(),0);
    parent[groundIndex] = groundIndex;
    for(const auto [term1,term2] : connections){
        size_t term1i = termToIndex(term1);
        size_t term2i = termToIndex(term2);
        unionFindUnion(parent,rank,term1i,term2i);
    }
    cc = std::vector<size_t>(3*components.size(),0);
    size_t groundRoot = unionFindSearch(parent,groundIndex);
    cc[groundRoot] = 1;
    this->numUniqueNodes = 1;
    for(size_t i = 0;i<components.size();i++){
        for(size_t t = 0;t < components[i]->getNumTerminals();t++){
            size_t termi = termToIndex({i+1,t});
            if(parent[termi] == 0){
                std::cerr <<
                    "Error: Floating terminals aren't allowed consider connecting to ground with a high value resistor!"
                          << std::endl;
                return false;
            }
            size_t root1 = unionFindSearch(parent,termi);
            if(cc[root1] == 0){
                cc[root1] = ++this->numUniqueNodes;
            }
            cc[termi] = cc[root1];
        }
    }
    for(size_t i = 1;i<numUniqueNodes;i++){
        voltages.push_back(0.0);
        currents.push_back(0.0);
    }
    for(size_t i = 0;i<voltageSources.size();i++){
        voltages.push_back(0.0);
        currents.push_back(0.0);
        // numUniqueNodes include ground thus i+1 cancels with numUniqueNodes - 1
        voltageSources[i]->setSourceIndex(i + numUniqueNodes);
    }
    for(size_t i = 1;i<=components.size();i++){
        auto& comp = components[i-1];
        for(size_t j = 0;j<comp->getNumTerminals();j++){
            size_t node = getNode({i,j});
            if(node == 0){
                std::cerr <<
                    "Error: Floating terminals aren't allowed consider connecting to ground with a high value resistor!"
                          << std::endl;
                return false;
            }
            comp->setTerminalNode(j,node-1);
        }
    }
    return true;
}

size_t CircuitSolver::getNode(Terminal term){
    return cc[termToIndex(term)];
}

void CircuitSolver::linmap(std::vector<double> &v, const std::vector<double> &w) noexcept{
    for(double& x:v)
        x = 0;
    for(auto& comp:components){
        comp->linmapStep(simTime,timeStep,v,w);
    }
}

void CircuitSolver::clear(){
    groundTerminal = {0,0};
    simTime = 0.0;
    isNonlinear = false;
    numUniqueNodes = 0;
    connections.clear();
    voltageSources.clear();
    components.clear();
    voltages.clear();
    currents.clear();
}

bool CircuitSolver::implicitEulerStep(){
    for(double& curr:currents) curr = 0;
    size_t numIters = numNewtonIters;
    for(size_t i = 0;i<numIters;i++){
        if(!newtonStep()){
            std::cerr << "Failing out of Newton step!" << std::endl;
            return false;
        }
    }
    simTime += timeStep;
    return true;
}

bool CircuitSolver::newtonStep(){
    for(double& c : currents) c = 0;
    for(auto& comp:components){
        comp->initializeCurrents(simTime,timeStep,currents,voltages);
        if(comp->hasError()){
            errorMessage = comp->getErrorMessage();
            return false;
        }
    }

    solve(voltages,currents);

    for(auto comp:components){
        if(comp->hasError()){
            errorMessage = comp->getErrorMessage();
            return false;
        }
    }
    for(auto comp:components){
        comp->update(voltages,currents);
    }
    return true;
}
