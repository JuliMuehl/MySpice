#include <SDL2/SDL.h>
#include <iostream>

#include "grapheditor.h"
#include "sdl2/graphicssdl.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define FONT_PATH "/Roboto-Regular.ttf"
#else
#define FONT_PATH "/home/juli/MySpice/assets/Roboto-Regular.ttf"
#endif

bool running;
SDL_Renderer* renderer;
GraphEditor* editor;
GraphicsSDL* g;
SDL_Window* window;

void mainLoop()
{
    editor->simulationStep();
    editor->draw(*g);
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            running = false;
            return;
#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
#endif
        }
        if(event.type == SDL_MOUSEMOTION){
            editor->mouseMoveEvent(event.motion.x,event.motion.y);
        }
        if(event.type == SDL_MOUSEBUTTONDOWN){
            int button = -1;
            if(event.button.button == SDL_BUTTON_LEFT) button = GraphEditor::MOUSE_BUTTON_LEFT;
            if(event.button.button == SDL_BUTTON_RIGHT) button = GraphEditor::MOUSE_BUTTON_RIGHT;
            editor->mousePressEvent(event.motion.x,event.motion.y,button);
        }
        if(event.type == SDL_MOUSEBUTTONUP){
            int button = -1;
            if(event.button.button == SDL_BUTTON_LEFT) button = GraphEditor::MOUSE_BUTTON_LEFT;
            if(event.button.button == SDL_BUTTON_RIGHT) button = GraphEditor::MOUSE_BUTTON_RIGHT;
            editor->mouseReleaseEvent(event.motion.x,event.motion.y,button);
        }
    }
    SDL_RenderPresent(renderer);
}
int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    window = SDL_CreateWindow(
        "MySpice",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1200, 600,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
    );

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    TTF_Init();
    g = new GraphicsSDL(renderer,FONT_PATH,12);
    editor = GraphEditor::getInstance();
    editor->loadRectifier();
    running = true;
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop,0,1);
    return 0;
#else
    while(running){
        mainLoop();
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
#endif
}
