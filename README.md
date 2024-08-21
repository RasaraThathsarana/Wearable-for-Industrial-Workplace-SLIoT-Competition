# Smart Wearable for Workplace Safety

## Overview

This project was developed as part of the SLIoT 2023 competition. The aim was to create a smart wearable device designed to monitor and ensure the safety of workers in industrial environments. The device is capable of measuring various health and environmental parameters, and it communicates the collected data to a central monitoring system in real-time. The system triggers alerts and notifications for workers, supervisors, and emergency personnel in case of hazardous conditions.

## Motivation

Workplace safety is a critical concern, particularly in regions where safety regulations are often neglected. In Sri Lanka, approximately 4,000 workplace accidents are reported daily, leading to a significant loss of productive workdays. This project addresses the pressing need for effective occupational safety measures by providing a smart, non-invasive, and real-time monitoring solution that integrates seamlessly into the daily routines of workers.

## Features

The smart wearable device includes the following features:
- **Body Monitoring**: Measures body temperature, heart rate, blood oxygen level, and movement using a gyroscope and accelerometer.
- **Environmental Monitoring**: Detects ambient temperature, humidity, and harmful gases like carbon monoxide and nitrogen-based gases.
- **Real-Time Communication**: Sends data wirelessly to a central MySQL database using the MQTT protocol, facilitating real-time monitoring and alerting.
- **Edge Computing**: Data processing is distributed between edge computing units and a central server, with edge units handling immediate triggers and alerts.

## Modules

- **NodeMCU ESP8266**: The microcontroller used for gathering and transmitting sensor data.
- **MAX30102**: Sensor for measuring heart rate and blood oxygen levels.
- **L3G4200D**: Sensor for measuring acceleration and gyroscopic movements.
- **Ublox NEO-6M**: GPS module for location tracking.
- **DHT11**: Sensor for ambient temperature and humidity measurement.
- **MQ-4**: Sensor for detecting harmful gases.

## Communication

The wearable devices use the MQTT protocol for communication, managed through Node-Red workflows. Sensor data is processed and stored in a MySQL database for further analysis and monitoring. Critical alerts are managed at the edge, with major alerts being broadcasted to all devices within a specific area.

## Web Application

The web application serves as the interface for both workers and administrators:
- **Worker Dashboard**: Workers can log in to view real-time data from their wearables, receive notifications, and access safety recommendations.
- **Admin Panel**: Administrators can monitor aggregated workplace statistics, analyze data by different sections of the workplace, and respond to hazardous conditions.

![image](https://github.com/user-attachments/assets/24d0b89f-ee03-4e77-abc3-43c007922aa8)

![image](https://github.com/user-attachments/assets/a47561ce-2cfc-48b5-848c-c0905033d406)

![image](https://github.com/user-attachments/assets/b4c9c0b9-9d28-4705-a510-277c560e7b5e)

![image](https://github.com/user-attachments/assets/8582a12a-c17a-4339-a355-58ef020f79e8)

![image](https://github.com/user-attachments/assets/bcb02f9f-4520-446b-a620-30cb6dae59f1)

## Getting Started

### Prerequisites

To set up the project locally, you need the following software installed:

- **Node.js**: For running the backend server.
- **MySQL**: For managing the central database.
- **MQTT Broker**: We used Mosquitto for handling the MQTT communication.
- **Node-Red**: For setting up workflows to process sensor data.

### Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/k-ranasinghe/workplace-safety-wearable.git
   ```

2. **Install Node.js dependencies**:
   ```bash
   cd Wearable-for-Industrial-Workplace-SLIoT-Competition
   cd backend
   npm install

   cd frontend
   npm install
   ```

3. **Set up MySQL database**:
   
   Log in to your MySQL server using the command:

   ```bash
   mysql -u root -p
   ```

   Create a new database for the project:

   ```bash
   CREATE DATABASE SLIoT;
   ```

   Import the initial schema and data into the newly created database:

   ```bash
   mysql -u root -p SLIoT < ../DatabaseExport.sql
   ```

4. **Configure MQTT and Node-Red**:
   
   Install Mosquitto MQTT broker and configure it to listen for incoming messages.
   
   Setup Node-Red workflows as in the provided documents.

5. **Run the application**:
   ```bash
   npm start
   ```

   If the above steps have been follosed correctly the web application will open in your default browser on `(http://localhost:3000)`.


## Usage

Once set up, the wearable devices will start sending sensor data to the central server. The data can be monitored via the web application, and alerts will be triggered based on the configured safety thresholds. Workers will be notified through their wearables, and administrators can take necessary actions through the web interface.

## Acknowledgments

- SLIoT 2023 organizers for providing the platform for this project.
- The open-source community for providing tools and libraries used in this project.



   
