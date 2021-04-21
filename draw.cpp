#include <stdlib.h>
#include <stdio.h>
#include "include/SDL2/SDL.h"
#include "draw.h"

Draw::Draw(int size){
    this->size = size;
}

Draw::~Draw(){
}

// Create the window and the renderer
void Draw::initialize(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(64 *size, 32 * size, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    r.w = size;
    r.h = size;
    SDL_RenderPresent(renderer);
}
//draw at point (x,y)
void Draw::drawPixel(int x, int y){
    r.x = x*size;
    r.y = y*size;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect( renderer, &r );

    SDL_RenderPresent(renderer);
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    //SDL_RenderClear(renderer);
}

void Draw::drawWindow(unsigned char gfx[]){
    for (int i = 0; i < 64*32;i++){
        int x = i % 64;
        int y = i/64;
        //printf("%d", gfx[i]);
        printf("draw\n");
        if(gfx[i] == 1){
            drawPixel(x,y);
        }
    }
}