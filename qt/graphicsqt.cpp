#include "graphicsqt.h"
#include <QBrush>
#include <QColor>

void GraphicsQT::setFillColorRGBf(double r, double g, double b)
{
    QBrush brush = painter->brush();
    brush.setColor(QColor::fromRgb((int)(r*255),(int)(g*255),(int)(b*255),255));
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
}

void GraphicsQT::setDrawColorRGBf(double r, double g, double b)
{
    QPen pen = painter->pen();
    pen.setColor(QColor::fromRgb(r*255,g*255,b*255));
    painter->setPen(pen);
}

void GraphicsQT::setLineWidth(int width)
{
    QPen pen = painter->pen();
    pen.setWidth(width);
    painter->setPen(pen);
}

void GraphicsQT::setFontSize(int size)
{
    QFont font = painter->font();
    font.setPointSize(size);
    painter->setFont(font);
}

void GraphicsQT::drawLine(int x0, int y0, int x1, int y1)
{
    QLine line = QLine(x0 + r.left(),y0 + r.top(),x1,y1);
    painter->drawLine(line);
}

void GraphicsQT::drawText(const char *txt, int x, int y)
{
    QString qtxt = QString(txt);
    painter->drawText(x + r.left(),y + r.top(),qtxt);
}

void GraphicsQT::drawRect(int x, int y, int width, int height)
{
    QBrush brush = painter->brush();
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(x+r.left(),y + r.top(),width,height);
    painter->setBrush(brush);
}

void GraphicsQT::drawArc(int xcenter, int ycenter,double radius,double angle1, double angle2)
{
    double angle16 = (angle1/M_PI)*180 * 16;
    double angle16diff = (angle2-angle1)/M_PI*180*16;
    painter->drawArc(xcenter + r.left() - radius,ycenter + r.top() - radius,2*radius,2*radius,angle16,angle16diff);
}

void GraphicsQT::fillRect(int x, int y, int width, int height)
{

    QColor color = painter->brush().color();
    painter->fillRect(x,y,width,height,painter->brush());
}

void GraphicsQT::fillCircle(int xcenter, int ycenter, double radius)
{
    QPen pen = painter->pen();
    painter->setPen(Qt::NoPen);
    QPointF center = QPoint(xcenter + r.left(),ycenter+r.top());
    painter->drawEllipse(center,radius,radius);
    painter->setPen(pen);
}

int GraphicsQT::getCanvasWidth()
{
    return r.width();
}

int GraphicsQT::getCanvasHeight()
{
    return r.height();
}

void GraphicsQT::pushClippingRect(int x, int y, int width, int height)
{
    clippingRegions.push_back(painter->clipRegion());
    painter->setClipRegion(QRegion(x,y,width,height));
}

void GraphicsQT::popClippingRect()
{
    if(clippingRegions.size() > 0){
        auto r = clippingRegions.back();
        auto clipOperation = clippingRegions.size() == 1 ? Qt::NoClip : Qt::ReplaceClip;
        painter->setClipRegion(r,clipOperation);
        clippingRegions.pop_back();
    }
}

void GraphicsQT::getTextSize(int *txtw, int *txth, const char *txt)
{
    auto font = painter->font();
    QFontMetrics fm(font);
    auto r = fm.boundingRect(txt);
    *txtw = r.width();
    *txth = r.height();
}

int GraphicsQT::getFontHeight()
{
    auto font = painter->font();
    QFontMetrics fm(font);
    return fm.height();
}
