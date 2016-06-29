# Py-Guidance-SDK
Unofficial Guidance SDK in Python

[Official Guidance SDK](https://github.com/dji-sdk/Guidance-SDK)
## Install
```bash
git clone https://github.com/dji-sdk/Guidance-SDK.git
# for x86_64 (PC)
cp so/x64/libDJI_guidance.so ./
# for ARMv8 (Raspberry PI 3)
cp so/XU3/ibDJI_guidance.so ./
make
```
### Demo
```bash
#test.py
make test
```
### Support API
```
obstacle_distance
ultrasonic
velocity
imu (firmware version 1.3.7 and below)
```
other API will be supported soon
