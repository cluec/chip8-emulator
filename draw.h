#ifndef DRAW_H
#define DRAW_H

#include "include/SDL2/SDL.h"

class Draw{
    public:
        Draw(int size);
        ~Draw();
        void initialize();
        void drawPixel(int x, int y);
        void clearPixel(int x, int y);
        void loop();
        void drawWindow(unsigned char gfx[],unsigned char oldGfx[]);
        void getInput(unsigned char key[]);
    private:
        SDL_Event event;
        SDL_Renderer *renderer;
        SDL_Window *window;
        SDL_Rect r;
        int size;
};

#endif