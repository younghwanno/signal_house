#include "etc.h"


void etc::set_UV(float UV) {this->UV = UV;}
void etc::set_rain1(int rain1) {this->rain1 = rain1;}
void etc::set_rain2(int rain2) {this->rain2 = rain2;}

void etc::set_human1(int human1) {this->human1 = human1;}
void etc::set_human2(int human2) {this->human2 = human2;}

void etc::set_vibration(int vibration) {this->vibration = vibration;}
void etc::set_dust(int dust) {this->dust = dust;}

void etc::set_current(int current) {this->current = current;}
void etc::set_current2(int current) {this->current2 = current;}

void etc::set_pump(int pump) {this->pump = pump;}
void etc::set_boiler(int boiler) {this->boiler = boiler;}

void etc::set_windows(int windows) {this->windows = windows;}
void etc::set_doors(int doors) {this->doors = doors;}
void etc::set_home_mode(int home_mode) {this->home_mode = home_mode;}

void etc::set_voltage(float voltage) { this->voltage = voltage;}
void etc::set_voltage2(float voltage) { this->voltage2 = voltage;}

void etc::set_load_cell(float load_cell) { this->load_cell = load_cell; }

void etc::set_Solar_to_Battery(int sb) {this->Solar_to_Battery = sb;}
void etc::set_Solar_to_DC(int sd) {this->Solar_to_DC = sd;}
void etc::set_Batter_to_DC(int bd) {this->Batter_to_DC = bd;}
void etc::set_garage_door(int door) {this->garage_door = door;}

void etc::set_solar_pannel_angle1(int angle) {this->solar_pannel_angle1 = angle;}
void etc::set_solar_pannel_angle2(int angle) {this->solar_pannel_angle2 = angle;}
void etc::set_solar_pannel_angle3(int angle) {this->solar_pannel_angle3 = angle;}
void etc::set_solar_pannel_angle4(int angle) {this->solar_pannel_angle4 = angle;}

void etc::set_park_sensor_flag(int flag) {this->park_sensor_flag = flag;}
void etc::set_rfid(int rfid) {this->rfid = rfid;}

void etc::set_light(int a[])
{
    for(int i = 0; i < 8 ;i++)
    {
        this->light[i] = a[i];
    }
}

void etc::set_battery1_capacity(float capacity) {this->battery1_capacity = capacity;}
void etc::set_battery2_capacity(float capacity) {this->battery2_capacity = capacity;}




float etc::get_UV() {return this->UV;}
int etc::get_rain1() {return this->rain1;}
int etc::get_rain2() {return this->rain2;}

int etc::get_human1() {return this->human1;}
int etc::get_human2() {return this->human2;}

int etc::get_vibration() {return this->vibration;}
int etc::get_dust() {return this->dust;}

int etc::get_current() {return this->current;}
int etc::get_current2() {return this->current2;}

int etc::get_pump() {return this->pump;}
int etc::get_boiler() {return this->boiler;}

int etc::get_windows() {return this->windows;}
int etc::get_doors() {return this->doors;}
int etc::get_home_mode() {return this->home_mode;}

float etc::get_voltage() {return this->voltage;}
float etc::get_voltage2() {return this->voltage2;}

float etc::get_load_cell() {return this->load_cell;}


int etc::get_Solar_to_Battery() {return this->Solar_to_Battery;}
int etc::get_Solar_to_DC() {return this->Solar_to_DC;}
int etc::get_Batter_to_DC() {return this->Batter_to_DC;}
int etc::get_garage_door() {return this->garage_door;}

int etc::get_solar_pannel_angle1() {return this->solar_pannel_angle1;}
int etc::get_solar_pannel_angle2() {return this->solar_pannel_angle2;}
int etc::get_solar_pannel_angle3() {return this->solar_pannel_angle3;}
int etc::get_solar_pannel_angle4() {return this->solar_pannel_angle4;}

int etc::get_park_sensor_flag() {return this->park_sensor_flag;}
int etc::get_rfid() {return this->rfid;}


int* etc::get_light() {return this->light;}

float etc::get_battery1_capacity() {return this->battery1_capacity;}
float etc::get_battery2_capacity() {return this->battery2_capacity;}
