#include "chip8.h"
#include <iostream>
#include "draw.h"
#include <time.h>
#include <chrono>
#include <Windows.h>





int main(int argv, char** args) {
    
    Chip8 my_Chip8;
    Draw window(20);
    my_Chip8.initialize();
    my_Chip8.load("roms\\tetris.c8");
    window.initialize();
    double cycles = 0;
    double instructions = 0;
    double curInstr = 0;
    double clockspeed = 500;
    double lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    while (1){
        double curTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        
        cycles += (curTime - lastTime) * (clockspeed/1000.0);
        //printf("cycles: %f\n", (curTime - lastTime));
        //printf("%d\n", (curInstr - instructions)/(curTime - lastTime));
        lastTime = curTime;
        //printf("%f\n", (int)(clockspeed/60));
        if (curInstr - instructions > (clockspeed/60)){
            if(my_Chip8.delay_timer > 0){
                my_Chip8.delay_timer--;
            }
            if(my_Chip8.sound_timer > 0){
                my_Chip8.sound_timer--;
            }
            instructions = curInstr;
        }
        if (cycles > 0){
            curInstr++;
            cycles--;
            my_Chip8.emulateCycle();
            if (my_Chip8.drawFlag){
                window.drawWindow(my_Chip8.gfx, my_Chip8.oldGfx);
                my_Chip8.drawFlag = false;
            }
            //window.getInput(my_Chip8.key);
        }
        else{
            Sleep(16);
        }
        window.getInput(my_Chip8.key);
        
        
        
    }
    
    printf("pogchamp\n");
    
    return 0;
    
}