#include "main.h"
#include "motorControls.h"

bool runMotors = true;
void disableMotors() {
  runMotors = false;
  stop();
}
void enableMotors() {
  runMotors = true;
}
void moveForwards(int speed) {
  if (!runMotors) return;
  motorSet(M_FL, speed);
  motorSet(M_FR, speed-5);
  motorSet(M_RL, speed);
  motorSet(M_RR, speed-5);
}
void moveBackwards(int speed) {
  if (!runMotors) return;
  motorSet(M_FL, -speed);
  motorSet(M_FR, -speed);
  motorSet(M_RL, -speed);
  motorSet(M_RR, -speed);
}
void stop() {
  motorStop(M_FL);
  motorStop(M_FR);
  motorStop(M_RL);
  motorStop(M_RR);
}

// x > -80 - 2
// -80>=x>-90 - 1
// -90 - ret
// -90>x>=-100 - 1
// -100 > x - 2
void turnLeft() {
  if (!runMotors) return;
  gyroReset(gyro);
  int gyroVal = 0;
  int correctSamples = 0;
  int target = -87;
  while (correctSamples != 30) {
    printf("Gyro: %d | Samples: %d\r\n", gyroVal, correctSamples);
    gyroVal = gyroGet(gyro);
    if (gyroVal > target) {
      // printf("going left\n");
      correctSamples = 0;
      turnLeftMs(1);
    }
    if (gyroVal < target)  {
      // printf("going right\n");;
      correctSamples = 0;
      turnRightMs(1);
    }
    if (gyroVal == target) {
      correctSamples++;
      taskDelay(1);
    }
  }
  stop();
}
void turnLeftMs(int ms) {
  if (!runMotors) return;
  motorSet(M_FL, -64);
  motorSet(M_FR, 64);
  motorSet(M_RL, -64);
  motorSet(M_RR, 64);
  taskDelay(ms);
  stop();
}

void turnRightBroken() {
  if (!runMotors) return;
  gyroReset(gyro);
  int gyroVal = 0;
  int correctSamples = 0;
  int target = 89;
  while (correctSamples != 30) {
    printf("Gyro: %d | Samples: %d\r\n", gyroVal, correctSamples);
    gyroVal = gyroGet(gyro);
    if (gyroVal > target) {
      correctSamples = 0;
      turnLeftMs(1);
    }
    if (gyroVal < target) {
      correctSamples = 0;
      turnRightMs(1);
    }
    if (gyroVal == target) {
      correctSamples++;
      taskDelay(1);
    }
    taskDelay(3);
  }
  stop();
}
void turnRight() {
  if (!runMotors) return;
  gyroReset(gyro);
  int gyroVal = 0;
  int correctSamples = 0;
  int target = 91;
  while (correctSamples != 30) {
    printf("Gyro: %d | Samples: %d\r\n", gyroVal, correctSamples);
    gyroVal = gyroGet(gyro);
    if (gyroVal > target) {
      // printf("going left\n");
      correctSamples = 0;
      turnLeftMs(1);
    }
    if (gyroVal < target)  {
      // printf("going right\n");;
      correctSamples = 0;
      turnRightMs(1);
    }
    if (gyroVal == target) {
      correctSamples++;
      taskDelay(1);
    }
  }
  stop();
}
void turnRightMs(int ms) {
  if (!runMotors) return;
  motorSet(M_FL, 64);
  motorSet(M_FR, -64);
  motorSet(M_RL, 64);
  motorSet(M_RR, -64);
  taskDelay(ms);
  stop();
}
void forwardUntilDistanceBroken(int cm) {
  if (!runMotors) return;
  int usLeftVal = ultrasonicGet(usLeft);
  int usRightVal = ultrasonicGet(usRight);
  printf("JINX_msg_Going forward until %d (current: %d, %d)\r\n", cm, usLeftVal, usRightVal);
  while ((usLeftVal > cm || usRightVal > cm) && (usLeftVal > 1 && usRightVal > 1)) {
    printf("JINX_msg_USLeft=%d USRight=%d\r\n", usLeftVal, usRightVal);
    taskDelay(100);
    usLeftVal = ultrasonicGet(usLeft);
    usRightVal = ultrasonicGet(usRight);
  }
  stop();
}
// 3440
void forwardUntilDistance(int cm) {
  if (!runMotors) return;
  setThreshold(cm);
  int usLeftVal = ultrasonicGet(usLeft);
  int usRightVal = ultrasonicGet(usRight);
  encoderReset(encLeft);
  encoderReset(encRight);

  moveForwards(64);
  gyroReset(gyro);
  bool isAdjusting = false;
  int turnCorrectionAmt = 100;
  while (true) {
    usLeftVal = ultrasonicGet(usLeft);
    usRightVal = ultrasonicGet(usRight);
    int gyroVal = gyroGet(gyro);
    //printf("JINX_msg_Looking for wall that is at most %d away. Current: (%d|%d)\r\n", cm, usLeftVal, usRightVal);
    if (gyroVal > 1) { // turning right
      isAdjusting = true;
      motorSet(M_FR, turnCorrectionAmt);
      motorSet(M_RR, turnCorrectionAmt);
    } else if (gyroVal < 1) { // turning left
      isAdjusting = true;
      motorSet(M_FL, turnCorrectionAmt);
      motorSet(M_RL, turnCorrectionAmt);
    } else if (isAdjusting && gyroVal == 0) {
      isAdjusting = false;
      moveForwards(64);
    }
    if ((usLeftVal < 1) || (usRightVal < 1)) {
      // printf("filtered %d:%d\n", usLeftVal, usRightVal);
      continue; // filter out bad values
    }
    if ((usLeftVal <= cm) && (usRightVal <= cm)) {
      printf("stopping\n");
      break;
    }
    taskDelay(100);
  }
  stop();
  setThreshold(0);
}
