#include "Wire.h"
#include "Arduino.h"

class i2c_sensor_trh_dp_read {  
public:
  uint8_t sda=22,scl=21;
  float r_val = 0, t_val = 0, d_val = 0;  
  bool is_27_i2c=false,is_28_i2c=false;
  uint32_t r_count = 0, t_count = 0, d_count=0; 
  void set_pin(uint8_t sda, uint8_t scl);
  void tcaselect(uint8_t i2c_bus); 
  void read_data_dp();
  void read_data_t_rh();
  void get_sensor_value();
  bool begin();
  uint8_t error,address;
  void get(int *sec, int *min, int *hour, int *day, int *month, int *year);
  void set(int sec, int min, int hour, int day, int month, int year);
  void set(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t month, uint16_t year);
  void start(void);
  void stop(void);
};
