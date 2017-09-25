#include "entrance.h"


void entrance::set_knock(int knock) {this->knock = knock;}
void entrance::set_body_sensor(int body_sensor) {this->body_sensor = body_sensor;}
void entrance::set_body_sensor_flag(int body_sensor) {this->body_sensor_flag = body_sensor;}



int entrance::get_knock() {return this->knock;}
int entrance::get_body_sensor() {return this->body_sensor;}
int entrance::get_body_sensor_flag() {return this->body_sensor_flag;}
