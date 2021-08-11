#!/bin/sh

#setup: call "chmod +x canInitPi.sh" in terminal
#use: call "./canInitPi.sh&" with the & setting it as a background task so the terminal can be reused.
sudo ip link set can0 up type can bitrate 500000
sudo ip link set can1 up type can bitrate 500000
sudo ifconfig can0 txqueuelen 65536
sudo ifconfig can1 txqueuelen 65536
echo "logging to canLog.txt"
candump can0 >> canLog.txt
