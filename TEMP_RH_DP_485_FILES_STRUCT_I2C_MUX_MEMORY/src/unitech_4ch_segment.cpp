#include "unitech_4ch_segment.h"
#define MUX_Address 0x77 // TCA9548A Encoders address
//TwoWire1 I2CDISPLAY = TwoWire1(0);

void unitech_4ch_segment::tcaselect(uint8_t i2c_bus) {
  Wire.begin(33,32);
  //0:Display / DP-2  | 1:TempRh | 2:DP | 3:RTC
    if (i2c_bus > 7) return;
    // Wire.beginTransmission(MUX_Address);
    // Wire.write(1 << i2c_bus);
    // Wire.endTransmission(); 

  uint8_t controlRegisterState=0;
  controlRegisterState = ((controlRegisterState & 0b11110000) | (0b100 | i2c_bus));
  Wire.beginTransmission(MUX_Address);
  Wire.write(controlRegisterState);
  Wire.endTransmission();
}


int unitech_4ch_segment::findindex_fromcharactor(char _char)
{
  if (_char == '0')     return 0;
  else if (_char == '1')    return 1;
  else if (_char == '2')    return 2;
  else if (_char == '3')    return 3;
  else if (_char == '4')    return 4;
  else if (_char == '5')    return 5;
  else if (_char == '6')    return 6;
  else if (_char == '7')    return 7;
  else if (_char == '8')    return 8;
  else if (_char == '9')    return 9;
  else if (_char == 'A')    return 10;
  else if (_char == 'B')    return 11;
  else if (_char == 'C')    return 12;
  else if (_char == 'D')    return 13;
  else if (_char == 'E')    return 14;
  else if (_char == 'F')    return 15;
  else if (_char == 'G')    return 16;
  else if (_char == 'H')    return 17;
  else if (_char == 'I')    return 18;
  else if (_char == 'J')    return 19;
  else if (_char == 'K')    return 20;
  else if (_char == 'L')    return 21;
  else if (_char == 'M')    return 22;
  else if (_char == 'N')    return 23;
  else if (_char == 'O')    return 24;
  else if (_char == 'P')    return 25;
  else if (_char == 'Q')    return 26;
  else if (_char == 'R')    return 27;
  else if (_char == 'S')    return 28;
  else if (_char == 'T')    return 29;
  else if (_char == 'U')    return 30;
  else if (_char == 'V')    return 31;
  else if (_char == 'W')    return 32;
  else if (_char == 'X')    return 33;
  else if (_char == 'Y')    return 34;
  else if (_char == 'Z')    return 35;
  else if (_char == ' ')    return 36;
  else if (_char == '-')    return 37;
}

uint8_t unitech_4ch_segment::i2c_write(uint8_t val){
  Wire.beginTransmission(_address);
  Wire.write(val);
  return Wire.endTransmission();
} // i2c_write

uint8_t unitech_4ch_segment::i2c_read(uint8_t addr,uint8_t *data,uint8_t size){
  uint8_t i,retcnt,val;
  
  i2c_write(addr);
  retcnt=Wire.requestFrom(_address, size);
  i=0;
  while(Wire.available() && i<size)    // slave may send less than requested
  {
    data[i++] = Wire.read();    // receive a byte as character
  }

  return retcnt;
} // i2c_read

void unitech_4ch_segment::_updateKeyram(){
  uint8_t curkeyram[6];

  i2c_read(HT16K33_KDAP, curkeyram, 6);
  _keyram[0]=curkeyram[1]<<8 | curkeyram[0]; // datasheet page 21, 41H is high 40H is low
  _keyram[1]=curkeyram[3]<<8 | curkeyram[2]; // or LSB MSB
  _keyram[2]=curkeyram[5]<<8 | curkeyram[4];
  return;
} // _updateKeyram

int8_t unitech_4ch_segment::readKey(boolean clear){  
  uint16_t diff;
  uint8_t key;
  int8_t i,j;

    // save the current state
  for (i=0;i<3;i++){
    if (clear){
      oldKeyData[i]=0;
    } else {
      oldKeyData[i]=_keyram[i];
    }
  }
    
  _updateKeyram();

  key=0; //the key that changed state
  for (i=0;i<3;i++){
    diff=_keyram[i] ^ oldKeyData[i]; //XOR old and new, any changed bit is set.
    if ( diff !=0 ){ // something did change
      for (j=0;j<13;j++){
  key++;
  if (((diff >> j) & 1) == 1){
    if (((_keyram[i] >> j) & 1)==0){
      return -key;
    }else{
      return key;
    }
  } // if keyram differs
      } // for j in bits
    } else {
      key+=13;
    } // if diff
  } // for i
  return 0; //apperently no new key was pressed - old might still be held down, pass clear=true to see it
} // readKey

void unitech_4ch_segment::show() {
  Wire.beginTransmission(addr);
  Wire.write(0x00); // start at address 0x0

  for (int i = 0; i < 8; i++) {
    Wire.write(displayBuffer[i] & 0xFF);
    Wire.write(displayBuffer[i] >> 8);
  }
  Wire.endTransmission();
}

void unitech_4ch_segment::clear() {
  for (int i = 0; i < 8; i++) {
    displayBuffer[i] = 0;
  }
}

void unitech_4ch_segment::setBrightness(uint8_t b) {
  if (b > 15) return;

  Wire.beginTransmission(addr);
  Wire.write(0xE0 | b); // Dimming command
  Wire.endTransmission();
}

void unitech_4ch_segment::blank() {
  static boolean blankOn;

  Wire.beginTransmission(addr);
  Wire.write(0x80 | blankOn); // Blanking / blinking command
  Wire.endTransmission();

  blankOn = !blankOn;
}

void unitech_4ch_segment::blink(uint8_t b) {
  if (b > 3) return;

  Wire.beginTransmission(addr);
  Wire.write(0x80 | b << 1 | 1); // Blinking / blanking command
  Wire.endTransmission();
}

bool unitech_4ch_segment::begin(){
  
  tcaselect(0);

  Wire.beginTransmission(addr);
  Wire.write(0x20 | 1); // turn on oscillator
  Wire.endTransmission();

  
  //_address=address | BASEHTADDR;
  
  i2c_write(HT16K33_SS  | HT16K33_SS_NORMAL); // Wakeup
  i2c_write(HT16K33_DSP | HT16K33_DSP_ON | HT16K33_DSP_NOBLINK); // Display on and no blinking
  i2c_write(HT16K33_RIS | HT16K33_RIS_OUT); // INT pin works as row output 
  i2c_write(HT16K33_DIM | HT16K33_DIM_16);  // Brightness set to max
  
  
  setBrightness(5);

}

void unitech_4ch_segment::display_write(String disp_1,String disp_2,String disp_3,String disp_4,int dp1,int dp2,int dp3,int dp4)
{
  String msg = "";
  for (int m = 0; m < 16; m++)
  msg += " ";

  //String temp_msg = disp_1 + disp_2 + disp_3+ disp_4;
  msg[0]= disp_1[0];
  msg[1]= disp_1[1];
  msg[2]= disp_1[2];
  msg[3]= disp_1[3];

  msg[4]= disp_2[0];
  msg[5]= disp_2[1];
  msg[6]= disp_2[2];
  msg[7]= disp_2[3];

  msg[8]= disp_3[0];
  msg[9]= disp_3[1];
  msg[10]= disp_3[2];
  msg[11]= disp_3[3];

  msg[12]= disp_4[0];
  msg[13]= disp_4[1];
  msg[14]= disp_4[2];
  msg[15]= disp_4[3];

  // for (int m = 0; m < temp_msg.length(); m++)
  // {
  //   if(m < 16)
  //     msg[m] = temp_msg[m]; 
  // }
  // // Serial.println(msg);
  // // Serial.println(temp_msg);

  for (int i = 0; i < 8; i++) 
    displayBuffer[i]=0;
  for (int m = 0; m < 16; m++)
  {
    int location = findindex_fromcharactor(msg[m]);
    
    for (int i = 0; i < 8; i++)
    {



      if (num_array[location][i] == 1)
        displayBuffer_disp[m][i] = factor[m];
      else if (i == 7)displayBuffer_disp[m][i] = 0;
      else displayBuffer_disp[m][i] = 0x0000;
     //Serial.print(" " + String(displayBuffer_disp[m][i]));
    }
    // Serial.println(String(dp1) + " | " + String(dp2) + " | " + String(dp3) + " | " + String(dp4));
    if(is_LED[0]  && m==0) displayBuffer_disp[m][7] = factor[m];
    if(dp1==2     && m==1)displayBuffer_disp[m][7] = factor[m];
    if(dp1==1     && m==2)displayBuffer_disp[m][7] = factor[m];    
    if(is_LED[1]  && m==3) displayBuffer_disp[m][7] = factor[m];

    if(is_LED[2]  && m==4) displayBuffer_disp[m][7] = factor[m];
    if(dp2==2     && m==5)displayBuffer_disp[m][7] = factor[m];
    if(dp2==1     && m==6)displayBuffer_disp[m][7] = factor[m];    
    if(is_LED[3]  && m==7) displayBuffer_disp[m][7] = factor[m];

    if(is_LED[4]  && m==8) displayBuffer_disp[m][7] = factor[m];
    if(dp3==2     && m==9)displayBuffer_disp[m][7] = factor[m];
    if(dp3==1     && m==10)displayBuffer_disp[m][7] = factor[m];    
    if(is_LED[5]  && m==11) displayBuffer_disp[m][7] = factor[m];

    // if(is_LED[6]  && m==12) displayBuffer_disp[m][7] = factor[m];
    // if(dp4==2    && m==13)displayBuffer_disp[m][7] = factor[m];
    // if(dp4==1     && m==14)displayBuffer_disp[m][7] = factor[m];    
    // if(is_LED[7]  && m==15) displayBuffer_disp[m][7] = factor[m];

    if(dp4==0)    is_LED[7]=0; else is_LED[7]=1;
    // is_LED[6]=1;
    // is_LED[9]=1;

    if(is_LED[6]  && m==12) displayBuffer_disp[m][7] = factor[m];
    if(is_LED[7]   && m==13)displayBuffer_disp[m][7] = factor[m];
    if(is_LED[8]    && m==14)displayBuffer_disp[m][7] = factor[m];    
    if(is_LED[9]  && m==15) displayBuffer_disp[m][7] = factor[m];     
 

    //Serial.println();
  }

  for (int i = 0; i < 8; i++) {
    for (int k = 0; k < 16; k++) {
      displayBuffer[i] += displayBuffer_disp[k][i];
      
    }
    delay(1);
  }
  tcaselect(0);
  show();
  // smile_bmp[0]=smile_bmp_find[findindex_fromcharactor(disp_1[0])];
  // bitWrite(smile_bmp[0], 7,is_LED[0]);//LOW
  // smile_bmp[1]=smile_bmp_find[findindex_fromcharactor(disp_1[1])];
  // if(dp1==2)bitWrite(smile_bmp[1], 7,1);else bitWrite(smile_bmp[1], 7,0);
  // smile_bmp[2]=smile_bmp_find[findindex_fromcharactor(disp_1[2])];
  // if(dp1==1)bitWrite(smile_bmp[2], 7,1);else bitWrite(smile_bmp[2], 7,0);
  // smile_bmp[3]=smile_bmp_find[findindex_fromcharactor(disp_1[3])];
  // bitWrite(smile_bmp[3], 7,is_LED[1]);//HIGH

  // smile_bmp[4]=smile_bmp_find[findindex_fromcharactor(disp_2[0])];
  // bitWrite(smile_bmp[4], 7,is_LED[2]);//LOW
  // smile_bmp[5]=smile_bmp_find[findindex_fromcharactor(disp_2[1])];
  // if(dp2==2)bitWrite(smile_bmp[5], 7,1);else bitWrite(smile_bmp[5], 7,0);
  // smile_bmp[6]=smile_bmp_find[findindex_fromcharactor(disp_2[2])];
  // if(dp2==1)bitWrite(smile_bmp[6], 7,1);else bitWrite(smile_bmp[6], 7,0);
  // smile_bmp[7]=smile_bmp_find[findindex_fromcharactor(disp_2[3])];
  // bitWrite(smile_bmp[7], 7,is_LED[3]);//HIGH

  // smile_bmp[8]=smile_bmp_find[findindex_fromcharactor(disp_3[0])];
  // bitWrite(smile_bmp[8], 7,is_LED[4]);//LOW
  // smile_bmp[9]=smile_bmp_find[findindex_fromcharactor(disp_3[1])];
  // if(dp3==2)bitWrite(smile_bmp[9], 7,1);else bitWrite(smile_bmp[9], 7,0);
  // smile_bmp[10]=smile_bmp_find[findindex_fromcharactor(disp_3[2])];
  // if(dp3==1)bitWrite(smile_bmp[10], 7,1);else bitWrite(smile_bmp[10], 7,0);
  // smile_bmp[11]=smile_bmp_find[findindex_fromcharactor(disp_3[3])];
  // bitWrite(smile_bmp[11], 7,is_LED[5]);//HIGH

  // smile_bmp[12]=smile_bmp_find[findindex_fromcharactor(disp_4[0])];
  // bitWrite(smile_bmp[12], 7,is_LED[6]);//LOW
  // smile_bmp[13]=smile_bmp_find[findindex_fromcharactor(disp_4[1])];
  // if(dp4==2)bitWrite(smile_bmp[13], 7,1);else bitWrite(smile_bmp[13], 7,0);
  // smile_bmp[14]=smile_bmp_find[findindex_fromcharactor(disp_4[2])];
  // if(dp4==1)bitWrite(smile_bmp[14], 7,1);else bitWrite(smile_bmp[14], 7,0);
  // smile_bmp[15]=smile_bmp_find[findindex_fromcharactor(disp_4[3])];
  // bitWrite(smile_bmp[15], 7,is_LED[7]);//HIGH
}

void unitech_4ch_segment::w_display_segment(String dis1, int dis1_decimal_place, String dis2, int dis2_decimal_place, String dis3, int dis3_decimal_place, String dis4, int dis4_decimal_place)
{
  //String msg=dis1 + dis2 + dis3 + dis4;
  static uint8_t led=0;
  static uint8_t lastKey=0;
  static int8_t dir=1;
  int8_t key;

  String msg="1234567890ABCDEF";

  display_write(dis1,dis2,dis3,dis4,dis1_decimal_place,dis2_decimal_place,dis3_decimal_place,dis4_decimal_place);
  
  
}
