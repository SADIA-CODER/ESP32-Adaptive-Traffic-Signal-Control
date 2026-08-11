# ESP32-Based Adaptive Traffic Signal Control System

An ESP32-based adaptive traffic signal control system designed to dynamically manage traffic signals according to real-time vehicle density.

The system uses non-vision sensors, including IR and ultrasonic sensors, to detect vehicle presence and estimate traffic density. Based on the detected traffic level, the ESP32 dynamically assigns green-light duration to the lane with higher traffic.

The system also provides emergency vehicle priority and pedestrian crossing functionality.

## Project Overview

Traditional traffic signals generally operate using fixed timing schedules. Such systems cannot respond effectively to changing traffic conditions and may result in unnecessary waiting, fuel consumption, and congestion.

This project addresses this problem by implementing an adaptive traffic signal controller using an ESP32 microcontroller.

The system continuously monitors two traffic lanes using IR and ultrasonic sensors. Vehicle density is classified as LOW, MEDIUM, or HIGH, and the green signal duration is adjusted accordingly.

## Key Features

- Real-time vehicle presence detection
- Traffic density estimation
- Adaptive traffic signal timing
- ESP32-based control
- IR sensors for vehicle detection
- Ultrasonic sensors for distance measurement
- Emergency vehicle priority
- Pedestrian crossing mechanism
- Serial Monitor monitoring
- Three traffic density levels
- Automatic red/yellow/green signal control
- Low-cost non-vision sensing approach

## Traffic Density Classification

The system estimates traffic density based on ultrasonic distance measurements.

| Distance | Traffic Level | Green Time |
|----------|---------------|------------|
| 5–20 cm | HIGH | 20 seconds |
| 20–40 cm | MEDIUM | 12 seconds |
| >40 cm | LOW | 6 seconds |

The lane with the higher density score is given priority.

## System Modes

### 1. Normal Adaptive Mode

Under normal operation:

1. IR sensors detect vehicle presence.
2. Ultrasonic sensors measure vehicle distance.
3. Traffic density is classified.
4. The lane with higher traffic receives priority.
5. The selected lane turns GREEN.
6. The signal changes to YELLOW.
7. The lane turns RED.
8. The system checks traffic again.

### 2. Emergency Mode

A dedicated IR sensor is used for emergency vehicle detection.

When an emergency vehicle is detected:

- Lane 1 immediately receives GREEN.
- Lane 2 remains RED.
- Emergency priority remains active for 10 seconds.
- The system then returns to adaptive traffic control.

### 3. Pedestrian Mode

A push button allows pedestrians to request crossing time.

When pressed:

- Both lanes turn RED.
- Pedestrians are given 5 seconds to cross.
- Adaptive traffic control then resumes.

## System Architecture

The system consists of four main layers:

### Sensing Layer

- IR Sensor 1
- IR Sensor 2
- Ultrasonic Sensor 1
- Ultrasonic Sensor 2
- Emergency IR Sensor
- Pedestrian Push Button

### Processing Layer

- ESP32 Microcontroller

### Actuation Layer

- Lane 1 Traffic Lights
- Lane 2 Traffic Lights

### Communication Layer

- Serial Monitor

## Block Diagram

```text
       IR Sensors
           │
           │
    Ultrasonic Sensors
           │
           ▼
    ┌──────────────┐
    │    ESP32     │
    │ Microcontroller│
    └──────┬───────┘
           │
     ┌─────┴─────┐
     │           │
     ▼           ▼
Traffic Lights  Serial
                Monitor

Emergency IR ─────► ESP32
Pedestrian Button ─► ESP32
