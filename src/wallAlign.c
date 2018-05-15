#include "main.h"
#include "motorControls.h"

void wallAlign() {
  int sameSamples = 0;
  while (true) {
    int usLeftVal = ultrasonicGet(usLeft);
    int usRightVal = ultrasonicGet(usRight);

    // Discard default values and restart the loop
    if (usLeftVal == ULTRA_BAD_RESPONSE
      || usLeftVal == 0
      || usRightVal == ULTRA_BAD_RESPONSE
      || usRightVal == 0) {
        taskDelay(10);
        continue;
    }
    printf("%d\t%d\n", usLeftVal, usRightVal);
    if (usLeftVal > usRightVal) {
      // Left side is further away, lets turn to the right a bit
      turnRightMs(1);
      sameSamples = 0;
    } else if (usRightVal > usLeftVal){
      // Right side is further away, lets turn to the left a bit
      turnLeftMs(1);
      sameSamples = 0;
    } else {
      // We're aligned with a wall. Lets return control to the rest of the
      // program
      sameSamples++;
      if (sameSamples == 30)
        return;
    }
    taskDelay(10);
  }
}
