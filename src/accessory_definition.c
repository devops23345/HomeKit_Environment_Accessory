/*
 * my_accessory.c
 * Define the accessory in C language using the Macro in characteristics.h
 *
 *  Created on: 2020-05-15
 *      Author: Mixiaoxiao (Wang Bin)
 */

#include <homekit/homekit.h>
#include <homekit/characteristics.h>

// Called to identify this accessory. See HAP section 6.7.6 Identify Routine
// Generally this is called when paired successfully or click the "Identify Accessory" button in Home APP.
void my_accessory_identify(homekit_value_t _value) {
	printf("accessory identify\n");
}

// For TEMPERATURE_SENSOR,
// the required characteristics are: CURRENT_TEMPERATURE
// the optional characteristics are: NAME, STATUS_ACTIVE, STATUS_FAULT, STATUS_TAMPERED, STATUS_LOW_BATTERY
// See HAP section 8.41 and characteristics.h

// (required) format: float; HAP section 9.35; min 0, max 100, step 0.1, unit celsius
homekit_characteristic_t cha_current_temperature = HOMEKIT_CHARACTERISTIC_(CURRENT_TEMPERATURE, 0);

// (optional) format: string; HAP section 9.62; max length 64
homekit_characteristic_t cha_temp_sensor_name = HOMEKIT_CHARACTERISTIC_(NAME, "Temperature Sensor");

// (optional) format: bool; HAP section 9.96
homekit_characteristic_t cha_temp_sensor_status_active = HOMEKIT_CHARACTERISTIC_(STATUS_ACTIVE, true);

// (optional) format: uint8; HAP section 9.97; 0 "No Fault", 1 "General Fault"
// homekit_characteristic_t cha_status_fault = HOMEKIT_CHARACTERISTIC_(STATUS_FAULT, 0);

// (optional) format: uint8; HAP section 9.100; 0 "Accessory is not tampered", 1 "Accessory is tampered with"
// homekit_characteristic_t cha_status_tampered = HOMEKIT_CHARACTERISTIC_(STATUS_TAMPERED, 0);

// (optional) format: uint8; HAP section 9.99; 0 "Battery level is normal", 1 "Battery level is low"
// homekit_characteristic_t cha_status_low_battery = HOMEKIT_CHARACTERISTIC_(STATUS_LOW_BATTERY, 0);

// example for humidity
homekit_characteristic_t cha_current_humidity  = HOMEKIT_CHARACTERISTIC_(CURRENT_RELATIVE_HUMIDITY, 0);

// (optional) format: string; HAP section 9.62; max length 64
homekit_characteristic_t cha_humidity_sensor_name = HOMEKIT_CHARACTERISTIC_(NAME, "Humidity Sensor");

// (optional) format: bool; HAP section 9.96
homekit_characteristic_t cha_humidity_sensor_status_active = HOMEKIT_CHARACTERISTIC_(STATUS_ACTIVE, true);

// example for occupancy sensor
homekit_characteristic_t cha_current_occupancy  = HOMEKIT_CHARACTERISTIC_(OCCUPANCY_DETECTED, 0);

// (optional) format: string; HAP section 9.62; max length 64
homekit_characteristic_t cha_occupancy_sensor_name = HOMEKIT_CHARACTERISTIC_(NAME, "Occupancy Sensor");

// (optional) format: bool; HAP section 9.96
homekit_characteristic_t cha_occupancy_sensor_status_active = HOMEKIT_CHARACTERISTIC_(STATUS_ACTIVE, true);

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_sensor, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {

#ifdef ACCY_NAME
//String accessory_name = ACCY_NAME; //nust be unique for each accy
            HOMEKIT_CHARACTERISTIC(NAME, ACCY_NAME),
            //printf("accessory_name:", ACCY_NAME);
#else /* ACCY_NAME */
            HOMEKIT_CHARACTERISTIC(NAME, "accessory_name"),
#endif /* ACCY_NAME*/
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.2"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
        HOMEKIT_SERVICE(TEMPERATURE_SENSOR, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Temperature Sensor"),
            &cha_current_temperature,
			&cha_temp_sensor_name,//optional
			&cha_temp_sensor_status_active,//optional
			//&cha_status_fault,//optional
			//&cha_status_tampered,//optional
			//&cha_status_low_battery,//optional
            NULL
        }),
		// Add this HOMEKIT_SERVICE if you has a HUMIDITY_SENSOR together
		
        HOMEKIT_SERVICE(HUMIDITY_SENSOR, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Humidity Sensor"),
            &cha_current_humidity,
            &cha_humidity_sensor_name,//optional
			&cha_humidity_sensor_status_active,//optional
            NULL
        }),
 		// Add this HOMEKIT_SERVICE if you has a OCCUPANCY_SENSOR together
		
            HOMEKIT_SERVICE(OCCUPANCY_SENSOR, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Occupancy Sensor"),
            &cha_current_occupancy,
            &cha_occupancy_sensor_name,//optional
			&cha_occupancy_sensor_status_active,//optional
            NULL
        }), 
        NULL
    }),
    NULL
};

homekit_server_config_t config = {
		.accessories = accessories,
		.password = "111-11-111"
};


