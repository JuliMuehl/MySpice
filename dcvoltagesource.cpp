#include "dcvoltagesource.h"
#include "grapheditor.h"

void DCVoltageSource::draw(GraphicsAdapter &g, int x, int y, int size) noexcept{
    GraphEditor::drawDCVoltageSource(g,x,y,size);
}
