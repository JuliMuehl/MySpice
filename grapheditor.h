#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include "graphicsadapter.h"
#include "rendercomponent.h"
#include "mouseeventobserver.h"
#include "scrollablepane.h"
#include "circuitsolver.h"

#include <array>
#include <resistor.h>
#include <vector>

class GraphEditor : public MouseEventObserver
{
    std::array<RenderComponent,6> prototypes;
    std::vector<RenderComponent> rendercomponents = {RenderComponent(40,40,nullptr,nullptr)};
    std::vector<std::tuple<size_t,size_t,size_t,size_t>> connections;
    bool shouldDrawUI = false;
    ScrollablePane plotPane, settingsPane;

    int oldWidth = -1,oldHeight = -1;
    int prototypeComponentSize = -1;
    int renderComponentSize = 80;

    constexpr static double barheight = 0.2;
    constexpr static double barpadding = barheight/2.0;
    constexpr static double editorheight = 1.0 - (barheight + barpadding);
    constexpr static int paneWidth = 250;
    int paneHeight;
    constexpr static int uimarginy = 50,uimarginx = 20;

    int hoveredComponent = -1;
    int selectedComponent = -1;
    int hoveredTerminal = -1;
    int selectedTerminal = -1;
    int mousex = -1,mousey = -1;
    int xPlotPane,yPlotPane;
    int xSettingsPane,ySettingsPane;
    constexpr static int restartButtonSize = 50;
    constexpr static int pauseButtonSize = 50;
    int xRestartButton,yRestartButton;
    int xPauseButton,yPauseButton;
    bool restartHovered = false,pauseHovered = false;
    bool simulationIsRunning = false,simulationPaused = false;
    CircuitSolver solver;

    void addConnection(size_t comp1,size_t term1,size_t comp2,size_t term2);
    void eraseComponent(size_t comp);
    void initializePrototypes(int componentSize, int padding);
    void resize(int width,int height);
    void drawComponentBar(GraphicsAdapter& g, int compsize);
    void drawUI(GraphicsAdapter& g);
    static inline GraphEditor* instance = nullptr;
    GraphEditor(){}
public:
    static void drawGroundTerminal(GraphicsAdapter& g,int x,int y,int size);
    static void drawEndpointTerminals(GraphicsAdapter& g,int x,int y,int size);
    static void drawCapacitor(GraphicsAdapter& g,int x,int y,int size);
    static void drawResistor(GraphicsAdapter& g,int x,int y,int size);
    static void drawInductor(GraphicsAdapter& g,int x,int y,int size);
    static void drawDiode(GraphicsAdapter& g,int x,int y,int size);
    static void drawDCVoltageSource(GraphicsAdapter& g,int x,int y,int size);
    static void drawACVoltageSource(GraphicsAdapter& g,int x,int y,int size);
    static void drawGround(GraphicsAdapter&g,int x,int y,int size);


    static inline GraphEditor* getInstance(){
        if(instance == nullptr)
            instance = new GraphEditor();
        return instance;
    }
    inline void eraseComponent(Component* comp){
        for(size_t i = 1;i<rendercomponents.size();i++){
            if(rendercomponents[i].getModel() == comp){
                eraseComponent(i);
                break;
            }
        }
    }
    
    void clear();
    void loadRectifier();

    void draw(GraphicsAdapter& g);

    static double terminalRadius(int size);
    void focusComponent(size_t);
    void addComponent(int x,int y, Component* model);
    void mouseMoveEvent(int x,int y) override;
    void mousePressEvent(int x, int y, int button) override;
    void mouseReleaseEvent(int x, int y, int button) override;
    void simulationStep();
private:
    bool checkMousePlot(int x, int y);
    bool checkMouseSettings(int x, int y);
    void drawPauseButton(GraphicsAdapter &g, int x, int y, int size);
    void drawRestartButton(GraphicsAdapter &g, int x, int y, int size);
    void restartSimulation();
    void resetSimulation();
};
#endif // GRAPHEDITOR_H
