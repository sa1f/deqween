/*
 * globals.h
 *
 *  Created on: Feb 13, 2018
 *      Author: i8b0b
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <string>
#include "graphics.h"
#include "servo.h"
#include "wifi.h"
#include "gps.h"

extern Graphics graphics;
extern Servo servo;
extern Wifi wifi;
extern GPS gps;
extern std::vector<std::string> weather_data;
//extern char * temperature;
#endif /* GLOBALS_H_ */
