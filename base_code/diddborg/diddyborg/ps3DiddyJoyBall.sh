#!/bin/bash

# Starts sixad and DiddyBorg after a delay interval
sleep 10
sixad --start &
sleep 10
/home/pi/diddyborg/diddyJoyBall.py > /dev/null
