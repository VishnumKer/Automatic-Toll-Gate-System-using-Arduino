# Automatic Toll Gate System using Arduino

## Overview

The **Automatic Toll Gate System using Arduino** is an innovative IoT project that demonstrates how simple electronic components and a microcontroller can automate real-life processes like toll collection. This project is ideal for school students and anyone just starting in the field of electronics, showcasing the power of automation in everyday applications.

## Project Description

This intelligent toll gate system uses RFID technology and infrared sensors to create a fully automated, hands-free toll collection system. When a vehicle arrives at the toll gate, the system automatically detects the vehicle, verifies the balance on the RFID card, deducts the toll, and opens the gate—all without any manual intervention.

### How It Works

1. **Vehicle Detection**: An entry IR sensor detects when a vehicle arrives at the toll gate
2. **Card Verification**: The RFID reader scans the vehicle's payment card and checks for sufficient balance
3. **Toll Deduction**: If the balance is adequate, the Arduino automatically deducts the toll amount
4. **Gate Opening**: The servo motor opens the gate to allow the vehicle to pass
5. **Vehicle Exit Confirmation**: A second IR sensor confirms when the vehicle has completely passed through
6. **Gate Closing**: The gate automatically closes after the vehicle exits

