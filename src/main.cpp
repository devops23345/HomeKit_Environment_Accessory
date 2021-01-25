/*
 * main.cpp
 *
 * This example shows how to:
 * 1. define a temperature sensor accessory and its characteristics (in my_accessory.c).
 * 2. report the sensor value to HomeKit (just random value here, you need to change it to your real sensor value).
 *
 *  Created on: 2020-05-15
 *      Author: Mixiaoxiao (Wang Bin)
 *
 * Note:
 *
 * You are recommended to read the Apple's HAP doc before using this library.
 * https://developer.apple.com/support/homekit-accessory-protocol/
 *
 * This HomeKit library is mostly written in C,
 * you can define your accessory/service/characteristic in a .c file,
 * since the library provides convenient Macro (C only, CPP can not compile) to do this.
 * But it is possible to do this in .cpp or .ino (just not so conveniently), do it yourself if you like.
 * Check out homekit/characteristics.h and use the Macro provided to define your accessory.
 *
 * Generally, the Arduino libraries (e.g. sensors, ws2812) are written in cpp,
 * you can include and use them in a .ino or a .cpp file (but can NOT in .c).
 * A .ino is a .cpp indeed.
 *
 * You can define some variables in a .c file, e.g. int my_value = 1;,
 * and you can access this variable in a .ino or a .cpp by writing extern "C" int my_value;.
 *
 * So, if you want use this HomeKit library and other Arduino Libraries together,
 * 1. define your HomeKit accessory/service/characteristic in a .c file
 * 2. in your .ino, include some Arduino Libraries and you can use them normally
 *                  write extern "C" homekit_characteristic_t xxxx; to access the characteristic defined in your .c file
 *                  write your logic code (eg. read sensors) and
 *                  report your data by writing your_characteristic.value.xxxx_value = some_data; homekit_characteristic_notify(..., ...)
 * done.
 */

#include <Arduino.h>
#include <arduino_homekit_server.h>
#include <nodemculed.h> //Controls blinking leds
#include <EasyNodeMCU_Wifi.h>
#include "temp_humidity_sensor.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);


//==============================
//==============================

// access your homekit characteristics defined in accessory_definition.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_current_temperature;
extern "C" homekit_characteristic_t cha_current_humidity;
extern "C" homekit_characteristic_t cha_current_occupancy;

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

uint8_t occupancy_toggle = 0;

#ifdef WIFI_SSID
const char* easy_Wifi_SSID = WIFI_SSID; //passed through biuld flags
const char* easy_Wifi_Password = WIFI_PASS; //passed through biuld flags
#endif /* WIFI_SSID */

// *****************************************************************************
// Function Definitions
// *****************************************************************************


// Homekit setup and loop
void my_homekit_setup() {

#if SERVER_RESET
	LOG_D("Reset Homekit Server");
	homekit_server_reset();
#endif /* SERVER_RESET*/

	arduino_homekit_setup(&config);

}
void my_homekit_report() {
	//float temperature_value = random_value(10, 30); // FIXME, read your real sensor here.
	float temperature_value = sensor_temp_val;
	cha_current_temperature.value.float_value = temperature_value;
	LOG_D("Current temperature: %.1f", temperature_value);
	homekit_characteristic_notify(&cha_current_temperature, cha_current_temperature.value);
	//TODO:  add fault handling and led trigger if sensor goes bad.

	//float relative humidty
	float humidity_value = sensor_humidity_val;
	cha_current_humidity.value.float_value = humidity_value;
	LOG_D("Current humidity: %.1f", humidity_value);
	homekit_characteristic_notify(&cha_current_humidity, cha_current_humidity.value);

	//occupancy state - toggle state --> replace with actual sensor in future
	if (occupancy_toggle == 0){
		occupancy_toggle = 1;
		LOG_D("Occupancy Toggle On");
	}
	else {
		occupancy_toggle = 0;
		LOG_D("Occupancy Toggle Off");

	}
	cha_current_occupancy.value.uint8_value = occupancy_toggle;
	LOG_D("Current occupancy: %d", occupancy_toggle);
	homekit_characteristic_notify(&cha_current_occupancy, cha_current_occupancy.value);

}

void my_homekit_loop() {
	arduino_homekit_loop();
	const uint32_t t = millis();
	if (t > next_report_millis) {
		// report sensor values every 10 seconds
		next_report_millis = t + 10 * 1000;
		my_homekit_report();
	}
	if (t > next_heap_millis) {
		// show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

	}
}
// *****************************************************************************
// setup()
// *****************************************************************************
void setup() {
	Serial.begin(115200);
    setup_LED();
    setBlinkState(BLINK_STATE_OK);
	setup_easy_wifi(easy_Wifi_SSID, easy_Wifi_Password);//wifi_connect();
	my_homekit_setup();
	sensor_setup();
}

// *****************************************************************************
// main loop()
// *****************************************************************************
void loop() {
	my_homekit_loop();
	sensor_loop();
	loop_easy_wifi();
	loop_LED();
	delay(10);
}

