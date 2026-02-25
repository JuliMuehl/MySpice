#include "scrollablepane.h"
#include <cmath>
#include <tuple>

inline std::tuple<int,int> ScrollablePane::getScrollPosition(int x,int y){
    int ypane = y + r;
    int xscroll = x+width-scrollbarWidth;
    int yscroll = ypane + scrollpercent*(paneHeight-scrollbarHeight);
    return {xscroll,yscroll};
}

void ScrollablePane::draw(GraphicsAdapter &g, int x, int y)
{
    fontheight = g.getFontHeight();
    totalheight = 0;
    for(auto& infoCard : infoCards){
        int deltay = infoCard->getHeight(width-2*r,fontheight) + paddingy;
        totalheight += deltay;
    }
    if(totalheight >= paneHeight){
        double frac = (double) paneHeight / totalheight;
        scrollbarHeight =  paneHeight * frac;
    }else{
        scrollbarHeight = paneHeight;
    }
    if(isVisible){
        int xpane = x + r,ypane = y + r;
        g.setFillColorRGBf(0.8,0.8,0.8);
        g.fillRoundedRect(x,y,width,height,r);
        g.pushClippingRect(xpane,ypane,paneWidth,paneHeight);
        int yi = ypane;
        yi -= (totalheight - paneHeight) * scrollpercent;
        for(auto& infoCard : infoCards){
            infoCard->draw(g,width-2*r,xpane,yi);
            int deltay = infoCard->getHeight(width-2*r,g.getFontHeight()) + paddingy;
            yi += deltay;
        }
        g.popClippingRect();

        if(scrolling || hovering)
            g.setFillColorRGBf(0.3,0.3,0.3);
        else
            g.setFillColorRGBf(0.6,0.6,0.6);
        auto [xscroll,yscroll] = getScrollPosition(x,y);
        g.fillRoundedRect(xscroll,yscroll,scrollbarWidth,scrollbarHeight,scrollbarWidth/2.0);
    }
}

inline void ScrollablePane::mouseMoveEvent(int x, int y)
{
    if(isVisible){
        auto [xscroll,yscroll] = getScrollPosition(0,0);
        double x0 = xscroll,x1 = xscroll + scrollbarWidth;
        double y0 = yscroll,y1 = yscroll + scrollbarHeight;
        hovering = x0 <= x && x <= x1 && y0 <= y && y <= y1;
        if(scrolling && totalheight > paneHeight){
            int dy = y - lasty;
            double dpercent = (double) dy / (paneHeight - scrollbarHeight);
            scrollpercent += dpercent;
            scrollpercent = fmin(fmax(scrollpercent,0.0),1.0);
        }
        lasty = y;

        int offsety = (totalheight - paneHeight) * scrollpercent;
        int ycard = r - offsety;
        int xcard = r;
        for(auto& infoCard : infoCards){
            infoCard->mouseMoveEvent(x-xcard,y-ycard);
            int deltay = infoCard->getHeight(width-2*r,fontheight) + paddingy;
            ycard += deltay;
        }
    }
}

void ScrollablePane::mousePressEvent(int x, int y, int button)
{
    if(isVisible){
        if(button == MOUSE_BUTTON_LEFT){
            if(!scrolling){
                auto [xscroll,yscroll] = getScrollPosition(0,0);
                double x0 = xscroll,x1 = xscroll + scrollbarWidth;
                double y0 = yscroll,y1 = yscroll + scrollbarHeight;
                scrolling = x0 <= x && x <= x1 && y0 <= y && y <= y1;
            }else{
                scrolling = false;
            }
        }
        int offsety = (totalheight - paneHeight) * scrollpercent;
        int ycard = r - offsety;
        int xcard = r;
        for(auto& infoCard : infoCards){
            infoCard->mousePressEvent(x-xcard,y-ycard,button);
            if(deletedCard){
                deletedCard = false;
                break;
            }
            int deltay = infoCard->getHeight(width-2*r,fontheight) + paddingy;
            ycard += deltay;
        }
    }
}

inline void ScrollablePane::mouseReleaseEvent(int x, int y, int button)
{
    if(isVisible){
        if(scrolling) {
            scrolling = false;
        }
        int offsety = (totalheight - paneHeight) * scrollpercent;
        int ycard = r - offsety;
        int xcard = r;
        for(auto& infoCard : infoCards){
            infoCard->mouseReleaseEvent(x-xcard,y-ycard,button);
            int deltay = infoCard->getHeight(width-2*r,fontheight) + paddingy;
            ycard += deltay;
        }
    }
}

