#ifndef GRAPHICSQT_H
#define GRAPHICSQT_H

#include "graphicsadapter.h"

#include <QPainter>
#include <QRect>

class GraphicsQT : public GraphicsAdapter
{
    QPainter* painter;
    QRect r;
    std::vector<QRegion> clippingRegions;
public:
    GraphicsQT(QPainter* painter,QRect r) : painter(painter), r(r){
        setDrawColorRGBf(0,0,0);
        setFillColorRGBf(0,0,0);
    }
    // GraphicsAdapter interface
public:
    void setFillColorRGBf(double r, double g, double b) override;
    void setDrawColorRGBf(double r, double g, double b) override;
    void setLineWidth(int) override;
    void setFontSize(int) override;
    void drawLine(int x0, int y0, int x1, int y1) override;
    void drawText(const char *txt, int x, int y) override;

    void drawArc(int xcenter, int ycenter, double radius, double angle0, double angle1) override;
    void drawRect(int x, int y, int width, int height) override;
    void fillRect(int x, int y, int width, int height) override;
    void fillCircle(int xcenter, int ycenter, double radius) override;
    int getCanvasWidth() override;
    int getCanvasHeight() override;
    void pushClippingRect(int x, int y, int width, int height) override;
    void popClippingRect() override;
    void getTextSize(int *txtw, int *txth, const char *txt) override;
    int getFontHeight() override;
};

#endif // GRAPHICSQT_H
