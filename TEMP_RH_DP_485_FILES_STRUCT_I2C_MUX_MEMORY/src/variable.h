#ifndef variable_h
#define variable_h

#include "Arduino.h"
#include "i2c_sensor_trh_dp_read.h"
#include "unitech_4ch_segment.h"
#include "SimpleModbusSlave.h"
#include "EEPROM.h"
#include "SPI.h"
#include "SPIFFS.h"
#include "Adafruit_ADS1015.h"
#include "RTC.h"
#include "RTClib.h"

// #include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include "WebServer.h"
#include <ESPmDNS.h>
#include <Update.h>
#include <esp_task_wdt.h>
#include "SPIFlash.h"

#define WDT_TIMEOUT 10

#define KEY1 34
#define KEY2 35
#define KEY3 27//39
#define KEY4 26//36

#define buzz_pin 12
#define door_status_pin 13

#define TOTALCHANNEL 3
#define PER_CH_ALARM 2
#define COMMUNICATION_SIZE 4
#define PASSWORD_SIZE 5
#define I2C_DATA_SIZE 6
#define OTHER_SIZE 3

#define DISPLAY_MENU_SCREEN_CONFIG_SIZE 80
#define DISPLAY_MENU_SCREEN_SETUP_SIZE 36
#define DISPLAY_MENU_SCREEN_CALI_SIZE 24
#define DISPLAY_MENU_SCREEN_CHANG_SIZE 4
#define DISPLAY_MENU_SCREEN_BUZZ_SIZE 4

typedef struct parameter_info 
{
  byte _id = 0;
  char _name[10];
  int _count = 0;
  byte _unit_id=0;   
  byte _decimal=0;
  int16_t _offset=0;
  byte _f_demping=20;
  int16_t _pv=0;
  bool _is_calibration = false;
  int16_t _lst_demping[20];
  byte _sensor_no = 0;
  byte _input_type = 0;
  int16_t _r_zero = 0;
  int16_t _r_span = 100;
  int16_t _c_zero = 0;
  int16_t _c_span = 100; 
  int16_t _f_c_zero  = 0;
  int16_t _f_c_span  = 100;
  int16_t _sensor_count = 0;
  bool _is_sensor_connect=false;
  bool _is_under=false;
  bool _is_over=false;
  int _cali_offset = 0;
  int _disp_status = 1;
  bool _is_firsttime=true;
  int ip_pval=0;
  int ip_retry=0;
  int _is_negative=1;
  int _display_no=0;
  int16_t _op_r_zero = 0;
  int16_t _op_r_span = 1000;
  uint16_t _op_c_zero = 27000;
  uint16_t _op_c_span = 5900; 
} PARAMETER_INFO_STRUCT;

typedef struct parameter_info_reg
{
  int _r_zero = 0;
  int _r_span = 100;
  int _c_zero = 0;
  int _c_span = 100;
  int _c_set_reg = 100;    
}PARAMETER_INFO_REG_STRUCT;;

typedef struct password_info
{
  int _id = 0;
  char _name[10];
  int _pass = 0;
}PASSWORD_INFO_STRUCT;

typedef struct other_info
{
  int _id = 0;
  char _name[10];
  int _value = 0;
}OTHER_INFO_STRUCT;

enum led_status
{
    LED_BLINK,
    LED_ON,
    LED_OFF
};

typedef struct parameter_alarm_info
{
  
    int _id = 0;
    int _para_id = 0;
    char _name[10];
    int _type = 0; //0-Low,1-High
    int _value = 0;
    int _delay = 0;
    int _display_no = 0; //Light Indication No.
    int _a_log = 0;
    int _is_led_status ;
    unsigned long _logtime=0;
    
    int _status=false;
    bool _is_ack=true;
    
}PARAMETER_ALARM_INFO_STRUCT;

typedef struct parameter_alarm_info_reg
{
    int _id = 0;
    int _para_id = 0;
    int _name = 0;
    int _type = 0; 
    int _value = 0;
    int _delay = 0;
    int _status= 0;    
    int _display_no= 0;    
}PARAMETER_ALARM_INFO_REG_STRUCT;

typedef struct communication
{
  int _id=0;
  char _name[10];
  int _value=0;
  int _value_1 = 0;
}COMMUNICATION_INFO_STRUCT;

enum i2c_type
{
  I2C_TEMP_RH,
  I2C_DP,
  I2C_ADS_1115,
  I2C_RTC
};

typedef struct i2c_data_info
{
  public:
  int _id=0;
  char _name[10];
  int _type = 0;
  int _pin_sda=0;
  int _pin_scl=0;
  int _address=0;
  int _no_of_values=0;
  int _retry=0;
  int32_t _lst_value[5];
  int32_t _lst_count[5];
  int _status=false;
  bool _connected=false;
}I2C_DATA_INFO_STRUCT;

enum data_type
{
  DTYPE_NONE,
  DTYPE_EN_DIS,
  DTYPE_YES_NO,
  DTYPE_INPUT_TYPE,
  DTYPE_PARITY,
  DTYPE_BUADRATE,
  DTYPE_NUMBER,
  DTYPE_ALARM_TYPE,
  DTYPE_ALARM_ON_OFF,
  DTYPE_CTYP
};

enum display_screen
{
  DESHBOARD,
  PASSWORD,
  SETTING,
  MODE_CONFIG,  
  MODE_CONFIG_ALARM_STATUS,
  MODE_CONFIG_ALARM_DELAY_TIME,
  MODE_CONFIG_INP_DEMPING,
  MODE_CONFIG_INP_DECIMAL,
  MODE_CONFIG_INP_STATUS,
  MODE_CONFIG_INP_NONG,
  MODE_CONFIG_TIME,
  MODE_CONFIG_COMMUNICATION,
  MODE_CONFIG_OFFSET,

  MODE_SETUP,
  MODE_SETUP_ZERO,
  MODE_SETUP_SPAN,
  MODE_SETUP_ALARM_SETPOINT,
  MODE_SETUP_OUTPUT_ZERO,
  MODE_SETUP_OUTPUT_SPAN,

  MODE_CALIBRATION,
  MODE_CALIBRATION_CAL_ZERO,
  MODE_CALIBRATION_CAL_SPAN,
  MODE_CALIBRATION_CAL_OUTPUT_ZERO,
  MODE_CALIBRATION_CAL_OUTPUT_SPAN,
  MODE_CALIBRATION_CAL_OFFSET,
  
  MODE_NONE,

  MODE_PASSOWRD,
  MODE_PASS_CHANGE_OLD,
  MODE_PASS_CHANGE_NEW,

  MODE_BUZZ,
  MODE_BUZZ_STATUS,
  MODE_BUZZ_FREQUENCY,
  UPDATE
};

typedef struct  display_menu_screen
{
  int _id=0;
  char _name[10];
  byte _type=0;
  bool _is_password=0;
  int _ref_id=0;
  display_screen _display_screen=MODE_PASSOWRD;
  int _index=0;
  data_type _d_type = DTYPE_NONE;
} DISPLAY_MENU_SCREEN_STRUCT;

enum modbus_menu
{
  NONE,
  MODBUS_MENU_ALARM_TYPE,
  MODBUS_MENU_ALARM_DELAY,
  MODBUS_MENU_ALARM_STATUS
};

class variable
{
  public:
    //////Controller Pin Defind
    int _pin_i2c_sda_1 = 32;
    int _pin_i2c_sda_2 = 1;
    int _pin_i2c_sda_3 = 23;
    int _pin_i2c_scl = 33;
    int _pin_analog_in_1 = 36;
    int _pin_analog_in_2 = 39;
    int _pin_analog_in_3 = 34;

    //////Communication_Parameter
    // LinkedList<communication*> _lst_communication=LinkedList<communication*>();
    //////Password Objects
    int pw_config = 1, pw_setup = 2, pw_calibration = 3, pw_rang = 4, pw_damp = 5;

    //////Modbus Objects
    unitech_4ch_segment _obj_display;

    int reset=0;
    int total_registor=100;
    signed int holdingRegs[100]; 
    String ip1_val="    ",ip2_val="    ",ip3_val="    ";
    int ip1_420_pval=0,ip2_420_pval=0,ip3_420_pval=0,ip4_420_pval=0;
    int ip1_i2c_pval=0,ip2_i2c_pval=0,ip3_i2c_pval=0,ip4_i2c_pval=0;
    int ip1_420_retry=0,ip2_420_retry=0,ip3_420_retry=0,ip4_420_retry=0;
    int ip1_i2c_retry=0,ip2_i2c_retry=0,ip3_i2c_retry=0,ip4_i2c_retry=0;
    
    bool is_key1_press=false,is_key2_press=false,is_key3_press=false,is_key4_press=false;
    bool _key_press_1=false,_key_press_2=false,_key_press_3=false,_key_press_4=false;
    long currentMillis = millis();


    //int displaytime=0;
    int sec, min, hour, day, month,year;
    //////Class Objects
    i2c_sensor_trh_dp_read i2c_data;
    bool is_i2c_read=true;
    
    I2C_DATA_INFO_STRUCT  *_lst_i2c_data_info;
    PARAMETER_INFO_STRUCT *_lst_parameter_info ;
     
    PARAMETER_ALARM_INFO_STRUCT *_lst_parameter_alarm_info ;
    PASSWORD_INFO_STRUCT   *_lst_password_info ;
    OTHER_INFO_STRUCT *_lst_other_info ;
    
    //LinkedList Modbus Objects
    // PARAMETER_INFO_REG_STRUCT  *_lst_parameter_info_reg ;       
    // PARAMETER_ALARM_INFO_REG_STRUCT *_lst_parameter_alarm_info_reg ; 
    COMMUNICATION_INFO_STRUCT *_lst_communication  ;

    DISPLAY_MENU_SCREEN_STRUCT  *_lst_display_menu_screen_config;
    DISPLAY_MENU_SCREEN_STRUCT  *_lst_display_menu_screen_setup;
    DISPLAY_MENU_SCREEN_STRUCT  *_lst_display_menu_screen_cali;
    DISPLAY_MENU_SCREEN_STRUCT  *_lst_display_menu_screen_chag;
    DISPLAY_MENU_SCREEN_STRUCT  *_lst_display_menu_screen_buzz;
    DISPLAY_MENU_SCREEN_STRUCT  *_lst_display_menu_screen;

    
    // PARAMETER_INFO_STRUCT *_lst_parameter_info_eeprom ;   
    // PARAMETER_ALARM_INFO_STRUCT *_lst_parameter_alarm_info_eeprom ;
    // COMMUNICATION_INFO_STRUCT *_lst_communication_eeprom ;
    // OTHER_INFO_STRUCT *_lst_other_info_eeprom ;
    // PASSWORD_INFO_STRUCT   *_lst_password_info_eeprom;


    //////Funcation
    
    //void sersor_configuration();

    display_screen _obj_display_screen;
    

    //Modbus
    SimpleModbusSlave _obj_modbus;
    bool isMTask=false;

    //AHU
    bool _obj_ahu_off=false,_obj_ahu_trip=false;
    led_status _led_ahu_off;
    led_status _led_ahu_trip;

    //ADS_1115_4_20
    int _ads_1115_arr[4];
    bool is_420_read=true;
    bool isI2CTask=false,isI2C_DESIPLAYTask=false;

    //Time
    // tmElements_t tm;

    rtc _objRtc;
    bool RtcRunning=false;
    DateTime tm;
    

    int _per_ch_alarm=2;


    int cursor_position = 43;
    char arr_num[12] = {'-', '^', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    String display_value = "";
    bool onetime_write = false;
    int scren_index = 0;
    int display_decimal=0;
    long millis_factory_reset = millis();
    long  millis_enable_wifi_ota = millis();
    long millis_is_negative = millis();
    bool is_negative=false;
    //DISPLAY_MENU
    String display_1 = "8888", display_2 = "8888", display_3 = "8888", display_4 = "8888";
    int decimal_1 = 0, decimal_2 = 0, decimal_3 = 0;
    String menu_setting_arr[6] = {"MODE","CONF","SETP"," CAL","CPAS","BUZZ"};
    int menu_scren_index = 0;
    int logout_time=60000;

    String dtype_arr_alarm_type[4] = {" LOW","HIGH","LOLO","HIHI"};    
    String dtype_arr_input_type[5] = {"TEMP","  RH","  DP","TIME","4-20"};
    String dtype_arr_alarm_on_off[2] = {" OFF","  ON"};
    String dtype_arr_yes_no[2] = {" YES","  NO"};
    String dtype_arr_en_dis[2] = {" DIS","  EN"};
    String dtype_arr_parity[3] = {"NONE"," ODD","EVEN"};
    String dtype_arr_buadrate[7] = {"4800","9600","19200","38400","56000","57600","115200"};
    String dtype_arr_ctyp[2] = {"LEAN","SQRE"};
    
    //PASSWORD
    int password_index = 0;
    bool isPassword=false;
    int32_t screen_password_default=0,screen_password=1111,screen_password_master=1357;
    bool is_screen_password_default=false,is_screen_password=false;

    int buadrate[7] = {4800,9600,19200,38400,56000,57600,115200};

    //BLINKING
    long blinking_Millis = millis();
    bool blink_segment = 0;
    int blink_segment_count = 0;
    int blink_decmial = 0;
    int blink_segment_time = 30;
    long display_segment_Millis = millis(),i2crad_Millis = millis();
    bool isReboot=false;

    //WiFi Access Port Status
    bool isWifiAccessPort=false;
    bool isAccessPointStart=false;

    bool write_modbus_one_time=true;

    int _menu_no = 0;

    // //OUTPUT_4_20
    // unsigned char led_1_pin = 15;
    // unsigned char led_2_pin = 23;
    // unsigned char led_3_pin = 18;
    // // setting PWM properties
    // const int freq = 100;
    // const int led_1 = 0,led_2 = 1,led_3 = 2;
    // const int resolution = 15; //Resolution 8, 10, 12, 15

    //MEMORY
    uint32_t _last_read_memory_address=0;
    uint32_t _memory_address=0;  
    long _last_memory_write_millis = millis();
    unsigned long lastLogTime=millis(),loginterval=60,total_restart=0;
    //DateTime tm;
    bool isMemoryRead=false;
    //bool RtcRunning=false;
    int rtc_retry=0;
    bool isSPIMemoryInitialize=false;
    bool _is_synchronize_time =false;
    bool isStorage=true;

    uint8_t _d[4096], _data[4096],_mBuffer[4096],_wMBuffer[13],_rMBuffer[13];
    uint16_t cnt;
    SPIFlash flash;
    tmElements_t _m_tm;
    const unsigned long period = 1000;  //the value is a number of milliseconds
    const int32_t Max_Address_length = (16777215-13);
    byte byteArray[4];
    int16_t modbusArray[2];

    byte e_door_status = 1;
    bool is_dorr_open=false;
    int _dorr_open_delay=false;
    uint8_t door_status = 0;

};

#endif
