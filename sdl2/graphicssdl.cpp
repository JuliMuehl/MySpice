#include "graphicssdl.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL_ttf.h"
#include <iostream>
void GraphicsSDL::loadFillColor()
{
    SDL_SetRenderDrawColor(renderer,fillR,fillG,fillB,255);
}

void GraphicsSDL::loadDrawColor()
{
    SDL_SetRenderDrawColor(renderer,drawR,drawG,drawB,255);
}

void GraphicsSDL::setDrawColorRGBf(double r, double g, double b)
{
    drawR = r*255;
    drawG = g*255;
    drawB = b*255;
}

void GraphicsSDL::setFillColorRGBf(double r, double g, double b)
{
    fillR = r*255;
    fillG = g*255;
    fillB = b*255;
}

void GraphicsSDL::setLineWidth(int width)
{
    lineWidth = (Uint8) width;
}

void GraphicsSDL::setFontSize(int fontSize)
{
    if(0 <= fontSize && fontSize < fonts.size()){
        if(fonts[fontSize] != nullptr)
            font = fonts[fontSize];
        else{
            font = TTF_OpenFont(fontPath,fontSize);

            std::cout << font << std::endl;
            fonts[fontSize] = font;
        }
    }
}

void GraphicsSDL::getTextSize(int *txtw, int *txth, const char *txt)
{
    TTF_SizeText(font,txt,txtw,txth);
}

void GraphicsSDL::drawText(const char *txt, int x, int y)
{
    loadDrawColor();
    SDL_Surface* txtsurface = TTF_RenderText_Blended(font, txt, {drawR,drawG,drawB,255});
    SDL_Texture* txttexture = SDL_CreateTextureFromSurface(renderer, txtsurface);
    SDL_Rect txtrect = {
        .x = x,
        .y = y - txtsurface->h,
        .w = txtsurface->w,
        .h = txtsurface->h
    };
    SDL_RenderCopy(renderer,txttexture,NULL,&txtrect);
    SDL_FreeSurface(txtsurface);
    SDL_DestroyTexture(txttexture);
}

void GraphicsSDL::drawLine(int x0, int y0, int x1, int y1)
{
    loadDrawColor();
    SDL_Point points[2] = {{x0,y0},{x1,y1}};
    SDL_RenderDrawLines(renderer,points,2);
}

void GraphicsSDL::drawLines(int* x,int* y,int n)
{
    loadDrawColor();
    linePoints.clear();
    for(int i = 0;i<n;i++){
        linePoints.push_back({.x = x[i],.y = y[i]});
    }
    SDL_RenderDrawLines(renderer,linePoints.data(),linePoints.size());
}


void GraphicsSDL::drawArc(int xcenter, int ycenter, double radius, double angle0, double angle1)
{
    angle0 += 0;
    angle1 += 0;
    int n = 128;
    int x0 = xcenter + radius * cosf(angle0), y0 = ycenter + radius*sinf(angle0);
    for(int i = 0;i<n;i++){
        double theta = angle0 + i*(angle1 - angle0) / n;
        int x1 = xcenter + radius*cosf(theta), y1 = ycenter - radius*sinf(theta);
        drawLine(x0,y0,x1,y1);
        x0 = x1;
        y0 = y1;
    }
}

void GraphicsSDL::drawRect(int x, int y, int width, int height)
{
    loadDrawColor();
    SDL_Rect r = {.x=x,.y=y,.w=width,.h=height};
    SDL_RenderDrawRect(renderer,&r);
}

void GraphicsSDL::fillRect(int x, int y, int width, int height)
{
    loadFillColor();
    SDL_Rect r = {.x=x,.y=y,.w=width,.h=height};
    SDL_RenderFillRect(renderer,&r);
}

void GraphicsSDL::fillCircle(int xcenter, int ycenter, double radius)
{
    filledCircleRGBA(renderer,xcenter,ycenter,radius,fillR,fillG,fillB,255);
}

void GraphicsSDL::pushClippingRect(int x, int y, int width, int height)
{
    if(SDL_RenderIsClipEnabled(renderer)){
        SDL_Rect clip;
        SDL_RenderGetClipRect(renderer, &clip);
        clippingRects.push_back(clip);
    }
    SDL_Rect clip = {.x=x,.y=y,.w=width,.h=height};
    SDL_RenderSetClipRect(renderer,&clip);
}

void GraphicsSDL::popClippingRect()
{
    if(!clippingRects.empty()){
        SDL_RenderSetClipRect(renderer,&clippingRects.back());
        clippingRects.pop_back();
    }else{
        SDL_RenderSetClipRect(renderer,NULL);
    }
}

int GraphicsSDL::getCanvasWidth()
{
    int w,h;
    SDL_GetRendererOutputSize(renderer,&w,&h);
    return w;
}

int GraphicsSDL::getCanvasHeight()
{
    int w,h;
    SDL_GetRendererOutputSize(renderer,&w,&h);
    return h;
}

int GraphicsSDL::getFontHeight()
{
    return TTF_FontHeight(font);
}

