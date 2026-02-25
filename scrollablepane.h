#ifndef SCROLLABLEPANE_H
#define SCROLLABLEPANE_H

#include "graphicsadapter.h"
#include "mouseeventobserver.h"
#include "infocard.h"
#include <vector>
#include <memory>
#include <cmath>

class ScrollablePane : MouseEventObserver
{
    int width,height;
    int scrollbarWidth;
    int scrollbarHeight;
    double scrollpercent = 0.0;
    double r;
    int paneWidth;
    int paneHeight;
    bool scrolling = false;
    bool hovering = false;
    int lastx = 0,lasty = 0;
    int totalheight = 0;
    int fontheight;
    bool deletedCard = false;
    constexpr static int paddingy = 10;
    constexpr static double rPercent = 0.05;
    std::vector<InfoCard*> infoCards;
    bool isVisible = false;
    std::tuple<int, int> getScrollPosition(int x, int y);
public:
    inline int getWidth(){
        return width;
    }
    inline int getHeight(){
        return height;
    }
    inline void setSize(int width,int height){
        if(this->width != width || this->height!= height){
            this->width = width;
            this->height = height;
            r = rPercent*width;
            paneWidth = width - 2*r;
            paneHeight = height - 2*r;
            scrollbarWidth = 0.5*rPercent*width;
        }
    }
    ScrollablePane(int totalWidth,int totalHeight){
        setSize(totalWidth,totalHeight);
    }
    ScrollablePane():ScrollablePane(0,0){}
    void draw(GraphicsAdapter& g,int x,int y);

    void setVisible(bool isVisible){
        this->isVisible = isVisible;
    }

    inline size_t registerInfoCard(InfoCard* infoCard){
        infoCards.push_back(infoCard);
        return infoCards.size() - 1;
    }

    inline void removeInfoCard(size_t i){
        delete infoCards[i];
        infoCards.erase(infoCards.begin() + i);
        deletedCard = true;
        if(i > 0){
            focusInfoCard(i-1);
        }else{
            if(infoCards.size() > 0)
                focusInfoCard(0);
        }
    }

    inline void focusInfoCard(size_t i){
        totalheight = 0;
        for(auto& card : infoCards){
            totalheight += card->getHeight(width-2*r,fontheight) + paddingy;
        }
        if(totalheight > paneHeight){
            int y = 0;
            for(int j = 0;j<i;j++){
                int dy = infoCards[j]->getHeight(width-2*r,fontheight) + paddingy;
                y += dy;
            }
            scrollpercent = fmax(fmin((double) y / (totalheight - paneHeight),1.0),0.0);
        }else{
            scrollpercent = 0.0;
        }
    }

    // MouseEventObserver interface
    void mouseMoveEvent(int x, int y);
    void mousePressEvent(int x, int y, int button);
    void mouseReleaseEvent(int x, int y, int button);
private:
};


#endif // SCROLLABLEPANE_H
