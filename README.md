# Overview

This is a project done for the SLIoT 2023 competition. A wearable was built with the capabilites 
of measuring Body Temperature, Heart Rate, Blood Oxygen Level, Gyroscope, Accelerometer, Ambient 
Temperature, Humidity, and Harmful Gas(Carbon, Nitrogen based gases) Monitoring. The sensor data 
is sent to a central database where the monitoring happens. In case of a trigger the user, 
supervisors, and emergency personnel are notified.

## Modules
* NodeMCU ESP8266 - Micro-Controller
* MAX30102 - Heart Rate, Blood Oxygen
* L3G4200D - Accelerometer, Gyroscope
* Ublox NEO-6M - GPS
* DHT11 - Ambient Temperature, Humidity
* MQ-4 - Gas Sensor

## Communication
The wearables send and receive messages using the MQTT protocol setup using Node-Red workflows. 
The sensor data is received to the Arduino IDE and also the MySQL database is also updated via Node-Red.

## Usage
The wearable sends the sensor data to edge computing devices, which are small devices fixed 
around the work area with a small amount of compute. The edge computing units cover different 
areas of the workplace. They collect the data and send them to the central server. The central 
server is used for monitoring and data analytics. <br>

Most of the triggers will be handled at the edge computing units, where a broadcast would be sent 
in case of a trigger. The wearable is equiped with a buzzer which would activate when a broadcast 
is received. Then the user can check the exact trigger using the web application. <br>

There are two types of messages sent to the wearable. Minor broadcasts which is sent to the wearable 
which triggered, major broadcast which is sent to all the devices covered by the edge unit. Major 
broadcasts include critical or potentially hazardous situations. Then the wearers need to immediately 
leave the area and emergency personnel are notified by the central server.

## Web application
The we app consists of a user login through which the workers can view the dashboard for their wearable. 
All the notifications and recommendations will be displayed in here as well. <br>

There is also an Admin panel to view grouped statistics of the workplace. The landing page will contains 
the statistics for the sections of the workplace. there are areas covered by individual edge computing 
units. To further analyze the data we can access the detailed view of that section. <br><br>

![image](https://github.com/user-attachments/assets/24d0b89f-ee03-4e77-abc3-43c007922aa8)

![image](https://github.com/user-attachments/assets/a47561ce-2cfc-48b5-848c-c0905033d406)

![image](https://github.com/user-attachments/assets/b4c9c0b9-9d28-4705-a510-277c560e7b5e)

![image](https://github.com/user-attachments/assets/8582a12a-c17a-4339-a355-58ef020f79e8)

![image](https://github.com/user-attachments/assets/bcb02f9f-4520-446b-a620-30cb6dae59f1)
