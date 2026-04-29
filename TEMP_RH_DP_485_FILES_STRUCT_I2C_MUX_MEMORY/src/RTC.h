#include "Wire.h"
#include "Arduino.h"
#include "TimeLib.h"

#define CLOCK_ADDRESS 104

class rtc
{
    uint8_t bin2bcd(uint8_t val)
    {
      return val + 6 * (val / 10);
    }

    uint8_t bcd2bin(uint8_t val)
    {
      return val - 6 * (val >> 4);
    }

    public :

    void start()
    {
        uint8_t sec;
        //Wire.begin(32,33);
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
    }

    bool get()
    {
        int sec, min, hr, day, month, year;
        //Wire.begin(32,33);
        bool status=false;
        Wire.beginTransmission(CLOCK_ADDRESS);
        Wire.write(byte(0x00));
        Wire.endTransmission();
        delay(10);
        Wire.requestFrom(CLOCK_ADDRESS, 7);
        long lastmillis = millis();    
        bool isReceive = false;

        while ((isReceive == false))
        {
            if (Wire.available()>=4)
            {
                isReceive=true;

                sec   = bcd2bin(Wire.read() & 0x7F);
                min   = bcd2bin(Wire.read());
                hr    = bcd2bin(Wire.read());
                        bcd2bin(Wire.read()); //day of week
                day   = bcd2bin(Wire.read());
                month = bcd2bin(Wire.read());
                year  = bcd2bin(Wire.read());
                // Serial.println("Get_Time"+String(year) + "-" + String(month) + "-" + String(day) + " " + String(hr) + ":" + String(min) + ":" + String(sec));
                setTime(hr,min,sec,day,month,year);
                status=true;
            }
            else if ((millis() - lastmillis) > 200)
                isReceive = true;
        }
        return status;
    }

    bool set(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t month, uint8_t year)
    {
    //  Serial.println("Set_Time" + String(year) + "-" + String(month) + "-" + String(day) + " " + String(hour) + ":" + String(min) + ":" + String(sec));   
    bool status=false;
    uint8_t ch;

    //Wire.begin(32,33);
    //get CH bit
    Wire.beginTransmission(CLOCK_ADDRESS);
    Wire.write(byte(0x00));
    Wire.endTransmission();

    Wire.requestFrom(CLOCK_ADDRESS, 1);
    long lastmillis = millis();    
        bool isReceive = false;

        while ((isReceive == false))
        {
            if (Wire.available()>=1)
            {
                isReceive=true;
                ch = Wire.read();
                
                
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
                status=true;
            }
            else if ((millis() - lastmillis) > 200)
                isReceive = true;
        }
        return status;
    }

};