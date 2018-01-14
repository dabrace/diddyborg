#include <stdio.h>
#include <unistd.h>
#include "ultraborg.h"

void setPosition(int servo, float pos) {

	printf("  Set servo %d to %.3f\n", servo, pos);

    UbSetServoPosition(servo, pos);

	printf("    Read: Raw position: %d, real position: %.3f.\n", 	
		UbGetRawServoPosition(servo),
		UbGetServoPosition(servo));

	sleep(1);
}


int main(void) {

	printf("Setup\n");

    // Setup I2C bus ready
    if (UbInitialise() != I2C_ERROR_OK) {
		printf("Fail.\n");
        return I2C_ERROR_FAILED;
    }

	int i;

	printf("Get Extensts:\n");

	for (i = 0; i < 4; i++) {
		printf("  Servo: %d, Startup: %d, Min: %d, Max: %d\n",
			i,
			UbGetServoStartup(i),
			UbGetServoMinimum(i),
			UbGetServoMaximum(i));
	}


	for (i = 0; i < 4; i++) {

		printf("\n");
		printf("Servo: %d\n", i);

		setPosition(i, -1);
		setPosition(i, 0);
		setPosition(i, 1);

	}

    return 0;
}

