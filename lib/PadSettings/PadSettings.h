#ifndef PadSettings_h // если библиотека Button не подключена
#define PadSettings_h // тогда подключаем ее

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#endif


class PadSettings {
	
public:
PadSettings(int max, int avarage, int low);

unsigned int angleToValue(int angle);

private:

double  MAX_VAL;
double  AVARAGE_VAL;
double  LOW_VAL;

};

#endif
