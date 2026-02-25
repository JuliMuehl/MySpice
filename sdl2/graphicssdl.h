#ifndef GRAPHICSSDL_H
#define GRAPHICSSDL_H

#include "graphicsadapter.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <array>

class GraphicsSDL : public GraphicsAdapter
{
private:
    Uint8 drawR,drawG,drawB;
    Uint8 fillR,fillG,fillB;
    Uint8 lineWidth = 1;
    SDL_Renderer* renderer;
    void loadFillColor();
    void loadDrawColor();
    std::vector<SDL_Rect> clippingRects;
    const char* fontPath;
    TTF_Font* font;
    std::vector<SDL_Point> linePoints;
    std::array<TTF_Font*,25> fonts = {0};
public:
    GraphicsSDL(SDL_Renderer* renderer,const char* fontPath,int fontSize) :
        renderer(renderer),fontPath(fontPath){
        setFontSize(fontSize);
    }
    // GraphicsAdapter interface
public:
    void setFillColorRGBf(double r, double g, double b);
    void setDrawColorRGBf(double r, double g, double b);
    void setLineWidth(int);
    void setFontSize(int);
    void getTextSize(int *txtw, int *txth, const char *txt);
    void drawText(const char *txt, int x, int y);
    void drawLine(int x0, int y0, int x1, int y1);
    void drawArc(int xcenter, int ycenter, double radius, double angle0, double angle1);
    void drawRect(int x, int y, int width, int height);
    void fillRect(int x, int y, int width, int height);
    void fillCircle(int xcenter, int ycenter, double radius);
    void pushClippingRect(int x, int y, int width, int height);
    void popClippingRect();
    int getCanvasWidth();
    int getCanvasHeight();
    int getFontHeight();
    void drawLines(int *x, int *y, int n);
};

#endif // GRAPHICSSDL_H
