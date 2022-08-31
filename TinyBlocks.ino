#include <Arduboy2.h>
#include <ATMlib.h>
#include "song.h"
#include "myfunctions.h"
#define frCount 3

int frameNumber = 0;
long int myDelay;
long int tempTime;
int dropTime = 0;
int slideTime = 0;
byte gameMode = 0;
bool paused = 0;
byte menuItem;
bool bgmusic = 1;

byte px, py, ps, pr; // player x,y,shape,rotation
byte nextTile;
byte lineCount, level;
int score, lines;
bool okToContinue = 0;

// Make an instance of arduboy used for many functions
Arduboy2 arduboy;
ATMsynth ATM;

#define ROW_DRIVE_WAIT_US 150
static constexpr uint8_t FBW = 128;
static constexpr uint8_t FBH = 64;
int FULLBUFF = FBH * FBW / 8;
static constexpr uint8_t FBP = 8;

// Support a minimum panel refresh rate of 120Hz
// This also effectively limits the refresh rate to 120Hz because the controller is not allowed to start
// a new frame faster than the period we set.
static uint16_t timer_counter = ((F_CPU/64)/(120-1));

static void send_cmds(uint8_t const* d, uint8_t n)
{
    Arduboy2::LCDCommandMode();
    while(n-- != 0)
        Arduboy2::SPItransfer(*d++);
    Arduboy2::LCDDataMode();
}

static void send_cmds_prog(uint8_t const* d, uint8_t n)
{
    Arduboy2::LCDCommandMode();
    while(n-- != 0)
        Arduboy2::SPItransfer(pgm_read_byte(d++));
    Arduboy2::LCDDataMode();
}

void paint(uint8_t image[], bool clear)
{
   uint16_t count;
  
   asm volatile (
       "   ldi   %A[count], %[len_lsb]               \n\t" //for (len = WIDTH * HEIGHT / 8)
       "   ldi   %B[count], %[len_msb]               \n\t"
       "1: ld    __tmp_reg__, %a[ptr]      ;2        \n\t" //tmp = *(image)
       "   out   %[spdr], __tmp_reg__      ;1        \n\t" //SPDR = tmp
       "   cpse  %[clear], __zero_reg__    ;1/2      \n\t" //if (clear) tmp = 0;
       "   mov   __tmp_reg__, __zero_reg__ ;1        \n\t"
       "2: sbiw  %A[count], 1              ;2        \n\t" //len --
       "   sbrc  %A[count], 0              ;1/2      \n\t" //loop twice for cheap delay
       "   rjmp  2b                        ;2        \n\t"
       "   st    %a[ptr]+, __tmp_reg__     ;2        \n\t" //*(image++) = tmp
       "   brne  1b                        ;1/2 :18  \n\t" //len > 0
       "   in    __tmp_reg__, %[spsr]                \n\t" //read SPSR to clear SPIF
       : [ptr]     "+&e" (image),
         [count]   "=&w" (count)
       : [spdr]    "I"   (_SFR_IO_ADDR(SPDR)),
         [spsr]    "I"   (_SFR_IO_ADDR(SPSR)),
         [len_msb] "M"   ((FBP * FBW * 2) >> 8),   // 8: pixels per byte
         [len_lsb] "M"   ((FBP * FBW * 2) & 0xFF), // 2: for delay loop multiplier
         [clear]   "r"   (clear)
    );
}


void setup() {
  arduboy.audio.on();
  ATM.play(music);
  
  arduboy.begin();
  screenBuffer = arduboy.getBuffer();
  //arduboy.setFrameRate(126); // way too high so that it doesn't mess with our other timing.
  //myDelay = 7430; // starting delay
  for (char y = 0; y < 16; y++) {
    for (char x = 0; x < 32; x++) {
      drawMiniTile(x, y, tileGFX + (frameNumber * 4));
    }
  }
  okToContinue = 0;


    static uint8_t const SETUP_CMDS[] PROGMEM =
    {
        0x21, (128 - FBW) / 2, 127 - (128 - FBW) / 2,
        0x22, 0, FBP - 1,
        0x7F,
        0xD5, 0xF0,
        0xD9, 0x22, // Use default precharge cycle settings
    };
    send_cmds_prog(SETUP_CMDS, sizeof(SETUP_CMDS));

    // disable timer0 overflow ISR (cannot use micros/millis/delay)
    bitWrite(TIMSK0, TOIE0, 0);
    
    TCCR3A = 0;
    TCCR3B = _BV(WGM32) | _BV(CS31) | _BV(CS30); // CTC mode, prescaler /64
    OCR3A = timer_counter;
    bitWrite(TIMSK3, OCIE3A, 1);

}

void loop() {
/*
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  while (micros() - tempTime < myDelay) {}
  tempTime = micros();
  arduboy.display();
    //Serial.write(screenBuffer, 128 * 8);

  frameNumber++;
  if (frameNumber == 3) {
    frameNumber = 0;
    UpdatePad(arduboy.buttonsState());
  }

  char oldMode = gameMode;
  switch (gameMode) {
    case 0:
      titleScreen();
      break;
    case 1:
      if (paused) {
        pauseMenu();
      } else {
        playGame();
      }
      break;
    case 3:
      gameOver();
      break;
  }
  if (oldMode != gameMode) {
    // clear screen on mode change only (too slow)
    arduboy.clear();
    clearPlayfield();
    if (gameMode == 0 || gameMode == 3) {
      for (char y = 0; y < 16; y++) {
        for (char x = 0; x < 32; x++) {
          drawMiniTile(x, y, tileGFX + (frameNumber * 4));
        }
      }
    }

  }
*/
    if (WDTCSR & _BV(WDE))
    {
        // disable ints and set magic key
        cli();
        *(volatile uint8_t*)0x800 = 0x77;
        *(volatile uint8_t*)0x801 = 0x77;
        for(;;);
    }

    // timer3 will still wake CPU
    Arduboy2::idle();


}

ISR(TIMER3_COMPA_vect)
{

      // Use NOPs (0xE3) for simple and precise delay
      static uint8_t const CONTRAST_CMDS0[] PROGMEM = { 0x81, 0xFF, };
      static uint8_t const CONTRAST_CMDS1[] PROGMEM = { 0x81, 0x80, };
      static uint8_t const LOCK_CMDS0[] PROGMEM = { 0x81, 0xFF, 0xA8, 63,
        0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3,
        0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3,
        0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3,
        0xE3, 0xE3, 0xE3, 0xE3, 0xE3,
        0xE3, 0xE3,
        0xA8, 0};

      TCCR3B = 0; // Stop Timer
      TCNT3 = 0; // Reset the counter

      OCR3A = timer_counter;

      paint(screenBuffer, false);

#if 1
      send_cmds_prog(CONTRAST_CMDS0, sizeof(CONTRAST_CMDS0));
#else
      if (n & 1)
          send_cmds_prog(CONTRAST_CMDS1, sizeof(CONTRAST_CMDS1));
      else
          send_cmds_prog(CONTRAST_CMDS0, sizeof(CONTRAST_CMDS0));
#endif

      send_cmds_prog(LOCK_CMDS0, sizeof(LOCK_CMDS0));
      TCCR3B = _BV(WGM32) | _BV(CS31) | _BV(CS30); // restart timer, prescaler /64




  frameNumber++;
  if (frameNumber == 3) {
    frameNumber = 0;
    UpdatePad(arduboy.buttonsState());
  }

  char oldMode = gameMode;
  switch (gameMode) {
    case 0:
      titleScreen();
      break;
    case 1:
      if (paused) {
        pauseMenu();
      } else {
        playGame();
      }
      break;
    case 3:
      gameOver();
      break;
  }
  if (oldMode != gameMode) {
    // clear screen on mode change only (too slow)
    arduboy.clear();
    clearPlayfield();
    if (gameMode == 0 || gameMode == 3) {
      for (char y = 0; y < 16; y++) {
        for (char x = 0; x < 32; x++) {
          drawMiniTile(x, y, tileGFX + (frameNumber * 4));
        }
      }
    }

  }


}

void drawShape(byte x1, signed char y1, byte shape, byte frame) {
  for (char y = 0; y < 4; y++) {
    if (y1 + y >= 0) {
      for (char x = 0; x < 4; x++) {
        byte mt = pgm_read_byte(shapeMap + (x + 4 * y) + shape * 64 + (frame * 16));
        if (mt > 1) {
          drawMiniTile(x1 + x, y1 + y, tileGFX + (mt * 12) + (frameNumber * 4));
        }
      }
    }
  }
}

void clearPlayfield() {
  for (char y = 0; y < 19; y++) {
    for (char x = 3; x < 13; x++) {
      playfield[x + 16 * y] = 0;
    }
  }
}


void drawPlayfield() {
  for (char y = 3; y < 19; y++) {
    for (char x = 1; x < 15; x++) {
      byte mt = playfield[x + 16 * y];
      drawMiniTile(x + 8, y - 3, tileGFX + mt + (frameNumber * 4));
    }
  }

  char text[] = "                       ";
  print(4, 1 , "SCORE");
  sprintf(text, "%05d", score);
  print(4, 2 , text);

  print(100, 1 , "LEVEL");
  sprintf(text, "%3d", level);
  print(104, 2 , text);

  print(100, 4 , "LINES");
  sprintf(text, "%3d", lines);
  print(104, 5 , text);

  print(4, 4 , "NEXT");

}


void checkLine() {
  if (py <= 1) {
    gameMode = 3;  // gameOver if off top of screen
  }

  score++; // increase score here as it's called whenever a tile drops

  for (char y = 0; y < 19; y++) {
    char line = 0;
    for (char x = 3; x < 13; x++) {
      line += playfield[x + 16 * y] != 0 ? 1 : 0;
    }
    if (line == 10) { // remove line
      lineCount++;
      if (lineCount == 10) {
        lineCount = 0;
        level++;
      }
      lines++;
      score += 10;
      for (char y1 = y; y > 0; y--) {
        for (char x = 2; x < 14; x++) {
          playfield[x + 16 * y] = playfield[x + 16 * (y - 1)];
        }
      }
    }
  }

}

bool check(signed char x1, signed char y1, char rot) {
  for (char y = 0; y < 4; y++) {
    if (y1 + y >= 0) {
      for (char x = 0; x < 4; x++) {
        byte mt = pgm_read_byte(shapeMap + (x + 4 * y) + ps * 64 + (rot * 16));
        if (mt > 1) {
          if ( playfield[(x1 + x) + 16 * (y1 + y)] != 0) {
            return 1;
          }
        }
      }
    }
  }
  return 0;
}

void stamp(signed char x1, signed char y1, byte shape, byte rot) {

  for (char y = 0; y < 4; y++) {
    if (y1 + y >= 0) {
      for (char x = 0; x < 4; x++) {
        byte mt = pgm_read_byte(shapeMap + (x + 4 * y) + shape * 64 + (rot * 16));
        if (mt > 1) {
          playfield[(x1 + x) + 16 * (y1 + y)] = mt * 12;
        }
      }
    }
  }
}

void pauseMenu() {

  //drawPlayfield();
  for (char y = 3; y < 8; y++) {
    for (char x = 1; x < 15; x++) {
      byte mt = playfield[x + 16 * y];
      drawMiniTile(x + 8, y - 3, tileGFX + mt + (frameNumber * 4));
    }
  }
  char y = 18;
  for (char x = 1; x < 15; x++) {
    byte mt = playfield[x + 16 * y];
    drawMiniTile(x + 8, y - 3, tileGFX + mt + (frameNumber * 4));
  }

  char tl = 0;
  // paused box
  for (char y = 0; y < 10; y++) {
    for (char x = 0; x < 14; x++) {
      tl = 0;
      if (y == 0 || y == 9) {
        tl = 3;
      }
      if (x == 0 || x == 13) {
        tl = 6;
      }
      if (y == 0 && x == 0) {
        tl = 10;
      }
      if (y == 0 && x == 13) {
        tl = 8;
      }
      if (y == 9 && x == 0) {
        tl = 9;
      }
      if (y == 9 && x == 13) {
        tl = 11;
      }
      //if(tl!=0)
      drawMiniTile(x + 9, y + 5, tileGFX + tl * 12 + (frameNumber * 4));
    }
  }

  print(52, 3 , "Paused", 1);
  print(52, 4 , "Continue", 1);
  print(52, 5 , "Music", 1);
  print(52, 6 , "Quit", 1);
  print(44, 4 + menuItem, "*", 1);
  if (bgmusic) {
    print(76, 5 , "On", 1);
  } else {
    print(76, 5 , "Off", 1);
  }

  if (!_A[HELD] && !_B[HELD]) {
    okToContinue = 1;
  }
  if (_A[NEW] && okToContinue) {
    switch (menuItem) {
      case 0: // return to game
        okToContinue = 0;
        paused = 0;
        arduboy.clear();
        break;
      case 1: // sound on/off
        bgmusic = 1 - bgmusic;
        break;
      case 2: // quit
        okToContinue = 0;
        paused = 0;
        score = 0;
        lines = 0;
        level = 0;
        gameMode = 0;
        break;
    }
  }

  if (frameNumber == 0) {
    if (_Up[NEW] && menuItem > 0) {
      menuItem--;
    }
    if (_Down[NEW] && menuItem < 2) {
      menuItem++;
    }
  }
}

void playGame() {
  if (frameNumber % frCount == 0) {

    if (_Left[NEW]) {
      if (check(px - 1, py, pr) == 0) {
        px--;
        slideTime = 0;
      }
    }
    if (_Right[NEW]) {
      if (check(px + 1, py, pr) == 0) {
        px++;
        slideTime = 0;
      }
    }
    if (_Left[HELD] && slideTime++ > 12) {
      if (check(px - 1, py, pr) == 0) {
        px--;
        slideTime = 12;
      }
    }
    if (_Right[HELD] && slideTime++ > 12) {
      if (check(px + 1, py, pr) == 0) {
        px++;
        slideTime = 12;
      }
    }

    if (_Down[HELD] && dropTime > 1) { // replace with timer
      dropTime = 0;
      if (check(px, py + 1, pr) == 0) {
        py++;
      } else {
        // place shape and create new one
        stamp(px, py, ps, pr);
        checkLine();
        py = 0; px = 6; ps = nextTile; nextTile = random(7); pr = 0;
      }
    }

    if (dropTime++ > 40 - (level * 4)) {
      dropTime = 0;
      if (check(px, py + 1, pr) == 0) {
        py++;
        dropTime = 0;
      } else {
        // place shape and create new one
        stamp(px, py, ps, pr);
        checkLine();
        py = 0; px = 6; ps = nextTile; nextTile = random(7); pr = 0;
      }
    }


    if (_A[NEW]) {
      if (check(px, py, (pr - 1) & 3) == 0) {
        pr--;
      } else if (check(px - 1, py, (pr - 1) & 3) == 0) {
        pr--; px--;
      } else if (check(px + 1, py, (pr - 1) & 3) == 0) {
        pr--; px++;
      } else if (check(px - 2, py, (pr - 1) & 3) == 0) {
        pr--; px -= 2;
      } else if (check(px + 2, py, (pr - 1) & 3) == 0) {
        pr--; px += 2;
      }
      pr &= 3;
    }

    if (_B[NEW]) {
      if (check(px, py, (pr + 1) & 3) == 0) {
        pr++;
      } else if (check(px - 1, py, (pr + 1) & 3) == 0) {
        pr++; px--;
      } else if (check(px + 1, py, (pr + 1) & 3) == 0) {
        pr++; px++;
      } else if (check(px - 2, py, (pr + 1) & 3) == 0) {
        pr++; px -= 2;
      } else if (check(px + 2, py, (pr + 1) & 3) == 0) {
        pr++; px += 2;
      }
      pr &= 3;
    }

    if (_A[HELD] && _B[HELD]) {
      menuItem = 0;
      paused = 1;
    }

  }


  // render screen
  drawPlayfield();
  drawShape(px + 8, py - 3, ps, pr);
  // next shape?
  drawShape(1, 11, nextTile, 0);

  /*
    char text[] = "               ";
    sprintf(text, "X:%d Y:%d  ",px,py);
    print(0, 0 , text);
  */

}

void titleScreen() {
  if (!_A[HELD]) {
    okToContinue = 1;
  }
  if (_A[NEW] && okToContinue) {
    okToContinue = 0;
    // set up fresh game
    px = 6; py = 0;
    score = 0;
    lines = 0;
    randomSeed(micros());
    ps = random(7);
    nextTile = random(7);
    level = 0;
    gameMode = 1;
  }
/*
    // adjust the delay if we see flickering
    if(_Up[HELD]){myDelay+=10;}
    if(_Down[HELD]){myDelay-=10;}
    if(_Left[NEW]){myDelay--;}
    if(_Right[NEW]){myDelay++;}
    if(myDelay<10){myDelay=1;}
*/

  for (char y = 0; y < 13; y++) {
    for (char x = 0; x < 32; x++) {
      byte mt = pgm_read_byte(titleMap + (x + 32 * y));
      if (mt != 1) drawMiniTile(x, y + 1, tileGFX + (mt * 12) + (frameNumber * 4));
    }
  }

  print(52, 7, "  PRESS A TO START ", 1);

  /*
    char text[] = "                       ";
    sprintf(text, "S:%d",myDelay);
    print(0, 7 , text);
  */
}

void gameOver() {
  if (!_A[HELD]) {
    okToContinue = 1;
  }
  if (_A[NEW] && okToContinue) {
    okToContinue = 0;
    gameMode = 0;
  }

  for (char y = 0; y < 15; y++) {
    for (char x = 0; x < 32; x++) {
      byte mt = pgm_read_byte(gameOverMap + (x + 32 * y));
      if (mt != 1) drawMiniTile(x, y, tileGFX + (mt * 12) + (frameNumber * 4));
    }
  }

}
