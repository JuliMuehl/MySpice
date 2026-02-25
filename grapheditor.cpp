#include "grapheditor.h"
#include <cmath>
#include <iostream>
#include "components.h"
#include "plotcard.h"
#include "scrollablepane.h"
#include "settingscard.h"

double GraphEditor::terminalRadius(int size)
{
    return (0.05 * size);
}

void GraphEditor::initializePrototypes(int componentSize,int padding)
{
    int xstep = componentSize + padding;
    prototypes[0] = RenderComponent(0*xstep,0,new Resistor(1000),nullptr);
    prototypes[1] = RenderComponent(1*xstep,0,new Capacitor(5e-6),nullptr);
    prototypes[2] = RenderComponent(2*xstep,0,new Inductor(1),nullptr);
    prototypes[3] = RenderComponent(3*xstep,0,new Diode(),nullptr);
    prototypes[4] = RenderComponent(4*xstep,0,new DCVoltageSource(5),nullptr);
    prototypes[5] = RenderComponent(5*xstep,0,new ACVoltageSource(5,1000),nullptr);
}

void GraphEditor::addConnection(size_t comp1, size_t term1, size_t comp2, size_t term2)
{
    connections.push_back({comp1,term1,comp2,term2});
}

void GraphEditor::eraseComponent(size_t comp)
{
    resetSimulation();
    //Never remove the Ground Component
    if(comp == 0) return;
    plotPane.removeInfoCard(comp-1);
    settingsPane.removeInfoCard(comp-1);
    delete rendercomponents[comp].getModel();
    rendercomponents.erase(rendercomponents.begin() + comp);
    std::vector<std::tuple<size_t,size_t,size_t,size_t>> newconnections;
    for(auto [comp1,term1,comp2,term2] : connections){
        if(comp1 != comp && comp2 != comp){
            if(comp1 > comp) comp1--;
            if(comp2 > comp) comp2--;
            newconnections.push_back({comp1,term1,comp2,term2});
        }
    }
    connections = newconnections;
}

void GraphEditor::drawEndpointTerminals(GraphicsAdapter &g, int x, int y, int size)
{
    double r = GraphEditor::terminalRadius(size);
    g.fillCircle(x - size/2 - r,y,r);
    g.fillCircle(x + size/2 + r,y,r);
}

void GraphEditor::drawCapacitor(GraphicsAdapter &g, int x, int y, int size)
{
    double plateX = 0.15 * size / 2.0;
    double plateY = 0.6 * size / 2.0;
    g.drawLine(x-plateX,y-plateY,x-plateX,y+plateY);
    g.drawLine(x+plateX,y-plateY,x+plateX,y+plateY);
    g.drawLine(x-size/2,y,x-plateX,y);
    g.drawLine(x+plateX,y,x+size/2,y);
}

void GraphEditor::drawResistor(GraphicsAdapter &g, int x, int y, int size)
{
    double left = x - 0.8*size/2;
    double right = x + 0.8*size/2;
    g.drawLine(x-size/2,y,left,y);
    double width = size*0.8;
    double height = size * 0.1;
    int nbumps = 10;
    for(int j = 0;j<nbumps;j++){
        double x1 = left + (j)*width/nbumps,
               x2 = x1 + 0.5 * width/nbumps,
               x3 = x2 + 0.5 * width/nbumps;
        double sign = j%2==0 ? 1:-1;
        g.drawLine(x1,y,x2,y+sign*height);
        g.drawLine(x2,y+sign*height,x3,y);
    }
    g.drawLine(right,y,x+size/2,y);
}

void GraphEditor::drawInductor(GraphicsAdapter &g, int x, int y, int size)
{
    double left = x - 0.8 * size/2;
    double right = x + 0.8 * size/2;
    g.drawLine(x - size/2,y,left,y);
    double width = size * 0.8;
    int nbumps = 4;
    double radius = 0.5 * width/nbumps;
    for(int i = 0;i<nbumps;i++){
        double xcenter = left + i * width/nbumps + radius;
        g.drawArc(xcenter,y,radius,0,1*M_PI);
    }
    g.drawLine(right,y,x+size/2,y);
}


void GraphEditor::drawDiode(GraphicsAdapter &g, int x, int y, int size)
{
    double diodeSize = 0.4 * size;
    g.drawLine(x-0.5*size,y,x+0.5*size,y);
    double diodeLeft = x-diodeSize/2;
    double diodeRight = x+diodeSize/2;
    g.drawLine(diodeLeft,y-0.5*diodeSize,diodeLeft,y+0.5*diodeSize);
    g.drawLine(diodeRight,y-0.5*diodeSize,diodeRight,y+0.5*diodeSize);
    g.drawLine(diodeLeft,y+0.5*diodeSize,diodeRight,y);
    g.drawLine(diodeLeft,y-0.5*diodeSize,diodeRight,y);
}

void GraphEditor::drawDCVoltageSource(GraphicsAdapter &g, int x, int y, int size)
{
    double radius = 0.6*size/2;
    g.drawArc(x,y,radius,0,2*M_PI);
    g.drawLine(x - size/2,y,x-radius,y);
    g.drawLine(x + radius,y,x+size/2,y);
    double signCenter = radius/2;
    double signSize = size/20.0;
    double xminus = x - signCenter;
    double xplus = x + signCenter;
    if(signSize <= radius){
        g.drawLine(xminus-signSize,y,xminus+signSize,y);
        g.drawLine(xplus-signSize,y,xplus+signSize,y);
        g.drawLine(xplus,y-signSize,xplus,y+signSize);
    }
}

void GraphEditor::drawACVoltageSource(GraphicsAdapter &g, int x, int y, int size)
{
    double radius = 0.6*size/2;
    g.drawArc(x,y,radius,0,2*M_PI);
    g.drawLine(x - size/2,y,x-radius,y);
    g.drawLine(x + radius,y,x+size/2,y);
    double xi = x - radius;
    double yi = y;
    int n = 128;
    for(int i = 0;i<n;i++){
        double xj = xi + 2*radius/n;
        double yj = y + 0.4 * radius * sin((xi - x)/radius*M_PI);
        g.drawLine(xi,yi,xj,yj);
        xi = xj;
        yi = yj;
    }
}

void GraphEditor::drawGround(GraphicsAdapter &g, int x, int y, int size)
{
    g.drawLine(x,y,x+0.5*size,y);
    g.drawLine(x,y-0.4*size,x,y+0.4*size);
    g.drawLine(x-0.1*size,y-0.3*size,x-0.1*size,y+0.3*size);
    g.drawLine(x-0.2*size,y-0.2*size,x-0.2*size,y+0.2*size);
}

void GraphEditor::drawComponentBar(GraphicsAdapter &g,int compsize)
{
    int padding = barpadding * g.getCanvasHeight();
    int size = compsize;
    int minx = prototypes[0].getX();
    int maxx = prototypes.back().getX();
    int componentBarWidth = maxx + size/2 - minx + size/2;
    int componentBarHeight = size;
    int componentBarLeft = (g.getCanvasWidth() - componentBarWidth)/2;
    int componentBarTop = g.getCanvasHeight() - size - padding;
    g.setFillColorRGBf(0.6,0.4,0.9);
    g.fillRect(componentBarLeft,componentBarTop,componentBarWidth,componentBarHeight);
    int compy = componentBarTop+size/2;
    g.fillCircle(componentBarLeft,compy,size/2);
    g.fillCircle(componentBarLeft+componentBarWidth,compy,size/2);
    for(auto& comp:prototypes){
        comp.draw(g,size,componentBarLeft+size/2,compy,false);
    }
}

inline void GraphEditor::drawRestartButton(GraphicsAdapter& g,int x,int y,int size){
    g.setDrawColorRGBf(0.8,0.8,0.8);
    g.setLineWidth(2);
    double r = size/4;
    if(restartHovered){
        g.setFillColorRGBf(0.3,0.9,0.3);
    }else{
        g.setFillColorRGBf(0.1,0.7,0.1);
    }
    g.fillRect(x-size/2,y-size/2,size,size);
    g.drawArc(x,y,r,0,1.5*M_PI);
    int arrowlen = r/2;
    int xarrow = x+r;
    g.drawLine(xarrow,y,xarrow,y+arrowlen);
    g.drawLine(xarrow,y+arrowlen,xarrow-arrowlen,y);
    g.drawLine(xarrow,y+arrowlen,xarrow+arrowlen,y);
}

inline void GraphEditor::drawPauseButton(GraphicsAdapter& g,int x,int y,int size){
    g.setDrawColorRGBf(0.8,0.8,0.8);
    int ymin = y-0.3*size;
    int ymax = y + 0.3*size;
    int xleft = x - 0.1*size;
    int xright = x + 0.1*size;
    if(pauseHovered){
        g.setFillColorRGBf(0.3,0.3,0.9);
    }else{
        g.setFillColorRGBf(0.1,0.1,0.7);
    }
    g.fillRect(x-size/2,y-size/2,size,size);
    g.drawLine(xleft,ymin,xleft,ymax);
    g.drawLine(xright,ymin,xright,ymax);
}

inline void GraphEditor::drawUI(GraphicsAdapter &g)
{
    settingsPane.setSize(paneWidth,paneHeight);
    plotPane.setSize(paneWidth,paneHeight);
    xPlotPane = uimarginx;
    yPlotPane = uimarginy;
    plotPane.draw(g,xPlotPane,yPlotPane);
    xSettingsPane = g.getCanvasWidth()-paneWidth-uimarginx;
    ySettingsPane = uimarginy;
    settingsPane.draw(g,xSettingsPane,ySettingsPane);
    xPauseButton = g.getCanvasWidth()/2 + pauseButtonSize/2;
    yPauseButton = pauseButtonSize/2;
    xRestartButton = g.getCanvasWidth()/2 - restartButtonSize/2;
    yRestartButton = restartButtonSize/2;
    drawPauseButton(g,xPauseButton,yPauseButton,pauseButtonSize);
    drawRestartButton(g,xRestartButton,yRestartButton,restartButtonSize);
}

void GraphEditor::draw(GraphicsAdapter &g)
{
    int width = g.getCanvasWidth(),height = g.getCanvasHeight();
    if(width != oldWidth || height != oldHeight || oldWidth < 0 || oldHeight < 0){
        resize(width,height);
        oldWidth = width;
        oldHeight = height;
    }
    g.setFillColorRGBf(0.1,0.1,0.2);
    g.fillRect(0,0,g.getCanvasWidth(),g.getCanvasHeight());
    g.setDrawColorRGBf(0.1,0.1,0.1);
    g.setLineWidth(2);
    for(auto& comp : rendercomponents){
        comp.draw(g,renderComponentSize,0,0,true);
    }
    if(hoveredTerminal < 0 && hoveredComponent >= 0){
        int x = rendercomponents[hoveredComponent].getX();
        int y = rendercomponents[hoveredComponent].getY();
        int size = renderComponentSize;
        g.drawRect(x-size/2,y-size/2,size,size);
    }
    if(hoveredTerminal >= 0 && hoveredComponent >= 0){
        double x = 0,y = 0;
        auto& comp = rendercomponents[hoveredComponent];
        comp.getTermPosition(renderComponentSize,&x,&y,hoveredTerminal);
        double r = terminalRadius(renderComponentSize);
        g.drawRect(x - r,y - r,2*r,2*r);
    }
    if(selectedTerminal >= 0 && selectedComponent >= 0){
        double x = 0,y = 0;
        auto& comp = rendercomponents[selectedComponent];
        comp.getTermPosition(renderComponentSize,&x,&y,selectedTerminal);
        g.drawLine(x,y,mousex,mousey);
    }
    for(auto [comp1,term1,comp2,term2] : connections){
        auto& rcomp1 = rendercomponents[comp1];
        auto& rcomp2 = rendercomponents[comp2];
        double x1 = 0,y1 = 0,x2 = 0,y2 = 0;
        rcomp1.getTermPosition(renderComponentSize,&x1,&y1,term1);
        rcomp2.getTermPosition(renderComponentSize,&x2,&y2,term2);
        g.drawLine(x1,y1,x2,y2);
    }
    drawComponentBar(g,prototypeComponentSize);

    paneHeight = g.getCanvasHeight() - 2*uimarginy;
    drawUI(g);
}

void GraphEditor::resize(int width,int height)
{
    double compsizeheight = fmin(barheight * height,100.0);
    double compsizewidth = 0.9*width/(prototypes.size() + 1);
    double compsize = fmin(fmin(compsizeheight,compsizewidth),renderComponentSize);
    double padding = 0.1*compsize;
    initializePrototypes(compsize,padding);
    prototypeComponentSize = compsize;
}

void GraphEditor::drawGroundTerminal(GraphicsAdapter &g, int x, int y, int size)
{
    double r = GraphEditor::terminalRadius(size);
    g.fillCircle(x + size/2 + r,y,r);
}

bool GraphEditor::checkMousePlot(int x,int y){
    return xPlotPane < x && x < xPlotPane + plotPane.getWidth() &&
           yPlotPane < y && y < yPlotPane + plotPane.getHeight();
}

bool GraphEditor::checkMouseSettings(int x,int y){
    return xSettingsPane < x && x < xSettingsPane + settingsPane.getWidth() &&
           ySettingsPane < y && y < ySettingsPane + settingsPane.getHeight();
}

void GraphEditor::resetSimulation(){
    if(simulationIsRunning){
        simulationIsRunning = false;
        solver = CircuitSolver();
        for(auto& comp:rendercomponents)
            comp.reset();
    }
}


void GraphEditor::restartSimulation(){
    resetSimulation();
    for(int i = 1;i<rendercomponents.size();i++){
        solver.addComponent(rendercomponents[i].getModel());
    }
    int groundTerminalComp = 0;
    int groundTerminalTerm = 0;
    for(auto& [comp1,term1,comp2,term2]: connections){
        if(comp1 == 0){
            if(groundTerminalComp == 0){
                groundTerminalComp = comp2;
                groundTerminalTerm = term2;
                solver.setGroundTerminal({groundTerminalComp,groundTerminalTerm});
                continue;
            }
            comp1 = groundTerminalComp;
            term1 = groundTerminalTerm;
        }
        if(comp2 == 0){
            if(groundTerminalComp == 0){
                groundTerminalComp = comp1;
                groundTerminalTerm = term1;
                solver.setGroundTerminal({groundTerminalComp,groundTerminalTerm});
                continue;
            }
            comp2 = groundTerminalComp;
            term2 = groundTerminalTerm;
        }
        std::cout << "Connection: " << comp1 << ":" << term1 << "," <<  comp2 << ":" << term2 << std::endl;
        solver.addConnection({comp1,term1},{comp2,term2});
    }
    simulationIsRunning = solver.computeNodes();
}

void GraphEditor::simulationStep()
{
    if(simulationIsRunning && !simulationPaused){
        double time = solver.getSimulationTime();
        for(size_t i = 1;i<rendercomponents.size();i++){
            double voltage = solver.getVoltage({i,1}) - solver.getVoltage({i,0});
            double current = rendercomponents[i].getModel()->getCurrent();
            rendercomponents[i].getPlotCard()->registerValues(time,current,voltage);
        }
        solver.implicitEulerStep();
    }
}

void GraphEditor::clear()
{
    for(auto& comp : rendercomponents)
        delete comp.getModel();
    rendercomponents.resize(1);
    connections.clear();
}

void GraphEditor::addComponent(int x,int y,Component* model)
{
    PlotCard* p = new PlotCard();
    SettingsCard* s = new SettingsCard(model);
    rendercomponents.push_back(RenderComponent(x,y,model,p));
    plotPane.registerInfoCard(p);
    settingsPane.registerInfoCard(s);
}

void GraphEditor::loadRectifier()
{
    clear();
    Diode* diodes[4] = {new Diode(),new Diode(),new Diode(),new Diode()};
    Resistor* r = new Resistor(1000);
    ACVoltageSource *v = new ACVoltageSource(5,200);
    Capacitor* c = new Capacitor(120*1e-6);
    int x = 300;
    addComponent(x + 200,100,v);
    addComponent(x + 100,200,diodes[0]);
    addComponent(x + 300,200,diodes[1]);
    addComponent(x + 100,300,diodes[2]);
    addComponent(x + 300,300,diodes[3]);
    addComponent(x + 200,400,r);
    addComponent(x + 200,500,c);
    addConnection(0,0,1,0);
    addConnection(1,0,2,1);
    addConnection(1,1,3,0);
    addConnection(1,0,4,0);
    addConnection(1,1,5,1);
    addConnection(2,0,6,1);
    addConnection(3,1,6,0);
    addConnection(4,1,6,0);
    addConnection(5,0,6,1);
    addConnection(6,0,7,0);
    addConnection(6,1,7,1);
    focusComponent(8);
    simulationIsRunning = true;
    simulationPaused = false;
    restartSimulation();
}

void GraphEditor::mouseMoveEvent(int x,int y)
{
    plotPane.mouseMoveEvent(x - xPlotPane,y - yPlotPane);
    settingsPane.mouseMoveEvent(x - xSettingsPane,y - ySettingsPane);

    pauseHovered = xPauseButton - pauseButtonSize/2 <= x && x <= xPauseButton + pauseButtonSize/2 &&
                   yPauseButton - pauseButtonSize/2 <= y && y <= yPauseButton + pauseButtonSize/2;
    restartHovered = xRestartButton - restartButtonSize/2 <= x && x <= xRestartButton + restartButtonSize/2 &&
                     yRestartButton - restartButtonSize/2 <= y && y <= yRestartButton + restartButtonSize/2;
    if(pauseHovered || restartHovered) return;

    mousex = x;
    mousey = y;
    if(selectedComponent >= 0 and selectedTerminal < 0){
        if(selectedComponent != 0){
            auto& scomp = rendercomponents[selectedComponent];
            scomp.setX(x);
            scomp.setY(y);
        }
    }
    hoveredTerminal = -1;
    hoveredComponent = -1;
    for(int i = 0;i<rendercomponents.size();i++){
        auto& comp = rendercomponents[i];
        if(comp.checkMouse(renderComponentSize,x,y)){
            hoveredComponent = i;
            return;
        }
        int term = comp.checkMouseTerminal(renderComponentSize,x,y);
        if(term >= 0){
            hoveredTerminal = term;
            hoveredComponent = i;
            return;
        }
    }
}

void GraphEditor::focusComponent(size_t component)
{
    settingsPane.setVisible(true);
    plotPane.setVisible(true);
    settingsPane.setSize(paneWidth,paneHeight);
    plotPane.setSize(paneWidth,paneHeight);
    plotPane.focusInfoCard(component-1);
    settingsPane.focusInfoCard(component-1);
}

void GraphEditor::mousePressEvent(int x, int y, int button)
{
    if(button == GraphEditor::MOUSE_BUTTON_LEFT){
        if(pauseHovered && simulationIsRunning){
            simulationPaused = !simulationPaused;
            return;
        }
        if(restartHovered){
            simulationIsRunning = true;
            simulationPaused = false;
            restartSimulation();
            return;
        }
    }
    if(checkMousePlot(x,y) || checkMouseSettings(x,y)){
        plotPane.mousePressEvent(x - xPlotPane,y - yPlotPane,button);
        settingsPane.mousePressEvent(x - xSettingsPane,y - ySettingsPane,button);
    }else{
        plotPane.setVisible(false);
        settingsPane.setVisible(false);
    }
    if(button == GraphEditor::MOUSE_BUTTON_LEFT){
        if(selectedComponent < 0){
            int padding = barpadding * oldHeight;
            int size = prototypeComponentSize;
            int minx = prototypes[0].getX();
            int maxx = prototypes.back().getX();
            int componentBarWidth = maxx + size/2 - minx + size/2;
            int componentBarLeft = (oldWidth - componentBarWidth)/2;
            int componentBarTop = oldHeight - size - padding;
            int offsetx = componentBarLeft + size/2, offsety = componentBarTop + size/2;

            for(int i = 0;i<prototypes.size();i++){
                if(prototypes[i].checkMouse(prototypeComponentSize,x - offsetx,y - offsety)){
                    auto& pcomp = prototypes[i];
                    selectedComponent = rendercomponents.size();
                    int x = offsetx + pcomp.getX(), y = offsety + pcomp.getY();
                    Component* model = pcomp.getModel()->copy();
                    auto plotCard = new PlotCard();
                    rendercomponents.push_back(RenderComponent(x,y,model,plotCard));
                    plotPane.registerInfoCard(plotCard);
                    settingsPane.registerInfoCard(new SettingsCard(model));
                    resetSimulation();
                    break;
                }
            }
        }
        if(hoveredTerminal >= 0 && selectedTerminal >= 0){
            addConnection(selectedComponent,selectedTerminal,hoveredComponent,hoveredTerminal);
            hoveredTerminal = -1;
            selectedTerminal = -1;
            hoveredComponent = -1;
            selectedComponent = -1;
        }
        if(hoveredComponent >= 0){
            selectedComponent = hoveredComponent;
        }
        if(hoveredTerminal >= 0){
            selectedTerminal = hoveredTerminal;
        }else{
            selectedTerminal = -1;
        }
    }
    if(button == GraphEditor::MOUSE_BUTTON_RIGHT){
        if(hoveredComponent >= 1){
            focusComponent(hoveredComponent);
        }
    }
}

void GraphEditor::mouseReleaseEvent(int x, int y, int button)
{
    plotPane.mouseReleaseEvent(x - xPlotPane,y - yPlotPane,button);
    settingsPane.mouseReleaseEvent(x - xSettingsPane,y - ySettingsPane,button);
    if(button == GraphEditor::MOUSE_BUTTON_LEFT && selectedComponent >= 0 && selectedTerminal < 0){
        selectedComponent = -1;
    }
}
