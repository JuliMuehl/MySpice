#include "rendercomponent.h"
#include "grapheditor.h"

void RenderComponent::draw(GraphicsAdapter &g, int size, int offsetx, int offsety, bool drawTerminals)
{
    g.setDrawColorRGBf(0.7,0.7,0.7);
    g.setFillColorRGBf(0.7,0.7,0.7);
    if(isGround()){
        if(drawTerminals) {
            GraphEditor::drawGroundTerminal(g,x+offsetx,y+offsety,size);
        }
        GraphEditor::drawGround(g,x+offsetx,y+offsety,size);
    }else{
        if(drawTerminals && model->getNumTerminals() == 2){

            GraphEditor::drawEndpointTerminals(g,x+offsetx,y+offsety,size);
        }
        model->draw(g,x+offsetx,y+offsety,size);
    }
}

void RenderComponent::getTermPosition(int size,double* termx,double* termy,int i){
    double r = GraphEditor::terminalRadius(size);
    int numTerminals = isGround() ? 1:model->getNumTerminals();
    if(numTerminals == 1){
        *termx = x + size/2 + r;
        *termy = y;
    }
    if(numTerminals == 2){
        if(i == 0){
            *termx = x - size/2.0 - r;
            *termy = y;
        }
        else if(i == 1){
            *termx = x + size/2 + r;
            *termy = y;
        }
    }
}

int RenderComponent::checkMouseTerminal(int size,int mousex,int mousey){
    int numTerminals = isGround() ? 1:model->getNumTerminals();
    for(int i = 0;i<numTerminals;i++){
        double termx = 0,termy = 0;
        getTermPosition(size,&termx,&termy,i);
        double r = GraphEditor::terminalRadius(size);
        if(termx - r <= mousex && mousex <= termx + r &&
           termy - r <= mousey && mousey <= termy + r){
            return i;
        }
    }

    return -1;
}
