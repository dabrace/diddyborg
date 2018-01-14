/* EXTREMELY CRUDE ULTRABORG SERVO EXAMPLE */

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <inttypes.h>

#include "ultraborg.h"

//// Constants /////
#define BUFFER_MAX              (32)
#define HANDLE_UNINITALISED     (-1)
#define UB_DEFAULT_PWM_MAX      (4000)  // Servo maximum limit when set to default 
#define UB_DEFAULT_PWM_MIN      (2000)  // Servo minimum limit when set to default
#define UB_DEFAULT_I2C_ADDRESS  (0x36)  // I2C address set by default (before using SET_I2C_ADD)
#define UB_USM_US_TO_MM         (0.171500)  // Conversion from 'GET_TIME_USM' commands to millimeters

//// Servo Command Mapping ////
const int UbCommandSetPWM[4] = { 5, 7, 9, 11 };
const int UbCommandGetPWM[4] = { 6, 8, 10, 12 };
const int UbCommandGetPWMMin[4] = { 17, 20, 23, 26 };
const int UbCommandGetPWMMax[4] = { 18, 21, 24, 27 };
const int UbCommandGetPWMBoot[4] = { 19, 22, 25, 28 };
const int UbCommandGetFilterUSM[4] = { 41, 42, 43, 44 };
const int UbCommandGetTimeUSM[4] = { 1, 2, 3, 4 };

#ifndef FALSE
#define FALSE                   (0)
#endif
#ifndef TRUE
#define TRUE                    (!FALSE)
#endif

///// Storage /////
uint8_t bufIn[BUFFER_MAX];
uint8_t bufOut[BUFFER_MAX];
char pathI2C[20];
int hI2C = HANDLE_UNINITALISED;
signed long UbServoMin[4] = { UB_DEFAULT_PWM_MIN, UB_DEFAULT_PWM_MIN, UB_DEFAULT_PWM_MIN, UB_DEFAULT_PWM_MIN};;  // The current minimum limit for servo
signed long UbServoMax[4] = { UB_DEFAULT_PWM_MAX, UB_DEFAULT_PWM_MAX, UB_DEFAULT_PWM_MAX, UB_DEFAULT_PWM_MAX };  // The current maximum limit for servo

///// Macros /////
#ifndef DIAG_PRINT
#define DIAG_PRINT(...)         do { if (diagnosticPrint) { printf(__VA_ARGS__); } } while (FALSE)
#endif

///// User values /////
int busNumber = 1;
int addressUltraBorg = UB_DEFAULT_I2C_ADDRESS;
int diagnosticPrint = TRUE;

///// I2C handling functions /////
int SendI2C(int hI2C, int bytes, uint8_t *pData) {
    int rc = write(hI2C, pData, bytes);
    if (rc == bytes) {
        return I2C_ERROR_OK;
    } else if (rc < 0) {
        DIAG_PRINT("I2C ERROR: Failed to send %d bytes!\n", bytes);
        return I2C_ERROR_FAILED;
    } else {
        DIAG_PRINT("I2C ERROR: Only sent %d of %d bytes!\n", rc, bytes);
        return I2C_ERROR_PARTIAL;
    }
}

int RecvI2C(int hI2C, int bytes, uint8_t *pData) {
    int rc = read(hI2C, pData, bytes);
    if (rc == bytes) {
        return I2C_ERROR_OK;
    } else if (rc < 0) {
        DIAG_PRINT("I2C ERROR: Failed to read %d bytes!\n", bytes);
        return I2C_ERROR_FAILED;
    } else {
        DIAG_PRINT("I2C ERROR: Only read %d of %d bytes!\n", rc, bytes);
        return I2C_ERROR_PARTIAL;
    }
}

///// UltraBorg functions /////


// Setup the I2C for talking to UltraBorg
int UbInitialise(void) {
    int i;

    DIAG_PRINT("Loading UltraBorg on bus %d\n", busNumber);

    // Clean buffers
    for (i = 0; i < BUFFER_MAX; ++i) {
        bufIn[i] = 0xCC;
        bufOut[i] = 0xCC;
    }

    // Open I2C device
    snprintf(pathI2C, sizeof(pathI2C)-1, "/dev/i2c-%d", busNumber);
    hI2C = open(pathI2C, O_RDWR);
    if (hI2C < 0) {
        DIAG_PRINT("I2C ERROR: Failed to open bus %d, are we root?\n", busNumber);
        return I2C_ERROR_FAILED;
    }

    if (ioctl(hI2C, I2C_SLAVE, addressUltraBorg) < 0) {
        DIAG_PRINT("I2C ERROR: Failed to set target address to %d!\n", addressUltraBorg);
        return I2C_ERROR_FAILED;
    }


	if (RecvI2C(hI2C, 2, bufIn) != I2C_ERROR_OK) {
        DIAG_PRINT("I2C ERROR: Failed to recive response.\n");
        return I2C_ERROR_FAILED;
	}

    // Read in the limits for this board
	for (i = 0; i < 3; i++) {
       	UbServoMin[i] = UbGetServoMinimum(i);
       	UbServoMax[i] = UbGetServoMaximum(i);
	}

    return I2C_ERROR_OK;

}


// Gets the minimum PWM level for servo output
// This corresponds to position -1
// The value is an integer where 2000 represents a 1 ms servo burst
unsigned int UbGetServoMinimum(int servo) {
    unsigned int pwmLevel;

	if (servo < 0 || servo > 3) {
		DIAG_PRINT("UltraBord Error:  Please pass servo {0, 1, 2, 3} only.\n");
		return 0;
	}

	bufOut[0] = UbCommandGetPWMMin[servo];
    if (SendI2C(hI2C, 1, bufOut) != I2C_ERROR_OK) {
        DIAG_PRINT("I2C ERROR: Failed sending command");
    }

	if (RecvI2C(hI2C, 2, bufIn) != I2C_ERROR_OK)
		return 0;

    pwmLevel = ((unsigned int)bufIn[1] << 8) + (unsigned int)bufIn[2];
    return pwmLevel;
}


// Gets the minimum PWM level for servo output
// This corresponds to position +1
// The value is an integer where 2000 represents a 1 ms servo burst
unsigned int UbGetServoMaximum(int servo) {
    unsigned int pwmLevel;

	if (servo < 0 || servo > 3) {
		DIAG_PRINT("UltraBord Error:  Please pass servo {0, 1, 2, 3} only.\n");
		return 0;
	}

	bufOut[0] = UbCommandGetPWMMax[servo];
    if (SendI2C(hI2C, 1, bufOut) != I2C_ERROR_OK) {
        DIAG_PRINT("I2C ERROR: Failed sending command.\n");
    }

	if (RecvI2C(hI2C, 2, bufIn) != I2C_ERROR_OK)
		return 0;

    pwmLevel = ((unsigned int)bufIn[1] << 8) + (unsigned int)bufIn[2];
    return pwmLevel;
}


// Sets the drive position for servo output 
// 0 is central, -1 is maximum left, +1 is maximum right
// servo is { 0, 1, 2, 3 }
void UbSetServoPosition(int servo, float position) {

	if (servo < 0 || servo > 3) {
		DIAG_PRINT("UltraBord Error:  Please pass servo {0, 1, 2, 3} only.\n");
		return;
	}

    float powerOut = (position + 1.0) / 2.0;
    unsigned int pwmDuty = (unsigned int)((powerOut * (UbServoMax[servo] - UbServoMin[servo])) + UbServoMin[servo]);

    bufOut[0] = UbCommandSetPWM[servo];
    bufOut[1] = (uint8_t)((pwmDuty >> 8) & 0xFF);
    bufOut[2] = (uint8_t)(pwmDuty & 0xFF);
    if (SendI2C(hI2C, 3, bufOut) != I2C_ERROR_OK) {
        DIAG_PRINT("I2C ERROR: Failed setting servo position.\n");
    }
}

unsigned int UbGetRawServoPosition(int servo) {
    unsigned int pwmLevel;

	if (servo < 0 || servo > 3) {
		DIAG_PRINT("UltraBord Error:  Please pass servo {0, 1, 2, 3} only.\n");
		return 0;
	}

	bufOut[0] = UbCommandGetPWM[servo];
    if (SendI2C(hI2C, 1, bufOut) != I2C_ERROR_OK) {
        DIAG_PRINT("I2C ERROR: Failed setting servo position.\n");
		return 0;
    }

	if (RecvI2C(hI2C, 3, bufIn) != I2C_ERROR_OK)
		return 0;

	pwmLevel = ((unsigned int)bufIn[1] << 8) + (unsigned int)bufIn[2];

	return pwmLevel;

}

float UbGetServoPosition(int servo) {
	unsigned int pwmLevel;
	float powerOut;
	
	pwmLevel = UbGetRawServoPosition(servo);

	if (pwmLevel == 0) return 0;

	powerOut = ((float)pwmLevel - UbServoMin[servo]) / (UbServoMax[servo] - UbServoMin[servo]);

	return (2.0 * powerOut) - 1.0;
}

unsigned int UbGetServoStartup(int servo) {
    unsigned int pwmLevel;

	if (servo < 0 || servo > 3) {
		DIAG_PRINT("UltraBord Error:  Please pass servo {0, 1, 2, 3} only.\n");
		return 0;
	}

	bufOut[0] = UbCommandGetPWMBoot[servo];
    if (SendI2C(hI2C, 1, bufOut) != I2C_ERROR_OK) {
        DIAG_PRINT("I2C ERROR: Failed setting servo position.\n");
		return 0;
    }

	if (RecvI2C(hI2C, 3, bufIn) != I2C_ERROR_OK)
		return 0;

	pwmLevel = ((unsigned int)bufIn[1] << 8) + (unsigned int)bufIn[2];
	return pwmLevel;
}


float UbGetDistance(int usm) {
	unsigned int time_us;

	if (usm < 0 || usm > 3) {
		DIAG_PRINT("UltraBord Error:  Please pass usm {0, 1, 2, 3} only.\n");
		return 0;
	}

	bufOut[0] = UbCommandGetFilterUSM[usm];
    if (SendI2C(hI2C, 1, bufOut) != I2C_ERROR_OK) {
        DIAG_PRINT("I2C ERROR: Failed setting servo position.\n");
		return 0;
    }

	if (RecvI2C(hI2C, 3, bufIn) != I2C_ERROR_OK)
    	return 0;

	time_us = ((unsigned int)bufIn[1] << 8) + (unsigned int)bufIn[2];
	if (time_us == 65535) time_us = 0;
	return (float)time_us * UB_USM_US_TO_MM;
}



float UbGetRawDistance(int usm) {
    unsigned int time_us;

	if (usm < 0 || usm > 3) {
		DIAG_PRINT("UltraBord Error:  Please pass usm {0, 1, 2, 3} only.\n");
		return 0;
	}

	bufOut[0] = UbCommandGetTimeUSM[usm];
    if (SendI2C(hI2C, 1, bufOut) != I2C_ERROR_OK) {
        DIAG_PRINT("I2C ERROR: Failed setting servo position.\n");
		return 0;
    }

	if (RecvI2C(hI2C, 3, bufIn) != I2C_ERROR_OK)
    	return 0;

    time_us = ((unsigned int)bufIn[1] << 8) + (unsigned int)bufIn[2];
    if (time_us == 65535) time_us = 0;
    return (float)time_us * UB_USM_US_TO_MM;
}

