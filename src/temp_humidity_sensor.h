#ifndef TEMP_HUMIDITY_SENSOR_H
#define TEMP_HUMIDITY_SENSOR_H

// current temperature & humidity, updated in loop()
extern float sensor_temp_val;
extern float sensor_humidity_val;

void sensor_setup();
void sensor_loop();


#endif /* TEMP_HUMIDITY_SENSOR_H */