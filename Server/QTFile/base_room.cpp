#include "base_room.h"

void base_room::set_datetime(QDateTime date) {this->date = date;}
void base_room::set_temperature(int temperature) {this->temperature = temperature;}
void base_room::set_humidity(int humidity) {this->humidity = humidity;}

void base_room::set_door(int door) {this->door = door;}
void base_room::set_window1(int window1) {this->window1 = window1;}
void base_room::set_window2(int window2) {this->window2 = window2;}


void base_room::set_LED_OnOff_state(int LED_onoff) {this->LED_OnOff_state = LED_onoff;}
void base_room::set_LED_mode(int LED_mode) {this->LED_mode = LED_mode;}
void base_room::set_LED_R(int R) {this->LED_R = R;}
void base_room::set_LED_G(int G) {this->LED_G = G;}
void base_room::set_LED_B(int B) {this->LED_B = B;}
void base_room::set_LED_total(int total) {this->LED_total = total;}

void base_room::set_Co2(int Co) {this->Co2 = Co;}
void base_room::set_Smoke(int Smoke) {this->Smoke = Smoke;}
void base_room::set_Flame(int Flame) {this->Flame = Flame;}




QDateTime base_room::get_datetime() {return this->date;}
int base_room::get_temperature() {return this->temperature;}
int base_room::get_humidity() {return this->humidity;}


int base_room::get_door() {return this->door;}
int base_room::get_window1() {return this->window1;}
int base_room::get_window2() {return this->window2;}


int base_room::get_LED_OnOff_state() {return this->LED_OnOff_state;}
int base_room::get_LED_mode() {return this->LED_mode;}
int base_room::get_LED_R() {return this->LED_R;}
int base_room::get_LED_G() {return this->LED_G;}
int base_room::get_LED_B() {return this->LED_B;}
int base_room::get_LED_total() {return this->LED_total;}

int base_room::get_Co2() {return this->Co2;}
int base_room::get_Smoke() {return this->Smoke;}
int base_room::get_Flmae() {return this->Flame;}
