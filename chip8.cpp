#include "chip8.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned char chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8(){

}

Chip8::~Chip8(){

}

// reset program counter, opcode, stack, index register
// load fontset
void Chip8::initialize(){
    std::cout << "Initialized";
    drawFlag = false;
    pc = 0x200;
    opcode = 0;
    index = 0;
    stackp = 15;

    //reset timers
    delay_timer = 0;
    sound_timer = 0;

    // clear display
    for (int i = 0; i < 2048; i++){
        gfx[i] = 0;
    }

    // clear stack
    for (int i = 0; i < 16; i++){
        stack[i] = 0;
    }

    // clear memory
    for (int i = 0; i < 4096; i++){
        memory[i] = 0;
    }
    //clear registers
    for (int i = 0; i < 16; i++){
        V[i] = 0;
    }
    // load fontset
    for (int i = 0; i < 80; i++){
        memory[i] = chip8_fontset[i];
    }
    srand(time(0));

}

bool Chip8::load(const char *fname){
    //open the game file
    FILE *game = fopen(fname, "rb");
    printf("opened %s\n", fname);
    if (game == NULL){
        fprintf(stderr, "File not found");
        exit(1);
    }

    // Get the size of the file
    fseek(game, 0 , SEEK_END);
    long filesize = ftell(game);
    rewind(game);

    //copy the file into a buffer
    char *buffer = (char*)malloc(sizeof(char) * filesize);
    if (buffer == NULL){
        fprintf(stderr, "Buffer Error");
        exit(1);
    }
    fread(buffer, sizeof(char),filesize, game);

    //copy the buffer into chip8 memory
    if (filesize - 3584){//Memory size is 4096, program counter starts at 0x200 = 3584 bytes
        for(int i = 0; i < filesize; i++){
            memory[i+512] = buffer[i];
        }
    }
    else{
        fprintf(stderr, "ROM not valid");
    }
    
    printf("loaded %s\n", fname);
    //free memory
    fclose(game);
    free(buffer);


}


void Chip8::emulateCycle(){
    // fetch opcode
    // opcodes are two bytes
    opcode = memory[pc] << 8 |memory[pc + 1];
    printf("%x\n", opcode);
    switch(opcode & 0xf000){
        case 0x0000: //00E0 or 00EE
            switch(opcode & 0x000f){
                case 0x0000: // (00E0) clear screen
                    for (int i = 0; i < 2048; i++){
                        gfx[i] = 0;
                    }
                    drawFlag = true;
                    pc+=2;
                    break;
                case 0x000e: // (00EE) return from subroutine
                    stackp++;
                    pc = stack[stackp];
                    pc+=2;
                    break;
            }
            break;
        case 0x1000: // (1NNNN) Jump to address NNN 
            pc = (opcode & 0x0FFF);
            break;
        case  0x2000: // (2NNNN) Calls subroutine at NNN (need to save current address)
            stack[stackp] = pc;
            stackp--;
            pc = (opcode & 0x0FFF);
            break;
        case 0x3000: // (3XNN) Skips next instruction if register VX == NN
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)){
                pc+=2;
            }
            pc+=2;
            break;
        case  0x4000://( 4XNN) Skips next instruction if register VX doesnt equal NN)
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)){
                pc+=2;
            }
            pc+=2;
            break;
        case 0x5000:// (5XY0) Skips next instruction if register VX equals VY
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]){
                pc+=2;
            }
            pc+=2;
            break;
        case 0x6000:// (6XNN) set register VX to NN
            V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
            pc+=2;
            break;
        case 0x7000:// (7XNN) Adds NN to VX (don't set carry flag)
            V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
            pc+=2;
            break;
        case 0x8000: 
            switch(opcode & 0x000F){
                case 0x0000://(8XY0) set register VX to VY
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc+=2;
                    break;
                case 0x0001: //(8XY1) Set register VX to VX | VY
                    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4]);
                    pc+=2;
                    break;	
                case 0x0002: //(8XY2) Set register VX to VX & VY
                    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4]);
                    pc+=2;
                    break;	
                case 0x0003: //(8XY3) Set register VX to VX ^ VY
                    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4]);
                    pc+=2;
                    break;	
                case 0x0004: //(8XY4) ADD register VY to VX. Set carry flag
                {
                    unsigned char temp = (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]);
                    if (temp < V[(opcode & 0x0F00) >> 8]){
                        V[15] = 1;
                    }
                    else{
                        V[15] = 0;
                    }
                    V[(opcode & 0x0F00) >> 8] = temp;
                    pc+=2;
                    	
                }
                    break;
                case 0x0005: //(8XY5) Subtract register VY from VX. Set carry flag
                    
                    if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]){
                        V[15] = 1;
                    }
                    else{
                        V[15] = 0;
                    }
                    V[(opcode & 0x0F00) >> 8]= (V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4]);
                    pc+=2;
                    break;	
                case 0x0006:// 8XY6 Store the least significant bit of VX in VF then shift right 1
                    V[15] = (V[(opcode & 0x0F00) >> 8] & 0x0001);
                    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] >> 1);
                    pc+=2;
                    break;
                case 0x0007:// (8XY7) VX = VY - VX 
                    
                    if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]){
                        V[15] = 0;
                    }
                    else{
                        V[15] = 1;
                    }
                    V[(opcode & 0x0F00) >> 8]= (V[(opcode & 0x00F0) >> 8] - V[(opcode & 0x0F00) >> 4]);
                    pc+=2;
                    break;	
                case 0x000E:// 8XYE Store the most significant bit of VX in VF then shift left 1
                    V[15] = ((V[(opcode & 0x0F00) >> 8] & 0x80) >> 7);
                    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] << 1);
                    pc+=2;
                    break;

            }
            break;
        case 0x9000:// (9XY0) skips next if instruction if VX != VY
            if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]){
                pc+=2;
            }
            pc+=2;
            break;
        case 0xA000:// (ANNN) Sets index register to NNN
            index = (opcode & 0x0FFF);
            pc+=2;
            break;
        case 0xB000:// (BNNN) PC = V0 + NNN
            pc = (opcode & 0x0FFF) + V[0];
            pc+=2;
            break;
        case 0xC000:// (CXNN) VX = rand()&NN
            V[(opcode & 0x0f00) >> 8] = (rand() & (opcode & 0x00ff));
        case 0xD000:// (DXYN) Draw a sprite at coordinate (VX, VY)
                    // N is the height of the sprite
                    // The sprite is 8 bits wide
        {
            unsigned char x = V[(opcode & 0x0f00) >> 8] % 64; // get x coordinate
            unsigned char y = V[(opcode & 0x00f0) >> 4] % 32; // y coordinate
            V[15] = 0;
            for (int i = 0; i < (opcode & 0x000f); i++){
                unsigned char pixel = memory[index + i];
                for (int j = 0; j < 8; j++){ 
                    if (((pixel & (0x80 >> j)) >> (7-j)) == 1){ //get single pixel from 8 pixel row
                        if (gfx[(y + i) * 64 + (x + j)] == 1){ //pixel is turned from on to off
                            V[15] = 1;
                        } 
                        gfx[(y + i) * 64 + (x + j)] ^=1; // flip the bit
                    }
                    
                }
            }
            pc+=2;
            drawFlag = true;
        }
            break;
        case 0xE000:
            switch (opcode & 0x000F){
                case 0x000E:// (EX9E) if key() == VX skip next instruction
                    if (key[V[(opcode & 0x0f00) >> 8]] == 1){
                        pc+=2;
                    }
                    pc+=2;
                    break;
                case 0x0001:// (EXA1) if key() != VX skip next instruction
                    if (key[V[(opcode & 0x0f00) >> 8]] == 0){
                        pc+=2;
                    }
                    pc+=2;
                    break;
            }
        case 0xF000:
            switch (opcode & 0x00FF){
                case 0x0007:// (FX07) vx = delay timer
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc+=2;
                    break;
                case 0x000A:// (FX0A) CHANGE THIS ONEEE
                    {
					bool keyPress = false;

					for(int i = 0; i < 16; ++i)
					{
						if(key[i] != 0)
						{
							V[(opcode & 0x0F00) >> 8] = i;
							keyPress = true;
						}
					}

					// If we didn't received a keypress, skip this cycle and try again.
					if(!keyPress)						
						return;

					pc += 2;					
				}
                    
                    break;
                case 0x0015:// (FX15) delay timer = vx
                    delay_timer = V[(opcode & 0x0F00) >> 8];
                    pc+=2;
                    break;
                case 0x0018:// (FX18)
                    sound_timer = V[(opcode & 0x0F00)>> 8];
                    pc+=2;
                    break;
                case 0x001E:// (FX1E)
                    index += (V[(opcode & 0x0F00) >> 8]);
                    pc+=2;
                    break;
                case 0x0029:// (FX29)
                    index = (V[((opcode & 0x0f00) >> 8)] * 5);
                    pc+=2;
                    break;
                case 0x0033:// (FX33)
                    memory[index+2] = V[(opcode & 0x0f00) >> 8] % 10;
                    memory[index+1]= (V[(opcode & 0x0f00) >> 8] / 10) % 10;
                    memory[index]= (V[(opcode & 0x0f00) >> 8] / 100) % 10;
                    pc+=2;
                    break;
                case 0x0055:// (FX55)
                    for (int i = index; i - index <= ((opcode & 0x0f00) >> 8); i++){
                        memory[i] = V[i-index];
                    }
                    pc+=2;
                    break;
                case 0x0065:// (FX65)
                    for (int i = index; i - index <= ((opcode & 0x0f00) >> 8); i++){
                        V[i-index] = memory[i];
                    }
                    pc+=2;
                    break;
            }

    }

}