#include "i2c_sensor_trh_dp_read.h"
#define CLOCK_ADDRESS 104
#define MUX_Address 0x77 // TCA9548A Encoders address

bool i2c_sensor_trh_dp_read::begin()
{

  return true;
}

//////////////////////////////////////////////////////Read Sensor//////////////////////////////////////////

void i2c_sensor_trh_dp_read::set_pin(uint8_t _sda, uint8_t _scl)
{
  //Wire.begin(_sda, _scl);
  Wire.begin(33,32);
}

void i2c_sensor_trh_dp_read::get_sensor_value()
{
  r_val = (((float)r_count * 100) / 16382)*10;
  t_val = ((((float)t_count * 165) / 16382) - 40)*10;
  d_val = ((float)d_count * (float)(0.0031) - 25.3748);
}
/* 
void i2c_sensor_trh_dp_read::read_data()
{

  //Wire.begin(22, 21);
  //delay(10);
  for (address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      if (address < 16)
        delay(10);
      if (address == 104)
      {
        sec = getSecond();
        min = getMinute();
        hour = getHour(false, false);
        day = getDate();
        month = getMonth(false);
        year = getYear();
              //Serial.println("RTC read SUCCESS : " + String(sec));
      }
      else
      {
        Wire.write(1);
        uint16_t ret_0, ret_1, ret_2, ret_3;
        int lpCnt = 0;
        char tmp;
        do {
          delay(10);
          Wire.requestFrom(address, 4);
          ret_0 = Wire.read();
          ret_1 = Wire.read();
          ret_2 = Wire.read();
          ret_3 = Wire.read();
          tmp = (0xC0 & ret_0);
          lpCnt++;
          if (lpCnt > 10)
            break;
        } while (tmp != 0);

        if (address == 39) {
          is_27_i2c = true;
          r_count = ret_0;
          r_count = ((r_count << 8) | (ret_1));
          t_count = ret_2;
          t_count = ((t_count << 8) | (ret_3 & 0xFC));
          t_count = (t_count >> 2);
        }
        else if (address == 40) {
          is_28_i2c = true;
          d_count = (ret_0 & 0x7f);
          d_count = ((d_count << 8) | (ret_1));
        }
      }
    }
    else {
      if (address == 39)
        is_27_i2c = false;
      else if (address == 40)
        is_28_i2c = false;
    }
  }
}
*/

void i2c_sensor_trh_dp_read::tcaselect(uint8_t i2c_bus) {
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

void i2c_sensor_trh_dp_read::read_data_t_rh()
{
  is_27_i2c = false;
  byte address;
  for (address = 1; address < 127; address++ )  {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0 && address == 39) {
        Wire.write(1);
        uint16_t ret_0, ret_1, ret_2, ret_3;
        int lpCnt = 0;
        char tmp;
        do {
          delay(10);
          Wire.requestFrom(address, 4);
          ret_0 = Wire.read();
          ret_1 = Wire.read();
          ret_2 = Wire.read();
          ret_3 = Wire.read();
          tmp = (0xC0 & ret_0);
          lpCnt++;
          if (lpCnt > 10)
            break;
        } while (tmp != 0);
        if (address == 39) {
          is_27_i2c = true;
          r_count = ret_0;
          r_count = ((r_count << 8) | (ret_1));
          t_count = ret_2;
          t_count = ((t_count << 8) | (ret_3 & 0xFC));
          t_count = (t_count >> 2);
          
        }          
    }
  }
  //Serial.println("TRH SENSOR STATUS : " + String(is_27_i2c));

}

void i2c_sensor_trh_dp_read::read_data_dp()
{
  
    // address=104;
    // Wire.beginTransmission(address);
    // if (Wire.endTransmission() == 0) {
    //   if (address < 16)
       
    //     delay(10);
    //   if (address == 104)
    //   {
    //     //sec = getSecond();
    //     // min = getMinute();
    //     // hour = getHour(false, false);
    //     // day = getDate();
    //     // month = getMonth(false);
    //     // year = getYear();
    //     //Serial.println("RTC read SUCCESS : " + String(sec));
    //   }
    // }

    
    int address=40;
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
        Wire.write(1);
        uint16_t ret_0, ret_1, ret_2, ret_3;
        int lpCnt = 0;
        char tmp;
        do {
          delay(10);
          Wire.requestFrom(address, 4);
          ret_0 = Wire.read();
          ret_1 = Wire.read();
          ret_2 = Wire.read();
          ret_3 = Wire.read();
          tmp = (0xC0 & ret_0);
          lpCnt++;
          if (lpCnt > 10)
            break;
        } while (tmp != 0);

        if (address == 40) {
          is_28_i2c = true;
          d_count = (ret_0 & 0x7f);
          d_count = ((d_count << 8) | (ret_1));
        }
        else
          is_28_i2c = false;
      }

    
    // else {
    //   if (address == 39)
    //     is_27_i2c = false;
    //   else if (address == 40)
    //     is_28_i2c = false;
    // }

  
}

uint8_t bin2bcd(uint8_t val)
{
  return val + 6 * (val / 10);
}

uint8_t bcd2bin(uint8_t val)
{
  return val - 6 * (val >> 4);
}

void i2c_sensor_trh_dp_read::get(int *sec, int *min, int *hour, int *day, int *month, int *year)
{
  Wire.beginTransmission(CLOCK_ADDRESS);
  Wire.write(byte(0x00));
  Wire.endTransmission();

  Wire.requestFrom(CLOCK_ADDRESS, 7);
  *sec   = bcd2bin(Wire.read() & 0x7F);
  *min   = bcd2bin(Wire.read());
  *hour  = bcd2bin(Wire.read());
           bcd2bin(Wire.read()); //day of week
  *day   = bcd2bin(Wire.read());
  *month = bcd2bin(Wire.read());
  *year  = bcd2bin(Wire.read()) + 2000;

  return;
}

void i2c_sensor_trh_dp_read::set(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t month, uint16_t year)
{
  uint8_t ch, d;

  if(year >= 2000)
  {
    year -= 2000;
  }

  //get CH bit
  Wire.beginTransmission(CLOCK_ADDRESS);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(CLOCK_ADDRESS, 1);
  ch = Wire.read();
  d  = Wire.read();
  
  if(ch & 0x80) //CH set?
  {
    ch = 0x80;
  }
  else
  {
    ch = 0x00;
  }

  //set time and date
  Wire.beginTransmission(CLOCK_ADDRESS);
  Wire.write(byte(0x00));
  Wire.write(bin2bcd(sec) | ch);
  Wire.write(bin2bcd(min));
  Wire.write(bin2bcd(hour));
  Wire.write(bin2bcd(0));
  Wire.write(bin2bcd(day));
  Wire.write(bin2bcd(month));
  Wire.write(bin2bcd(year));
  Wire.endTransmission();

  return;
}

void i2c_sensor_trh_dp_read::set(int sec, int min, int hour, int day, int month, int year)
{
  return set((uint8_t)sec, (uint8_t)min, (uint8_t)hour, (uint8_t)day, (uint8_t)month, (uint16_t)year);
}

void i2c_sensor_trh_dp_read::start(void)
{
  uint8_t sec;

  //get second and CH bit
  Wire.beginTransmission(CLOCK_ADDRESS);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(CLOCK_ADDRESS, 1);
  sec = Wire.read();
  Wire.read();

  //set second and clear CH bit
  Wire.beginTransmission(CLOCK_ADDRESS);
  Wire.write(byte(0x00));
  Wire.write(sec & 0x7F);
  Wire.endTransmission();

  return;
}

void i2c_sensor_trh_dp_read::stop(void)
{
  uint8_t sec, d;

  //get second and CH bit
  Wire.beginTransmission(CLOCK_ADDRESS);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(CLOCK_ADDRESS, 1);
  sec = Wire.read();
  d   = Wire.read();

  //set second and CH bit
  Wire.beginTransmission(CLOCK_ADDRESS);
  Wire.write(byte(0x00));
  Wire.write(sec | 0x80); //hold
  Wire.endTransmission();

  return;
}
