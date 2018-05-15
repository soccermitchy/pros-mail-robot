/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "motorControls.h"
/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
void operatorControl() {
	bool ledValue = false;
	while (1) {
		setState(0);
		printf("JINX_msg_Waiting for button\r\n");
		//writeJINXMessage("Waiting on button");

		while (digitalRead(D_BUMP_READY)) {
			digitalWrite(D_LED_GREEN, ledValue);
			ledValue = !ledValue;
			taskDelay(10);
		}
		taskDelay(100);
		printf("JINX_msg_Aligning with wall\r\n");
		setState(1);
		// wallAlign();
		setState(2);
		printf("JINX_msg_Aligned. Waiting for door to open\r\n");

		int usLeftVal = ultrasonicGet(usLeft);
		int usRightVal = ultrasonicGet(usRight);
		// while (usLeftVal < 60 && usRightVal < 60) {
		// 		taskDelay(10);
		// 		usLeftVal = ultrasonicGet(usLeft);
		// 		usRightVal = ultrasonicGet(usRight);
		// }

		setState(3);
		printf("JINX_msg_Door opened\r\n");
		taskDelay(1000);
		forwardUntilDistance(180);
		wallAlign();
		setState(4);
		printf("JINX_msg_Found wall\r\n");
		turnLeft();
		setState(5);
		moveForwards(63);
		printf("JINX_msg_Turned\r\n");
		forwardUntilDistance(120);
		setState(6);
		printf("JINX_msg_Found long hall end\r\n");
		wallAlign();
		turnRight();
		setState(7);
		moveForwards(63);
		taskDelay(18500);
		stop();
		setState(8);
		printf("JINX_msg_Near elevators\r\n");
		turnRight();
		setState(9);
		printf("JINX_msg_Turned\r\n");
		forwardUntilDistance(60);
		setState(10);
		printf("JINX_msg_Waiting on elevator\r\n");
		usLeftVal = ultrasonicGet(usLeft);
		usRightVal = ultrasonicGet(usRight);
		while (usLeftVal < 80 && usRightVal < 80) {
				taskDelay(10);
				usLeftVal = ultrasonicGet(usLeft);
				usRightVal = ultrasonicGet(usRight);
		}
		printf("JINX_msg_Elevator opened\r\n");
		setState(11);
		forwardUntilDistance(60);
		setState(12);
		printf("JINX_msg_Inside elevator\r\n");
		turnRight();
		turnRight();
		setState(13);
		printf("JINX_msg_Turned. Waiting on elevator.\r\n");
		usLeftVal = ultrasonicGet(usLeft);
		usRightVal = ultrasonicGet(usRight);
		while (usLeftVal < 80 && usRightVal < 80) {
				taskDelay(10);
				usLeftVal = ultrasonicGet(usLeft);
				usRightVal = ultrasonicGet(usRight);
		}
		printf("JINX_msg_Elevator opened. Going forward.\r\n");
		setState(14);
		moveForwards(63);
		taskDelay(3000);
		stop();
		printf("JINX_msg_Outside elevator\r\n");
		setState(15);
		turnRight();
		printf("JINX_msg_Turned\r\n");
		setState(16);
		forwardUntilDistance(60);
		setState(17);
		printf("JINX_msg_Waiting on door.\r\n");
		usLeftVal = ultrasonicGet(usLeft);
		usRightVal = ultrasonicGet(usRight);
		while (usLeftVal < 80 && usRightVal < 80) {
				taskDelay(10);
				usLeftVal = ultrasonicGet(usLeft);
				usRightVal = ultrasonicGet(usRight);
		}
		printf("JINX_msg_Door open, going through door.\r\n");
		setState(18);
		moveForwards(63);
		taskDelay(3000);
		stop();
		setState(19);
		printf("JINX_msg_Turning into mailroom.\r\n");
		turnLeft();
		printf("JINX_msg_Entering mailroom\r\n");
		setState(20);
		forwardUntilDistance(60);
		setState(21);
		printf("JINX_msg_Inside mailroom");
		stop();
	}
}
void operatorControl_() {
	bool ledValue = true;
	while (digitalRead(D_BUMP_READY)) {
		digitalWrite(D_LED_GREEN, ledValue);
		ledValue = !ledValue;
		taskDelay(50);
	}
	//moveForwards(127);
	//turnLeft();
	forwardUntilDistance(60);
	wallAlign();
	turnRight();
	printf("DONE\r\n");
	while (true) {
		taskDelay(50);
		//printf("%d\t%d\n", encoderGet(encLeft), encoderGet(encRight));
	}
}
void operatorControl__() {
	while(true) {
		printf("%d\n", gyroGet(gyro));
		taskDelay(50);
	}
}
