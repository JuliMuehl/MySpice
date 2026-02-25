#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "component.h"
#include "plotcard.h"
#include <cmath>

class RenderComponent
{
public:
    inline int getX(){
        return x;
    }
    inline int getY(){
        return y;
    }
    inline void setX(int x){
        this->x = x;
    }
    inline void setY(int y){
        this->y = y;
    }
    inline Component* getModel(){
        return model;
    }
    inline PlotCard* getPlotCard(){
        return plotCard;
    }
    inline bool checkMouse(int size,int mousex,int mousey){
        return std::abs(mousex - x) <= size/2.0 && std::abs(mousey - y) <= size/2.0;
    }
    void reset(){
        if(plotCard != nullptr)
            plotCard->reset();
        if(model != nullptr)
            model->reset();
    }
    int checkMouseTerminal(int size, int mousex, int mousey);

    inline bool isGround(){
        return model == nullptr;
    }
    void clear();
    void draw(GraphicsAdapter& g, int size, int offsetx, int offsety, bool drawTerminals);
    RenderComponent(int x,int y,Component* modelp, PlotCard* plotCard):
        x(x),y(y),
        model(modelp),
        plotCard(plotCard){}
    RenderComponent():model(nullptr),plotCard(nullptr),x(0),y(0){}
    void getTermPosition(int size,double *termx, double *termy, int i);
private:
    int x,y;
    Component* model;
    PlotCard* plotCard;
};

#endif // RENDERCOMPONENT_H
