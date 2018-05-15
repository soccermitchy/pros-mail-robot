#include "main.h"
#include "JINX.h"

void JINXData() {
  printf("JINX Datasender started\r\n");
  while (true) {
    int usLeftVal = ultrasonicGet(usLeft);
    int usRightVal = ultrasonicGet(usRight);
    // bool btnVal = !digitalRead(D_BUMP_READY);
    // if (usLeftVal > 0 || usRightVal >0) {
      printf("JINX_LeftUltrasonic_%d\r\n", usLeftVal);
      printf("JINX_RightUltrasonic_%d\r\n", usRightVal);
    // }
    // printf("JINX_LeftEncoder_%d\r\n", encoderGet(encLeft));
    // printf("JINX_RightEncoder_%d\r\n", encoderGet(encRight));
    // printf("JINX_ReadyButton_%d\r\n", !btnVal);
    // printf("JINX_State_%d\r\n", getState());
    // printf("JINX_UltrasonicThreshold_%d\r\n", getThreshold());
    taskDelay(500);
  }
}
