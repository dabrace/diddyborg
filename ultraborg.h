#ifndef ULTRABORG_H
#define ULTRABORG_H

#define I2C_ERROR_OK            (0)
#define I2C_ERROR_FAILED        (1)
#define I2C_ERROR_PARTIAL       (2)

/***** Servo functions *****/

// Setup the I2C for talking to UltraBorg
int UbInitialise(void);

// Gets the max/min PWM level for servo
// This corresponds to position -1 & +1
// The value is an integer where 2000 represents a 1 ms servo burst
// servo is { 0, 1, 2, 3 }
unsigned int UbGetServoMinimum(int servo);
unsigned int UbGetServoMaximum(int servo);

// Sets the drive position for servo output #1
// 0 is central, -1 is maximum left, +1 is maximum right
// servo is { 0, 1, 2, 3 }
void UbSetServoPosition(int servo, float position);

// Get raw position
// servo is { 0, 1, 2, 3 }
unsigned int UbGetRawServoPosition(int servo);

// Get real position
// servo is { 0, 1, 2, 3 }
float UbGetServoPosition(int servo);

/***** Startup functions *****/

// Gets the startup PWM level for servo output
// This can be anywhere in the minimum to maximum range
// The value is an integer where 2000 represents a 1 ms servo burst
// servo is { 0, 1, 2, 3 }
unsigned int UbGetServoStartup(int servo);

/***** Ultrasonic functions *****/

// Gets the filtered distance for ultrasonic module in millimeters
// Returns 0 for no object detected or no ultrasonic module attached
// If you need a faster response try UbGetRawDistance1 instead (no filtering)
// usm is { 0, 1, 2, 3 }
float UbGetDistance(int usm);

// Gets the raw distance for ultrasonic module in millimeters
// Returns 0 for no object detected or no ultrasonic module attached
// For a filtered (less jumpy) reading use UbGetDistance1
// usm is { 0, 1, 2, 3 }
float UbGetRawDistance(int usm);

#endif

