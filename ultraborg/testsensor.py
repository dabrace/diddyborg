#!/usr/bin/env python
# coding: latin-1
 

# Import the libraries we need
import UltraBorg
import time
import math
import sys
 
# Settings
distanceMin = 100.0             # Minimum distance in mm, corresponds to DiddyBorg reversing at 100%
distanceMax = 300.0             # Maximum distance in mm, corresponds to DiddyBorg driving at 100%
 
# Setup the library ready for use
import UltraBorg
# Board #1, address 10
UB1 = UltraBorg.UltraBorg()
UB1.i2cAddress = 10
UB1.Init()
# Board #2, address 11
#UB2 = UltraBorg.UltraBorg()
#UB2.i2cAddress = 11
#UB2.Init()
# Board #3, address 12
#UB3 = UltraBorg.UltraBorg()
#UB3.i2cAddress = 12
#UB3.Init()


# Start the UltraBorg
#UB = UltraBorg.UltraBorg()      # Create a new UltraBorg object
#UB.Init()                       # Set the board up (checks the board is connected)
 
# Setup the PicoBorg Reverse
# Power settings
voltageIn = 12.0                        # Total battery voltage to the PicoBorg Reverse
voltageOut = 6.0                        # Maximum motor voltage
 
# Setup the power limits
if voltageOut > voltageIn:
    maxPower = 1.0
else:
    maxPower = voltageOut / float(voltageIn)
 
# Calculate our divisor
distanceDiv = (distanceMax - distanceMin) / 2.0
 
# Loop over the sequence until the user presses CTRL+C
print 'Press CTRL+C to finish'
try:
    # Initial settings
    speed = 0.0
    driveLeft = 0.0
    driveRight = 0.0
    while True:
        # Read all four ultrasonic values, we use the filtered values so we get accurate readings
        usm1 = UB1.GetDistance1()
        usm2 = UB1.GetDistance2()
        usm3 = UB1.GetDistance3()
        usm4 = UB1.GetDistance4()
        # Convert to the nearest millimeter
        # usm1 = int(usm1)
        # usm2 = int(usm2)
        # usm3 = int(usm3)
        usm4 = int(usm4)
        # Determine the speed of DiddyBorg based on the distance readings
        # Wait between readings
	print 'Distance: ', usm1, usm2, usm3
        time.sleep(.1)
except KeyboardInterrupt:
    # User has pressed CTRL+C
    print 'Done'
