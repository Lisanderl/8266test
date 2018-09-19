
#include <AngleSettings.h>

using namespace std;


//set values for your servo, possible reverse 
AngleSettings::AngleSettings(int low, int max )
{
    MAX_VAL =  max;
    LOW_VAL =  low;
    if(LOW_VAL > MAX_VAL) {
        coef = (LOW_VAL - MAX_VAL)/180.0;
        return;
    }
    coef = (max-low)/180.0;
}


unsigned int AngleSettings::angle180ToValue(double angle) {

    if(angle >= 180.0) {
      return MAX_VAL;
    }else if( angle <= 0.0 ){
       return LOW_VAL;
    }
    if(LOW_VAL >= MAX_VAL) {
        return LOW_VAL - (angle*coef);
    }
    return (angle*coef)+LOW_VAL;
}

