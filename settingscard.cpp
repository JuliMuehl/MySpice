#include "settingscard.h"

#include "grapheditor.h"

void SettingsCard::updateValues(){
    for(int i = 0;i<unitSliders.size();i++){
        model->setField(i,unitSliders[i].getValue());
    }
}

int SettingsCard::getHeight(int width, int fontheight)
{
    double r = roundingRadius(width);
    buttonwidth = width-2*r;
    buttonheight = fontheight;
    double totalHeight = 2*r + iconSize + buttonheight;
    for(const auto& u : unitSliders){
        totalHeight += u.getHeight(width,fontheight) + sliderPadding;
    }
    totalHeight -= sliderPadding;
    return totalHeight;
}

void SettingsCard::draw(GraphicsAdapter &g, int width, int x, int y)
{
    this->width = width;
    double r = roundingRadius(width);
    fontheight = g.getFontHeight();
    int height = getHeight(width,fontheight);
    g.setFillColorRGBf(0.1,0.1,0.1);
    g.fillRoundedRect(x,y,width,height,r);
    g.setDrawColorRGBf(0.9,0.9,0.9);
    model->draw(g,x+width/2,y+r+iconSize/2,iconSize);
    int yi = iconSize + y + r;
    for(auto& u : unitSliders){
        u.draw(g,width,x,yi);
        yi += u.getHeight(width,fontheight) + sliderPadding;
    }

    if(removeHovered){
        g.setFillColorRGBf(1.0,0.3,0.3);
    }else{
        g.setFillColorRGBf(0.7,0.1,0.1);
    }
    g.fillRoundedRect(x+r,yi,buttonwidth,buttonheight,buttonheight/2);
    g.getTextSize(&txtw,&txth,rmtxt);
    int txtx = x + (width - txtw)/2,txty = yi+fontheight;
    g.drawText(rmtxt,txtx,txty);
}

void SettingsCard::mouseMoveEvent(int x, int y)
{
    double r = roundingRadius(width);

    int yi = iconSize + r;
    for(auto& u : unitSliders){
        u.mouseMoveEvent(x,y-yi);
        yi += u.getHeight(width,fontheight) + sliderPadding;
    }

    int buttonx = (width - txtw)/2,buttony = yi;
    removeHovered = buttonx <= x && x <= buttonx + buttonwidth &&
                    buttony <= y && y <= buttony + buttonheight ;

}

void SettingsCard::mousePressEvent(int x, int y, int button)
{
    double r = roundingRadius(width);

    int yi = iconSize + r;
    for(auto& u : unitSliders){
        u.mousePressEvent(x,y-yi,button);
        yi += u.getHeight(width,fontheight) + sliderPadding;
    }
    if(button == MOUSE_BUTTON_LEFT && removeHovered){
        GraphEditor::getInstance()->eraseComponent(model);
    }
}

void SettingsCard::mouseReleaseEvent(int x, int y, int button)
{
    double r = roundingRadius(width);
    int yi = iconSize + r;
    for(auto& u : unitSliders){
        u.mouseReleaseEvent(x,y-yi,button);
        yi += u.getHeight(width,fontheight) + sliderPadding;
    }
    updateValues();
}
