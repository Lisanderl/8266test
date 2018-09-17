
#include <PadSettings.h>

using namespace std;

PadSettings::PadSettings(int max, int avarage, int low)
{
    MAX_VAL = (double) max/90;
    AVARAGE_VAL = (double) avarage;
    LOW_VAL = (double)  low/90;

}


unsigned int PadSettings::angleToValue(int angle) {
  unsigned  int val = 0;
    if(angle > 0) {
        val = MAX_VAL * angle;
    }else {
        val = LOW_VAL * angle;
    }

    return val;
}