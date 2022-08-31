#include "gfx.h"
unsigned char * screenBuffer;


// Button updates
#define HELD 0
#define NEW 1
#define RELEASE 2
byte CompletePad, ExPad, TempPad;
bool _A[3], _B[3], _Up[3], _Down[3], _Left[3], _Right[3];

void UPDATEPAD(int pad, int var) {
  _B[pad] = (var & B_BUTTON) >> 2; // 4
  _A[pad] = (var & A_BUTTON) >> 3; // 8
  _Down[pad] = (var & DOWN_BUTTON) >> 4; // 16
  _Left[pad] = (var & LEFT_BUTTON) >> 5; // 32
  _Right[pad] = (var & RIGHT_BUTTON) >> 6; // 64
  _Up[pad] = (var & UP_BUTTON) >> 7; // 128
}
void UpdatePad(int joy_code)
{
  ExPad = CompletePad;
  CompletePad = joy_code;
  UPDATEPAD(HELD, CompletePad); // held
  UPDATEPAD(RELEASE, (ExPad & (~CompletePad))); // released
  UPDATEPAD(NEW, (CompletePad & (~ExPad))); // newpress
}



void drawTile(int16_t x, int16_t y, const uint8_t *bitmap) {
  // draws 8x8 tile
  for (byte t = 0; t < 8; t++) {
    screenBuffer[(x * 8) + (y * 128) + t] = pgm_read_byte(bitmap + t);
  }
}


void drawMiniTile(int16_t x, int16_t y, const uint8_t *bitmap) {
  // draws 4x4 tile
  byte y1 = y&1;
  byte y2 = y/2;
  byte myByte;
  int offset = (x*4) + (y2 * 128);
  if(y1){
    for (byte t = 0; t < 4; t++) {
        screenBuffer[offset + t] = (screenBuffer[offset + t]&0x0f) | ((pgm_read_byte(bitmap + t) & 0x0F) << 4);
    }  
  }else{
    for (byte t = 0; t < 4; t++) {
        screenBuffer[offset + t] = (screenBuffer[offset + t]&0xf0) | (pgm_read_byte(bitmap + t) & 0x0F);
    }  
  }

}



// print text
void print(char x, char y, const char* text, bool color = 0) {
  if(color==1){
    for (char t = 0; t < strlen(text); t++) {
      char character = text[t] - 32;
      for (byte t1 = 0; t1 < 4; t1++) {
        if(x+t1>=0 && x+t1<=127){screenBuffer[x + (y * 128) + t1] = ~(pgm_read_byte(myFont + ((character * 3) + t1)) << 2) & 255;}
      }
      if(x+3>=0 && x+3<=127){screenBuffer[x + (y * 128) + 3] = 255;}
      x += 4;
    }
  }else{
    for (char t = 0; t < strlen(text); t++) {
      char character = text[t] - 32;
      for (byte t1 = 0; t1 < 4; t1++) {
        if(x+t1>=0 && x+t1<=127){screenBuffer[x + (y * 128) + t1] = (pgm_read_byte(myFont + ((character * 3) + t1)) << 2) & 255;}
      }
      if(x+3>=0 && x+3<=127){screenBuffer[x + (y * 128) + 3] = 0;}
      x += 4;
    }
  }
}


