#include "Arduino.h"
#include "Wire.h"

#define BASEHTADDR 0x70
#define HT16K33_SS            B00100000 // System setup register
#define HT16K33_SS_NORMAL     B00000001 // System setup - oscillator in normal mode
#define HT16K33_DSP           B10000000 // Display setup
#define HT16K33_DSP_ON        B00000001 // Display setup - display on
#define HT16K33_DSP_NOBLINK   B00000000 // Display setup - no blink
#define HT16K33_RIS           B10100000 // ROW/INT Set
#define HT16K33_RIS_OUT       B00000000 // Set INT as row driver output
#define HT16K33_DIM           B11100000 // Dimming set
#define HT16K33_DIM_16        B00001111 // Dimming set - 16/16
#define HT16K33_KDAP          B01000000 // Key Address Data Pointer

class unitech_4ch_segment {  
  uint8_t _keyram[3];
  uint8_t oldKeyData[3];
public:
  void tcaselect(uint8_t i2c_bus); 
  bool is_key1_press, is_key2_press, is_key3_press, is_key4_press, is_decimal1=1, is_decimal2=1, is_decimal3=1, is_decimal4=1, is_mmwc;
  bool is_LED[10]={0,0,0,0,0,0,0,0,0,0};
  bool begin();
  void w_display_segment(int val);
  void w_display_segment(String dis1, int dis1_decimal_place, String dis2, int dis2_decimal_place, String dis3, int dis3_decimal_place, String dis4, int dis4_decimal_place);
  void display_write(String disp_1,String disp_2,String disp_3,String disp_4,int dp1,int dp2,int dp3,int dp4);
  int findindex_fromcharactor(char _char);
  
  uint8_t i2c_write(uint8_t val);
  uint8_t i2c_read(uint8_t addr,uint8_t *data,uint8_t size);
  void _updateKeyram();
  int8_t readKey(boolean clear);
  void show();
  void clear();
  void setBrightness(uint8_t b);
  void blank();
  void blink(uint8_t b);

  
  uint16_t  factor[16] = {0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000};
  uint16_t displayBuffer[8];
  uint16_t displayBuffer_disp[16][8] = {
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}, //1
    {0x0002, 0x0002, 0x0000, 0x0002, 0x0002, 0x0000, 0x0002, 0x0000}, //2
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0004, 0x0000, 0x0000}, //4
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0008, 0x0000, 0x0000}, //8
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0010, 0x0000, 0x0000}, //10
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0020, 0x0000, 0x0000}, //20
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0040, 0x0000, 0x0000}, //40
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0080, 0x0000, 0x0000}, //80
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0100, 0x0000, 0x0000}, //100
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0200, 0x0000, 0x0000}, //200
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0400, 0x0000, 0x0000}, //400
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0800, 0x0000, 0x0000}, //800
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1000, 0x0000, 0x0000}, //1000
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2000, 0x0000, 0x0000}, //2000
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4000, 0x0000, 0x0000}, //4000
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x0000, 0x0000} //8000
  };


  const uint8_t addr = 0x70; // HT16K33 default address
  uint8_t _address = 0x70;
  int num_array[39][8] = {
    {1, 1, 1, 1, 1, 1, 0, 0}, // 0   "0"          AAA
    {0, 1, 1, 0, 0, 0, 0, 0}, // 1   "1"         F   B
    {1, 1, 0, 1, 1, 0, 1, 0}, // 2   "2"         F   B
    {1, 1, 1, 1, 0, 0, 1, 0}, // 3   "3"          GGG
    {0, 1, 1, 0, 0, 1, 1, 0}, // 4   "4"         E   C
    {1, 0, 1, 1, 0, 1, 1, 0}, // 5   "5"         E   C
    {1, 0, 1, 1, 1, 1, 1, 0}, // 6   "6"          DDD
    {1, 1, 1, 0, 0, 0, 0, 0}, // 7   "7"
    {1, 1, 1, 1, 1, 1, 1, 0}, // 8   "8"
    {1, 1, 1, 1, 0, 1, 1, 0}, // 9   "9"
    {1, 1, 1, 0, 1, 1, 1, 0}, // 10  'A'
    {0, 0, 1, 1, 1, 1, 1, 0}, // 11  'b'
    {1, 0, 0, 1, 1, 1, 0, 0}, // 12  'C'
    {0, 1, 1, 1, 1, 0, 1, 0}, // 13  'd'
    {1, 0, 0, 1, 1, 1, 1, 0}, // 14  'E'
    {1, 0, 0, 0, 1, 1, 1, 0}, // 15  'F'
    {1, 0, 1, 1, 1, 1, 0, 0}, // 16  'G'
    {0, 1, 1, 0, 1, 1, 1, 0}, // 17  'H'
    {0, 1, 1, 0, 0, 0, 0, 0}, // 18  'I'
    {0, 1, 1, 1, 0, 0, 0, 0}, // 19  'J'
    {0, 1, 1, 0, 1, 1, 1, 0}, // 20  'K'  Same as 'H'
    {0, 0, 0, 1, 1, 1, 0, 0}, // 21  'L'
    {1, 0, 1, 0, 1, 0, 1, 0}, // 22  'M'  NO DISPLAY
    {0, 0, 1, 0, 1, 0, 1, 0}, // 23  'n'
    {1, 1, 1, 1, 1, 1, 0, 0}, // 24  'O'
    {1, 1, 0, 0, 1, 1, 1, 0}, // 25  'P'
    {1, 1, 1, 0, 0, 1, 1, 0}, // 26  'q'
    {0, 0, 0, 0, 1, 0, 1, 0}, // 27  'r'
    {1, 0, 1, 1, 0, 1, 1, 0}, // 28  'S'
    {0, 0, 0, 1, 1, 1, 1, 0}, // 29  't'
    {0, 1, 1, 1, 1, 1, 0, 0}, // 30  'U'
    {0, 1, 1, 1, 1, 1, 0, 0}, // 31  'V'  Same as 'U'
    {0, 0, 0, 0, 0, 0, 0, 0}, // 32  'W'  NO DISPLAY
    {0, 1, 1, 0, 1, 1, 1, 0}, // 33  'X'  Same as 'H'
    {0, 1, 1, 1, 0, 1, 1, 0}, // 34  'y'
    {1, 1, 0, 1, 1, 0, 1, 0}, // 35  'Z'  Same as '2'
    {0, 0, 0, 0, 0, 0, 0, 0}, // 36  ' '  BLANK
    {0, 0, 0, 0, 0, 0, 1, 0}, // 37  '-'  DASH
  };

};
