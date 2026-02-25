#ifndef SETTINGSCARD_H
#define SETTINGSCARD_H

#include "infocard.h"
#include "unitslider.h"
#include "component.h"
#include <vector>

class SettingsCard : public InfoCard
{
    Component* model;
    std::vector<UnitSlider> unitSliders;
    int width,fontheight;
    bool removeHovered = false;
    constexpr static int iconSize = 50;
    constexpr static int sliderPadding = 10;
    const char* rmtxt = "Remove Component";
    int txtw,txth;
    int buttonwidth,buttonheight;
public:
    // InfoCard interface
    SettingsCard(Component* model): model(model){
        const auto& fnames = model->getFieldNames();
        const auto& unitNames = model->getFieldUnits();
        for(int i = 0;i<fnames.size();i++){
            unitSliders.push_back(UnitSlider(fnames[i],unitNames[i]));
            unitSliders.back().setValue(model->getFieldValue(i));
        }
    }
    int getHeight(int width, int fontheight);
    void draw(GraphicsAdapter &g, int width, int x, int y);
    // MouseEventObserver interface
    void mouseMoveEvent(int x, int y);
    void mousePressEvent(int x, int y, int button);
    void mouseReleaseEvent(int x, int y, int button);
private:
    void updateValues();
};



#endif // SETTINGSCARD_H
