#ifndef GRAPHICSADAPTER_H
#define GRAPHICSADAPTER_H

class GraphicsAdapter
{
public:
    virtual void setFillColorRGBf(double r,double g,double b) = 0;
    virtual void setDrawColorRGBf(double r,double g,double b) = 0;
    virtual void setLineWidth(int) = 0;
    virtual void setFontSize(int) = 0;
    virtual void getTextSize(int* txtw,int* txth,const char* txt) = 0;
    virtual void drawText(const char* txt,int x,int y) = 0;
    virtual void drawLine(int x0,int y0,int x1,int y1) = 0;
    virtual void drawLines(int* x,int* y, int n){
        for(int i = 1;i<n;i++){
            drawLine(x[i-1],y[i-1],x[i],y[i]);
        }
    }
    virtual void drawArc(int xcenter,int ycenter,double radius,double angle0, double angle1) = 0;
    virtual void drawRect(int x,int y,int width,int height) = 0;
    virtual void fillRect(int x,int y,int width,int height) = 0;
    virtual void fillCircle(int xcenter,int ycenter,double radius) = 0;
    virtual void pushClippingRect(int x,int y,int width,int height) = 0;
    virtual void popClippingRect() = 0;
    virtual void fillRoundedRect(int x,int y,int width,int height,double r){
        fillCircle(x+r,y+r,r);
        fillCircle(x+width-r,y+r,r);
        fillCircle(x+r,y+height-r,r);
        fillCircle(x+width-r,y+height-r,r);
        fillRect(x,y+r,width,height-2*r);
        fillRect(x+r,y,width-2*r,r);
        fillRect(x+r,y+height-r,width-2*r,r);
    }
    virtual int getCanvasWidth() = 0;
    virtual int getCanvasHeight() = 0;
    virtual int getFontHeight() = 0;
};

#endif // GRAPHICSADAPTER_H
