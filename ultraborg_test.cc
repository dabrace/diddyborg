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

	return 0;
}

