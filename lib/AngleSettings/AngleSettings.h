#ifndef AngleSettings_h // если библиотека Button не подключена
#define AngleSettings_h // тогда подключаем ее

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#endif


class AngleSettings {
	
public:
AngleSettings(int low, int max );

unsigned int angle180ToValue(double angle);

private:

double  MAX_VAL;
double  LOW_VAL;
double  coef;

};

#endif
