#include "chip8.h"
#include <iostream>
#include "draw.h"




int main(int argv, char** args) {
    Chip8 my_Chip8;
    Draw window(20);
    my_Chip8.initialize();
    my_Chip8.load("roms\\pong.c8");
    window.initialize();
    while (1){
        my_Chip8.emulateCycle();
        if (my_Chip8.drawFlag){
            window.drawWindow(my_Chip8.gfx, my_Chip8.oldGfx);
            my_Chip8.drawFlag = false;
        }
        window.getInput(my_Chip8.key);
    }
    
    printf("pogchamp\n");
    
    return 0;
    
}