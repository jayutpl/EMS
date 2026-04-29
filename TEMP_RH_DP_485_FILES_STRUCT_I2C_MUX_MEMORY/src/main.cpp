#include "variable.h"
#include "TimeLib.h"
#include <WiFiAP.h>

#define EEPROM_SIZE 2024


#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 0
#endif

void display_7led_screen();

variable _obj_variable;
TaskHandle_t Task1;
int password_index = 0;
int logout_time = 60000;
bool blink_segment = 0;
int blink_decmial = 0;
int blink_segment_time = 25, blink_1secound = 0, blink_1secound_time = 40;
int buadrate[5] = {4800, 9600, 19200, 28800, 115200};
bool set_date_time = 0, set_secound = 0, set_hour = 0, set_minute = 0, set_day = 0, set_month = 0, set_year = 0;
uint8_t _m_secound = 0, _m_hour = 0, _m_minute = 0, _m_day = 0, _m_month = 0, _m_year = 0;
uint8_t _s_second = 0, _s_hour = 0, _s_minute = 0, _s_day = 0, _s_month = 0, _s_year = 0;
unsigned long hooter_millis = 0,blink_segment_millis=0,i2cread_millis = 0, i2c_d_read_millis = 0,door_open_millis=0; 
int hooter_time_count = 1;
int refresh_modbus_manu = 0, modbus_manu_password = 1357;
String display_1 = "8888", display_2 = "8888", display_3 = "8888", display_4 = "8888";
int decimal_1 = 0, decimal_2 = 0, decimal_3 = 0;
bool isWriteDisplay = true, is_relay_1_on = false;
tmElements_t _m_tm;
String update ="UPDT";


const char* host = "esp32-webupdate";
WebServer server(80);
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><label for='html'>0.56_TEMP_RH_DP_INSTRUMENT Version 2.0.2</label><br><input type='file' name='update'><input type='submit' value='Update'></form>";

#pragma region File Read_eeprom

int alarm_index = 0;

void menu_generate()
{
  #pragma region CONFIG
  int item_menu_name=0;
  // int item_menu_name=0;
  int count=1;
  //DECIMAL PLACE
  for(int j=0;j<TOTALCHANNEL;j++)
  {
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name , (String((j+1)) + " DP").c_str() ); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=j;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
    item_menu_name ++;
    // count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name , (String((j+1)) + " DP").c_str()); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=j;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_INP_DECIMAL;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NUMBER;
    count++;
    item_menu_name ++;
  }

  //ALARM_STATUS_ENABLE_DISABLE
  if(true)
  {
    alarm_index=0;
    for(int j=0;j<TOTALCHANNEL;j++)
    {
      for(int i=0; i < PER_CH_ALARM;i++)
      {
        
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
        strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name, (String((j+1)) + "AS" + String((i+1))).c_str()); 
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=alarm_index;
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
        item_menu_name ++;
        count++;
        //VALUE
        
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
        strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name, (String((j+1)) +"AS" + String((i+1))).c_str()); 
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=alarm_index;
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_ALARM_STATUS;
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_ALARM_ON_OFF;
        item_menu_name ++;
        count++;
        alarm_index++;
      }
   }
    //ALARM_DELAY_TIME
    alarm_index=0;
    for(int j=0;j<TOTALCHANNEL ;j++)
    {
      for(int i=0;i<PER_CH_ALARM;i++)
      {

       _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
       strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,(String((j+1)) + "AD" + String((i+1))).c_str()); 
       _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
       _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=alarm_index;
       _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
       _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
       _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
       item_menu_name ++;
        count++;
      //VALUE

        _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
        strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,(String((j+1)) +"AD" + String((i+1))).c_str()); 
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=alarm_index;
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_ALARM_DELAY_TIME;
        _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NUMBER;
        item_menu_name ++;
        count++;
        alarm_index++;
      }
    }
  }
  
  //INPUT_CHANNEL_ENABLE_DISABLE
  
  for(int i=0;i<TOTALCHANNEL;i++)
  {
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,("DIS" + String((i+1))).c_str()); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;    
    item_menu_name ++;
    // count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,("DIS" + String((i+1))).c_str()); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_INP_STATUS;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_EN_DIS;
    item_menu_name ++;
    count++;
  }  
  
  //INPUT_CHANNEL_NO-NAGATIVE
  if(true)
  {
    for(int i=0;i<TOTALCHANNEL;i++)
  {
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,("NON" + String((i+1))).c_str()); 
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=i;
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;    
    item_menu_name ++;
    // count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,  ("NON" + String((i+1))).c_str()); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_INP_NONG;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_EN_DIS;
    item_menu_name ++;
    count++;
  } 
  }

  //NODE
  if(true)
  {
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"NODE"); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
    item_menu_name ++;
    // count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"NODE"); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_COMMUNICATION;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;
  }

  //MODE_CONFIG_BAUD
  if(true)
  {
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"BAUD"); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
    item_menu_name ++;
    // count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"BAUD"); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=1;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_COMMUNICATION;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_BUADRATE;
    item_menu_name ++;
    count++;
  }

  //MODE_CONFIG_PRIT
  if(true)
  {
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"PRIT"); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
    item_menu_name ++;
    // count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"PRIT"); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=2;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_COMMUNICATION;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_PARITY;
    item_menu_name ++;
    count++;
  }

  //MODE_CONFIG_OFFS
  for(int j=0;j<TOTALCHANNEL;j++)
  {
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name ,  (String((j+1)) +"CLV").c_str()); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=j;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
    item_menu_name ++;
    // count++;
    //VALUE
    
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
   strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name , ( String((j+1)) +"CLV").c_str()); 
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=j;
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_OFFSET;
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;
  }

  //MODE_CALIBRATION_CAL_OFFSET
  for(int j=0;j<TOTALCHANNEL;j++)
  {
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name , (String((j+1)) +"CLO").c_str()); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=j;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
    item_menu_name ++;
    // count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,( String((j+1)) +"CLO").c_str()); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=j;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CALIBRATION_CAL_OFFSET;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;
  }

  //INPUT_DEMPING
  for(int i=0;i<TOTALCHANNEL;i++)
  {
    
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name, ("DMP" + String((i+1))).c_str()); 
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;    
    item_menu_name ++;
    // count++;
    //VALUE
    
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
   strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,("DMP" + String((i+1))).c_str()); 
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=i;
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_INP_DEMPING;
   _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;
  }

  
  //TIME SETTING
  
  if(true)
  {
    //TIME_DAY
    if(true)
    {
      
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
      strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"TDAY"); 
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=3;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
      item_menu_name ++;
      count++;
      //VALUE
      
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
      strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"TDAY"); 
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=3;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_TIME;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NUMBER;
      item_menu_name ++;
      count++;
    }
    // Serial.println("item_menu_name_d  -"+ String(item_menu_name));

    //TIME_MONTH
    if(true)
    {
      
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
      strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"TMON"); 
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=4;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
      item_menu_name ++;
      count++;
      //VALUE
      
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
      strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"TMON"); 
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=4;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_TIME;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NUMBER;
      
      item_menu_name ++;
      
      count++;
    }
    // Serial.println("item_menu_name_m  -"+ String(item_menu_name));

    //TIME_YEAR
    if(true)
    {
      
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
      strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"TYER"); 
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=5;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
      item_menu_name ++;
      count++;
      //VALUE
      
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
      strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"TYER"); 
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=5;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_TIME;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NUMBER;
      item_menu_name ++;
      count++;
    }
  // Serial.println("item_menu_name_y  -"+ String(item_menu_name));
    //TIME_HOUR
    if(true)
    {
      
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
      strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"THUR"); 
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=2;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
      item_menu_name ++;
      count++;
      //VALUE
      
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
      strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"THUR");  
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=2;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_TIME;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NUMBER;
      item_menu_name ++;
      count++;
    }
  // Serial.println("item_menu_name_h  -"+ String(item_menu_name));
    //TIME_MINUTE
    if(true)
    {
      
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
      strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"TMIN"); 
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=1;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
      item_menu_name ++;
      count++;
      //VALUE
      
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
      strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"TMIN"); 
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=1;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_TIME;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NUMBER;
      item_menu_name ++;
      count++;
    }
  // Serial.println("item_menu_name_min  -"+ String(item_menu_name));
    //TIME_SECOND
    if(true)
    {
      
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
      strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"TSEC"); 
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
      item_menu_name ++;
      count++;
      //VALUE
      
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
      strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"TSEC"); 
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_TIME;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NUMBER;
      item_menu_name ++;
      count++;
    }
  // Serial.println("item_menu_name_sec  -"+ String(item_menu_name));
    //TIME_ENABLE
    if(true)
    {
      
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
      strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"T_EN"); 
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=1;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=6;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_NONE;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_NONE;
      item_menu_name ++;
      count++;
      //VALUE
      
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._id = count; 
      strcpy(_obj_variable._lst_display_menu_screen_config[item_menu_name]._name,"T_EN");
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._type=2;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._index=6;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._ref_id=0;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._display_screen=MODE_CONFIG_TIME;
      _obj_variable._lst_display_menu_screen_config[item_menu_name]._d_type=DTYPE_EN_DIS;
      item_menu_name ++;
      count++;
    }
  }
  
  // Serial.println("item_menu_name_CONFIG" + String(item_menu_name) );
  #pragma endregion CONFIG
  //  Serial.println("item_menu_name_config" + String(item_menu_name) ); 
  #pragma region SETUP
  count = 1;
  item_menu_name=0;
  
  for(int i=0;i<TOTALCHANNEL;i++)
  { 
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._id = count; 
    if(TOTALCHANNEL==1)
    strcpy(_obj_variable._lst_display_menu_screen_setup[item_menu_name]._name,"ZERO"); 
    else
    strcpy(_obj_variable._lst_display_menu_screen_setup[item_menu_name]._name, ("ZER" + String((i+1))).c_str()); 
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._d_type=DTYPE_NONE;    
    item_menu_name ++;
    count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._id = count; 
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._display_screen=MODE_SETUP_ZERO;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;

    
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._id = count; 
    if(TOTALCHANNEL==1)
    strcpy(_obj_variable._lst_display_menu_screen_setup[item_menu_name]._name,"SPAN"); 
    else
    strcpy(_obj_variable._lst_display_menu_screen_setup[item_menu_name]._name,("SPA" + String((i+1))).c_str()); 
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._d_type=DTYPE_NONE;    
     item_menu_name ++;
    count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._id = count; 
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._display_screen=MODE_SETUP_SPAN;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;
  }
  
  //ALARM SETPOINT
  if(true)
  {
    int alarm_index=0;
    for(int j=0;j<TOTALCHANNEL;j++)
    {
      for(int i=0;i< PER_CH_ALARM ;i++)
      {
        
        _obj_variable._lst_display_menu_screen_setup[item_menu_name]._id = count; 
        if(TOTALCHANNEL==1)
        strcpy(_obj_variable._lst_display_menu_screen_setup[item_menu_name]._name, (" SP" + String((i+1))).c_str());
        else
        strcpy(_obj_variable._lst_display_menu_screen_setup[item_menu_name]._name, (String((j+1)) + "SP" + String((i+1))).c_str()); 
        _obj_variable._lst_display_menu_screen_setup[item_menu_name]._type=1;
        _obj_variable._lst_display_menu_screen_setup[item_menu_name]._index= alarm_index;
        _obj_variable._lst_display_menu_screen_setup[item_menu_name]._ref_id=0;
        _obj_variable._lst_display_menu_screen_setup[item_menu_name]._display_screen=MODE_NONE;
        _obj_variable._lst_display_menu_screen_setup[item_menu_name]._d_type=DTYPE_NONE;
        item_menu_name ++;
        count++;
        //VALUE
        
        _obj_variable._lst_display_menu_screen_setup[item_menu_name]._id = count; 
        if(TOTALCHANNEL==1)
        strcpy(_obj_variable._lst_display_menu_screen_setup[item_menu_name]._name, (" SP" + String((i+1))).c_str());
        else
        strcpy(_obj_variable._lst_display_menu_screen_setup[item_menu_name]._name, (String((j+1)) + "SP" + String((i+1))).c_str()); 
        _obj_variable._lst_display_menu_screen_setup[item_menu_name]._type=2;
        _obj_variable._lst_display_menu_screen_setup[item_menu_name]._index=alarm_index;
        _obj_variable._lst_display_menu_screen_setup[item_menu_name]._ref_id=0;
        _obj_variable._lst_display_menu_screen_setup[item_menu_name]._display_screen=MODE_SETUP_ALARM_SETPOINT;
        _obj_variable._lst_display_menu_screen_setup[item_menu_name]._d_type=DTYPE_NUMBER;
        item_menu_name ++;
        count++;
        // Serial.println(String(_obj_variable._lst_display_menu_screen_setup[item_menu_name]._index) +" |  "+ String(i) );
        alarm_index++;
      }
    }
  }
 
  //OP_L
  for(int i=0;i<TOTALCHANNEL;i++)
  {
    
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._id = count; 
    if(TOTALCHANNEL==1)
    strcpy(_obj_variable._lst_display_menu_screen_setup[item_menu_name]._name,"OP-L");
    else
    strcpy(_obj_variable._lst_display_menu_screen_setup[item_menu_name]._name,(String(i+1) + "OPL").c_str());
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._d_type=DTYPE_NONE;
    
    item_menu_name ++;
    count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._id = count; 
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._display_screen=MODE_SETUP_OUTPUT_ZERO;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;

    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._id = count; 
    if(TOTALCHANNEL==1)
    strcpy(_obj_variable._lst_display_menu_screen_setup[item_menu_name]._name,"OP-H" );
    else
    strcpy(_obj_variable._lst_display_menu_screen_setup[item_menu_name]._name,(String(i+1) + "OPH").c_str()); 
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._d_type=DTYPE_NONE;   
    item_menu_name ++;
    count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._id = count; 
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._display_screen=MODE_SETUP_OUTPUT_SPAN;
    _obj_variable._lst_display_menu_screen_setup[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;
  }
  
  #pragma endregion SETUP

  // Serial.println("item_menu_name_setup" + String(item_menu_name) );
  #pragma region CALI
  count=1;
  item_menu_name=0;
  
  for(int i=0;i<TOTALCHANNEL;i++)
  {
    
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._id = count; 
    if(TOTALCHANNEL==1)
    strcpy(_obj_variable._lst_display_menu_screen_cali[item_menu_name]._name,"CALZ"); 
    else
    strcpy(_obj_variable._lst_display_menu_screen_cali[item_menu_name]._name,(String(i+1) + "CLZ").c_str()); 
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._d_type=DTYPE_NONE;    
    item_menu_name++;
    count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._id = count; 
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._display_screen=MODE_CALIBRATION_CAL_ZERO;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;

    
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._id = count; 
    if(TOTALCHANNEL==1)
    strcpy(_obj_variable._lst_display_menu_screen_cali[item_menu_name]._name,"CALS"); 
    else
   strcpy(_obj_variable._lst_display_menu_screen_cali[item_menu_name]._name,(String(i+1) + "CLS").c_str()); 
   _obj_variable._lst_display_menu_screen_cali[item_menu_name]._type=1;
   _obj_variable._lst_display_menu_screen_cali[item_menu_name]._index=i;
   _obj_variable._lst_display_menu_screen_cali[item_menu_name]._ref_id=0;
   _obj_variable._lst_display_menu_screen_cali[item_menu_name]._display_screen=MODE_NONE;
   _obj_variable._lst_display_menu_screen_cali[item_menu_name]._d_type=DTYPE_NONE;    
    item_menu_name ++;
    count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._id = count; 
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._display_screen=MODE_CALIBRATION_CAL_SPAN;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;
  }
  //MODE_CALIBRATION_OUTPUT_4_20
  for(int i=0;i<TOTALCHANNEL;i++)
  {
    
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_cali[item_menu_name]._name,(String(i+1) + "CLL").c_str()); 
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._d_type=DTYPE_NONE;    
    item_menu_name ++;
    count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._id = count; 
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._display_screen=MODE_CALIBRATION_CAL_OUTPUT_ZERO;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;
  
    
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_cali[item_menu_name]._name,(String(i+1) + "CLH").c_str()); 
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._d_type=DTYPE_NONE;    
    item_menu_name ++;
    count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._id = count; 
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._index=i;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._display_screen=MODE_CALIBRATION_CAL_OUTPUT_SPAN;
    _obj_variable._lst_display_menu_screen_cali[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;
  }
// Serial.println("item_menu_name_chang" + String(item_menu_name) );
  #pragma endregion CALI

  // Serial.println("item_menu_name_chang" + String(item_menu_name) );
  #pragma region PASSWORD_CHANGE
  count=1;
  item_menu_name=0;
  
  //MODE_PASS_CHANGE_OLD
  if(true)
  {
    
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_chag[item_menu_name]._name,"OPAS"); 
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._index=7;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._d_type=DTYPE_NONE;
    item_menu_name ++;
    count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_chag[item_menu_name]._name,"OPAS"); 
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._index=7;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._display_screen=MODE_PASS_CHANGE_OLD;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;
  }

  //MODE_PASS_CHANGE_NEW
  if(true)
  {
    
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_chag[item_menu_name]._name,"NPAS"); 
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._index=8;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._d_type=DTYPE_NONE;
    item_menu_name ++;
    count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_chag[item_menu_name]._name,"NPAS"); 
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._index=8;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._display_screen=MODE_PASS_CHANGE_NEW;
    _obj_variable._lst_display_menu_screen_chag[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;
  }

  #pragma endregion
  // Serial.println("item_menu_name_pass" + String(item_menu_name) );

  #pragma region BUZZ
  //MODE_BUZZ_STATUS
  item_menu_name = 0;
  if(true)
  {
    
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_buzz[item_menu_name]._name,"BUZZ"); 
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._type=1;
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._index=0;
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._display_screen=MODE_NONE;
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._d_type=DTYPE_NONE;
    item_menu_name ++;
    count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_buzz[item_menu_name]._name,"BUZZ"); 
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._index=0;
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._display_screen=MODE_BUZZ_STATUS;
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._d_type=DTYPE_EN_DIS;
    item_menu_name ++;
    count++;
  }

  //MODE_BUZZ_STATUS
  if(true)
  {
    
   _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._id = count; 
   strcpy(_obj_variable._lst_display_menu_screen_buzz[item_menu_name]._name,"FREQ"); 
   _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._type=1;
   _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._index=1;
   _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._ref_id=0;
   _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._display_screen=MODE_NONE;
   _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._d_type=DTYPE_NONE;
    item_menu_name ++;
    count++;
    //VALUE
    
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._id = count; 
    strcpy(_obj_variable._lst_display_menu_screen_buzz[item_menu_name]._name,"FREQ"); 
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._type=2;
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._index=1;
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._ref_id=0;
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._display_screen=MODE_BUZZ_FREQUENCY;
    _obj_variable._lst_display_menu_screen_buzz[item_menu_name]._d_type=DTYPE_NUMBER;
    item_menu_name ++;
    count++;
  }

  #pragma endregion
  // Serial.println("item_menu_name_buzz" + String(item_menu_name) );
}

void byte_to_byte(int address,byte value)
{ 
  EEPROM.write(address,value);
  EEPROM.commit();
}

void short_to_byte(int address,signed int value)
{  
  EEPROM.writeShort(address,value);
  EEPROM.commit();
  // byte * valuePtr = (byte *) &value;
  // for (size_t i = 0; i < sizeof(value); i++)
  //   byteArray[i]=(byte)(*(valuePtr + i));
}

void ushort_to_byte(int address,unsigned int value)
{  
  EEPROM.writeUShort(address,value);
  EEPROM.commit();
  // byte * valuePtr = (byte *) &value;
  // for (size_t i = 0; i < sizeof(value); i++)
  //   byteArray[i]=(byte)(*(valuePtr + i));
}

void string_to_byte(int address,String value)
{  
  for(int i=0;i<value.length();i++)
    EEPROM.write(address,value[i]);
  EEPROM.commit();  
}

byte EEPROM_READ_BYTE(int address)
{
  byte b1 = EEPROM.read(address);
  return b1;
}

signed int EEPROM_READ_SHORT(int address)
{
  signed int val = EEPROM.readShort(address);  
  return val;
}

String EEPROM_READ_STRING(int address)
{
  String val="";
  for(int i=0;i<20;i++)
  {
    val += char(EEPROM.read(address));  
    address++;
  }
  return val;
}

void Configuration_WriteEEPROM_AllTime()
{  
  int address=100;int k = 0;
  for(int i=0;i<TOTALCHANNEL;i++)
  {
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._decimal); address += 1;
    short_to_byte(address,_obj_variable._lst_parameter_info[i]._offset); address += 2;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._f_demping); address += 1;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._is_calibration); address += 1;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._sensor_no); address += 1;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._cali_offset); address += 1;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._input_type); address += 1;
    short_to_byte(address,_obj_variable._lst_parameter_info[i]._r_zero); address += 2;
    short_to_byte(address,_obj_variable._lst_parameter_info[i]._r_span); address += 2;
    ushort_to_byte(address,_obj_variable._lst_parameter_info[i]._c_zero); address += 2;
    ushort_to_byte(address,_obj_variable._lst_parameter_info[i]._c_span); address += 2;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._disp_status); address += 1;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._unit_id); address += 1;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._is_negative); address += 1;

    short_to_byte(address,_obj_variable._lst_parameter_info[i]._op_r_zero); address += 2;
    short_to_byte(address,_obj_variable._lst_parameter_info[i]._op_r_span); address += 2;
    ushort_to_byte(address,_obj_variable._lst_parameter_info[i]._op_c_zero); address += 2;
    ushort_to_byte(address,_obj_variable._lst_parameter_info[i]._op_c_span); address += 2;

    for(int j=0;j<PER_CH_ALARM;j++)
    {
      byte_to_byte(address,_obj_variable._lst_parameter_alarm_info[k]._type); address += 1;
      short_to_byte(address,_obj_variable._lst_parameter_alarm_info[k]._value); address += 2;
      byte_to_byte(address,_obj_variable._lst_parameter_alarm_info[k]._delay); address += 1;
      byte_to_byte(address,_obj_variable._lst_parameter_alarm_info[k]._display_no); address += 1;
      byte_to_byte(address,_obj_variable._lst_parameter_alarm_info[k]._status); address += 1;
      strcpy(_obj_variable._lst_parameter_alarm_info[k]._name ,( String(_obj_variable._lst_parameter_alarm_info[k]._para_id) + "IP" + String(j+1)).c_str());
      k++;
    }    
  }

  //COMMUNICATION
  byte_to_byte(address,_obj_variable._lst_communication[0]._value); address += 1;                  //NODE,1,
  byte_to_byte(address,_obj_variable._lst_communication[1]._value); address += 1;                  //BOUD,1,
  byte_to_byte(address,_obj_variable._lst_communication[2]._value); address += 1;                  //PARI,0,
  byte_to_byte(address,_obj_variable._lst_communication[3]._value); address += 1;                  //STOP,1,
  
  //BUZZER
  byte_to_byte(address,_obj_variable._lst_other_info[0]._value); address += 1;  
  byte_to_byte(address,_obj_variable._lst_other_info[1]._value); address += 1;  
  // //OTHER
  short_to_byte(address,_obj_variable._lst_other_info[2]._value); address += 1; //RTC
  
  //PASSWORD
  short_to_byte(address,_obj_variable._lst_password_info[0]._pass); address += 2;    //CONF
  short_to_byte(address,_obj_variable._lst_password_info[1]._pass); address += 2;    //SETP

  _obj_variable.screen_password_default = _obj_variable._lst_password_info[0]._pass;
  _obj_variable.screen_password = _obj_variable._lst_password_info[1]._pass;

  EEPROM.commit();
}


void Configuration_WriteEEPROM()
{
  // Serial.println("CONFIG WRITE");
  int address=100;int k = 0;
  for(int i=0;i<TOTALCHANNEL;i++)
  {
    
    if(i==0)//DP1
    {
      _obj_variable._lst_parameter_info[0]._input_type     =1;
      _obj_variable._lst_parameter_info[0]._sensor_no      =0;
      _obj_variable._lst_parameter_info[0]._is_calibration =0;
      _obj_variable._lst_parameter_info[0]._is_negative    =0;
      _obj_variable._lst_parameter_info[0]._f_demping      =5;
      _obj_variable._lst_parameter_info[0]._r_zero         =0;
      _obj_variable._lst_parameter_info[0]._r_span         =600;
      _obj_variable._lst_parameter_info[0]._c_zero         =8192;
      _obj_variable._lst_parameter_info[0]._c_span         =11380;
      _obj_variable._lst_parameter_info[0]._disp_status    =1;
      _obj_variable._lst_parameter_info[0]._decimal        =1;
      _obj_variable._lst_parameter_info[0]._offset         =1;
      _obj_variable._lst_parameter_info[0]._f_demping      =10;
      _obj_variable._lst_parameter_info[0]._cali_offset    =0;
      _obj_variable._lst_parameter_info[0]._unit_id        =0;

      _obj_variable._lst_parameter_info[0]._op_r_zero=_obj_variable._lst_parameter_info[0]._r_zero;
      _obj_variable._lst_parameter_info[0]._op_r_span=_obj_variable._lst_parameter_info[0]._r_span;
      _obj_variable._lst_parameter_info[0]._op_c_zero=27000;
      _obj_variable._lst_parameter_info[0]._op_c_span=5900;
    }
     
     if(i==1)
     {
      _obj_variable._lst_parameter_info[1]._input_type     =1;
      _obj_variable._lst_parameter_info[1]._sensor_no      =1;
      _obj_variable._lst_parameter_info[1]._is_calibration =0;
      _obj_variable._lst_parameter_info[1]._is_negative    =0;
      _obj_variable._lst_parameter_info[1]._f_demping      =5;
      _obj_variable._lst_parameter_info[1]._r_zero         =0;
      _obj_variable._lst_parameter_info[1]._r_span         =999;
      _obj_variable._lst_parameter_info[1]._c_zero         =8192;
      _obj_variable._lst_parameter_info[1]._c_span         =11380;
      _obj_variable._lst_parameter_info[1]._disp_status    =1;
      _obj_variable._lst_parameter_info[1]._decimal        =1;
      _obj_variable._lst_parameter_info[1]._offset         =1;
      _obj_variable._lst_parameter_info[1]._f_demping      =10;
      _obj_variable._lst_parameter_info[1]._cali_offset    =0;
      _obj_variable._lst_parameter_info[1]._unit_id        =0;
      _obj_variable._lst_parameter_info[1]._op_r_zero=_obj_variable._lst_parameter_info[1]._r_zero;
      _obj_variable._lst_parameter_info[1]._op_r_span=_obj_variable._lst_parameter_info[1]._r_span;
      _obj_variable._lst_parameter_info[1]._op_c_zero=27000;
      _obj_variable._lst_parameter_info[1]._op_c_span=5900;
     }

     if(i==2)
     {
      _obj_variable._lst_parameter_info[2]._input_type     =2;
      _obj_variable._lst_parameter_info[2]._sensor_no      =0;
      _obj_variable._lst_parameter_info[2]._is_calibration =1;
      _obj_variable._lst_parameter_info[2]._is_negative    =0;
      _obj_variable._lst_parameter_info[2]._f_demping      =5;
      _obj_variable._lst_parameter_info[2]._r_zero         =0;
      _obj_variable._lst_parameter_info[2]._r_span         =120;
      _obj_variable._lst_parameter_info[2]._c_zero         =8192;
      _obj_variable._lst_parameter_info[2]._c_span         =11380;
      _obj_variable._lst_parameter_info[2]._disp_status    =1;
      _obj_variable._lst_parameter_info[2]._decimal        =1;
      _obj_variable._lst_parameter_info[2]._offset         =1;
      _obj_variable._lst_parameter_info[2]._f_demping      =10;
      _obj_variable._lst_parameter_info[2]._cali_offset    =0;
      _obj_variable._lst_parameter_info[2]._unit_id        =0;
      _obj_variable._lst_parameter_info[2]._op_r_zero=_obj_variable._lst_parameter_info[0]._r_zero;
      _obj_variable._lst_parameter_info[2]._op_r_span=_obj_variable._lst_parameter_info[0]._r_span;
      _obj_variable._lst_parameter_info[2]._op_c_zero=27000;
      _obj_variable._lst_parameter_info[2]._op_c_span=5900;
     }
      
    _obj_variable._lst_parameter_info[i]._id = (i+1); //byte_to_byte(address,_obj_variable._lst_parameter_info[i]._id); address += 1;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._decimal); address += 1;
    short_to_byte(address,_obj_variable._lst_parameter_info[i]._offset); address += 2;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._f_demping); address += 1;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._is_calibration); address += 1;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._sensor_no); address += 1;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._cali_offset); address += 1;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._input_type); address += 1;
    short_to_byte(address,_obj_variable._lst_parameter_info[i]._r_zero); address += 2;
    short_to_byte(address,_obj_variable._lst_parameter_info[i]._r_span); address += 2;
    ushort_to_byte(address,_obj_variable._lst_parameter_info[i]._c_zero); address += 2;
    ushort_to_byte(address,_obj_variable._lst_parameter_info[i]._c_span); address += 2;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._disp_status); address += 1;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._unit_id); address += 1;
    byte_to_byte(address,_obj_variable._lst_parameter_info[i]._is_negative); address += 1;

    short_to_byte(address,_obj_variable._lst_parameter_info[i]._op_r_zero); address += 2;
    short_to_byte(address,_obj_variable._lst_parameter_info[i]._op_r_span); address += 2;
    ushort_to_byte(address,_obj_variable._lst_parameter_info[i]._op_c_zero); address += 2;
    ushort_to_byte(address,_obj_variable._lst_parameter_info[i]._op_c_span); address += 2;

    strcpy(_obj_variable._lst_parameter_info[i]._name , (String(_obj_variable._lst_parameter_info[i]._id) + "IP").c_str());
    
    _obj_variable._lst_parameter_info[i]._pv  =  0;
    _obj_variable._lst_parameter_info[i]._count  =  0;
    _obj_variable._lst_parameter_info[i].ip_pval =0;
    _obj_variable._lst_parameter_info[i]._is_under=false;
    _obj_variable._lst_parameter_info[i]._is_over=false;

    for(int j=0;j<PER_CH_ALARM;j++)
    {
      _obj_variable._lst_parameter_alarm_info[k]._type = j;
      
      if(j==0 )//LOW-LOLO
      {
    
        if(i==0)
        _obj_variable._lst_parameter_alarm_info[k]._value = 200;
        if(i==1)
        _obj_variable._lst_parameter_alarm_info[k]._value = 200;
        if(i==2)
        _obj_variable._lst_parameter_alarm_info[k]._value = 10;
        
        _obj_variable._lst_parameter_alarm_info[k]._display_no = (i*2);
      }
      else if(j==1 )//HIGH-HIHI
      {
        if(i==0)
        _obj_variable._lst_parameter_alarm_info[k]._value = 400;
        if(i==1)
        _obj_variable._lst_parameter_alarm_info[k]._value = 400;
        if(i==2)
        _obj_variable._lst_parameter_alarm_info[k]._value = 40;

        _obj_variable._lst_parameter_alarm_info[k]._display_no = (i*2)+1;
      }
       
      _obj_variable._lst_parameter_alarm_info[k]._delay = 0; 
      _obj_variable._lst_parameter_alarm_info[k]._status = 1;

      _obj_variable._lst_parameter_alarm_info[k]._id=(k+1); //byte_to_byte(address,_obj_variable._lst_parameter_alarm_info[k]._id); address += 1;
      _obj_variable._lst_parameter_alarm_info[k]._para_id=(_obj_variable._lst_parameter_info[i]._id); //byte_to_byte(address,_obj_variable._lst_parameter_alarm_info[k]._para_id); address += 1;
      byte_to_byte(address,_obj_variable._lst_parameter_alarm_info[k]._type); address += 1;
      short_to_byte(address,_obj_variable._lst_parameter_alarm_info[k]._value); address += 2;
      byte_to_byte(address,_obj_variable._lst_parameter_alarm_info[k]._delay); address += 1;
      byte_to_byte(address,_obj_variable._lst_parameter_alarm_info[k]._display_no); address += 1;
      byte_to_byte(address,_obj_variable._lst_parameter_alarm_info[k]._status); address += 1;
      strcpy(_obj_variable._lst_parameter_alarm_info[k]._name ,( String(_obj_variable._lst_parameter_alarm_info[k]._para_id) + "IP" + String(j+1)).c_str());
      k++;
    }
    // Serial.println("CONFIG_ADD  "+String(address));
  }

  // Serial.println("Write Comm Address : " + String(address));
  byte_to_byte(address,1); address += 1;                  //NODE,1,
  byte_to_byte(address,1); address += 1;                  //BOUD,1,
  byte_to_byte(address,0); address += 1;                  //PARI,0,
  byte_to_byte(address,1); address += 1;                  //STOP,1,
  // Serial.println("OTHER_ADD  "+String(address));
  //BUZZER
  byte_to_byte(address,1); address += 1;
  // Serial.println("Buzzer Write Fre  "+String(address));
  byte_to_byte(address,25); address += 1;
  
  // //OTHER
  short_to_byte(address,1); address += 1; //RTC
  
  //PASSWORD
  short_to_byte(address,0000); address += 2;    //CONF
  short_to_byte(address,1111); address += 2;    //SETP

  // for(int i=0;i< I2C_DATA_SIZE ;i++)
  // {
  //   i2c_data_info *_obj_sensor = new i2c_data_info();
  //   _obj_sensor->_sensor_model_1=address; 
  //   _obj_variable._lst_i2c_data_info_eeprom.add(_obj_sensor);
  //   byte_to_byte(address,_obj_variable._lst_i2c_data_info[i]._sensor_model);  address += 1;
  // }
  //_obj_variable._lst_communication.add(_obj);

  EEPROM.commit();
   }

void Configuration_ReadEEPROM()
{
  int address=100, k =0;
  
  for(int i=0;i< TOTALCHANNEL ; i++)
  {
    _obj_variable._lst_parameter_info[i]._id = (i+1);
    //_obj_variable._lst_parameter_info[i]._id = EEPROM_READ_BYTE(address);      //_obj_variable._lst_parameter_info_eeprom[i]._id = address; 
    //address += 1;
    _obj_variable._lst_parameter_info[i]._decimal = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_info_eeprom[i]._decimal = address; 
    address += 1;
    _obj_variable._lst_parameter_info[i]._offset = EEPROM_READ_SHORT(address); //_obj_variable._lst_parameter_info_eeprom[i]._offset = address; 
    address += 2;
    _obj_variable._lst_parameter_info[i]._f_demping = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_info_eeprom[i]._f_demping = address; 
    address += 1;
    _obj_variable._lst_parameter_info[i]._is_calibration = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_info_eeprom[i]._is_calibration = address; 
    address += 1;
    _obj_variable._lst_parameter_info[i]._sensor_no = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_info_eeprom[i]._sensor_no = address; 
    address += 1;
    _obj_variable._lst_parameter_info[i]._cali_offset = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_info_eeprom[i]._cali_offset = address; 
    address += 1;
    _obj_variable._lst_parameter_info[i]._input_type = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_info_eeprom[i]._input_type = address; 
    address += 1;
    _obj_variable._lst_parameter_info[i]._r_zero = EEPROM_READ_SHORT(address); //_obj_variable._lst_parameter_info_eeprom[i]._r_zero = address; 
    address += 2;
    _obj_variable._lst_parameter_info[i]._r_span = EEPROM_READ_SHORT(address); //_obj_variable._lst_parameter_info_eeprom[i]._r_span = address; 
    address += 2;
    _obj_variable._lst_parameter_info[i]._c_zero = EEPROM_READ_SHORT(address); //_obj_variable._lst_parameter_info_eeprom[i]._c_zero = address; 
    address += 2;
    _obj_variable._lst_parameter_info[i]._c_span = EEPROM_READ_SHORT(address); //_obj_variable._lst_parameter_info_eeprom[i]._c_span = address; 
    address += 2;
    _obj_variable._lst_parameter_info[i]._disp_status = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_info_eeprom[i]._disp_status = address; 
    address += 1;
    _obj_variable._lst_parameter_info[i]._unit_id = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_info_eeprom[i]._unit_id = address; 
    address += 1;
    _obj_variable._lst_parameter_info[i]._is_negative = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_info_eeprom[i]._is_negative = address; 
    address += 1;

    _obj_variable._lst_parameter_info[i]._op_r_zero = EEPROM_READ_SHORT(address); //_obj_variable._lst_parameter_info_eeprom[i]._op_r_zero = address; 
    address += 2;
    _obj_variable._lst_parameter_info[i]._op_r_span = EEPROM_READ_SHORT(address); //_obj_variable._lst_parameter_info_eeprom[i]._op_r_span = address; 
    address += 2;
    _obj_variable._lst_parameter_info[i]._op_c_zero = EEPROM_READ_SHORT(address); //_obj_variable._lst_parameter_info_eeprom[i]._op_c_zero = address; 
    address += 2;
    _obj_variable._lst_parameter_info[i]._op_c_span = EEPROM_READ_SHORT(address); //_obj_variable._lst_parameter_info_eeprom[i]._op_c_span = address; 
    address += 2;

    strcpy(_obj_variable._lst_parameter_info[i]._name , (String(_obj_variable._lst_parameter_info[i]._id) + "IP").c_str());
    
    // _obj_variable._lst_parameter_info_reg[i]._r_zero = menu_modbus_address; menu_modbus_address++;
    // _obj_variable._lst_parameter_info_reg[i]._r_span = menu_modbus_address; menu_modbus_address++;
    // _obj_variable._lst_parameter_info_reg[i]._c_zero = menu_modbus_address; menu_modbus_address++;
    // _obj_variable._lst_parameter_info_reg[i]._c_span = menu_modbus_address; menu_modbus_address++;
    // _obj_variable._lst_parameter_info_reg[i]._c_set_reg = menu_modbus_address; menu_modbus_address++;


    _obj_variable._lst_parameter_info[i]._pv  =  0;
    _obj_variable._lst_parameter_info[i]._count  =  0;
    _obj_variable._lst_parameter_info[i].ip_pval = 0;
    _obj_variable._lst_parameter_info[i].ip_retry = 0;
    _obj_variable._lst_parameter_info[i]._is_under=false;
    _obj_variable._lst_parameter_info[i]._is_over=false;
    _obj_variable._lst_parameter_info[i]._is_sensor_connect = false;
    _obj_variable._lst_parameter_info[i]._is_firsttime = true;
    _obj_variable._lst_parameter_info[i]._sensor_count = 0;
    _obj_variable._lst_parameter_info[i]._display_no = 0;

    for(int j =0 ; j < _obj_variable._lst_parameter_info[i]._f_demping ;j++)
    _obj_variable._lst_parameter_info[i]._lst_demping[j] = 0;

    //menu_modbus_address=31;
    for(int j=0;j<PER_CH_ALARM;j++)
    {
      _obj_variable._lst_parameter_alarm_info[k]._id=(k+1); 
      _obj_variable._lst_parameter_alarm_info[k]._para_id=(_obj_variable._lst_parameter_info[i]._id);

      //_obj_variable._lst_parameter_alarm_info[k]._id = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_alarm_info_eeprom[k]._id= address; 
      //address += 1;
      //_obj_variable._lst_parameter_alarm_info[k]._para_id = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_alarm_info_eeprom[k]._para_id= address; 
      //address += 1;
      _obj_variable._lst_parameter_alarm_info[k]._type = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_alarm_info_eeprom[k]._type= address; 
      address += 1;
      _obj_variable._lst_parameter_alarm_info[k]._value = EEPROM_READ_SHORT(address); //_obj_variable._lst_parameter_alarm_info_eeprom[k]._value= address; 
      address += 2;
      _obj_variable._lst_parameter_alarm_info[k]._delay = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_alarm_info_eeprom[k]._delay= address; 
      address += 1;
      _obj_variable._lst_parameter_alarm_info[k]._display_no = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_alarm_info_eeprom[k]._display_no= address; 
      address += 1;
      _obj_variable._lst_parameter_alarm_info[k]._status = EEPROM_READ_BYTE(address); //_obj_variable._lst_parameter_alarm_info_eeprom[k]._status= address; 
      address += 1;
      
      strcpy(_obj_variable._lst_parameter_alarm_info[k]._name ,( String(_obj_variable._lst_parameter_alarm_info[k]._para_id) + "IP" + String(j+1)).c_str());
       
      //Modbus Register Mapping
      // _obj_variable._lst_parameter_alarm_info_reg[k]._value = modbus_alarm_sp_address; modbus_alarm_sp_address++;
      // _obj_variable._lst_parameter_alarm_info_reg[k]._status = menu_modbus_address;
      // _obj_variable._lst_parameter_alarm_info_reg[k]._display_no = menu_modbus_address;
      // _obj_variable._lst_parameter_alarm_info_reg[k]._type = menu_modbus_address; menu_modbus_address++;

      _obj_variable._lst_parameter_alarm_info[k]._a_log = 0; 
      _obj_variable._lst_parameter_alarm_info[k]._is_led_status =0;
      _obj_variable._lst_parameter_alarm_info[k]._logtime =0;
      _obj_variable._lst_parameter_alarm_info[k]._is_ack = false;
      k++;
    }
  
  }
  
  // Serial.println("Read Comm Address : " + String(address));
  for(int k=0;k < COMMUNICATION_SIZE ;k++)
  {
    _obj_variable._lst_communication[k]._value = EEPROM_READ_BYTE(address); 
    //_obj_variable._lst_communication_eeprom[k]._value_1 = address; 
    address += 1; 
    // Serial.println(_obj_variable._lst_communication_eeprom[k]._value_1);
  }
  
  #pragma region other_info
  
  // Serial.println("Other_Read   " + String(address));

    //_obj_variable._lst_other_info_eeprom[0]._value = address;
    _obj_variable._lst_other_info[0]._id    = 1 ;
    _obj_variable._lst_other_info[0]._value =  EEPROM_READ_BYTE(address); address += 1; ;
    strcpy(_obj_variable._lst_other_info[0]._name  , "BUZZER") ;
    // Serial.println("Buzzer " +String(_obj_variable._lst_other_info[0]._value));

    // Serial.println("Buzzer Fre  "+String(address));
    //_obj_variable._lst_other_info_eeprom[1]._value = address;
    _obj_variable._lst_other_info[1]._id    = 2 ;
    _obj_variable._lst_other_info[1]._value=25;
    _obj_variable._lst_other_info[1]._value =  EEPROM_READ_BYTE(address); address += 1; ;
    strcpy(_obj_variable._lst_other_info[1]._name  , "BUZZ_FRE") ;
    // Serial.println("Buzzer_Freq " +String(_obj_variable._lst_other_info[1]._value));
    
    //_obj_variable._lst_other_info_eeprom[2]._value = address;
    _obj_variable._lst_other_info[2]._id    = 3 ;
    _obj_variable._lst_other_info[2]._value = EEPROM_READ_BYTE(address); address += 1;
    strcpy(_obj_variable._lst_other_info[2]._name  , "RTC") ;
    // Serial.println("RTC  " +String(_obj_variable._lst_other_info[2]._value));

  #pragma endregion
  
  // Serial.println("Password_Read   " + String(address));
  #pragma region password_info

  //_obj_variable._lst_password_info_eeprom[0]._pass = address;
  _obj_variable._lst_password_info[0]._id =1;
  _obj_variable._lst_password_info[0]._pass = EEPROM_READ_SHORT(address);address += 2;
  strcpy(_obj_variable._lst_password_info[0]._name , "CONF");
  
  //_obj_variable._lst_password_info_eeprom[1]._pass = address;
  _obj_variable._lst_password_info[1]._id =1;
  _obj_variable._lst_password_info[1]._pass = EEPROM_READ_SHORT(address);address += 2;
  strcpy(_obj_variable._lst_password_info[1]._name , "SETP");
 
  _obj_variable.screen_password_default = _obj_variable._lst_password_info[0]._pass;
  _obj_variable.screen_password = _obj_variable._lst_password_info[1]._pass;

  Serial.println("_obj_variable.screen_password_default  "+String(_obj_variable.screen_password_default));
  Serial.println("_obj_variable.screen_password  "+String(_obj_variable.screen_password));

  #pragma endregion

}

// void comm_name_list()
// {
//   _obj_variable._lst_comm->add("NODE");
//   _obj_variable._lst_comm->add("BOUD");
//   _obj_variable._lst_comm->add("PARI");
//   _obj_variable._lst_comm->add("STOP");
//   _obj_variable._lst_comm->add("DATA");
//   _obj_variable._lst_comm->add("IP1 ");
//   _obj_variable._lst_comm->add("IP2 ");
//   _obj_variable._lst_comm->add("IP3 ");
//   _obj_variable._lst_comm->add("IP4 ");
//   _obj_variable._lst_comm->add("SUB1");
//   _obj_variable._lst_comm->add("SUB2");
//   _obj_variable._lst_comm->add("SUB3");
//   _obj_variable._lst_comm->add("SUB4");
//   _obj_variable._lst_comm->add("GET1");
//   _obj_variable._lst_comm->add("GET2");
//   _obj_variable._lst_comm->add("GET3");
//   _obj_variable._lst_comm->add("GET4");
//   _obj_variable._lst_comm->add("DN11");
//   _obj_variable._lst_comm->add("DN12");
//   _obj_variable._lst_comm->add("DN13");
//   _obj_variable._lst_comm->add("DN14");
//   _obj_variable._lst_comm->add("DN21");
//   _obj_variable._lst_comm->add("DN22");
//   _obj_variable._lst_comm->add("DN23");
//   _obj_variable._lst_comm->add("DN24");
//   _obj_variable._lst_comm->add("SIP1");
//   _obj_variable._lst_comm->add("SIP2");
//   _obj_variable._lst_comm->add("SIP3");
//   _obj_variable._lst_comm->add("SIP4");
//   _obj_variable._lst_comm->add("PRNO");
//   // _obj_variable._lst_comm->add("WIFU");
//   // _obj_variable._lst_comm->add("WIFP");
//   // _obj_variable._lst_comm->add("RFFR");
//   // _obj_variable._lst_comm->add("RFFA");
//   // _obj_variable._lst_comm->add("RFMO");
//   // _obj_variable._lst_comm->add("RFAN");
//   // _obj_variable._lst_comm->add("RFNO");
//   // _obj_variable._lst_comm->add("RFNE");
//   // _obj_variable._lst_comm->add("RFBU");
//   // _obj_variable._lst_comm->add("RFPA");
// }

void get_i2c_data_info()
{
    _obj_variable._lst_i2c_data_info[0]._id =1;
    strcpy(_obj_variable._lst_i2c_data_info[0]._name ,"TRH");
    _obj_variable._lst_i2c_data_info[0]._pin_sda =14;
    _obj_variable._lst_i2c_data_info[0]._pin_scl =13;
    _obj_variable._lst_i2c_data_info[0]._no_of_values =2;
    _obj_variable._lst_i2c_data_info[0]._status =1;
    _obj_variable._lst_i2c_data_info[0]._type = I2C_TEMP_RH;
    _obj_variable._lst_i2c_data_info[0]._connected = false;

    _obj_variable._lst_i2c_data_info[1]._id = 2;
    strcpy(_obj_variable._lst_i2c_data_info[1]._name ,"DP1");
    _obj_variable._lst_i2c_data_info[1]._pin_sda = 32;
    _obj_variable._lst_i2c_data_info[1]._pin_scl = 33;
    _obj_variable._lst_i2c_data_info[1]._no_of_values = 1;
    _obj_variable._lst_i2c_data_info[1]._status = 1;
    _obj_variable._lst_i2c_data_info[1]._type = I2C_DP;
    _obj_variable._lst_i2c_data_info[1]._connected = false;

    _obj_variable._lst_i2c_data_info[2]._id =3;
    strcpy(_obj_variable._lst_i2c_data_info[2]._name ,"ADC");
    _obj_variable._lst_i2c_data_info[2]._pin_sda = 32;
    _obj_variable._lst_i2c_data_info[2]._pin_scl = 33;
    _obj_variable._lst_i2c_data_info[2]._no_of_values =4;
    _obj_variable._lst_i2c_data_info[2]._status =0;
    _obj_variable._lst_i2c_data_info[2]._type = I2C_ADS_1115;
    _obj_variable._lst_i2c_data_info[2]._connected = false;

    _obj_variable._lst_i2c_data_info[3]._id =4;
    strcpy(_obj_variable._lst_i2c_data_info[3]._name ,"RTC");
    _obj_variable._lst_i2c_data_info[3]._pin_sda =32;
    _obj_variable._lst_i2c_data_info[3]._pin_scl =33;
    _obj_variable._lst_i2c_data_info[3]._no_of_values =6;
    _obj_variable._lst_i2c_data_info[3]._status = 1;
    _obj_variable._lst_i2c_data_info[3]._type = I2C_RTC;
    _obj_variable._lst_i2c_data_info[3]._connected = false;

    _obj_variable._lst_i2c_data_info[4]._id =5;
    strcpy(_obj_variable._lst_i2c_data_info[4]._name ,"DP2");
    _obj_variable._lst_i2c_data_info[4]._pin_sda =27;
    _obj_variable._lst_i2c_data_info[4]._pin_scl =14;
    _obj_variable._lst_i2c_data_info[4]._no_of_values =2;
    _obj_variable._lst_i2c_data_info[4]._status =0;
    _obj_variable._lst_i2c_data_info[4]._type = I2C_DP;
    _obj_variable._lst_i2c_data_info[4]._connected = false;

    _obj_variable._lst_i2c_data_info[5]._id =6;
    strcpy(_obj_variable._lst_i2c_data_info[5]._name ,"DP3");
    _obj_variable._lst_i2c_data_info[5]._pin_sda =25;
    _obj_variable._lst_i2c_data_info[5]._pin_scl =26;
    _obj_variable._lst_i2c_data_info[5]._no_of_values =1;
    _obj_variable._lst_i2c_data_info[5]._status =0;
    _obj_variable._lst_i2c_data_info[5]._type = I2C_DP;
    _obj_variable._lst_i2c_data_info[5]._connected = false;
    
    for (int i = 0; i < I2C_DATA_SIZE ; i++)
    {
      for (int j = 0; j < _obj_variable._lst_i2c_data_info[i]._no_of_values; j++)
      {
        if(_obj_variable._lst_i2c_data_info[i]._type  ==  I2C_ADS_1115 || _obj_variable._lst_i2c_data_info[i]._type  == I2C_TEMP_RH  || _obj_variable._lst_i2c_data_info[i]._type  ==  I2C_DP)
        {
          _obj_variable._lst_i2c_data_info[i]._lst_value[j] = 0;
          _obj_variable._lst_i2c_data_info[i]._lst_count[j] = 0;
        }
      }      
    }
    
}

// i2c_data_info *_obj_honeywell_t_rh_1 = new i2c_data_info();
    // _obj_honeywell_t_rh_1->_id = 1; 
    // _obj_honeywell_t_rh_1->_name="TRH1"; 
    // _obj_honeywell_t_rh_1->_pin_sda=14; 
    // _obj_honeywell_t_rh_1->_pin_scl =13; 
    // _obj_honeywell_t_rh_1->_no_of_values =2; 
    // _obj_honeywell_t_rh_1->_status =0;
    // _obj_honeywell_t_rh_1->_type = I2C_TEMP_RH;
    // _obj_variable._lst_i2c_data_info.add(_obj_honeywell_t_rh_1);
  // i2c_data_info *honeywell_dp_1 = new i2c_data_info();
    // honeywell_dp_1->_id = 2; 
    // honeywell_dp_1->_name="DP1 "; 
    // honeywell_dp_1->_pin_sda=32; 
    // honeywell_dp_1->_pin_scl =33; 
    // honeywell_dp_1->_no_of_values =1; 
    // honeywell_dp_1->_status =1;
    // honeywell_dp_1->_type = I2C_DP;
    // _obj_variable._lst_i2c_data_info.add(honeywell_dp_1);
  // i2c_data_info *ads_1115 = new i2c_data_info();
    // ads_1115->_id = 3; 
    // ads_1115->_name="ADC "; 
    // ads_1115->_pin_sda=32; 
    // ads_1115->_pin_scl =33; 
    // ads_1115->_no_of_values =4; 
    // ads_1115->_status =0;
    // ads_1115->_type = I2C_ADS_1115;
    // _obj_variable._lst_i2c_data_info.add(ads_1115);
  // i2c_data_info *rtc_ds1307 = new i2c_data_info();
    // rtc_ds1307->_id = 4; 
    // rtc_ds1307->_name="RTC "; 
    // rtc_ds1307->_pin_sda=32; 
    // rtc_ds1307->_pin_scl =33; 
    // rtc_ds1307->_no_of_values =6; 
    // rtc_ds1307->_status =0;
    // rtc_ds1307->_type = I2C_RTC;
    // _obj_variable._lst_i2c_data_info.add(rtc_ds1307);
  // i2c_data_info *honeywell_t_rh_2 = new i2c_data_info();
    // honeywell_t_rh_2->_id = 5; 
    // honeywell_t_rh_2->_name="TRH2"; 
    // honeywell_t_rh_2->_pin_sda=32; 
    // honeywell_t_rh_2->_pin_scl =33; 
    // honeywell_t_rh_2->_no_of_values =2; 
    // honeywell_t_rh_2->_status =0;
    // honeywell_t_rh_2->_type = I2C_TEMP_RH;
    // _obj_variable._lst_i2c_data_info.add(honeywell_t_rh_2);
  // i2c_data_info *honeywell_t_rh_3 = new i2c_data_info();
    // honeywell_t_rh_3->_id = 6; 
    // honeywell_t_rh_3->_name="TRH3"; 
    // honeywell_t_rh_3->_pin_sda=25; 
    // honeywell_t_rh_3->_pin_scl =26; 
    // honeywell_t_rh_3->_no_of_values =2; 
    // honeywell_t_rh_3->_status =0;
    // honeywell_t_rh_3->_type = I2C_TEMP_RH;
    // _obj_variable._lst_i2c_data_info.add(honeywell_t_rh_3);
  // i2c_data_info *honeywell_dp_2 = new i2c_data_info();
    // honeywell_dp_2->_id = 7; 
    // honeywell_dp_2->_name="DP2 "; 
    // honeywell_dp_2->_pin_sda=14; 
    // honeywell_dp_2->_pin_scl =13; 
    // honeywell_dp_2->_no_of_values =1; 
    // honeywell_dp_2->_status =1;
    // honeywell_dp_2->_type = I2C_DP;
    // _obj_variable._lst_i2c_data_info.add(honeywell_dp_2);
  // i2c_data_info *honeywell_dp_3 = new i2c_data_info();
    // honeywell_dp_3->_id = 8; 
    // honeywell_dp_3->_name="DP3 "; 
    // honeywell_dp_3->_pin_sda=25; 
    // honeywell_dp_3->_pin_scl =26; 
    // honeywell_dp_3->_no_of_values =1; 
    // honeywell_dp_3->_status =1;
    // honeywell_dp_3->_type = I2C_DP;    
    // _obj_variable._lst_i2c_data_info.add(honeywell_dp_3);
  // for (int i = 0; i < _obj_variable._lst_i2c_data_info.size(); i++)
    // {
    //   for (int j = 0; j < _obj_variable._lst_i2c_data_info[i]._no_of_values; j++)
    //   {
    //     _obj_variable._lst_i2c_data_info[i]._lst_value->add(0);
    //     _obj_variable._lst_i2c_data_info[i]._lst_count->add(0);
    //   }      
    // }
// }

#pragma endregion endfileread

void int32_t_to_int16_t(int32_t value)
{
  _obj_variable.modbusArray[0] = (int16_t) ((value >> 16) & 0xFFFF) ;
  _obj_variable.modbusArray[1] = (int16_t) ((value) & 0xFFFF) ;
}


void ota_wifi_access_point()
{
  // MDNS.begin(host);
  server.on("/", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", serverIndex);
  });
  server.on("/update", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
  }, []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        Serial.printf("Update: %s\n", upload.filename.c_str());

        if (upload.filename == "spiffs.bin") {
                if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_SPIFFS)) { //start with max available size
                    Update.printError(Serial);
                    // Serial.println("SPIFFS");
                }
            } else {
                if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH)) { //start with max available size
                    Update.printError(Serial);
                    // Serial.println("FIRMWARE");
                }
            }
        // if (!Update.begin()) { //start with max available size
        //   Update.printError(Serial);
        // }

        
        
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
          update = "DONE";
          _obj_variable._obj_display.w_display_segment(update, 0, "    ",0, "    ", 0, "    ", 0);
  
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      } else {
        Serial.printf("Update Failed Unexpectedly (likely broken connection): status=%d\n", upload.status);
      }
  });
  server.begin();
  // MDNS.addService("http", "tcp", 80);
  // Serial.printf("Ready! Open http://%s.local in your browser\n", host);
}

void set_alarm_flag(int j, bool is_generate)
{
  if (is_generate)
  {
    // Serial.println(String(j)+"  | "+ String(is_generate));
    if (((millis() - _obj_variable._lst_parameter_alarm_info[j]._logtime) / 1000) > _obj_variable._lst_parameter_alarm_info[j]._delay)
    {
      // Serial.println(String(j)+"  | "+ String(_obj_variable._lst_parameter_alarm_info[j]._a_log));
      is_relay_1_on = true;
      if (_obj_variable._lst_parameter_alarm_info[j]._a_log == 0)
      {
        _obj_variable._lst_parameter_alarm_info[j]._a_log = 1;
        _obj_variable._lst_parameter_alarm_info[j]._is_ack = false;
        
      }
    }
  }
  else
  {
    _obj_variable._lst_parameter_alarm_info[j]._logtime = millis();
    _obj_variable._lst_parameter_alarm_info[j]._a_log = 0;
  }
}

void set_alarm_led_status()
{
  for (int i = 0; i < 6 ; i++)
  {
    bool isFind = false;
    int _is_led_status = LED_OFF;
    for (int j = 0; j < (TOTALCHANNEL * PER_CH_ALARM); j++)
    {
      if (_obj_variable._lst_parameter_alarm_info[j]._display_no == i)
      {
        isFind = true;
        if (_obj_variable._lst_parameter_alarm_info[j]._a_log == 1 && !_obj_variable._lst_parameter_alarm_info[j]._is_ack) //ALARM STATUS
        {
          _is_led_status = LED_BLINK;
        }
        else if (_obj_variable._lst_parameter_alarm_info[j]._a_log == 1 && _obj_variable._lst_parameter_alarm_info[j]._is_ack) //ALARM STATUS
        {
          if (_is_led_status != LED_BLINK)
            _is_led_status = LED_ON;
        }
      } 
    }

    if (isFind)
    {
      for (int j = 0; j < (TOTALCHANNEL * PER_CH_ALARM); j++)
      {
        if (_obj_variable._lst_parameter_alarm_info[j]._display_no == i)
        {
          _obj_variable._lst_parameter_alarm_info[j]._is_led_status = _is_led_status;
          // Serial.println(String(_obj_variable._lst_parameter_alarm_info[j]._is_led_status )+ " LED_STATUS  -  "+String(j));
        }
          
      }
    }
  }
}

void alarm_generate()
{
  
  for (int i = 0; i < TOTALCHANNEL ; i++)
  {
    for (int j = 0; j < (TOTALCHANNEL * PER_CH_ALARM); j++)
    {
      if (_obj_variable._lst_parameter_info[i]._id == _obj_variable._lst_parameter_alarm_info[j]._para_id)
      {
        if (_obj_variable._lst_parameter_alarm_info[j]._status && _obj_variable._lst_parameter_info[i]._disp_status)
        {
          float c_val = float(_obj_variable._lst_parameter_info[i]._pv);
          // Serial.println("C_Val  "+String(_obj_variable._lst_parameter_alarm_info[j]._type)+ " | "+String(i));
          if (_obj_variable._lst_parameter_alarm_info[j]._type == 0 || _obj_variable._lst_parameter_alarm_info[j]._type == 2) //LOW
          {
            if (_obj_variable._lst_parameter_alarm_info[j]._value > c_val)
            {
              set_alarm_flag(j, true);
              // Serial.println(" LOW_GENERATE   "+ String(j));
            }
              
            //else if (_obj_variable._lst_parameter_info[i]._is_over || _obj_variable._lst_parameter_info[i]._is_under || _obj_variable._lst_parameter_info[i]._is_sensor_connect)
            //   set_alarm_flag(j, true);
            
            else
              set_alarm_flag(j, false);
          }
          else if (_obj_variable._lst_parameter_alarm_info[j]._type == 1 || _obj_variable._lst_parameter_alarm_info[j]._type == 3) //HIGH
          {
            if (_obj_variable._lst_parameter_alarm_info[j]._value < c_val)
              set_alarm_flag(j, true);
            //else if (_obj_variable._lst_parameter_info[i]._is_over || _obj_variable._lst_parameter_info[i]._is_under || _obj_variable._lst_parameter_info[i]._is_sensor_connect)
            //   set_alarm_flag(j, true);
            else
              set_alarm_flag(j, false);
          }
        }
        else
        {
          _obj_variable._lst_parameter_alarm_info[j]._a_log = 0;
          _obj_variable._lst_parameter_alarm_info[j]._is_led_status = LED_OFF;
        }
      }
    }
  }
}

// String CheckStringLength(String str, int len, String ex_char)
// {
//   String val = "";
//   for (int i = 0; i < len; i++)
//     val += ex_char;

//   for (int i = str.length()-1; i >= 0; i--)
//   {
//     if(i<len)
//       val[i] = str[i];
//     else
//       val[0] = str[i];
//   }

//   return val;
// }

String CheckStringLength(String str, int len, String ex_char)
{
  String val = "";
  for (int i = str.length(); i < len; i++)
    val += ex_char;

  val += str;
  //val = val.substring(0,len);


  return val;
}


String CheckStringLength(String str, int len, String ex_char, int decimal)
{
  String val = "";

  for (int i = str.length(); i < len; i++)
    val += ex_char;

  if (decimal == 1 && str.length() == 4)
  {
    if (str[0] == ' ' && str[1] == ' ' && str[2] == ' ')
      str[2] = '0';
  }
  if (decimal == 2 && str.length() == 4)
  {
    if (str[0] == ' ' && str[1] == ' ' && str[2] == ' ')
    {
      str[1] = '0';
      str[2] = '0';
      str[2] = '0';
    }
    else if (str[0] == ' ' && str[1] == ' ')
    {
      str[1] = '0';
      str[2] = '0';
    }
    // else if(str[0]==' ' )
    //   {str[1]='0';}
  }

  val += str;
  return val;
}

int find_avg_value(int index,int16_t pv)
{
  int _avg_val = 0;
  for (int k = 1; k < (_obj_variable._lst_parameter_info[index]._f_demping) ; k++)
  {
    // if(index==2)
    //   Serial.println(String(k) + " : " + String(_obj_variable._lst_parameter_info[index]._lst_demping[k]));
    _avg_val += _obj_variable._lst_parameter_info[index]._lst_demping[k];
    _obj_variable._lst_parameter_info[index]._lst_demping[k-1] = _obj_variable._lst_parameter_info[index]._lst_demping[k];
  }
  _obj_variable._lst_parameter_info[index]._lst_demping[_obj_variable._lst_parameter_info[index]._f_demping-1] = pv;
  _avg_val += pv;
  int count = _obj_variable._lst_parameter_info[index]._f_demping;
  // if(index==2)
  //   Serial.println("Avg : " + String(_avg_val) + " | count : " + String(count));
  if (count > 0)
    _avg_val /= count;
  return _avg_val;
}

void set_comm()
{
  Configuration_WriteEEPROM_AllTime();
  // Serial.println(String(boudrate[_obj_variable._lst_communication[1]._value])+"||"+ String(_obj_variable._lst_communication[1]._value_1)+"||"+String(_obj_variable._lst_communication[0]._value)+"||"+String(_obj_variable._lst_communication.get(3)->_value)+"||"+ String(_obj_variable.total_registor));
  
  _obj_variable._obj_modbus.modbus_configure(buadrate[_obj_variable._lst_communication[1]._value], _obj_variable._lst_communication[0]._value, _obj_variable._lst_communication[2]._value, _obj_variable._lst_communication[3]._value, 22, _obj_variable.total_registor, 0);
}

bool is_WriteFile = false;
bool is_save_alarm = false;

void cheack_modebus_changes()
{
  //Set_Point
  int sp_m_count=6;  
  for(int i=0; i< TOTALCHANNEL*PER_CH_ALARM;i++)
  {
    if(_obj_variable.holdingRegs[sp_m_count] != _obj_variable._lst_parameter_alarm_info[i]._value)
    {
      _obj_variable._lst_parameter_alarm_info[i]._value = _obj_variable.holdingRegs[sp_m_count];
      is_save_alarm =true;
    }
    sp_m_count++;
  }
  
  if(is_save_alarm)
  {
   Configuration_WriteEEPROM_AllTime();
   is_save_alarm =false;
  }
  
  
  //RTC
  if (_obj_variable.holdingRegs[28] == 1)
  {
    _s_minute = _obj_variable.holdingRegs[25] / 100;
    _s_second = _obj_variable.holdingRegs[25] - (_s_minute * 100);
    _s_day = _obj_variable.holdingRegs[26] / 100;
    _s_hour = _obj_variable.holdingRegs[26] - (_s_day* 100);
    _s_year = _obj_variable.holdingRegs[27] / 100;
    _s_month = _obj_variable.holdingRegs[27] - (_s_year * 100);
  
    set_date_time = true;
    _obj_variable.holdingRegs[28] = 0;

    //Serial.println("Set Time ");
  }

  int ahu_trip = _obj_variable.holdingRegs[21] / 100;
  int ahu_off = _obj_variable.holdingRegs[21] - (ahu_trip * 100);

  if (ahu_off == 1)
  {
    is_relay_1_on =true;
    if (!_obj_variable._obj_ahu_off)
    {
      _obj_variable._led_ahu_off = LED_BLINK;
      _obj_variable._obj_ahu_off = true;
      
    }
  }
  else
  {
    _obj_variable._led_ahu_off = LED_OFF;
    _obj_variable._obj_ahu_off = false;
  }

  if (ahu_trip == 1)
  {
    is_relay_1_on =true;
    if (!_obj_variable._obj_ahu_trip)
    {
      _obj_variable._led_ahu_trip = LED_BLINK;
      _obj_variable._obj_ahu_trip = true;
    }
  }
  else
  {
    _obj_variable._led_ahu_trip = LED_OFF;
    _obj_variable._obj_ahu_trip = false;
  }

  
  if(_obj_variable.RtcRunning)
    _obj_variable.holdingRegs[88] = 1;
  else
    _obj_variable.holdingRegs[88] = 0;

  if(_obj_variable.isSPIMemoryInitialize)
    _obj_variable.holdingRegs[89] = 1;
  else
    _obj_variable.holdingRegs[89] = 0;

  int32_t_to_int16_t(_obj_variable._memory_address);
  _obj_variable.holdingRegs[90] = _obj_variable.modbusArray[0];
  _obj_variable.holdingRegs[91] = _obj_variable.modbusArray[1];

  int32_t_to_int16_t(_obj_variable._last_read_memory_address);
  _obj_variable.holdingRegs[92] = _obj_variable.modbusArray[0];
  _obj_variable.holdingRegs[93] = _obj_variable.modbusArray[1];

  int32_t_to_int16_t((_obj_variable._memory_address - _obj_variable._last_read_memory_address)/13);
  _obj_variable.holdingRegs[94] = _obj_variable.modbusArray[0];
  _obj_variable.holdingRegs[95] = _obj_variable.modbusArray[1];

  int door_panding_record_status=0;
  int recordstatus = ((_obj_variable._memory_address - _obj_variable._last_read_memory_address)/13);
  //Serial.println("recordstatus : " + String(recordstatus));
  if(recordstatus>0)
  {
    door_panding_record_status = 0 +(10*1);
    //Serial.println("door_panding_record_status : " + String(door_panding_record_status) + " | "+String(_obj_variable.door_status));
  }
  else
  {
    door_panding_record_status = 0 +(10*0);
    //Serial.println("else door_panding_record_status : " + String(door_panding_record_status) + " | "+String(_obj_variable.door_status));
  }
  _obj_variable.holdingRegs[1] = door_panding_record_status + ( _obj_variable._lst_parameter_info[0]._decimal* 100);
  _obj_variable.holdingRegs[3] = door_panding_record_status + ( _obj_variable._lst_parameter_info[1]._decimal* 100);
  _obj_variable.holdingRegs[5] = door_panding_record_status + ( _obj_variable._lst_parameter_info[1]._decimal* 100);

  if(_obj_variable.holdingRegs[96] != _obj_variable.loginterval)
  {
    _obj_variable.loginterval = _obj_variable.holdingRegs[96];
    EEPROM.writeInt(20,_obj_variable.loginterval);
    EEPROM.commit();
  }

  
}

String IntToString(int Val, bool isZero)
{
  String sVal = "";
  String tstr = String(Val);
  int count = 0;
  if (tstr.length() >= 5)
  {
    if (tstr[0] == '-' && tstr[1] == '1')
    {
      sVal = "^";
      count = 2;
    }
    for (int i = count; i < tstr.length(); i++)
      sVal += tstr[i];
  }
  else
  {
    if (Val < 0)
    {
      tstr = String(abs(Val));
      sVal = "-000";
    }
    else if (isZero)
    {
      sVal = "0000";
    }
    else
      sVal = "    ";
    int cCount = 3;
    for (int i = (tstr.length() - 1); i >= 0; i--)
    {
      sVal[cCount] = tstr[i];
      cCount--;
    }
  }

  return sVal;
}

char modulodevition(int val)
{
  int num = (val % 10);
  String snum=String(num);  
  return snum[0];
}

String IntToString(int num,int decimal)
{
  String msg = "";
  for(int i=0;i<4;i++)
    msg+=" ";
  
  bool isValNegative=false;
  if(num < 0) { isValNegative=true;num*=-1;}  
  if(num > 0) { msg[3] = modulodevition(num); num=num/10;} else { if(isValNegative) {msg[3]='-';isValNegative=false;} else msg[3]=' '; }
  if(num > 0) { msg[2] = modulodevition(num); num=num/10;} else { if(isValNegative) {msg[2]='-';isValNegative=false;} else msg[2]=' '; }
  if(num > 0) { msg[1] = modulodevition(num); num=num/10;} else { if(isValNegative) {msg[1]='-';isValNegative=false;} else msg[1]=' '; }
  if(num > 0) { msg[0] = modulodevition(num); num=num/10;} else { if(isValNegative) {msg[0]='-';isValNegative=false;} else msg[0]=' '; }

  if(decimal==1)
  {
    String tMsg=msg;
    if(msg[3]==' ') msg[3]='0';
    if(msg[2]=='-') {msg[2]='0'; msg[1]=tMsg[2]; msg[0]=tMsg[1];}
    //Serial.println("msg : " + msg " | " + tMsg);
  }

  if(decimal==2)
  {
    String tMsg=msg;
    if(msg[3]==' ') msg[3]='0';
    if(msg[2]==' ' || msg[2]=='-') {msg[2]='0';msg[1]=tMsg[2];}tMsg=msg;
    if(msg[1]==' ' || msg[1]=='-') {msg[1]='0'; msg[0]=tMsg[1];}tMsg=msg;
    //Serial.println("msg : " + msg " | " + tMsg);
  }
  if(decimal==0)
  {
    if(msg[3]==' ') msg[3]='0';
  }

  //Serial.println("msg : " + msg);
  return msg;
}

void ads_1115_read(int address)
{
  Adafruit_ADS1115 ads(address);
  ads.setGain(GAIN_ONE);
  ads.begin();
  for (int j = 0; j < 4; j++)
  {

    int _420_val = (short)ads.readADC_SingleEnded(j);
    if (_420_val < 32767)
      _obj_variable._ads_1115_arr[j] = _420_val;

    //Serial.println(String(j) + " | " + String(_420_val));
  }
}

// bool filder_sensor_ip(int s_val, int index, int max)
// {
//   if (abs((int)s_val - _obj_variable._lst_parameter_info[index].ip_pval) >= 1 && abs((int)s_val - _obj_variable._lst_parameter_info[index].ip_pval) <= 3)
//   {
//     _obj_variable._lst_parameter_info[index].ip_retry++;
//     if (_obj_variable._lst_parameter_info[index].ip_retry >= 10)
//     {
//       // if (s_val < max)
//       //   _obj_variable._lst_parameter_info[index].ip_pval = (int)s_val;
//       // else
//          _obj_variable._lst_parameter_info[index].ip_pval = max;
//       //_obj_variable._lst_parameter_info.get(index)->ip_pval = (int)s_val;
//        _obj_variable._lst_parameter_info[index].ip_retry = 0;
//     }
//   }
//   else
//   {
//     // if (s_val < max)
//     //    _obj_variable._lst_parameter_info[index].ip_pval = (int)s_val;
//     // else
//       _obj_variable._lst_parameter_info[index].ip_pval = max;
//     //_obj_variable._lst_parameter_info.get(index)->ip_pval = (int)s_val;
//     _obj_variable._lst_parameter_info[index].ip_retry = 0;
//   }
// }

void read_i2c_type(int i)
{
  if (_obj_variable._lst_i2c_data_info[i]._type == I2C_TEMP_RH) //I2C_TEMP_RH
  {
    _obj_variable.i2c_data.set_pin(_obj_variable._lst_i2c_data_info[i]._pin_sda, _obj_variable._lst_i2c_data_info[i]._pin_scl);
    _obj_variable.i2c_data.tcaselect(3);
    _obj_variable.i2c_data.read_data_t_rh();
    _obj_variable.i2c_data.get_sensor_value();

    if (_obj_variable._lst_i2c_data_info[i]._no_of_values >= 2)
    {
      if (_obj_variable.i2c_data.is_27_i2c)
      {
        _obj_variable._lst_i2c_data_info[i]._retry = 0;
        _obj_variable._lst_i2c_data_info[i]._lst_value[0] = (int)_obj_variable.i2c_data.t_val;
        _obj_variable._lst_i2c_data_info[i]._lst_value[1] = (int)_obj_variable.i2c_data.r_val;

        _obj_variable._lst_i2c_data_info[i]._lst_count[0] = (int)_obj_variable.i2c_data.t_count;
        _obj_variable._lst_i2c_data_info[i]._lst_count[1] = (int)_obj_variable.i2c_data.r_count;
        _obj_variable._lst_i2c_data_info[i]._connected = true;
      }
      else
      {
         _obj_variable._lst_i2c_data_info[i]._retry++;
        if(_obj_variable._lst_i2c_data_info[i]._retry>=5)
        {
          _obj_variable._lst_i2c_data_info[i]._retry = 0;

          _obj_variable._lst_i2c_data_info[i]._lst_value[0] = 0;
          _obj_variable._lst_i2c_data_info[i]._lst_value[1] = 0;

          _obj_variable._lst_i2c_data_info[i]._lst_count[0] = 0;
          _obj_variable._lst_i2c_data_info[i]._lst_count[1] = 0;
          _obj_variable._lst_i2c_data_info[i]._connected = false;
        }
      }
      //Serial.println("TEMP : " + String(_obj_variable.i2c_data.t_val) +","+ String(_obj_variable.i2c_data.t_count) + " | RH : " + String(_obj_variable.i2c_data.r_val)+","+ String(_obj_variable.i2c_data.r_count));
    }
  }
  else if (_obj_variable._lst_i2c_data_info[i]._type == I2C_DP) //I2C_DP
  {
    _obj_variable.i2c_data.set_pin(_obj_variable._lst_i2c_data_info[i]._pin_sda, _obj_variable._lst_i2c_data_info[i]._pin_scl);
    _obj_variable.i2c_data.tcaselect(1);
    _obj_variable.i2c_data.d_count = _obj_variable._lst_i2c_data_info[i]._lst_count[0];
    _obj_variable.i2c_data.read_data_dp();
    
    if(_obj_variable.i2c_data.is_28_i2c)
    {
       //Serial.println(String(_obj_variable._lst_i2c_data_info[i]._name) + " : " + String(_obj_variable.i2c_data.d_count) + " | SDA : " +String(_obj_variable._lst_i2c_data_info[i]._pin_sda) + " | SCL : " + String(_obj_variable._lst_i2c_data_info[i]._pin_scl));
      _obj_variable._lst_i2c_data_info[i]._retry=0;
      _obj_variable._lst_i2c_data_info[i]._lst_value[0] =  (int)_obj_variable.i2c_data.d_val;
      _obj_variable._lst_i2c_data_info[i]._lst_count[0] =  (int)_obj_variable.i2c_data.d_count;
      _obj_variable._lst_i2c_data_info[i]._connected = true;
    }
    else
    {
      //Serial.println(_obj_variable._lst_i2c_data_info[i]._name + " : NOT FIND SENSOR");
      _obj_variable._lst_i2c_data_info[i]._retry++;
      if(_obj_variable._lst_i2c_data_info[i]._retry>=5)
      {
        _obj_variable._lst_i2c_data_info[i]._retry = 0;
        _obj_variable._lst_i2c_data_info[i]._lst_value[0] = 0;
        _obj_variable._lst_i2c_data_info[i]._lst_count[0] = 0;
        _obj_variable._lst_i2c_data_info[i]._connected = false;
      }
    //Serial.println("DP : " + String(_obj_variable.i2c_data.d_count));
    }
  }
  else if (_obj_variable._lst_i2c_data_info[i]._type == I2C_ADS_1115) //I2C_ADS_1115
  {
    ads_1115_read(72); //0x48
    if (_obj_variable._lst_i2c_data_info[i]._no_of_values == 4)
    {
      _obj_variable._lst_i2c_data_info[i]._lst_value[0] =  _obj_variable._ads_1115_arr[0];
      _obj_variable._lst_i2c_data_info[i]._lst_value[1] =  _obj_variable._ads_1115_arr[1];
      _obj_variable._lst_i2c_data_info[i]._lst_value[2] =  _obj_variable._ads_1115_arr[2];
      _obj_variable._lst_i2c_data_info[i]._lst_value[3] =  _obj_variable._ads_1115_arr[3];

      _obj_variable._lst_i2c_data_info[i]._lst_count[0] = _obj_variable._ads_1115_arr[0];
      _obj_variable._lst_i2c_data_info[i]._lst_count[1] = _obj_variable._ads_1115_arr[1];
      _obj_variable._lst_i2c_data_info[i]._lst_count[2] = _obj_variable._ads_1115_arr[2];
      _obj_variable._lst_i2c_data_info[i]._lst_count[3] = _obj_variable._ads_1115_arr[3];

      //Serial.println(String(_obj_variable._lst_i2c_data_info[i]._lst_count->get(0)) + " | " + String(_obj_variable._lst_i2c_data_info[i]._lst_count->get(1)) + " | " + String(_obj_variable._lst_i2c_data_info[i]._lst_count->get(2)) + " | " + String(_obj_variable._lst_i2c_data_info[i]._lst_count->get(3)));
    }
  }
  else if (_obj_variable._lst_i2c_data_info[i]._type == I2C_RTC && _obj_variable._lst_other_info[2]._value == 1) //I2C_RTC
  {
    // Wire.begin(32,33);
        // Serial.println("Set_Date_TIME_RTC" + String(set_date_time));
        // _obj_variable._objRtc.get();
        // Serial.println(String(year()) + "/" + String(month()) + "/" + String(day()) + "  " + String(hour()) + ":" + String(minute()) + ":" + String(second()));
        bool isDisplayWrite=false;
        if (set_secound && (_m_secound != second()))
        {
           set_secound = 0;
          _s_second = _m_secound;
          set_date_time=1;
          _m_secound=0;
          isDisplayWrite=true;
        }
        if (set_minute  && (_m_minute != minute()))
        {
          set_minute = 0;
          _s_minute = _m_minute;
          set_date_time=1;
          _m_minute=0;
          isDisplayWrite=true;
        }
        if (set_hour  && (_m_hour != hour()))
        {
          set_hour = 0;
          _s_hour = _m_hour;
          set_date_time=1;
          _m_hour=0;
          isDisplayWrite=true;
        }
        if (set_day  && (_m_day != day()))
        {
          set_day = 0;
          _s_day = _m_day;
          set_date_time=1;
          _m_day=0;
          isDisplayWrite=true;
        }
        if (set_month  && (_m_month!= month()))
        {
          set_month = 0;
          _s_month = _m_month;
          set_date_time=1;
          _m_month=0;
          isDisplayWrite=true;
        }
        if (set_year  && (_m_year != year()-2000))
        {
          set_year = 0;
          _s_year = _m_year;
          set_date_time=1;
          _m_year=0;
          isDisplayWrite=true;
        }
        
        //Serial.println("set_date_time : " + String(set_date_time));
        if(!_obj_variable.RtcRunning)
        {
          _obj_variable.i2c_data.tcaselect(2);
          _obj_variable._objRtc.get(); 
        }
        //SET_MODBUS
        if (set_date_time)
        {
          _obj_variable.i2c_data.tcaselect(2);
          //Serial.println(String(_s_year) + "/" + String(_s_month) + "/" + String(_s_day) + "  " + String(_s_hour) + ":" + String(_s_minute) + ":" + String(_s_second));
          set_date_time = 0;
          _obj_variable._objRtc.set(_s_second,_s_minute,_s_hour,_s_day,_s_month,_s_year);
          _obj_variable._objRtc.get(); 
        }
  }
  //Serial.println(String(_obj_variable._lst_i2c_data_info[i]._type)+ "  |  " +String(_obj_variable._lst_other_info[2]._value)+"  |  "+String(i) +" |  " +Serial.println(String(_obj_variable._lst_i2c_data_info[i]._name)));
}

void display_setting(String str, int index, int decimal)
{
  // Serial.println(String("STR  "  )+String(index)+"  |  "+String(str));
  if (index == 0)
  {
    _obj_variable.display_1 = str;
    _obj_variable.decimal_1 = decimal;
    // Serial.println("Val  "+ String(_obj_variable.display_1));
  }
  else if (index == 1)
  {
    _obj_variable.display_2 = str;
    _obj_variable.decimal_2 = decimal;
  }
  else if (index == 2)
  {
    
    _obj_variable.display_3 = str;
    _obj_variable.decimal_3 = decimal;
    //Serial.println("Display 3 : " + str + " | " + _obj_variable.display_3);
  }
}

void comm_i2c_read_temp()
{
   is_relay_1_on = false;
  for (int i = 0; i < I2C_DATA_SIZE ; i++)
  {
    if (_obj_variable._lst_i2c_data_info[i]._status)
    {
      for (int j = 0; j < TOTALCHANNEL ; j++)
      {
        // Serial.println(String(_obj_variable._lst_parameter_info[j]._is_calibration) +"  | " +String(j)  ) ;
        // Serial.println();
        
        if (_obj_variable._lst_parameter_info[j]._input_type == _obj_variable._lst_i2c_data_info[i]._id)
        {
          // Serial.println(String(_obj_variable._lst_parameter_info[j]._input_type)+" -   " +String(_obj_variable._lst_i2c_data_info[i]._id) );
          _obj_variable._lst_parameter_info[j]._is_sensor_connect = _obj_variable._lst_i2c_data_info[i]._connected;

          //Serial.println(String(_obj_variable._lst_parameter_info[j]._name) + " | " + String(_obj_variable._lst_parameter_info[j]._is_sensor_connect) + " | " + String(_obj_variable._lst_i2c_data_info[i]._connected));

          if (_obj_variable._lst_parameter_info[j]._is_calibration)
          {
            //USED MAP FUCATION
            //int val = map(_obj_variable._lst_i2c_data_info[i]._lst_count->get(_obj_variable._lst_parameter_info[j]._sensor_no), _obj_variable._lst_parameter_info[j]._c_zero, _obj_variable._lst_parameter_info[j]._c_span, _obj_variable._lst_parameter_info[j]._r_zero, _obj_variable._lst_parameter_info[j]._r_span);
            int16_t val = 0;
            if (_obj_variable._lst_i2c_data_info[i]._type == I2C_DP)
              val = map(_obj_variable._lst_i2c_data_info[i]._lst_count[_obj_variable._lst_parameter_info[j]._sensor_no], _obj_variable._lst_parameter_info[j]._c_zero, _obj_variable._lst_parameter_info[j]._c_span, 0, _obj_variable._lst_parameter_info[j]._r_span);
            else
              val = map(_obj_variable._lst_i2c_data_info[i]._lst_count[_obj_variable._lst_parameter_info[j]._sensor_no], _obj_variable._lst_parameter_info[j]._c_zero, _obj_variable._lst_parameter_info[j]._c_span, _obj_variable._lst_parameter_info[j]._r_zero, _obj_variable._lst_parameter_info[j]._r_span);
              //filder_sensor_ip(val, j, _obj_variable._lst_parameter_info[j]._r_span);
            
            // Serial.println(String(_obj_variable._lst_parameter_info[j]._name) + " | " + String(val) + " | " + String(_obj_variable._lst_parameter_info[j].ip_pval));
            if(_obj_variable._lst_parameter_info[j]._is_firsttime && _obj_variable._lst_i2c_data_info[i]._connected)
            {
              for(int k=0;k<_obj_variable._lst_parameter_info[j]._f_demping;k++)
              {
                //Serial.println(String(_obj_variable._lst_parameter_info[j]._name) + " | " + String(_obj_variable._lst_parameter_info[j]._f_demping) + " | " + String(k) + " | " + String(val));
                _obj_variable._lst_parameter_info[j]._lst_demping[k] = val;
              }

              _obj_variable._lst_parameter_info[j]._is_firsttime = false;
            }
            
            val = find_avg_value(j,val);
            if (val > (_obj_variable._lst_parameter_info[j]._r_zero + _obj_variable._lst_parameter_info[j]._cali_offset) && val < (_obj_variable._lst_parameter_info[j]._r_span - _obj_variable._lst_parameter_info[j]._cali_offset))
            {
              val += _obj_variable._lst_parameter_info[j]._offset;
              //Serial.println(String(_obj_variable._lst_parameter_info[j]._name) + " | " + String(_obj_variable._lst_parameter_info[j]._offset) + " | " + String(val));
            }
              
            //Serial.println(String(_obj_variable._lst_parameter_info[j]._name) +  + " | " + String(val));

            if((_obj_variable._lst_parameter_info[j]._r_zero-10)>val) // UNDER
            {
              _obj_variable._lst_parameter_info[j]._is_under=true;
              _obj_variable._lst_parameter_info[j]._is_over=false;
            }
            else if((_obj_variable._lst_parameter_info[j]._r_span+10)<val) // OVER
            {
              _obj_variable._lst_parameter_info[j]._is_over=true;
              _obj_variable._lst_parameter_info[j]._is_under=false;
            }
            else
            {
              _obj_variable._lst_parameter_info[j]._is_over=false;
              _obj_variable._lst_parameter_info[j]._is_under=false;
            }

            if (val < 0 && _obj_variable._lst_parameter_info[j]._is_negative)
              val = 0;

            _obj_variable._lst_parameter_info[j]._pv = val;
          }
          else
          {
            //DIRECT VALUE
            int16_t val = _obj_variable._lst_i2c_data_info[i]._lst_value[_obj_variable._lst_parameter_info[j]._sensor_no];
            //filder_sensor_ip(val, j, _obj_variable._lst_parameter_info[j]._r_span);
            //Serial.println(_obj_variable._lst_parameter_info[j]._name + " | " + val + " | " + _obj_variable._lst_parameter_info[j].ip_pval);
            
            if(_obj_variable._lst_parameter_info[j]._is_firsttime && _obj_variable._lst_i2c_data_info[i]._connected)
            {
              for(int k=0;k<_obj_variable._lst_parameter_info[j]._f_demping;k++)
              {
                //Serial.println(String(_obj_variable._lst_parameter_info[j]._name) + " | " + String(_obj_variable._lst_parameter_info[j]._f_demping) + " | " + String(k) + " | " + String(val));
                _obj_variable._lst_parameter_info[j]._lst_demping[k] =val;
              }

              _obj_variable._lst_parameter_info[j]._is_firsttime = false;
            }
            
            val = find_avg_value(j,val);
            // if (val > (_obj_variable._lst_parameter_info[j]._r_zero + _obj_variable._lst_parameter_info[j]._cali_offset) && val < (_obj_variable._lst_parameter_info[j]._r_span - _obj_variable._lst_parameter_info[j]._cali_offset))
            {
              val += _obj_variable._lst_parameter_info[j]._offset;
              // Serial.println(String(_obj_variable._lst_parameter_info[j]._offset) +"  | " +String(j) +"  Else ") ;
            }
              

            if((_obj_variable._lst_parameter_info[j]._r_zero-100)>val) // UNDER
            {
              _obj_variable._lst_parameter_info[j]._is_under=true;
              _obj_variable._lst_parameter_info[j]._is_over=false;
            }
            else if((_obj_variable._lst_parameter_info[j]._r_span+100)<val) // OVER
            {
              _obj_variable._lst_parameter_info[j]._is_over=true;
              _obj_variable._lst_parameter_info[j]._is_under=false;
            }
            else
            {
              _obj_variable._lst_parameter_info[j]._is_over=false;
              _obj_variable._lst_parameter_info[j]._is_under=false;
            }

            if (val < 0 && _obj_variable._lst_parameter_info[j]._is_negative)
              val = 0;

            _obj_variable._lst_parameter_info[j]._pv = val;
          }
          
          _obj_variable._lst_parameter_info[j]._count = _obj_variable._lst_i2c_data_info[i]._lst_count[_obj_variable._lst_parameter_info[j]._sensor_no];
          // Serial.println("Dispaly_ststus" + String(_obj_variable._lst_parameter_info[j]._disp_status));
          if (_obj_variable._lst_parameter_info[j]._disp_status == 1)
          {
            //Serial.println("PV : " + String(_obj_variable._lst_parameter_info[j]._pv) + " | " + String(_obj_variable._lst_parameter_info[j]._is_over) + " | " + String(_obj_variable._lst_parameter_info[j]._is_under));
            if (_obj_variable._lst_parameter_info[j]._is_sensor_connect && !_obj_variable._lst_parameter_info[j]._is_over && !_obj_variable._lst_parameter_info[j]._is_under)
            {
              //display_setting(CheckStringLength(IntToString(_obj_variable._lst_parameter_info[j]._pv, false), 4, " ", _obj_variable._lst_parameter_info[j]._decimal), j, _obj_variable._lst_parameter_info[j]._decimal);
              display_setting(CheckStringLength(IntToString(_obj_variable._lst_parameter_info[j]._pv, _obj_variable._lst_parameter_info[j]._decimal), 4, " ", _obj_variable._lst_parameter_info[j]._decimal), j, _obj_variable._lst_parameter_info[j]._decimal);
            }
            else if(_obj_variable._lst_parameter_info[j]._is_sensor_connect &&  _obj_variable._lst_parameter_info[j]._is_over)
              display_setting("OVER", j, 0);
            else if(_obj_variable._lst_parameter_info[j]._is_sensor_connect && _obj_variable._lst_parameter_info[j]._is_under)
              display_setting("UNDR", j, 0);
            else
              display_setting("OPEN", j, 0);
          }
          else
          {
            display_setting("    ", j, 0);

          }
        }
      }
    }
  }

  alarm_generate();
  set_alarm_led_status();
  cheack_modebus_changes();
 
 //Serial.println("TIME STATUS : " + String(_obj_variable._lst_other_info[2]._value));

  if (_obj_variable._lst_other_info[2]._value == 0)
  {
    _obj_variable.display_4 = "    ";
    _obj_variable.blink_decmial = 0;
  }
  else
  {
    _obj_variable.display_4 = CheckStringLength(String((minute() + hour() * 100)), 4, "0");
    //Serial.println("Time : " + _obj_variable.display_4);
    if (_obj_variable.blink_decmial == 2)
      _obj_variable.blink_decmial = 0;
    else
      _obj_variable.blink_decmial = 2;

     
  }
}

void i2c_read_sensor()
{
  for (int i = 0; i < I2C_DATA_SIZE; i++)
  {
    if (_obj_variable._lst_i2c_data_info[i]._status)
    {
      read_i2c_type(i);
    }
  }
}

void comm_i2c_read(void *pvParameters)
{
  i2c_read_sensor();
  _obj_variable.isI2CTask = false;
  vTaskDelete(NULL);
}

char arr_num[11] = {'-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

int findcharindex_nagative_posative(char ch, int cur_position)
{
  int index = 0;
  for (int i = 0; i < 11; i++)
  {
    if (arr_num[i] == ch)
    {
      index = i + 1;
    }
  }

  if (index >= 11)
    index = 0;
  if (index == 0 && cur_position != 0)
    index += 1;
  else if (index == 1 && cur_position != 0)
    index += 1;
  // Serial.println("index : " + String(index));
  return index;
}

int findcharindex_only_posative(char ch, int cur_position)
{
  int index = 0;
  for (int i = 0; i < 11; i++)
  {
    if (arr_num[i] == ch)
    {
      index = i + 1;
    }
  }
  if (index >= 11)
    index = 1;
  return index;
}

String incrimentel(String pass_value, bool up, bool shift, bool nagative_posative, bool only_posative, bool on_off)
{
  if (shift)
    _obj_variable.cursor_position--;
  if (_obj_variable.cursor_position < 0)
    _obj_variable.cursor_position = 3;

  pass_value = CheckStringLength(pass_value, 4, "0");
  if (up)
  {
    if (nagative_posative)
      pass_value[_obj_variable.cursor_position] = arr_num[findcharindex_nagative_posative(pass_value[_obj_variable.cursor_position],  _obj_variable.cursor_position)];
    if (only_posative)
      pass_value[_obj_variable.cursor_position] = arr_num[findcharindex_only_posative(pass_value[_obj_variable.cursor_position], _obj_variable.cursor_position)];
    if (on_off)
    {
      if (pass_value == "0000")
        pass_value = "0001";
      else if (pass_value == "0001")
        pass_value = "0000";
    }
  }

  return pass_value;
}

int StringToInt(String Val)
{
  String istr = "";
  int count = 0;
  if (Val[0] == '^')
  {
    istr = "-1";
    count++;
  }
  for (int i = count; i < Val.length(); i++)
    istr += Val[i];
  return istr.toInt();
}

String IntToString(int Val)
{
  String sVal = "";
  String tstr = String(Val);
  int count = 0;
  if (tstr.length() >= 5)
  {
    if (tstr[0] == '-' && tstr[1] == '1')
    {
      sVal = "^";
      count = 2;
    }
    for (int i = count; i < tstr.length(); i++)
      sVal += tstr[i];
  }
  else
  {
    if (Val < 0)
    {
      tstr = String(abs(Val));
      sVal = "-000";
    }
    else
    {
      sVal = "0000";
    }
    int cCount = 3;
    for (int i = (tstr.length() - 1); i >= 0; i--)
    {
      sVal[cCount] = tstr[i];
      cCount--;
    }
  }

  return sVal;
}

#pragma region memory

long DatetimetoLong(DateTime _dt_time)
{
  long time = 0;
  time += _dt_time.day();
  time *= 100;
  //Serial.print(String(time));
  time +=_dt_time.month();
  time *= 100;
  //Serial.print("," + String(time));
  time += (_dt_time.year() - 2000);
  time *= 100;
  //Serial.print("," + String(time));
  time += _dt_time.hour();
  time *= 100;
  //Serial.print("," + String(time));
  time += _dt_time.minute();
  //Serial.println("," + String(time));
  return time;
}

void uint32_to_byte(uint32_t value)
{  
  byte * valuePtr = (byte *) &value;
  for (size_t i = 0; i < sizeof(value); i++)
    _obj_variable.byteArray[i]=(byte)(*(valuePtr + i));
}

bool check_memory_write(uint32_t addrIn,uint32_t sz)
{
    //uint32_t sz = 13;
    uint32_t _SPI_PAGESIZE = 4096;
    //uint32_t addrIn = 4090;
    bool addressOverflow = false,wstatus=false;
    uint64_t currentAddress = 0;
    uint32_t _length = sz;
    uint32_t _maxBytes = _SPI_PAGESIZE - (addrIn % _SPI_PAGESIZE);  // Force the first set of bytes to stay within the first page
    //Console.WriteLine("_maxBytes : " + _maxBytes + " | _length : " + _length + " | _SPI_PAGESIZE : " + _SPI_PAGESIZE + " | " + (int)(addrIn / _SPI_PAGESIZE));
    //Serial.println("_maxBytes : " + String(_maxBytes) + " | _length : " + String(_length) + " | _SPI_PAGESIZE : " + String(_SPI_PAGESIZE) + " | " + String((int)(addrIn / _SPI_PAGESIZE)));
    if (_maxBytes > _length)
    {
      int sAddress = (int)(addrIn / _SPI_PAGESIZE);
      //Serial.println("if _maxBytes : " + String(_maxBytes) + " | _length : " + String(_length)+ " | Start : " + String(_obj_variable._memory_address) + " | End : " + String((sAddress * _SPI_PAGESIZE) + _SPI_PAGESIZE));
      bool rstatus = _obj_variable.flash.readByteArray((sAddress * _SPI_PAGESIZE),_obj_variable._mBuffer,_SPI_PAGESIZE);
      _obj_variable.flash.eraseSection((sAddress * _SPI_PAGESIZE),_SPI_PAGESIZE);
      //Serial.println("Read From Memory");
      // int count=0;
      // for(int i=0;i<4096;i++)
      // {
      //   Serial.print(String(_mBuffer[i]) + ",");
      //   count++;
      //   if(count==13)
      //   {Serial.println();count=0;}
      // }
      // Serial.println();

      for (uint16_t i = 0; i < _length; ++i)
      {
        //Serial.println(String(i) + " : " + String(addrIn + i) + " INDEX : " + String((addrIn + i) - ((sAddress * _SPI_PAGESIZE))));
        _obj_variable._mBuffer[(addrIn + i) - ((sAddress * _SPI_PAGESIZE))] = _obj_variable._wMBuffer[i];
        //_nextByte(WRITE, *p++);
      }
   
      if ((sAddress * _SPI_PAGESIZE) >= _obj_variable.Max_Address_length)
      {
        _obj_variable._memory_address=0;  wstatus=false;   
      }
      else
      {
         wstatus = _obj_variable.flash.writeByteArray((sAddress * _SPI_PAGESIZE),_obj_variable._mBuffer,_SPI_PAGESIZE);
      }
      
      //CHIP_DESELECT
    }
    else
    {
        uint32_t writeBufSz;
        uint32_t data_offset = 0;

        do
        {
            writeBufSz = (_length <= _maxBytes) ? _length : _maxBytes;
            int sAddress = (int)(addrIn / _SPI_PAGESIZE);
            // Serial.println(String("writeBufSz")+" |  "+ String(writeBufSz)+" | "+ String("_length")+" |  "+ String(_length)+"  | "+String("_maxBytes")+" |  "+ String(_maxBytes));
            if(data_offset==0)
            {
              // Serial.println("Data_offset=0");
              //Serial.println("else _maxBytes : " + String(_maxBytes) + " | _length : " + String(_length) + " | writeBufSz : " + String(writeBufSz) + " | data_offset : " + String(data_offset) + " | Start : " + String((sAddress * _SPI_PAGESIZE)) + " | End : " + String((sAddress * _SPI_PAGESIZE) + _SPI_PAGESIZE));
              bool rstatus = _obj_variable.flash.readByteArray((sAddress * _SPI_PAGESIZE),_obj_variable._mBuffer,_SPI_PAGESIZE);
              _obj_variable.flash.eraseSection((sAddress * _SPI_PAGESIZE),_SPI_PAGESIZE);
            }
            else
            {
              // Serial.println("Data_offset_else");
              //Serial.println("else _maxBytes : " + String(_maxBytes) + " | _length : " + String(_length) + " | writeBufSz : " + String(writeBufSz) + " | data_offset : " + String(data_offset) + " | Start : " + String(_SPI_PAGESIZE + ((sAddress) * _SPI_PAGESIZE)) + " | End : " + String(_SPI_PAGESIZE + ((sAddress) * _SPI_PAGESIZE) + _SPI_PAGESIZE));
              bool rstatus = _obj_variable.flash.readByteArray((_SPI_PAGESIZE + ((sAddress) * _SPI_PAGESIZE)),_obj_variable._mBuffer,_SPI_PAGESIZE);
              _obj_variable.flash.eraseSection((_SPI_PAGESIZE + ((sAddress) * _SPI_PAGESIZE)),_SPI_PAGESIZE);
            }
            // Serial.println(writeBufSz);
            for (uint16_t i = 0; i < writeBufSz; ++i)
            {
              if (data_offset == 0)
              {
                //Serial.println(i + " : " + String(addrIn + i + data_offset) + " INDEX : " + String((addrIn + i + data_offset) -((sAddress * _SPI_PAGESIZE))));
                _obj_variable._mBuffer[((addrIn + i + data_offset) - ((sAddress * _SPI_PAGESIZE)))] = _obj_variable._wMBuffer[i];
                // Serial.println(_wMBuffer[i]);
              }
              else
              {
                //Serial.println(i + " : " + String(addrIn + i + data_offset) + " INDEX : " + String(i));
                _obj_variable._mBuffer[i]=_obj_variable._wMBuffer[i+data_offset];
                // Serial.println(_wMBuffer[i+data_offset]);
              }
              //_nextByte(WRITE, *p++);

            }
            if(data_offset==0)
            {
              if ((sAddress * _SPI_PAGESIZE) >= _obj_variable.Max_Address_length)
              {
                _obj_variable._memory_address=0;  
                wstatus=false;  
              }
              else
              {
                wstatus = _obj_variable.flash.writeByteArray((sAddress * _SPI_PAGESIZE),_obj_variable._mBuffer,_SPI_PAGESIZE);
              }
            }
            else
            {
              if ((_SPI_PAGESIZE + ((sAddress) * _SPI_PAGESIZE)) >= _obj_variable.Max_Address_length)
              { 
                _obj_variable._memory_address=0;  wstatus=false;
              }
              else
              {
                wstatus = _obj_variable.flash.writeByteArray((_SPI_PAGESIZE + ((sAddress) * _SPI_PAGESIZE)),_obj_variable._mBuffer,_SPI_PAGESIZE);
              }
            }

            //CHIP_DESELECT
            if (!addressOverflow)
            {
              currentAddress += writeBufSz;
            }
            else
            {
             if (data_offset >= addressOverflow)
             {
                currentAddress = 0x00;
                addressOverflow = false;
             }
            }
            data_offset += writeBufSz;
            _length -= writeBufSz;
            _maxBytes = _SPI_PAGESIZE;   // Now we can do up to 256 bytes per loop

        } while (_length > 0);
    }
    return wstatus;  
}

void write_to_spi_memory(DateTime tm)
{
  int16_t para_1 =0,para_2 =0,para_3 =0;
  if(TOTALCHANNEL>=1)
    para_1 = (int16_t) _obj_variable._lst_parameter_info[0]._pv;  
  if(TOTALCHANNEL>=2)
    para_2 = (int16_t) _obj_variable._lst_parameter_info[1]._pv;
  if(TOTALCHANNEL>=3)
    para_3 = (int16_t) _obj_variable._lst_parameter_info[2]._pv;
  memset(_obj_variable._wMBuffer,0,13);

  _obj_variable._wMBuffer[0] = tm.day();
  _obj_variable._wMBuffer[1] = tm.month();
  _obj_variable._wMBuffer[2] = tm.year()-2000;
  _obj_variable._wMBuffer[3] = tm.hour();
  _obj_variable._wMBuffer[4] = tm.minute();
  _obj_variable._wMBuffer[5] = tm.second();
  _obj_variable._wMBuffer[6] = (para_1 >> 8);
  _obj_variable._wMBuffer[7] = (para_1);
  _obj_variable._wMBuffer[8] = (para_2 >> 8);
  _obj_variable._wMBuffer[9] =  (para_2);
  _obj_variable._wMBuffer[10] = (para_3 >> 8);
  _obj_variable._wMBuffer[11] =  (para_3);
  _obj_variable._wMBuffer[12] = _obj_variable.door_status;//'>'
  // Serial.println(_obj_variable._memory_address);
  //Serial.println("Write Time : " + String(tm.day())+" | "+String(tm.month())+" | "+String((tm.year()))+" | "+String(tm.hour())+" | "+String(tm.minute())+" | "+String(tm.second()));

  if((_obj_variable._memory_address)>=_obj_variable.Max_Address_length)
  _obj_variable._memory_address=0;
  if(check_memory_write(_obj_variable._memory_address,13))
  {
    //Serial.println("Memory_Address"+String(_obj_variable._memory_address));
   _obj_variable._memory_address+=13;
   EEPROM.writeInt(1,_obj_variable._memory_address);
   EEPROM.commit();
  }  
}

void read_memory()
{
  if(_obj_variable._last_read_memory_address < _obj_variable._memory_address)
  {
    bool rstatus = _obj_variable.flash.readByteArray(_obj_variable._last_read_memory_address,_obj_variable._rMBuffer,13);
    _obj_variable.holdingRegs[62]=_obj_variable._rMBuffer[0];
    _obj_variable.holdingRegs[63]=_obj_variable._rMBuffer[1];
    _obj_variable.holdingRegs[64]=_obj_variable._rMBuffer[2];
    _obj_variable.holdingRegs[65]=_obj_variable._rMBuffer[3];
    _obj_variable.holdingRegs[66]=_obj_variable._rMBuffer[4];
    _obj_variable.holdingRegs[67]=_obj_variable._rMBuffer[5];

    //Serial.println(String(_obj_variable._rMBuffer[0]) + "/" + String(_obj_variable._rMBuffer[1]) + "/" + String(_obj_variable._rMBuffer[2]) + " " + String(_obj_variable._rMBuffer[3]) + ":" + String(_obj_variable._rMBuffer[4]) + ":" + String(_obj_variable._rMBuffer[5]));

    signed int para1 = _obj_variable._rMBuffer[6];
    para1 <<= 8;
    para1 += _obj_variable._rMBuffer[7];

    signed int para2 = _obj_variable._rMBuffer[8];
    para2 <<= 8;
    para2 += _obj_variable._rMBuffer[9];

    signed int para3 = _obj_variable._rMBuffer[10];
    para3 <<= 8;
    para3 += _obj_variable._rMBuffer[11];

    _obj_variable.holdingRegs[68]=para1;
    _obj_variable.holdingRegs[69]=para2;
    _obj_variable.holdingRegs[70]=para3;
    _obj_variable.holdingRegs[71]=_obj_variable._rMBuffer[12];

    EEPROM.writeInt(10,_obj_variable._last_read_memory_address);
    EEPROM.commit();
    _obj_variable._last_read_memory_address+=13;
    _obj_variable.holdingRegs[60]= 0;
    _obj_variable.holdingRegs[61]= 1;
    
  }
  else
  {
    _obj_variable.holdingRegs[61]= 2;
  }
  
  _obj_variable.isMemoryRead=false;
}

void data_store(bool isMinusMinute,bool isStore2Record)
{
  DateTime tm = _obj_variable.tm;
  
  if (isMinusMinute)
   tm = _obj_variable.tm - TimeSpan(0,0,1,0);//Local time 60 Second behind UTC.
  
  unsigned long cTime= millis();
  unsigned long diff = cTime - _obj_variable.lastLogTime;
  diff/=1000;
  //Serial.println("Diff : " + String(diff) + " | isMinusMinute : " + String(isMinusMinute) + " | _obj_variable.lastLogTime : " + String( _obj_variable.lastLogTime));
  if (diff >= _obj_variable.loginterval || isMinusMinute || isStore2Record)
  {
    //Serial.println("Diff : " + String(diff) + " | " + String(cTime) + " | " + String( _obj_variable.lastLogTime));
    _obj_variable.lastLogTime = cTime;
    _obj_variable._is_synchronize_time = true;
    write_to_spi_memory(tm);
  } 
}

#pragma endregion read memory

#pragma region Modbus Details

void modebus_conf_write_onetime()
{
  int sp_m_count=6;  
  for (int i = 0; i <(TOTALCHANNEL * PER_CH_ALARM); i++)
  {
    _obj_variable.holdingRegs[sp_m_count] = _obj_variable._lst_parameter_alarm_info[i]._value;
    sp_m_count++;
  }
  _obj_variable.holdingRegs[96] = _obj_variable.loginterval;
}

// void comm_modbus_slave(void *pvParameters)
// {
//   //_obj_variable._obj_modbus.modbus_update(_obj_variable.holdingRegs);
//   //vTaskDelay(5 / portTICK_PERIOD_MS);
//   if (_obj_variable._obj_modbus.modbus_update(_obj_variable.holdingRegs))
//   {
//     if(_obj_variable.holdingRegs[60]==1)
//     {
//       _obj_variable.holdingRegs[61]=0;
//       _obj_variable.isMemoryRead=true;
//     }
//     _obj_variable._is_synchronize_time = false;
//   }
//   _obj_variable.isMTask = false;
//   vTaskDelete(NULL);
// }


void comm_modbus_slave(void *pvParameters)
{
  while(1)
  {
    while(1)
    {
      if (_obj_variable._obj_modbus.modbus_update(_obj_variable.holdingRegs))
      {
        if(_obj_variable.holdingRegs[60]==1)
        {
          _obj_variable.holdingRegs[61]=0;
          _obj_variable.isMemoryRead=true;
        }
        _obj_variable._is_synchronize_time = false;
      }
      _obj_variable.isMTask = false;
      vTaskDelay(5 / portTICK_PERIOD_MS);
    }
  }
}


#pragma endregion

#pragma region Display_7_Segment

void get_key()
{
  if (digitalRead(KEY4)) _obj_variable.is_key4_press = 1; else _obj_variable.is_key4_press = 0;
  if (digitalRead(KEY3)) _obj_variable.is_key3_press = 1; else _obj_variable.is_key3_press = 0;
  if (digitalRead(KEY2)) _obj_variable.is_key2_press = 1; else _obj_variable.is_key2_press = 0;
  if (digitalRead(KEY1)) _obj_variable.is_key1_press = 1; else _obj_variable.is_key1_press = 0;
  
  if (_obj_variable.is_key1_press)
  {
    _obj_variable._key_press_1 = _obj_variable.is_key1_press;
    // Serial.println("Key_1 : " + String(_obj_variable.is_key1_press));
  }
  if (_obj_variable.is_key2_press)
  {
    _obj_variable._key_press_2 = _obj_variable.is_key2_press;
    // Serial.println("Key_2 : " + String(_obj_variable.is_key2_press));
  }
  if (_obj_variable.is_key3_press)
  {
    _obj_variable._key_press_3 = _obj_variable.is_key3_press;
    // Serial.println("Key_3 : " + String(_obj_variable.is_key3_press));
  }
  if (_obj_variable.is_key4_press)
  {
    _obj_variable._key_press_4 = _obj_variable.is_key4_press;
    // Serial.println("Key_4 : " + String(_obj_variable.is_key4_press));
  }
}

void key_reset()
{
  _obj_variable._key_press_1 = false;
  _obj_variable._key_press_2 = false;
  _obj_variable._key_press_3 = false;
  _obj_variable._key_press_4 = false;
  _obj_variable.currentMillis  = millis();
}

void value_to_data_type_display_value_1(display_menu_screen * _obj_display_menu_screen)
{
  String str_blink = _obj_variable.display_value;
  if (blink_segment)
  { 
    if(_obj_variable._lst_display_menu_screen->_d_type== DTYPE_NONE)
      _obj_variable._obj_display.w_display_segment(_obj_variable.display_value, 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen->_d_type == DTYPE_EN_DIS)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_en_dis[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen->_d_type == DTYPE_YES_NO)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_yes_no[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen->_d_type == DTYPE_INPUT_TYPE)
    {
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_input_type[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    }
    else if(_obj_variable._lst_display_menu_screen->_d_type == DTYPE_PARITY)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_parity[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen->_d_type == DTYPE_BUADRATE)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_buadrate[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen->_d_type == DTYPE_NUMBER)
    {
      _obj_variable._obj_display.w_display_segment(CheckStringLength(str_blink, 4, "0"), _obj_variable.display_decimal, "    ", 0, "    ", 0, "    ", 0);
    }
    else if(_obj_variable._lst_display_menu_screen->_d_type == DTYPE_ALARM_TYPE)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_alarm_type[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen->_d_type == DTYPE_ALARM_ON_OFF)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_alarm_on_off[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen->_d_type == DTYPE_CTYP)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_ctyp[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
  }
  else
  {
    str_blink[_obj_variable.cursor_position] = ' ';
    if(_obj_variable._lst_display_menu_screen->_d_type == DTYPE_NUMBER)
    {
      //Serial.println("Blinking : " + CheckStringLength(str_blink, 4, "0") + " | Position : " + (_obj_variable.cursor_position));
      _obj_variable._obj_display.w_display_segment(CheckStringLength(str_blink, 4, "0"), _obj_variable.display_decimal, "    ", 0, "    ", 0, "    ", 0);
    }
    // else      
    //   _obj_variable._obj_display.w_display_segment("    ", 0, "    ", 0, "    ", 0, "    ", 0);
  }
}

void value_to_data_type_display_value_config(int _index)
{
  String str_blink = _obj_variable.display_value;
  if (blink_segment)
  { 
    if(_obj_variable._lst_display_menu_screen_config[_index]._d_type== DTYPE_NONE)
      _obj_variable._obj_display.w_display_segment(_obj_variable.display_value, 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_config[_index]._d_type == DTYPE_EN_DIS)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_en_dis[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_config[_index]._d_type == DTYPE_YES_NO)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_yes_no[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_config[_index]._d_type == DTYPE_INPUT_TYPE)
    {
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_input_type[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    }
    else if(_obj_variable._lst_display_menu_screen_config[_index]._d_type == DTYPE_PARITY)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_parity[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_config[_index]._d_type == DTYPE_BUADRATE)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_buadrate[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_config[_index]._d_type== DTYPE_NUMBER)
    {
      _obj_variable._obj_display.w_display_segment(CheckStringLength(str_blink, 4, "0"), _obj_variable.display_decimal, "    ", 0, "    ", 0, "    ", 0);
    }
    else if(_obj_variable._lst_display_menu_screen_config[_index]._d_type == DTYPE_ALARM_TYPE)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_alarm_type[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_config[_index]._d_type == DTYPE_ALARM_ON_OFF)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_alarm_on_off[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_config[_index]._d_type == DTYPE_CTYP)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_ctyp[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
  }
  else
  {
    str_blink[_obj_variable.cursor_position] = ' ';
    if(_obj_variable._lst_display_menu_screen_config[_index]._d_type == DTYPE_NUMBER)
    {
      //Serial.println("Blinking : " + CheckStringLength(str_blink, 4, "0") + " | Position : " + (_obj_variable.cursor_position));
      _obj_variable._obj_display.w_display_segment(CheckStringLength(str_blink, 4, "0"), _obj_variable.display_decimal, "    ", 0, "    ", 0, "    ", 0);
    }
   
  }
}

void value_to_data_type_display_value_setup(int _index)
{
  String str_blink = _obj_variable.display_value;
  
  if (blink_segment)
  { 
    if(_obj_variable._lst_display_menu_screen_setup[_index]._d_type== DTYPE_NONE)
      _obj_variable._obj_display.w_display_segment(_obj_variable.display_value, 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_setup[_index]._d_type == DTYPE_EN_DIS)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_en_dis[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_setup[_index]._d_type == DTYPE_YES_NO)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_yes_no[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_setup[_index]._d_type == DTYPE_INPUT_TYPE)
    {
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_input_type[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    }
    else if(_obj_variable._lst_display_menu_screen_setup[_index]._d_type == DTYPE_PARITY)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_parity[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_setup[_index]._d_type == DTYPE_BUADRATE)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_buadrate[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_setup[_index]._d_type== DTYPE_NUMBER)
    {
      _obj_variable._obj_display.w_display_segment(CheckStringLength(str_blink, 4, "0"), _obj_variable.display_decimal, "    ", 0, "    ", 0, "    ", 0);
    }
    else if(_obj_variable._lst_display_menu_screen_setup[_index]._d_type == DTYPE_ALARM_TYPE)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_alarm_type[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_setup[_index]._d_type == DTYPE_ALARM_ON_OFF)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_alarm_on_off[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_setup[_index]._d_type == DTYPE_CTYP)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_ctyp[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
  }
  else
  {
    // Serial.println(str_blink);
    str_blink[_obj_variable.cursor_position] =' ';
    
    if(_obj_variable._lst_display_menu_screen_setup[_index]._d_type == DTYPE_NUMBER)
    {
      // Serial.println("Blinking : " + CheckStringLength(str_blink, 4, "0") + " | Position : " + (_obj_variable.cursor_position));
      _obj_variable._obj_display.w_display_segment(CheckStringLength(str_blink, 4, "0"), _obj_variable.display_decimal, "    ", 0, "    ", 0, "    ", 0);
    }
   
  }
}

void value_to_data_type_display_value_cali(int _index)
{
  String str_blink = _obj_variable.display_value;
  if (blink_segment)
  { 
    if(_obj_variable._lst_display_menu_screen_cali[_index]._d_type== DTYPE_NONE)
      _obj_variable._obj_display.w_display_segment(_obj_variable.display_value, 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_cali[_index]._d_type == DTYPE_EN_DIS)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_en_dis[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_cali[_index]._d_type == DTYPE_YES_NO)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_yes_no[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_cali[_index]._d_type == DTYPE_INPUT_TYPE)
    {
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_input_type[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    }
    else if(_obj_variable._lst_display_menu_screen_cali[_index]._d_type == DTYPE_PARITY)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_parity[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_cali[_index]._d_type == DTYPE_BUADRATE)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_buadrate[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_cali[_index]._d_type== DTYPE_NUMBER)
    {
      _obj_variable._obj_display.w_display_segment(CheckStringLength(str_blink, 4, "0"), _obj_variable.display_decimal, "    ", 0, "    ", 0, "    ", 0);
    }
    else if(_obj_variable._lst_display_menu_screen_cali[_index]._d_type == DTYPE_ALARM_TYPE)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_alarm_type[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_cali[_index]._d_type == DTYPE_ALARM_ON_OFF)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_alarm_on_off[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_cali[_index]._d_type == DTYPE_CTYP)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_ctyp[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
  }
  else
  {
    str_blink[_obj_variable.cursor_position] = ' ';
    if(_obj_variable._lst_display_menu_screen_cali[_index]._d_type == DTYPE_NUMBER)
    {
      //Serial.println("Blinking : " + CheckStringLength(str_blink, 4, "0") + " | Position : " + (_obj_variable.cursor_position));
      _obj_variable._obj_display.w_display_segment(CheckStringLength(str_blink, 4, "0"), _obj_variable.display_decimal, "    ", 0, "    ", 0, "    ", 0);
    }
   
  }
}

void value_to_data_type_display_value_chang(int _index)
{
  String str_blink = _obj_variable.display_value;
  if (blink_segment)
  { 
    if(_obj_variable._lst_display_menu_screen_chag[_index]._d_type== DTYPE_NONE)
      _obj_variable._obj_display.w_display_segment(_obj_variable.display_value, 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_chag[_index]._d_type == DTYPE_EN_DIS)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_en_dis[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_chag[_index]._d_type == DTYPE_YES_NO)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_yes_no[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_chag[_index]._d_type == DTYPE_INPUT_TYPE)
    {
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_input_type[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    }
    else if(_obj_variable._lst_display_menu_screen_chag[_index]._d_type == DTYPE_PARITY)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_parity[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_chag[_index]._d_type == DTYPE_BUADRATE)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_buadrate[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_chag[_index]._d_type== DTYPE_NUMBER)
    {
      _obj_variable._obj_display.w_display_segment(CheckStringLength(str_blink, 4, "0"), _obj_variable.display_decimal, "    ", 0, "    ", 0, "    ", 0);
    }
    else if(_obj_variable._lst_display_menu_screen_chag[_index]._d_type == DTYPE_ALARM_TYPE)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_alarm_type[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_chag[_index]._d_type == DTYPE_ALARM_ON_OFF)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_alarm_on_off[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_chag[_index]._d_type == DTYPE_CTYP)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_ctyp[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
  }
  else
  {
    str_blink[_obj_variable.cursor_position] = ' ';
    if(_obj_variable._lst_display_menu_screen_chag[_index]._d_type == DTYPE_NUMBER)
    {
      //Serial.println("Blinking : " + CheckStringLength(str_blink, 4, "0") + " | Position : " + (_obj_variable.cursor_position));
      _obj_variable._obj_display.w_display_segment(CheckStringLength(str_blink, 4, "0"), _obj_variable.display_decimal, "    ", 0, "    ", 0, "    ", 0);
    }
   
  }
}

void value_to_data_type_display_value_buzz(int _index)
{
  String str_blink = _obj_variable.display_value;
  if (blink_segment)
  { 
    if(_obj_variable._lst_display_menu_screen_buzz[_index]._d_type== DTYPE_NONE)
      _obj_variable._obj_display.w_display_segment(_obj_variable.display_value, 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_buzz[_index]._d_type == DTYPE_EN_DIS)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_en_dis[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_buzz[_index]._d_type == DTYPE_YES_NO)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_yes_no[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_buzz[_index]._d_type == DTYPE_INPUT_TYPE)
    {
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_input_type[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    }
    else if(_obj_variable._lst_display_menu_screen_buzz[_index]._d_type == DTYPE_PARITY)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_parity[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_buzz[_index]._d_type == DTYPE_BUADRATE)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_buadrate[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_buzz[_index]._d_type== DTYPE_NUMBER)
    {
      _obj_variable._obj_display.w_display_segment(CheckStringLength(str_blink, 4, "0"), _obj_variable.display_decimal, "    ", 0, "    ", 0, "    ", 0);
    }
    else if(_obj_variable._lst_display_menu_screen_buzz[_index]._d_type == DTYPE_ALARM_TYPE)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_alarm_type[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_buzz[_index]._d_type == DTYPE_ALARM_ON_OFF)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_alarm_on_off[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else if(_obj_variable._lst_display_menu_screen_buzz[_index]._d_type == DTYPE_CTYP)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.dtype_arr_ctyp[_obj_variable.display_value.toInt()], 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
  }
  else
  {
    str_blink[_obj_variable.cursor_position] = ' ';
    if(_obj_variable._lst_display_menu_screen_buzz[_index]._d_type == DTYPE_NUMBER)
    {
      //Serial.println("Blinking : " + CheckStringLength(str_blink, 4, "0") + " | Position : " + (_obj_variable.cursor_position));
      _obj_variable._obj_display.w_display_segment(CheckStringLength(str_blink, 4, "0"), _obj_variable.display_decimal, "    ", 0, "    ", 0, "    ", 0);
    }
   
  }
}

void display_7led_screen()
{
  get_key();
  if (_obj_variable._obj_display_screen == DESHBOARD)
  {
    _obj_variable.menu_scren_index=0;

    if(_obj_variable.isReboot)
    {
      //Serial.println("Instrument Restart");
      ESP.restart();
    }
    // Serial.println("Dispalay_Value" + String(_obj_variable.display_1));
    _obj_variable._obj_display.w_display_segment(_obj_variable.display_1, _obj_variable.decimal_1, _obj_variable.display_2, _obj_variable.decimal_2, _obj_variable.display_3, _obj_variable.decimal_3, _obj_variable.display_4, _obj_variable.blink_decmial);
     
    if (!_obj_variable.is_key1_press && !_obj_variable.is_key2_press && !_obj_variable.is_key3_press && !_obj_variable.is_key4_press)
    {
      if (_obj_variable._key_press_1)
      {
        key_reset();
        _obj_variable._obj_display_screen = DESHBOARD;
        for (int j = 0; j < (TOTALCHANNEL * PER_CH_ALARM); j++)
        {
          if (_obj_variable._lst_parameter_alarm_info[j]._status)
          {
            if (_obj_variable._lst_parameter_alarm_info[j]._is_led_status == LED_BLINK)
            {
              _obj_variable._lst_parameter_alarm_info[j]._is_led_status = LED_ON;
              _obj_variable._lst_parameter_alarm_info[j]._is_ack = true;
            }
          }
        }

        if (_obj_variable._led_ahu_off == LED_BLINK)
        {
          _obj_variable._led_ahu_off = LED_ON;
        }
        if (_obj_variable._led_ahu_trip == LED_BLINK)
        {
          _obj_variable._led_ahu_trip = LED_ON;
        }
      }
      //if(_obj_variable._key_press_2) { key_reset(); _obj_variable._obj_display_screen = SETPOINT; }
      if (_obj_variable._key_press_4)
      {
        key_reset();
        _obj_variable._obj_display_screen = SETTING;
        _obj_variable.onetime_write = true;
        _obj_variable.scren_index = 0;
      }
    }

    //RESET EPPROM
    if(_obj_variable.is_key3_press && _obj_variable.is_key4_press)
    {
      long diff = millis()-_obj_variable.millis_factory_reset;
      if(diff>=5000)
      {
        //Serial.println("Factory Reset");
        EEPROM.write(0,100);
        EEPROM.commit();
        ESP.restart();
      }
    }
    else
      _obj_variable.millis_factory_reset = millis();


       //ENABLE_WIFI_OTA
    if(_obj_variable.is_key1_press && _obj_variable.is_key2_press)
    {
      long diff = millis()-_obj_variable.millis_enable_wifi_ota;
      if(diff>=5000)
      {
        if(!_obj_variable.isAccessPointStart)
        {
          const char *ssid = "UTPL-1407";
          const char *password = "Unitech123";
          // Serial.print("SSID : ");
          // Serial.println(ssid);
          //Serial.print("Password : ");
          //Serial.println(password);
          WiFi.softAP(ssid, password);
          
          // IPAddress myIP = WiFi.softAPIP();
          // Serial.print("AP IP address: ");
          // Serial.println(myIP);
          // _obj_variable.isWifiAccessPort=true;
          _obj_variable.isWifiAccessPort=true;
          _obj_variable.isAccessPointStart=true;
          ota_wifi_access_point();
          // _obj_variable.isAccessPointStart=true;
          _obj_variable._obj_display_screen = UPDATE;
          // Serial.println(_obj_variable._obj_display_screen );
          _obj_variable.onetime_write = true;
        }
        _obj_variable.millis_enable_wifi_ota = millis();
      }
    }
    else
      _obj_variable.millis_enable_wifi_ota = millis();
    

    _obj_variable.currentMillis = millis();
  }
  else if (_obj_variable._obj_display_screen == PASSWORD)
  {
    if (_obj_variable.onetime_write)
    {
      _obj_variable.display_value = "0000";
      _obj_variable.onetime_write = false;
      _obj_variable.cursor_position = 3;
    }
    String str_blink = _obj_variable.display_value;
    if (blink_segment)
      _obj_variable._obj_display.w_display_segment(CheckStringLength(_obj_variable.display_value, 4, "0"), 0, "    ", 0, "    ", 0, "    ", 0);
    else
    {
      str_blink[_obj_variable.cursor_position] = ' ';
      _obj_variable._obj_display.w_display_segment(CheckStringLength(str_blink, 4, "0"), 0,"    ", 0, "    ", 0, "    ", 0);
    }

    if (_obj_variable.logout_time + _obj_variable.currentMillis <= millis())
      _obj_variable._obj_display_screen = DESHBOARD;

    if (_obj_variable.is_key1_press)
      _obj_variable._key_press_1 = _obj_variable.is_key1_press;
    if (_obj_variable.is_key2_press)
      _obj_variable._key_press_2 = _obj_variable.is_key2_press;
    if (_obj_variable.is_key3_press)
      _obj_variable._key_press_3 = _obj_variable.is_key3_press;
    if (_obj_variable.is_key4_press)
      _obj_variable._key_press_4 = _obj_variable.is_key4_press;

    if (!_obj_variable.is_key1_press && !_obj_variable.is_key2_press && !_obj_variable.is_key3_press && !_obj_variable.is_key4_press)
    {
      if (_obj_variable._key_press_1)
      {
        key_reset();
        _obj_variable.display_value = (incrimentel(_obj_variable.display_value, 0, 1, 0, 1, 0));
        // Serial.println("Display_value" + String(_obj_variable.display_value));
      }
      if (_obj_variable._key_press_2)
      {
        key_reset();
        _obj_variable.display_value = (incrimentel(_obj_variable.display_value, 1, 0, 0, 1, 0));
      }
      if (_obj_variable._key_press_3)
      {
        key_reset();
        _obj_variable.isPassword=false;
        int password = StringToInt(_obj_variable.display_value);
        // Serial.println("Password : " + String(password));
        if(_obj_variable.screen_password==password || _obj_variable.screen_password_master==password)
        {
          //Serial.println("Password True");
          _obj_variable.isPassword=true;
          
        }
        if(_obj_variable.screen_password==password || _obj_variable.screen_password_master==password || _obj_variable.screen_password_default == password)
        {
          // Serial.println("Passwod_Index"+ String(_obj_variable.password_index));
          if (_obj_variable.password_index == 1)
          {
            //_obj_variable.isPassword=true;
            _obj_variable._obj_display_screen = MODE_CONFIG;
            _obj_variable.onetime_write = true;
            _obj_variable.scren_index = 0;
          }
          if (_obj_variable.password_index == 2)
          {
            //_obj_variable.isPassword=true;
            _obj_variable._obj_display_screen = MODE_SETUP;
            _obj_variable.onetime_write = true;
            _obj_variable.scren_index = 0;
          }
          if (_obj_variable.password_index == 3)
          {
            //_obj_variable.isPassword=true;
            _obj_variable._obj_display_screen = MODE_CALIBRATION;
            _obj_variable.onetime_write = true;
            _obj_variable.scren_index = 0;
          }
          if (_obj_variable.password_index == 4)
          {
            //_obj_variable.isPassword=true;
            _obj_variable._obj_display_screen = MODE_PASSOWRD;
            _obj_variable.onetime_write = true;
            _obj_variable.scren_index = 0;
          }
          if (_obj_variable.password_index == 5)
          {
            //_obj_variable.isPassword=true;
            _obj_variable._obj_display_screen = MODE_BUZZ;
            _obj_variable.onetime_write = true;
            _obj_variable.scren_index = 0;
          //   Serial.println(_obj_variable._obj_display_screen);
          }
        }
        _obj_variable.onetime_write = true;
      }
      if (_obj_variable._key_press_4)
      {
        key_reset();
        
          _obj_variable._obj_display_screen = SETTING;
          _obj_variable.onetime_write = true;          
                
      }
    }
  }
  else if (_obj_variable._obj_display_screen == SETTING)
  {
    //menu_setting_arr

    if (_obj_variable.onetime_write)
    {
      //Serial.println("_obj_variable.menu_scren_index : " + String(_obj_variable.menu_scren_index));
      _obj_variable.display_value = _obj_variable.menu_setting_arr[_obj_variable.menu_scren_index];
      // Serial.println("display_value : " + String(_obj_variable.display_value));
      _obj_variable.onetime_write = false;
      _obj_variable.cursor_position = 3;
    }
    // if (blink_segment)
      _obj_variable._obj_display.w_display_segment(_obj_variable.display_value, 0, "    ", 0, "    ", 0, "    ", 0);

    if (_obj_variable.logout_time + _obj_variable.currentMillis <= millis())
      _obj_variable._obj_display_screen = DESHBOARD;
  
    if (!_obj_variable.is_key1_press && !_obj_variable.is_key2_press && !_obj_variable.is_key3_press && !_obj_variable.is_key4_press)
    {
      if (_obj_variable._key_press_1)
      {
        key_reset();
        //_obj_variable._obj_display_screen = DESHBOARD;
      }
      if (_obj_variable._key_press_2)
      {
        key_reset();
        _obj_variable.onetime_write = true;
        _obj_variable.menu_scren_index++;
        if (_obj_variable.menu_scren_index >= 6)
          _obj_variable.menu_scren_index = 1;
      }
      if (_obj_variable._key_press_3)
      {
        key_reset();
        if(_obj_variable.menu_scren_index==0)
        {
          _obj_variable.onetime_write = true;
          _obj_variable.menu_scren_index++;
        }
        else
        {
        _obj_variable._obj_display_screen = PASSWORD;
        _obj_variable.onetime_write = true;
        _obj_variable.password_index = _obj_variable.menu_scren_index;        
        }
        
      }
      if (_obj_variable._key_press_4)
      {
        key_reset();
        _obj_variable._obj_display_screen = DESHBOARD;
      }
    }
  }
  else if (_obj_variable._obj_display_screen == MODE_CONFIG)
  {
    //data_type _dtype=DTYPE_NONE;
    //Serial.print("scren_index : " + String(_obj_variable.scren_index));
    //Serial.println(" | _display_screen : " + String(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen));
    if (_obj_variable.onetime_write)
    {
      // Serial.print("scren_index : " + String(_obj_variable.scren_index));
      // Serial.println(" | _display_screen : " + String(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen));
      // Serial.println( " Display-val  " +String(_obj_variable.display_value));
      if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_ALARM_STATUS)
      {
        _obj_variable.display_decimal = 0;
        if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < (TOTALCHANNEL * PER_CH_ALARM))
        {
          if( _obj_variable._lst_parameter_alarm_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._status)
            _obj_variable.display_value ="1";
          else
            _obj_variable.display_value ="0";
        }
      }  
      else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_ALARM_DELAY_TIME)
      {
        _obj_variable.display_decimal = 0;
        if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < (TOTALCHANNEL * PER_CH_ALARM))
        {
          _obj_variable.display_value = String(_obj_variable._lst_parameter_alarm_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._delay);
          
        }
      }
      else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_INP_DEMPING)
      {
        _obj_variable.display_decimal = 0;
        if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < TOTALCHANNEL)
        {
          _obj_variable.display_value = String(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._f_demping);
        }
      }
      else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_INP_DECIMAL)
      {
        _obj_variable.display_decimal = 0;
        if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < TOTALCHANNEL)
        {
          _obj_variable.display_value = String(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._decimal);
        }
      }
      else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_INP_STATUS)
      {
        _obj_variable.display_decimal = 0;
        if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < TOTALCHANNEL)
        {
          _obj_variable.display_value = String(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._disp_status);
        }
      }
      else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_INP_NONG)
      {
        _obj_variable.display_decimal = 0;
        if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < TOTALCHANNEL)
        {
          _obj_variable.display_value = String(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._is_negative);
        }
      }
      else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_TIME)
      {
        if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index==0)        
          _obj_variable.display_value = second();
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index == 1)        
          _obj_variable.display_value = minute();
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index == 2)        
          _obj_variable.display_value = hour();
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index == 3)        
          _obj_variable.display_value = day();
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index == 4)        
          _obj_variable.display_value = month();
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index == 5)        
          _obj_variable.display_value = year()-2000;
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index == 6)  
        {
          _obj_variable.display_value = _obj_variable._lst_other_info[2]._value;  
        }      
          // Serial.println(_obj_variable.display_value);
      }
      else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_COMMUNICATION)
      {
        _obj_variable.display_decimal = 0;
        if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < COMMUNICATION_SIZE)
        {
          _obj_variable.display_value = String(_obj_variable._lst_communication[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._value);
        }
      }
      else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_OFFSET)
      {
         
        if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < TOTALCHANNEL)
        {
          _obj_variable.display_value = String(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._offset);
          
          // Serial.println(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index);
          _obj_variable.display_decimal = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._decimal;
        }
      }
      else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CALIBRATION_CAL_OFFSET)
      {
         
        if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < TOTALCHANNEL)
        {
          _obj_variable.display_value = String(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._cali_offset);
          _obj_variable.display_decimal = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._decimal;
        }
      }
      
      else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_NONE)
        _obj_variable.display_value = _obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._name;
      
      if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_NUMBER)
      {
        _obj_variable.display_value = CheckStringLength(_obj_variable.display_value, 4, "0");
      }
      _obj_variable.onetime_write = false;
      _obj_variable.cursor_position = 3;
    }
    
    value_to_data_type_display_value_config(_obj_variable.scren_index);

    if (_obj_variable.logout_time + _obj_variable.currentMillis <= millis())
    _obj_variable._obj_display_screen = DESHBOARD;
  
    if (!_obj_variable.is_key1_press && !_obj_variable.is_key2_press && !_obj_variable.is_key3_press && !_obj_variable.is_key4_press)
    {
      if (_obj_variable._key_press_1)
      {
        key_reset();
        if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen!=MODE_NONE)
        {
          if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_NUMBER)
          {
            
            _obj_variable.display_value = (incrimentel(_obj_variable.display_value, 0, 1, 1, 0, 0));            
            
          }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_INPUT_TYPE)
          {
            
            _obj_variable.display_value = String((_obj_variable.display_value.toInt()-1));
          }

          if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_INPUT_TYPE) { if (_obj_variable.display_value.toInt() <0) _obj_variable.display_value = String(13); }
        }
      }
      if (_obj_variable._key_press_2)
      {
        key_reset();
        if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen!=MODE_NONE)
        {
          if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_INPUT_TYPE)
          _obj_variable.display_value = String((_obj_variable.display_value.toInt()+1));
          else
          _obj_variable.display_value = incrimentel(_obj_variable.display_value, 1, 0, 1, 0, 0);
          // Serial.println("display_value : " + String(_obj_variable.display_value));
          if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_NONE) { }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_EN_DIS) { if (_obj_variable.display_value.toInt() > 1) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_YES_NO) { if (_obj_variable.display_value.toInt() > 1) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_INPUT_TYPE) { if (_obj_variable.display_value.toInt() > 13) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_PARITY) { if (_obj_variable.display_value.toInt() > 2) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_BUADRATE) { if (_obj_variable.display_value.toInt() > 6) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_NUMBER) { }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_ALARM_TYPE) { if (_obj_variable.display_value.toInt() > 3) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_ALARM_ON_OFF) { if (_obj_variable.display_value.toInt() > 1) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._d_type == DTYPE_CTYP) { if (_obj_variable.display_value.toInt() > 1) _obj_variable.display_value = String(0); }
        }
      }
      if (_obj_variable._key_press_3)
      {
        key_reset();  

        if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_ALARM_STATUS)
        {
          _obj_variable.display_decimal = 0;
          if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < (TOTALCHANNEL * PER_CH_ALARM))
          {
            // Serial.println("Status  "+String(_obj_variable._lst_parameter_alarm_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._status)+ "   |   "+String(_obj_variable.display_value));
            if( (_obj_variable._lst_parameter_alarm_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._status) !=_obj_variable.display_value.toInt())
            {
              _obj_variable._lst_parameter_alarm_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._status = _obj_variable.display_value.toInt();
              // modebus_conf_write_onetime();
              // Serial.println("Set_ALARM  "+String(_obj_variable._lst_parameter_alarm_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._status));
              Configuration_WriteEEPROM_AllTime();
            }
          }
        }  
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_ALARM_DELAY_TIME)
        {
          _obj_variable.display_decimal = 0;
          if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < (TOTALCHANNEL * PER_CH_ALARM))
          {
            // Serial.println("Display_Val  "+String(_obj_variable.display_value)+"  |  " +String(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index)+"  |  "+String(_obj_variable._lst_parameter_alarm_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._delay));
            if(_obj_variable.display_value.toInt() != _obj_variable._lst_parameter_alarm_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._delay)
            {
              _obj_variable._lst_parameter_alarm_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._delay = _obj_variable.display_value.toInt();
              modebus_conf_write_onetime();
              Configuration_WriteEEPROM_AllTime();
            }
          }
        }
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_INP_DEMPING)
        {
          _obj_variable.display_decimal = 0;
          if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < TOTALCHANNEL)
          {
            if(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._f_demping != _obj_variable.display_value.toInt())
            {
              _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._f_demping = _obj_variable.display_value.toInt();
              modebus_conf_write_onetime();
              Configuration_WriteEEPROM_AllTime();
            }
          }
        }
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_INP_DECIMAL)
        {
          _obj_variable.display_decimal = 0;
          if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < TOTALCHANNEL)
          {
            if(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._decimal != _obj_variable.display_value.toInt())
            {
              // Serial.println("Display_value  -  "+ String(_obj_variable.display_value));
              
              _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._decimal = _obj_variable.display_value.toInt();
              // Serial.println("_value  -  "+ String(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index));
              modebus_conf_write_onetime();
              Configuration_WriteEEPROM_AllTime();
            }
          }
        }
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_INP_STATUS)
        {
          _obj_variable.display_decimal = 0;
          if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < TOTALCHANNEL)
          {
            if(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._disp_status != _obj_variable.display_value.toInt())
            {
              _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._disp_status = _obj_variable.display_value.toInt();
              modebus_conf_write_onetime();
              Configuration_WriteEEPROM_AllTime();
            }
          }
        }
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_INP_NONG)
        {
          _obj_variable.display_decimal = 0;
          if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < TOTALCHANNEL)
          {
             if(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._is_negative != _obj_variable.display_value.toInt())
            {
              _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._is_negative = _obj_variable.display_value.toInt();
              modebus_conf_write_onetime();
              Configuration_WriteEEPROM_AllTime();
            }
          }
        }
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_TIME)
        {
          if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index==0)   
          {     
            _m_secound = (StringToInt(_obj_variable.display_value));
            set_secound = 1;
          }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index == 1)        
          {
            _m_minute = (StringToInt(_obj_variable.display_value));
            set_minute = 1;
          }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index == 2)        
          {
            _m_hour = (StringToInt(_obj_variable.display_value));
            set_hour = 1;
          }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index == 3)        
          {
            _m_day = (StringToInt(_obj_variable.display_value));
            set_day = 1;
          }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index == 4)        
          {
            _m_month = (StringToInt(_obj_variable.display_value));
            set_month = 1;
          }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index == 5)        
          {
            _m_year = (StringToInt(_obj_variable.display_value));
            set_year = 1;
          }
          else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index == 6)        
          {
            _obj_variable._lst_other_info[2]._value = _obj_variable.display_value.toInt();
            Configuration_WriteEEPROM_AllTime();
          }
        }
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_COMMUNICATION)
        {
          _obj_variable.display_decimal = 0;
          if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < COMMUNICATION_SIZE)
          {
            if(_obj_variable._lst_communication[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._value != _obj_variable.display_value.toInt())
            {
             _obj_variable._lst_communication[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._value = _obj_variable.display_value.toInt();
              // modebus_conf_write_onetime();
              set_comm();
            }
          }
        }
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CONFIG_OFFSET)
        {
          
          if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < TOTALCHANNEL)
          {
            if(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._offset != _obj_variable.display_value.toInt())
            {
              _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._offset = _obj_variable.display_value.toInt();
              // Serial.println(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._offset);
              // Serial.println(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index);
              modebus_conf_write_onetime();
              Configuration_WriteEEPROM_AllTime();
            }
          }
        }
        else if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._display_screen==MODE_CALIBRATION_CAL_OFFSET)
        {
          
          if(_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index < TOTALCHANNEL)
          {
            if(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._cali_offset != _obj_variable.display_value.toInt())
            {
              _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_config[_obj_variable.scren_index]._index]._cali_offset = _obj_variable.display_value.toInt();
              modebus_conf_write_onetime();
              Configuration_WriteEEPROM_AllTime();
            }
          }
        }
        
        // Serial.println("Config Menu Size : " + String(DISPLAY_MENU_SCREEN_CONFIG_SIZE ) + " | Index : " + String(_obj_variable.scren_index));
        _obj_variable.scren_index++;
        if(!_obj_variable.isPassword  &&  _obj_variable.scren_index >= (DISPLAY_MENU_SCREEN_CONFIG_SIZE-39 ))
          _obj_variable.scren_index=0;

        if (_obj_variable.scren_index >= DISPLAY_MENU_SCREEN_CONFIG_SIZE )
          _obj_variable.scren_index = 0;  
        //_obj_variable._obj_display_screen = _obj_variable._lst_display_menu_screen_config.get(scren_index)->_display_screen;
        _obj_variable.onetime_write = true;
        //scren_index = 0;
        //password_index = scren_index;
        //scren_index = 0;
        
      }
      if (_obj_variable._key_press_4)
      {
        key_reset();
        _obj_variable._obj_display_screen = SETTING;
        _obj_variable.onetime_write = true;
      }
    }
  } 
  else if (_obj_variable._obj_display_screen == MODE_SETUP)
  {
    if (_obj_variable.onetime_write)
    {
      //Serial.print("scren_index : " + String(_obj_variable.scren_index));
      //Serial.println(" | _display_screen : " + String(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._display_screen));
      if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._display_screen==MODE_SETUP_ZERO)
      {
        if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index < TOTALCHANNEL)
        {
          _obj_variable.display_decimal = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._decimal;
          _obj_variable.display_value = String((int32_t)_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._r_zero);  
          //_obj_variable.display_value = CheckStringLength(IntToString(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._r_zero, true), 4, "0", 0);
        }
        // else
        //   Serial.println(" | OutOfSize : " + String(TOTALCHANNEL));
      }
      else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._display_screen==MODE_SETUP_SPAN)
      {
        _obj_variable.display_decimal = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._decimal;
        _obj_variable.display_value = String((int32_t)_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._r_span);  
        //_obj_variable.display_value = CheckStringLength(IntToString(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._r_span, true), 4, "0", 0);
        //Serial.println(String(_obj_variable.display_value));
      }
      else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._display_screen==MODE_SETUP_ALARM_SETPOINT)
      {
        _obj_variable.display_decimal = _obj_variable._lst_parameter_info[_obj_variable._lst_parameter_alarm_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._para_id-1]._decimal;
        _obj_variable.display_value = String(_obj_variable._lst_parameter_alarm_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._value);   
        //_obj_variable.display_value = CheckStringLength(IntToString(_obj_variable._lst_parameter_alarm_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._value, true), 4, "0", 0);
        // Serial.println(String(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._name) + " : " + String(_obj_variable.display_value) + " : " + String(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index));
        // Serial.println(String(_obj_variable.scren_index) + " Screen_Indexx ");
      }      
      else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._display_screen==MODE_SETUP_OUTPUT_ZERO)
      {
        _obj_variable.display_decimal = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._decimal;
        _obj_variable.display_value = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._op_r_zero; 
        //Serial.println(String(display_value));
      }
      else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._display_screen==MODE_SETUP_OUTPUT_SPAN)
      {
        _obj_variable.display_decimal = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._decimal;
        _obj_variable.display_value = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._op_r_span;  
        //Serial.println(String(display_value));
      }
      else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._display_screen==MODE_NONE)
        _obj_variable.display_value = _obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._name;
      
      // if(_obj_variable._lst_display_menu_screen_setup.get(scren_index)->_d_type == DTYPE_INPUT_TYPE)
      // {
      //   Serial.print(String(display_value) + " | ");
      //   Serial.println(CheckStringLength(dtype_arr_input_type[display_value.toInt()], 4, "0"));
      // }
      if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._d_type == DTYPE_NUMBER)
        _obj_variable.display_value = CheckStringLength(_obj_variable.display_value, 4, "0");
      _obj_variable.onetime_write = false;
      _obj_variable.cursor_position = 3;
    }
    
    value_to_data_type_display_value_setup(_obj_variable.scren_index);

    if (_obj_variable.logout_time + _obj_variable.currentMillis <= millis())
      _obj_variable._obj_display_screen = DESHBOARD;
  
    if (!_obj_variable.is_key1_press && !_obj_variable.is_key2_press && !_obj_variable.is_key3_press && !_obj_variable.is_key4_press)
    {
      if (_obj_variable._key_press_1)
      {
        key_reset();
        if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._display_screen!=MODE_NONE)
        {
          if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._d_type == DTYPE_NUMBER)
            _obj_variable.display_value = (incrimentel(_obj_variable.display_value, 0, 1, 1, 0, 0));
        }
      }
      if (_obj_variable._key_press_2)
      {
        key_reset();
        if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._display_screen!=MODE_NONE)
        {
          if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._d_type == DTYPE_INPUT_TYPE)
          _obj_variable.display_value = String((_obj_variable.display_value.toInt()+1));
          else
          _obj_variable.display_value = incrimentel(_obj_variable.display_value, 1, 0, 1, 0, 0);
          //Serial.println("display_value : " + String(_obj_variable.display_value));
          if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._d_type == DTYPE_NONE) { }
          else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._d_type == DTYPE_EN_DIS) {if (_obj_variable.display_value.toInt() > 1) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._d_type == DTYPE_YES_NO) { if (_obj_variable.display_value.toInt() > 1) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._d_type == DTYPE_INPUT_TYPE) { if (_obj_variable.display_value.toInt() > 13) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._d_type == DTYPE_PARITY) { if (_obj_variable.display_value.toInt() > 2) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._d_type == DTYPE_BUADRATE) { if (_obj_variable.display_value.toInt() > 6) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._d_type == DTYPE_NUMBER) { }
          else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._d_type == DTYPE_ALARM_TYPE) { if (_obj_variable.display_value.toInt() > 3) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._d_type == DTYPE_ALARM_ON_OFF) { if (_obj_variable.display_value.toInt() > 1) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._d_type == DTYPE_CTYP) { if (_obj_variable.display_value.toInt() > 1) _obj_variable.display_value = String(0); }
        }
      }
      if (_obj_variable._key_press_3)
      {
        key_reset(); 

        if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._display_screen==MODE_SETUP_ZERO)
        {
          if(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._r_zero != _obj_variable.display_value.toInt())
          {
            _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._r_zero = _obj_variable.display_value.toInt();  
            Configuration_WriteEEPROM_AllTime();
            modebus_conf_write_onetime();
            //Serial.println(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._name + " : r_zero :" + String(_obj_variable.display_value));
          }
          // else
          //   Serial.println(" | OutOfSize : " + String(TOTALCHANNEL));
        }
        else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._display_screen==MODE_SETUP_SPAN)
        {
          if(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._r_span != _obj_variable.display_value.toInt())
          {
            _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._r_span = _obj_variable.display_value.toInt();  
            Configuration_WriteEEPROM_AllTime();
            modebus_conf_write_onetime();
            //Serial.println(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._name + " : r_span :" + String(_obj_variable.display_value));
          }
        }
        else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._display_screen==MODE_SETUP_ALARM_SETPOINT)
        {
          if(_obj_variable._lst_parameter_alarm_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._value != _obj_variable.display_value.toInt())
          {
            _obj_variable._lst_parameter_alarm_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._value = _obj_variable.display_value.toInt();  
            
            modebus_conf_write_onetime();
 
            // Serial.println(String(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._name) + " : " + String(_obj_variable.display_value));
            Configuration_WriteEEPROM_AllTime();
            // Serial.println("Configuration_WriteEEPROM_AllTime");
        
          }
        }        
        else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._display_screen==MODE_SETUP_OUTPUT_ZERO)
        {
          if(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._op_r_zero != _obj_variable.display_value.toInt())
          {
            _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._op_r_zero = _obj_variable.display_value.toInt();  
            Configuration_WriteEEPROM_AllTime();
            modebus_conf_write_onetime();
            //Serial.println(String(_obj_variable.display_value));
          }
          //Serial.println(String(display_value));
        }
        else if(_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._display_screen==MODE_SETUP_OUTPUT_SPAN)
        {
          if(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._op_r_span != _obj_variable.display_value.toInt())
          {
            _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._op_r_span = _obj_variable.display_value.toInt();  
            Configuration_WriteEEPROM_AllTime();
            modebus_conf_write_onetime();
            //Serial.println(String(_obj_variable.display_value));
          }  
          //Serial.println(String(display_value));
        }

        _obj_variable.scren_index++;
        // Serial.println(_obj_variable.scren_index);
        if(!_obj_variable.isPassword && _obj_variable.scren_index>=DISPLAY_MENU_SCREEN_SETUP_SIZE-12)
          _obj_variable.scren_index=0;
        if (_obj_variable.scren_index >= DISPLAY_MENU_SCREEN_SETUP_SIZE)
          _obj_variable.scren_index = 0;  
        //_obj_variable._obj_display_screen = _obj_variable._lst_display_menu_screen_setup.get(scren_index)->_display_screen;
        _obj_variable.onetime_write = true;
        //scren_index = 0;
        //password_index = scren_index;
        //scren_index = 0;
        
      }
      if (_obj_variable._key_press_4)
      {
        key_reset();
        _obj_variable._obj_display_screen = SETTING;
        _obj_variable.onetime_write = true;
      }
    }
  }
  else if (_obj_variable._obj_display_screen == MODE_CALIBRATION)
  {
    //data_type _dtype=DTYPE_NONE;
    if (_obj_variable.onetime_write)
    {

      if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._display_screen==MODE_CALIBRATION_CAL_ZERO)
      {
        if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index < TOTALCHANNEL)
        {
          _obj_variable.display_value = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._c_zero;  
          
        }
        // else
        //   Serial.println(" | OutOfSize : " + String(TOTALCHANNEL));
      }
      else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._display_screen==MODE_CALIBRATION_CAL_SPAN)
      {
        _obj_variable.display_value = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._c_span;  
        //Serial.println(String(_obj_variable.display_value));
      }
      else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._display_screen==MODE_CALIBRATION_CAL_OUTPUT_ZERO)
      {
        //_obj_variable.display_value = 30000;
       _obj_variable.display_value = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_zero;   
        int32_t _op_r_zero = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_r_zero;
        int32_t _op_r_span = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_r_span;
        int32_t _op_c_zero = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_zero;
        int32_t _op_c_span = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_span;
        int32_t output_f =map(_op_r_zero,_op_r_zero,_op_r_span,_op_c_zero,_op_c_span);
        // if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==0)
          // ledcWrite(_obj_variable.led_1, output_f);  
        // else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==1)
          // ledcWrite(_obj_variable.led_2, output_f);
        // else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==2)
          // ledcWrite(_obj_variable.led_3, output_f);
        //Serial.println("OP_L COUNT : " + _obj_variable.display_value + " | Index : " + String(_obj_variable._lst_display_menu_screen_cali.get(_obj_variable.scren_index)->_index));
      }
      else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._display_screen==MODE_CALIBRATION_CAL_OUTPUT_SPAN)
      {
        //_obj_variable.display_value = 4000;
        _obj_variable.display_value = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_span;  
        int32_t _op_r_zero = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_r_zero;
        int32_t _op_r_span = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_r_span;
        int32_t _op_c_zero = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_zero;
        int32_t _op_c_span = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_span;
        int32_t output_f =map(_op_r_span,_op_r_zero,_op_r_span,_op_c_zero,_op_c_span);
        // if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==0)
          // ledcWrite(_obj_variable.led_1, output_f);  
        // else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==1)
          // ledcWrite(_obj_variable.led_2, output_f);
        // else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==2)
          // ledcWrite(_obj_variable.led_3, output_f);
        //Serial.println("OP_H COUNT : " + _obj_variable.display_value + " | Index : " + String(_obj_variable._lst_display_menu_screen_cali.get(_obj_variable.scren_index)->_index));
        //Serial.println(String(_obj_variable.display_value));
      }      
      
      else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._display_screen==MODE_NONE)
        _obj_variable.display_value = _obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._name;
       
      if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._d_type == DTYPE_NUMBER)
        _obj_variable.display_value = CheckStringLength(_obj_variable.display_value, 4, "0");
      _obj_variable.onetime_write = false;
      _obj_variable.cursor_position = 3;
    }

    if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._display_screen==MODE_CALIBRATION_CAL_ZERO || _obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._display_screen==MODE_CALIBRATION_CAL_SPAN)
    {
      if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index < TOTALCHANNEL)
      {
        _obj_variable.display_decimal = 0;        
        //_obj_variable.display_value = _obj_variable._lst_ads_info.get(0)->_slave[0];
        for (int i = 0; i < I2C_DATA_SIZE; i++)
        {
          if (_obj_variable._lst_i2c_data_info[i]._status)
          {
	          if (_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._input_type == _obj_variable._lst_i2c_data_info[i]._id)
              _obj_variable.display_value = String(_obj_variable._lst_i2c_data_info[i]._lst_count[_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._sensor_no]);
          }
        }
      }
    }
    

    value_to_data_type_display_value_cali(_obj_variable.scren_index);
    

    if (_obj_variable.logout_time + _obj_variable.currentMillis <= millis())
      _obj_variable._obj_display_screen = DESHBOARD;
  
    if (!_obj_variable.is_key1_press && !_obj_variable.is_key2_press && !_obj_variable.is_key3_press && !_obj_variable.is_key4_press)
    {
      if (_obj_variable._key_press_1)
      {
        key_reset();
        //Serial.println("_key_press_1 scren_index : " + String(_obj_variable.scren_index));
        if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._display_screen!=MODE_NONE)
        {
          if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._d_type == DTYPE_NUMBER)
          {
            //if(_obj_variable.scren_index==5)
            if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._display_screen == MODE_CALIBRATION_CAL_OUTPUT_ZERO)
            {
              if(_obj_variable.is_negative)
                _obj_variable.display_value = String(_obj_variable.display_value.toInt()-5);
              else
                _obj_variable.display_value = String(_obj_variable.display_value.toInt()+5);

              int32_t _op_r_zero = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_r_zero;
              int32_t _op_r_span = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_r_span;
              int32_t _op_c_zero = _obj_variable.display_value.toInt();
              int32_t _op_c_span = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_span;
              int32_t output_f =map(_op_r_zero,_op_r_zero,_op_r_span,_op_c_zero,_op_c_span);
              //// ledcWrite(ledChannel, output_f);
              // if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==0)
                // ledcWrite(_obj_variable.led_1, output_f);  
              // else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==1)
                // ledcWrite(_obj_variable.led_2, output_f);
              // else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==2)
                // ledcWrite(_obj_variable.led_3, output_f); 
            }
            //else if(_obj_variable.scren_index==7)
            else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._display_screen == MODE_CALIBRATION_CAL_OUTPUT_SPAN)
            {
              if(_obj_variable.is_negative)
                _obj_variable.display_value = String(_obj_variable.display_value.toInt()-5);
              else
                _obj_variable.display_value = String(_obj_variable.display_value.toInt()+5);

              int32_t _op_r_zero = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_r_zero;
              int32_t _op_r_span = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_r_span;
              int32_t _op_c_zero = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_zero;
              int32_t _op_c_span = _obj_variable.display_value.toInt();
              int32_t output_f =map(_op_r_span,_op_r_zero,_op_r_span,_op_c_zero,_op_c_span);
              //// ledcWrite(ledChannel, output_f);
              // if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==0)
                // ledcWrite(_obj_variable.led_1, output_f);  
              // else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==1)
                // ledcWrite(_obj_variable.led_2, output_f);
              // else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==2)
                // ledcWrite(_obj_variable.led_3, output_f);
            }
            // else if(_obj_variable.scren_index==9)
            // {
            //   if(_obj_variable.is_negative)
            //     _obj_variable.display_value = String(_obj_variable.display_value.toInt()-1);
            //   else
            //     _obj_variable.display_value = String(_obj_variable.display_value.toInt()+1);
            // }
            _obj_variable.display_value = (incrimentel(_obj_variable.display_value, 0, 0, 1, 0, 0));
          }
        }
      }
      if (_obj_variable._key_press_2)
      {
        key_reset();
        // Serial.println("_key_press_2 scren_index : " + String(_obj_variable.scren_index));
        if(_obj_variable.scren_index==0 || _obj_variable.scren_index==2 || _obj_variable.scren_index==4 || _obj_variable.scren_index==6 || _obj_variable.scren_index==8|| _obj_variable.scren_index==10|| _obj_variable.scren_index==12|| _obj_variable.scren_index==14|| _obj_variable.scren_index==16|| _obj_variable.scren_index==18|| _obj_variable.scren_index==20|| _obj_variable.scren_index==22|| _obj_variable.scren_index==24|| _obj_variable.scren_index==26|| _obj_variable.scren_index==28|| _obj_variable.scren_index==30)
        _obj_variable.scren_index+=2;
        else
        _obj_variable.scren_index++;
        if(!_obj_variable.isPassword && _obj_variable.scren_index>= DISPLAY_MENU_SCREEN_CALI_SIZE-12)
          _obj_variable.scren_index=0;
        if (_obj_variable.scren_index >= DISPLAY_MENU_SCREEN_CALI_SIZE)
          _obj_variable.scren_index = 0;  

        _obj_variable.onetime_write = true;
      }
      if (_obj_variable._key_press_3)
      {
        key_reset();  

        if(_obj_variable.scren_index==0 || _obj_variable.scren_index==2 || _obj_variable.scren_index==4 || _obj_variable.scren_index==6 || _obj_variable.scren_index==8|| _obj_variable.scren_index==10|| _obj_variable.scren_index==12|| _obj_variable.scren_index==14|| _obj_variable.scren_index==16|| _obj_variable.scren_index==18|| _obj_variable.scren_index==20|| _obj_variable.scren_index==22|| _obj_variable.scren_index==24|| _obj_variable.scren_index==26|| _obj_variable.scren_index==28|| _obj_variable.scren_index==30)
        {
          _obj_variable.scren_index++;
        
        // if(!_obj_variable.isPassword && _obj_variable.scren_index>=DISPLAY_MENU_SCREEN_CALI_SIZE-6)
        //   _obj_variable.scren_index=0;
        // if (_obj_variable.scren_index >= DISPLAY_MENU_SCREEN_CALI_SIZE)
        //   _obj_variable.scren_index = 0;  
        //_obj_variable._obj_display_screen = _obj_variable._lst_display_menu_screen_setup.get(scren_index)->_display_screen;
          _obj_variable.onetime_write = true;
        }
        if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._display_screen==MODE_CALIBRATION_CAL_ZERO)
        {
        if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index < TOTALCHANNEL && !_obj_variable.onetime_write)
        {
          if(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._c_zero != _obj_variable.display_value.toInt())
          {
            _obj_variable._obj_display.w_display_segment("----", 0, "    ", 0, "    ", 0, "    ", 0);
            _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._c_zero = _obj_variable.display_value.toInt();  
            if(_obj_variable.isPassword)
            {
               //_obj_variable._lst_fcount.get(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._input_type)->_c_span = _obj_variable.display_value.toInt();
               //set_fcount();
            }
            Configuration_WriteEEPROM_AllTime();
            modebus_conf_write_onetime();
            delay(2);
          }
        }
        // else
        //   Serial.println(" | OutOfSize : " + String(TOTALCHANNEL));
      }
        else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._display_screen==MODE_CALIBRATION_CAL_SPAN)
        {
          if(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._c_span != _obj_variable.display_value.toInt() && !_obj_variable.onetime_write)
          {
            _obj_variable._obj_display.w_display_segment("----", 0, "    ", 0, "    ", 0, "    ", 0);
            _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._c_span = _obj_variable.display_value.toInt();  
            if(_obj_variable.isPassword)
            {
               //_obj_variable._lst_fcount.get(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._input_type)->_c_span = _obj_variable.display_value.toInt();
               //set_fcount();
            }
            Configuration_WriteEEPROM_AllTime();
            modebus_conf_write_onetime();
            delay(2);
          }
        }
        else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._display_screen==MODE_CALIBRATION_CAL_OUTPUT_ZERO)
        {
          if(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_zero != _obj_variable.display_value.toInt() && !_obj_variable.onetime_write)
          {
            //Serial.println("Save _op_c_zero : " + String(_obj_variable.onetime_write));.
            
            _obj_variable._obj_display.w_display_segment("----", 0, "    ", 0, "    ", 0, "    ", 0);
            _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_zero = _obj_variable.display_value.toInt();  
            Configuration_WriteEEPROM_AllTime();
            modebus_conf_write_onetime();
            int32_t _op_r_zero = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_r_zero;
            int32_t _op_r_span = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_r_span;
            int32_t _op_c_zero = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_zero;
            int32_t _op_c_span = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_span;
            int32_t output_f =map(_op_r_zero,_op_r_zero,_op_r_span,_op_c_zero,_op_c_span);
            //// ledcWrite(ledChannel, output_f);
            // if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==0)
              // ledcWrite(_obj_variable.led_1, output_f);  
            // else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==1)
              // ledcWrite(_obj_variable.led_2, output_f);
            // else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==1)
              // ledcWrite(_obj_variable.led_3, output_f);
            
            Configuration_WriteEEPROM_AllTime();
            modebus_conf_write_onetime();
            delay(2);
          }    
        }
        else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._display_screen==MODE_CALIBRATION_CAL_OUTPUT_SPAN)
        {
          if(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_span != _obj_variable.display_value.toInt() && !_obj_variable.onetime_write)
          {
            //Serial.println("Save _op_c_span : " + String(_obj_variable.onetime_write));
            _obj_variable._obj_display.w_display_segment("----", 0, "    ", 0, "    ", 0, "    ", 0);
            _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_span = _obj_variable.display_value.toInt();  
            Configuration_WriteEEPROM_AllTime();
            modebus_conf_write_onetime();
            int32_t _op_r_zero = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_r_zero;
            int32_t _op_r_span = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_r_span;
            int32_t _op_c_zero = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_zero;
            int32_t _op_c_span = _obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index]._op_c_span;
            int32_t output_f =map(_op_r_span,_op_r_zero,_op_r_span,_op_c_zero,_op_c_span);
            // if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==0)
              // ledcWrite(_obj_variable.led_1, output_f);  
            // else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==1)
              // ledcWrite(_obj_variable.led_2, output_f);
            // else if(_obj_variable._lst_display_menu_screen_cali[_obj_variable.scren_index]._index==1)
              // ledcWrite(_obj_variable.led_3, output_f);
            
            Configuration_WriteEEPROM_AllTime();
            modebus_conf_write_onetime();
            delay(2);
          }
          //Serial.println(String(_obj_variable.display_value));
        }
      }
      if (_obj_variable._key_press_4)
      {
        key_reset();
        _obj_variable._obj_display_screen = SETTING;
        _obj_variable.onetime_write = true;
      }
    }
  
    if(_obj_variable.is_key1_press)
    {
      long diff = millis()-_obj_variable.millis_is_negative;
      if(diff>=5000)
      {
        if(!_obj_variable.is_negative)
        {
          //Serial.println("isNegative Start");
          _obj_variable._obj_display.w_display_segment("----", 0, "    ", 0, "    ", 0, "    ", 0);            
          _obj_variable.is_negative=true;
          _obj_variable.millis_is_negative = millis();
          delay(2);
        }
        else
        {
          //Serial.println("isPositive Start");
          _obj_variable._obj_display.w_display_segment("----", 0, "    ", 0, "    ", 0, "    ", 0);            
          _obj_variable.is_negative=false;
          _obj_variable.millis_is_negative = millis();
          delay(2);
        }
      }
    }
    else
      _obj_variable.millis_is_negative = millis();
  }
  else if (_obj_variable._obj_display_screen == MODE_PASSOWRD)
  {
    //data_type _dtype=DTYPE_NONE;
    if (_obj_variable.onetime_write)
    {
      if(_obj_variable._lst_display_menu_screen_chag[_obj_variable.scren_index]._display_screen==MODE_PASS_CHANGE_OLD)
      {
        _obj_variable.display_decimal = 0;
        _obj_variable.display_value = "0000";  
        // Serial.println("OLD_PASS");
      }
      else if(_obj_variable._lst_display_menu_screen_chag[_obj_variable.scren_index]._display_screen==MODE_PASS_CHANGE_NEW)
      {
        _obj_variable.display_decimal = 0;
        _obj_variable.display_value = "0000";
        // Serial.println("NEW_PASS");
      }
      else if(_obj_variable._lst_display_menu_screen_chag[_obj_variable.scren_index]._display_screen==MODE_NONE)
      _obj_variable.display_value = _obj_variable._lst_display_menu_screen_chag[_obj_variable.scren_index]._name;

      _obj_variable.onetime_write = false;
      _obj_variable.cursor_position = 3;
    }
    
    value_to_data_type_display_value_chang(_obj_variable.scren_index);

    if (_obj_variable.logout_time + _obj_variable.currentMillis <= millis())
      _obj_variable._obj_display_screen = DESHBOARD;
  
    if (!_obj_variable.is_key1_press && !_obj_variable.is_key2_press && !_obj_variable.is_key3_press && !_obj_variable.is_key4_press)
    {
      if (_obj_variable._key_press_1)
      {
        key_reset();
        if(_obj_variable._lst_display_menu_screen_chag[_obj_variable.scren_index]._display_screen!=MODE_NONE)
        {
          if(_obj_variable._lst_display_menu_screen_chag[_obj_variable.scren_index]._d_type == DTYPE_NUMBER)
            _obj_variable.display_value = (incrimentel(_obj_variable.display_value, 0, 1, 1, 0, 0));
        }
      }
      if (_obj_variable._key_press_2)
      {
        key_reset();
        if(_obj_variable._lst_display_menu_screen_chag[_obj_variable.scren_index]._display_screen!=MODE_NONE)
        {
          _obj_variable.display_value = incrimentel(_obj_variable.display_value, 1, 0, 1, 0, 0);
          if(_obj_variable._lst_display_menu_screen_chag[_obj_variable.scren_index]._d_type == DTYPE_NUMBER) { }
        }
      }
      if (_obj_variable._key_press_3)
      {
        key_reset(); 
        // Serial.println(String(_obj_variable._lst_display_menu_screen_chag[_obj_variable.scren_index]._display_screen) + " | " + String(_obj_variable.scren_index));
        if(_obj_variable._lst_display_menu_screen_chag[_obj_variable.scren_index]._display_screen==MODE_PASS_CHANGE_OLD)
        {
          //Serial.println("Pass : " + String(_obj_variable.screen_password_default) + " | " + _obj_variable.display_value);
          // Serial.println(String("OLD_PASS" ));
          if( _obj_variable.screen_password_default == _obj_variable.display_value.toInt())
          {
            // Serial.println("_obj_variable.is_screen_password_default");
            _obj_variable.is_screen_password_default=true;
          }
          else if( _obj_variable.screen_password == _obj_variable.display_value.toInt())
          {
            // Serial.println("_obj_variable.is_screen_password");
            _obj_variable.is_screen_password=true;
          }
        }
        else if(_obj_variable._lst_display_menu_screen_chag[_obj_variable.scren_index]._display_screen==MODE_PASS_CHANGE_NEW)
        {
          // Serial.println(String("NEW_PASS" ));
          if(_obj_variable.is_screen_password_default)
          {
            _obj_variable.screen_password_default = _obj_variable.display_value.toInt();
            _obj_variable._lst_password_info[0]._pass = _obj_variable.screen_password_default;
            Configuration_WriteEEPROM_AllTime();
          }
          else if(_obj_variable.is_screen_password)
          {
            _obj_variable.screen_password = _obj_variable.display_value.toInt();
            _obj_variable._lst_password_info[1]._pass = _obj_variable.screen_password;
            Configuration_WriteEEPROM_AllTime();            
          }
        }

        _obj_variable.scren_index++;
        if(!_obj_variable.isPassword && _obj_variable.scren_index >= DISPLAY_MENU_SCREEN_CHANG_SIZE)
          _obj_variable.scren_index=0;
        if (_obj_variable.scren_index >= DISPLAY_MENU_SCREEN_CHANG_SIZE)
          _obj_variable.scren_index = 0;  
        
        _obj_variable.onetime_write = true;
      }
      if (_obj_variable._key_press_4)
      {
        key_reset();
        _obj_variable._obj_display_screen = SETTING;
        _obj_variable.onetime_write = true;
      }
    }
  }
  else if (_obj_variable._obj_display_screen == MODE_BUZZ)
  {
    //data_type _dtype=DTYPE_NONE;
    if (_obj_variable.onetime_write)
    {
      if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._display_screen==MODE_BUZZ_STATUS)
      {
        if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._index < TOTALCHANNEL)
        {
          _obj_variable.display_decimal = 0;
          _obj_variable.display_value = _obj_variable._lst_other_info[_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._index]._value;  
        }
      }
      else if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._display_screen==MODE_BUZZ_FREQUENCY)
      {
        _obj_variable.display_decimal = 0;
        _obj_variable.display_value = _obj_variable._lst_other_info[_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._index]._value;  
      }
      else if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._display_screen==MODE_NONE)
        _obj_variable.display_value = _obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._name;
      
      // Serial.println("Buzzer"+String(_obj_variable.display_value));
      _obj_variable.onetime_write = false;
      _obj_variable.cursor_position = 3;
    }
    
    value_to_data_type_display_value_buzz(_obj_variable.scren_index);

    if (_obj_variable.logout_time + _obj_variable.currentMillis <= millis())
      _obj_variable._obj_display_screen = DESHBOARD;
  
    if (!_obj_variable.is_key1_press && !_obj_variable.is_key2_press && !_obj_variable.is_key3_press && !_obj_variable.is_key4_press)
    {
      if (_obj_variable._key_press_1)
      {
        key_reset();
        if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._display_screen!=MODE_NONE)
        {
          if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._d_type == DTYPE_NUMBER)
            _obj_variable.display_value = (incrimentel(_obj_variable.display_value, 0, 1, 1, 0, 0));
        }
      }
      if (_obj_variable._key_press_2)
      {
        key_reset();
        if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._display_screen!=MODE_NONE)
        {
          if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._d_type == DTYPE_INPUT_TYPE)
          _obj_variable.display_value = String((_obj_variable.display_value.toInt()+1));
          else
          _obj_variable.display_value = incrimentel(_obj_variable.display_value, 1, 0, 1, 0, 0);
          //Serial.println("display_value : " + String(_obj_variable.display_value));
          if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._d_type == DTYPE_NONE) { }
          else if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._d_type == DTYPE_EN_DIS) {if (_obj_variable.display_value.toInt() > 1) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._d_type == DTYPE_YES_NO) { if (_obj_variable.display_value.toInt() > 1) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._d_type == DTYPE_INPUT_TYPE) { if (_obj_variable.display_value.toInt() > 13) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._d_type == DTYPE_PARITY) { if (_obj_variable.display_value.toInt() > 2) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._d_type == DTYPE_BUADRATE) { if (_obj_variable.display_value.toInt() > 6) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._d_type == DTYPE_NUMBER) { }
          else if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._d_type == DTYPE_ALARM_TYPE) { if (_obj_variable.display_value.toInt() > 3) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._d_type == DTYPE_ALARM_ON_OFF) { if (_obj_variable.display_value.toInt() > 1) _obj_variable.display_value = String(0); }
          else if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._d_type == DTYPE_CTYP) { if (_obj_variable.display_value.toInt() > 1) _obj_variable.display_value = String(0); }
        }
      }
      if (_obj_variable._key_press_3)
      {
        key_reset(); 

        if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._display_screen==MODE_BUZZ_STATUS)
        {
          if(_obj_variable._lst_other_info[_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._index]._value != _obj_variable.display_value.toInt())
          {
            _obj_variable._lst_other_info[_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._index]._value = _obj_variable.display_value.toInt();  
            Configuration_WriteEEPROM_AllTime();
            // modebus_conf_write_onetime();
            // Serial.println(_obj_variable._lst_parameter_info[_obj_variable._lst_display_menu_screen_setup[_obj_variable.scren_index]._index]._name + " : r_zero :" + String(_obj_variable.display_value));
          }
          
        }
        else if(_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._display_screen==MODE_BUZZ_FREQUENCY)
        {
          if(_obj_variable._lst_other_info[_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._index]._value != _obj_variable.display_value.toInt())
          {
            _obj_variable._lst_other_info[_obj_variable._lst_display_menu_screen_buzz[_obj_variable.scren_index]._index]._value = _obj_variable.display_value.toInt();  
            Configuration_WriteEEPROM_AllTime();
            // modebus_conf_write_onetime();
          }
        }

        _obj_variable.scren_index++;
        if(!_obj_variable.isPassword && _obj_variable.scren_index>=DISPLAY_MENU_SCREEN_BUZZ_SIZE)
          _obj_variable.scren_index=0;
        if (_obj_variable.scren_index >= DISPLAY_MENU_SCREEN_BUZZ_SIZE)
          _obj_variable.scren_index = 0;  
        
        _obj_variable.onetime_write = true;
      }
      if (_obj_variable._key_press_4)
      {
        key_reset();
        _obj_variable._obj_display_screen = SETTING;
        _obj_variable.onetime_write = true;
      }
    }
  }
  else if (_obj_variable._obj_display_screen == UPDATE)
  {
    // if(_obj_variable.onetime_write)
    {
      _obj_variable._obj_display.w_display_segment(update, 0, "    ",0, "    ", 0, "    ", 0);
      _obj_variable.onetime_write= false;
    }
     
   
    if (!_obj_variable.is_key1_press && !_obj_variable.is_key2_press && !_obj_variable.is_key3_press && !_obj_variable.is_key4_press)
    {
      if (_obj_variable._key_press_1)
      {
        key_reset();
        // _obj_variable._obj_display_screen = DESHBOARD;
        // _obj_variable.isAccessPointStart=false;
        // _obj_variable.onetime_write = true;
      }
      if (_obj_variable._key_press_2)
      {
        key_reset();
        // _obj_variable._obj_display_screen = DESHBOARD;
        // _obj_variable.isAccessPointStart=false;
        // _obj_variable.onetime_write = true;
       
      }
      if (_obj_variable._key_press_3)
      {
        key_reset();
        _obj_variable._obj_display_screen = DESHBOARD;
        _obj_variable.isAccessPointStart=false;
        _obj_variable.isWifiAccessPort = false;
        _obj_variable.onetime_write = true;
        WiFi.softAPdisconnect(true);
      
      }
      if (_obj_variable._key_press_4)
      {
        key_reset();
        _obj_variable._obj_display_screen = DESHBOARD;
        _obj_variable.isAccessPointStart=false;
        _obj_variable.isWifiAccessPort = false;
        _obj_variable.onetime_write = true;
        WiFi.softAPdisconnect(true);
      }
    }
     
  }
   
  for (int i = 0; i < TOTALCHANNEL; i++)
  {
    // if (_obj_variable._lst_parameter_info[i]._disp_status)
    {
      if(i==0)
      {
        _obj_variable.holdingRegs[0] = _obj_variable._lst_parameter_info[0]._pv;
        //_obj_variable.holdingRegs[1] = _obj_variable._lst_parameter_info[0]._decimal;
      }
      else if(i==1)
      {
        _obj_variable.holdingRegs[2] = _obj_variable._lst_parameter_info[1]._pv;
        //_obj_variable.holdingRegs[3] = _obj_variable._lst_parameter_info[1]._decimal;
      }
      else if(i==2)
      {
        _obj_variable.holdingRegs[4] = _obj_variable._lst_parameter_info[2]._pv;
        //_obj_variable.holdingRegs[5] = _obj_variable._lst_parameter_info[2]._decimal;
      }
      
      int pcount = 0, val = 0;
      for (int j = 0; j < (TOTALCHANNEL * PER_CH_ALARM); j++)
      {
        if (_obj_variable._lst_parameter_info[i]._id == _obj_variable._lst_parameter_alarm_info[j]._para_id)
        {
          if (_obj_variable._lst_parameter_alarm_info[j]._status)
          {
            val += (_obj_variable._lst_parameter_alarm_info[j]._a_log * pow(10, pcount));
            pcount++;
          }
        }
      }      
      
      if (i == 0)
        _obj_variable.holdingRegs[18] = val;
      else if (i == 1)
        _obj_variable.holdingRegs[19] = val;
      else if (i == 2)
        _obj_variable.holdingRegs[20] = val;
    }

    // if(_obj_variable._obj_display_screen != MODE_CALIBRATION)
    // {
    //       int32_t _op_r_zero = _obj_variable._lst_parameter_info[i]._op_r_zero;
    //       int32_t _op_r_span = _obj_variable._lst_parameter_info[i]._op_r_span;
    //       int32_t _op_c_zero = _obj_variable._lst_parameter_info[i]._op_c_zero;
    //       int32_t _op_c_span = _obj_variable._lst_parameter_info[i]._op_c_span;
    //       int32_t _pv=_obj_variable._lst_parameter_info[i]._pv;
    //       if(i==0)
    //         _obj_variable.holdingRegs[0] = _obj_variable._lst_parameter_info[i]._pv;
    //       else if(i==1)
    //         _obj_variable.holdingRegs[1] = _obj_variable._lst_parameter_info[i]._pv;
    //       else if(i==2)
    //       {  _obj_variable.holdingRegs[2] = _obj_variable._lst_parameter_info[i]._pv;
    //           //Serial.println("_pv : " + String(_pv) + " | _op_r_zero : " + String(_op_r_zero) + " | _op_r_span : " + String(_op_r_span) + " | _op_c_zero : " + String(_op_c_zero)+ " | _op_c_span : " + String(_op_c_span));        
    //       }
    //       if(_op_r_zero>_pv)
    //       {
    //         //Serial.println(_obj_variable._lst_parameter_info.get(i)->_name + "Below OP Zero");
    //         // if(i==0)
    //           // ledcWrite(_obj_variable.led_1, _op_c_zero+1500);  
    //         // else if(i==1)
    //           // ledcWrite(_obj_variable.led_2, _op_c_zero+1500);
    //         // else if(i==2)
    //           // ledcWrite(_obj_variable.led_3, _op_c_zero+1500);
    //       }
    //       else if(_op_r_span<_pv)
    //       {
    //         //Serial.println(_obj_variable._lst_parameter_info.get(i)->_name + "Above OP Span");
    //         // if(i==0)
    //           // ledcWrite(_obj_variable.led_1, _op_c_span-1600);  
    //         // else if(i==1)
    //           // ledcWrite(_obj_variable.led_2, _op_c_span-1600);
    //         // else if(i==2)
    //           // ledcWrite(_obj_variable.led_3, _op_c_span-1600);
    //       }
    //       else
    //       {
            
    //         int32_t output_f = map(_pv,_op_r_zero,_op_r_span,_op_c_zero,_op_c_span);
    //         // if(i==0)
    //           // ledcWrite(_obj_variable.led_1, output_f);  
    //         // else if(i==1)
    //           // ledcWrite(_obj_variable.led_2, output_f);
    //         // else if(i==2)
    //         {
    //           // ledcWrite(_obj_variable.led_3, output_f);
    //           //Serial.print(" | output_f : " + String(output_f));
    //         }
    //       }
    //       //Serial.println();


    // }
  }

  _obj_variable.holdingRegs[22] = second() + (minute() * 100);
  _obj_variable.holdingRegs[23] = hour() + (day() * 100);
  _obj_variable.holdingRegs[24] = month() + (year() - 2000) * 100;


  _obj_variable.isI2C_DESIPLAYTask = false;
  //vTaskDelete( NULL );
 
}

#pragma endregion

void setup()
{
  Serial.begin(115200);
  if(!EEPROM.begin(EEPROM_SIZE))
    Serial.println("EEPROM READ FAIL");

  // esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  // esp_task_wdt_add(NULL); //add current thread to WDT watch
  //_obj_variable.holdingRegs = _obj_variable.holdingRegs[_obj_variable.total_registor];
  pinMode(buzz_pin, OUTPUT);

  pinMode(KEY1, INPUT);
  pinMode(KEY2, INPUT);
  pinMode(KEY3, INPUT);
  pinMode(KEY4, INPUT);
  pinMode(door_status_pin, INPUT);
  
  // pinMode(_obj_variable.led_1_pin,OUTPUT);
  // digitalWrite(_obj_variable.led_1_pin,1);  
  // pinMode(_obj_variable.led_2_pin,OUTPUT);
  // digitalWrite(_obj_variable.led_2_pin,1);
  // pinMode(_obj_variable.led_3_pin,OUTPUT);
  // digitalWrite(_obj_variable.led_3_pin,1);  
  // //delay(1000);
  // ledcSetup(_obj_variable.led_1, _obj_variable.freq, _obj_variable.resolution);
  // ledcAttachPin(_obj_variable.led_1_pin, _obj_variable.led_1);
  // //delay(1000);
  // ledcSetup(_obj_variable.led_2, _obj_variable.freq, _obj_variable.resolution);
  // ledcAttachPin(_obj_variable.led_2_pin, _obj_variable.led_2);
  // //delay(1000);
  // ledcSetup(_obj_variable.led_3, _obj_variable.freq, _obj_variable.resolution);
  // ledcAttachPin(_obj_variable.led_3_pin, _obj_variable.led_3);

  // ledcWrite(_obj_variable.led_1, 28000);  
  // ledcWrite(_obj_variable.led_2, 28000);
  // ledcWrite(_obj_variable.led_3, 28000);
  
  
  
  digitalWrite(buzz_pin, 0);
  _obj_variable._obj_display.begin();
  _obj_variable._obj_display.w_display_segment("TEMP", 0, "  RH", 0, "  DP", 0, "TIME", 0);
  
  //SPIFFS.begin(true);

  _obj_variable._lst_parameter_info = (PARAMETER_INFO_STRUCT*)malloc(TOTALCHANNEL * sizeof(PARAMETER_INFO_STRUCT));  
  _obj_variable._lst_parameter_alarm_info = (PARAMETER_ALARM_INFO_STRUCT*)malloc(TOTALCHANNEL * PER_CH_ALARM * sizeof(PARAMETER_ALARM_INFO_STRUCT));  
  _obj_variable._lst_communication = (COMMUNICATION_INFO_STRUCT*)malloc(COMMUNICATION_SIZE * sizeof(COMMUNICATION_INFO_STRUCT));
  _obj_variable._lst_i2c_data_info = (I2C_DATA_INFO_STRUCT*)malloc(8 * sizeof(I2C_DATA_INFO_STRUCT));
  _obj_variable._lst_password_info = (PASSWORD_INFO_STRUCT*)malloc(5 * sizeof(PASSWORD_INFO_STRUCT));
  _obj_variable._lst_other_info = (OTHER_INFO_STRUCT*)malloc(7 * sizeof(OTHER_INFO_STRUCT));
   
  _obj_variable._lst_display_menu_screen_config = (DISPLAY_MENU_SCREEN_STRUCT*)malloc(80 * sizeof(DISPLAY_MENU_SCREEN_STRUCT));
  _obj_variable._lst_display_menu_screen_setup  = (DISPLAY_MENU_SCREEN_STRUCT*)malloc(36 * sizeof(DISPLAY_MENU_SCREEN_STRUCT));
  _obj_variable._lst_display_menu_screen_cali   = (DISPLAY_MENU_SCREEN_STRUCT*)malloc(24 * sizeof(DISPLAY_MENU_SCREEN_STRUCT));
  _obj_variable._lst_display_menu_screen_chag   = (DISPLAY_MENU_SCREEN_STRUCT*)malloc(4  * sizeof(DISPLAY_MENU_SCREEN_STRUCT));
  _obj_variable._lst_display_menu_screen_buzz   = (DISPLAY_MENU_SCREEN_STRUCT*)malloc(4  * sizeof(DISPLAY_MENU_SCREEN_STRUCT));
   
  int write_eeprom = EEPROM.read(0); 
  if(write_eeprom != 135 )
  {
    
    Configuration_WriteEEPROM();
    EEPROM.writeInt(1,_obj_variable._memory_address);
    EEPROM.commit();
    EEPROM.writeInt(10,_obj_variable._last_read_memory_address);
    EEPROM.commit();
    EEPROM.writeInt(20,_obj_variable.loginterval);
    EEPROM.commit();
    EEPROM.write(0,135);
    EEPROM.commit();
    Serial.println("EEPROM_WRITE");
  }
  
  _obj_variable._memory_address = EEPROM.readInt(1);      
  _obj_variable._last_read_memory_address = EEPROM.readInt(10);
  _obj_variable.loginterval = EEPROM.readInt(20);

  Configuration_ReadEEPROM();

  get_i2c_data_info();
  
  menu_generate();

  _obj_variable.isSPIMemoryInitialize = _obj_variable.flash.begin(MB(16));
  if(!_obj_variable.isSPIMemoryInitialize)
  {
      _obj_variable.isSPIMemoryInitialize = _obj_variable.flash.begin(MB(16));
  }
  Serial.println("_obj_variable.isSPIMemoryInitialize : " + String(_obj_variable.isSPIMemoryInitialize));
  int recordstatus = ((_obj_variable._memory_address - _obj_variable._last_read_memory_address)/13);
  if(recordstatus>0)
    read_memory();
 
  int door_panding_record_status=0;  
  //Serial.println("recordstatus : " + String(recordstatus));
  if(recordstatus>0)
  {
    door_panding_record_status = 0 +(10*1);
    //Serial.println("door_panding_record_status : " + String(door_panding_record_status) + " | "+String(_obj_variable.door_status));
  }
  else
  {
    door_panding_record_status = 0 +(10*0);
    //Serial.println("else door_panding_record_status : " + String(door_panding_record_status) + " | "+String(_obj_variable.door_status));
  }
  _obj_variable.holdingRegs[1] = door_panding_record_status + ( _obj_variable._lst_parameter_info[0]._decimal* 100);
  _obj_variable.holdingRegs[3] = door_panding_record_status + ( _obj_variable._lst_parameter_info[1]._decimal* 100);
  _obj_variable.holdingRegs[5] = door_panding_record_status + ( _obj_variable._lst_parameter_info[1]._decimal* 100);

  for (int j = 0; j < I2C_DATA_SIZE ; j++)
  {
    int status = 0;
    for (int i = 0; i < TOTALCHANNEL ; i++)
    {
      if (_obj_variable._lst_parameter_info[i]._input_type == _obj_variable._lst_i2c_data_info[j]._id)
      {
        if(_obj_variable._lst_parameter_info[i]._disp_status == 1)
        {
          status = 1;            
        }
      }        
    }
    _obj_variable._lst_i2c_data_info[j]._status = status;
  }
        

  //_obj_variable._obj_modbus.modbus_configure(9600, 1, 0, 1, 22, _obj_variable.total_registor, 0);
  _obj_variable._obj_modbus.modbus_configure(_obj_variable.dtype_arr_buadrate[_obj_variable._lst_communication[1]._value].toInt(), _obj_variable._lst_communication[0]._value, _obj_variable._lst_communication[2]._value, _obj_variable._lst_communication[3]._value, 22, _obj_variable.total_registor, 0);
  //Serial.println(String(boudrate[_obj_variable._lst_communication.get(1)->_value])+"||"+ String(_obj_variable._lst_communication.get(0)->_value)+"||"+String(_obj_variable._lst_communication.get(2)->_value)+"||"+String(_obj_variable._lst_communication.get(3)->_value)+"||"+ String(_obj_variable.total_registor));
  _obj_variable._obj_display_screen = DESHBOARD; 

  modebus_conf_write_onetime();
  _obj_variable.i2c_data.tcaselect(2);
  _obj_variable._objRtc.start();

  i2c_read_sensor();
  comm_i2c_read_temp();
  // xTaskCreatePinnedToCore(comm_modbus_slave, "comm_modbus_slave", 2048, NULL, 1, &Task1, 0);
  if (_obj_variable._lst_other_info[2]._value == 1)
  {
    _obj_variable._lst_i2c_data_info[3]._status = true;
    Wire.begin(32,33);
    //_obj_variable._objRtc.set(0,28,12,10,12,21);
    if(_obj_variable._objRtc.get())
    {
      // Serial.println("Rtc Read Successfully");
      _obj_variable.RtcRunning=true; 
      
    }
    else
      _obj_variable.RtcRunning=false;
  }
  else
    _obj_variable._lst_i2c_data_info[3]._status = false;

  xTaskCreatePinnedToCore(comm_modbus_slave, "comm_modbus_slave", 2048, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
}

bool hooter_on = false;
bool buzz_togel = 0;

void leg_blinking()
{
  if (((millis() - blink_segment_millis) / 10) > blink_segment_time)
  {
    
    blink_segment = !blink_segment;
    blink_segment_millis = millis();
    
    //Alarm LED blink
    bool isBlink = false;
    for (int j = 0; j < (TOTALCHANNEL * PER_CH_ALARM); j++)
    {
      if (_obj_variable._lst_parameter_alarm_info[j]._status)
      {
        if (_obj_variable._lst_parameter_alarm_info[j]._is_led_status == LED_BLINK)
        {
          isBlink = true;
          _obj_variable._obj_display.is_LED[_obj_variable._lst_parameter_alarm_info[j]._display_no] = blink_segment;
          // Serial.println("Display_no" + String(_obj_variable._lst_parameter_alarm_info[j]._display_no)+ "   |  "+String( j)  );
        }
        else if (_obj_variable._lst_parameter_alarm_info[j]._is_led_status == LED_ON)
        {
          _obj_variable._obj_display.is_LED[_obj_variable._lst_parameter_alarm_info[j]._display_no] = 1;
        }
        else if (_obj_variable._lst_parameter_alarm_info[j]._is_led_status == LED_OFF)
        {
          _obj_variable._obj_display.is_LED[_obj_variable._lst_parameter_alarm_info[j]._display_no] = 0;
        }
        //Serial.print(_obj_variable._obj_modbus.someArray[_obj_variable._lst_parameter_alarm_info[j]._display_no]);
        //Serial.print(" | ");
      }
    }

    if (_obj_variable._led_ahu_off == LED_BLINK)
    {
      isBlink = true;
      _obj_variable._obj_display.is_LED[6] = blink_segment;
      Serial.println("AHU_OFF");
    }

    else if (_obj_variable._led_ahu_off == LED_ON)
      _obj_variable._obj_display.is_LED[6] = 1;
    else if (_obj_variable._led_ahu_off == LED_OFF)
      _obj_variable._obj_display.is_LED[6] = 0;

    if (_obj_variable._led_ahu_trip == LED_BLINK)
    {
      isBlink = true;
      _obj_variable._obj_display.is_LED[9] = blink_segment;
      //Serial.println("AHU_TRIP");
    }

    else if (_obj_variable._led_ahu_trip == LED_ON)
      _obj_variable._obj_display.is_LED[9] = 1;
    else if (_obj_variable._led_ahu_trip == LED_OFF)
      _obj_variable._obj_display.is_LED[9] = 0;

    

    hooter_on = isBlink;
  }
 
}

void data_store_check()
{
  unsigned long diff = (millis() - _obj_variable._obj_modbus.last_connected_millis) / 1000;
  //Serial.println("Log Interval : " + String(_obj_variable.loginterval) + " | diff : " + String(diff));
  if (diff >= _obj_variable.loginterval && _obj_variable.isStorage)
  {
    //Serial.println("Data Store To Memory 1 Min Interval");
    if (!_obj_variable._obj_modbus.isStore)
    {
      data_store(true,false);
      data_store(false,true);
      _obj_variable._obj_modbus.isStore = true;
    }
    else
      data_store(false,false);  
  }
  else
  {
    //Serial.println("Read Memory : " + String(_obj_variable.isMemoryRead));
    if(_obj_variable.isMemoryRead)
    {
      //Serial.println("read_memory");
      read_memory();
    } 
    if( _obj_variable.holdingRegs[87] == 1)
    {
      _obj_variable._memory_address = 0;
      _obj_variable._last_read_memory_address = 0;
      EEPROM.writeInt(1,_obj_variable._memory_address);
      EEPROM.commit();
      EEPROM.writeInt(10,_obj_variable._last_read_memory_address);
      EEPROM.commit();
      _obj_variable.holdingRegs[87] = 0;
    }
  }
}

void door_info()
{
  int door = digitalRead(door_status_pin);
  if(door==1)
    door=0;
  else
    door=1;
  // Serial.println("Door_Status   "+String (door));
  if(_obj_variable.e_door_status == 1 )
  {
    _obj_variable.door_status = door;
    
    _obj_variable.holdingRegs[49] = _obj_variable.door_status;
  }
  else 
  {
    _obj_variable.door_status=1;
    _obj_variable.holdingRegs[49]=0;
  }
 
  if(_obj_variable.door_status == 0 && ((millis() - door_open_millis)/1000)> _obj_variable._dorr_open_delay)
  {
    _obj_variable.is_dorr_open = true;
  }
  else if(_obj_variable.door_status == 1)
  {
    _obj_variable.is_dorr_open = false;
    door_open_millis =millis();
  }
}

void loop()
{
  if(_obj_variable.isWifiAccessPort)
  {
    server.handleClient();
  }
  else
  {
  
    _obj_variable.tm = DateTime(year(),month(),day(),hour(), minute(), second());
    
    // if (!_obj_variable.isMTask)
    // {
    //   _obj_variable.isMTask = true;
    //   xTaskCreate(comm_modbus_slave, "comm_modbus_slave", 2048, NULL, 1, NULL);
    // }
    //if (((millis() - i2cread_millis) / 1000) >= 1 && !_obj_variable.isI2CTask && !_obj_variable.isI2C_DESIPLAYTask)
    if (((millis() - i2cread_millis) / 1000) >= 1)
    {
      //_obj_variable.isI2CTask = true;
      //xTaskCreate(comm_i2c_read, "comm_i2c_read", 10000, NULL, 1, NULL);
      i2cread_millis = millis();
      i2c_read_sensor();
      comm_i2c_read_temp();
      // Serial.println(String(day())+ "/" + String(month()) + "/ " + String(year()) + "/ " + String(hour()) + "/ " + String(minute()) + "/ " + String(second()) );
    }
    
    door_info();
    data_store_check();
    leg_blinking();



    if ((((millis() - hooter_millis) / 10) > _obj_variable._lst_other_info[1]._value) && (_obj_variable._lst_other_info[0]._value && hooter_on))
    {
      digitalWrite(buzz_pin, buzz_togel);
      buzz_togel = !buzz_togel;
      hooter_millis = millis();
    }
    else if (!_obj_variable._lst_other_info[0]._value)
    {
      leg_blinking();
      buzz_togel = 0;
      digitalWrite(buzz_pin, buzz_togel);
      hooter_millis = millis();
    }
    else if (!hooter_on)
    {
      leg_blinking();
      buzz_togel = 0;
      digitalWrite(buzz_pin, buzz_togel);
      hooter_millis = millis();
    }
  }
  
  if (((millis() - i2c_d_read_millis) / 10) >= 5)
  {
    isWriteDisplay = true;
    _obj_variable.isI2C_DESIPLAYTask = true;
    display_7led_screen();
    //xTaskCreate(display_7led_screen, "display_7led_screen", 10000, NULL, 1, NULL);
    i2c_d_read_millis = millis();
  }

  
  delay(1);

}