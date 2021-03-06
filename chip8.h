#ifndef CHIP8_H
#define CHIP8_H

#define MEMORYSIZE 4096



class Chip8{
    public:
        Chip8();
        ~Chip8();
        void initialize();
        bool load(const char *fname);
        unsigned char getGFX();
        void emulateCycle();
        bool drawFlag;
        unsigned char gfx[64 * 32];
        unsigned char oldGfx[64 * 32];
        unsigned char key[16];
        unsigned char delay_timer;
        unsigned char sound_timer;
    private:
        unsigned short opcode;       // opcodes are two byes
        unsigned char memory[MEMORYSIZE];  // 4k memory
        unsigned char V[16];         // 16 8 bit registers
        unsigned short index;        //index register 12 bits
        unsigned short pc;           //program counter 12 bits
        
        
        unsigned short stack[16];
        unsigned char stackp;
        
        
};




#endif