#include "chip8.h"
#include <iostream>
#include "draw.h"




int main(int argv, char** args) {
    Chip8 my_Chip8;
    Draw window(20);
    my_Chip8.initialize();
    my_Chip8.load("roms\\BC_test.ch8");
    window.initialize();
    for (int i = 0; i <100000; i++){
        my_Chip8.emulateCycle();
        if (my_Chip8.drawFlag){
            window.drawWindow(my_Chip8.gfx);
            my_Chip8.drawFlag = false;
        }
    }
    
    printf("pogchamp\n");
    
    return 0;
    
}