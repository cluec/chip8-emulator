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

//clear at point (x,y)
void Draw::clearPixel(int x, int y){
    r.x = x*size;
    r.y = y*size;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect( renderer, &r );
    SDL_RenderPresent(renderer);
    
}

void Draw::drawWindow(unsigned char gfx[], unsigned char oldGfx[]){
    for (int i = 0; i < 64*32;i++){
        int x = i % 64;
        int y = i/64;
        //printf("%d", gfx[i]);
        
        if(gfx[i] == 1 ){
            drawPixel(x,y);
        }
        else if(gfx[i] == 0 && oldGfx[i] == 1){
            clearPixel(x,y);
        }
        oldGfx[i] = gfx[i];
    }
}

void Draw::getInput(unsigned char key[]){
    SDL_Event event;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    while(SDL_PollEvent(&event)){
        for (int i = 0; i < 16; i++){
            key[i] = 0;
        }
    if (keys[SDL_SCANCODE_1]){
        key[0] = 1;
    }
    if (keys[SDL_SCANCODE_2]){
        key[1] = 1;
    }
    if (keys[SDL_SCANCODE_3]){
        key[2] = 1;
    }
    if (keys[SDL_SCANCODE_4]){
        key[3] = 1;
    }
    if (keys[SDL_SCANCODE_Q]){
        key[4] = 1;
    }
    if (keys[SDL_SCANCODE_W]){
        key[5] = 1;
    }
    if (keys[SDL_SCANCODE_E]){
        key[6] = 1;
    }
    if (keys[SDL_SCANCODE_R]){
        key[7] = 1;
    }
    if (keys[SDL_SCANCODE_A]){
        key[8] = 1;
    }
    if (keys[SDL_SCANCODE_S]){
        key[9] = 1;
    }
    if (keys[SDL_SCANCODE_D]){
        key[10] = 1;
    }
    if (keys[SDL_SCANCODE_F]){
        key[11] = 1;
    }
    if (keys[SDL_SCANCODE_Z]){
        key[12] = 1;
    }
    if (keys[SDL_SCANCODE_X]){
        key[13] = 1;
    }
    if (keys[SDL_SCANCODE_C]){
        key[14] = 1;
    }
    if (keys[SDL_SCANCODE_V]){
        key[15] = 1;
    }
    
    if (event.type == SDL_QUIT){
        exit(0);
    }
    }

}