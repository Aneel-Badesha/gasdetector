# Multi Purpose Fire and Gas Detector

An embedded fire and gas detection system built with C programming language on Debian Linux and cross-compiled for deployment on BeagleBone Green hardware. This system provides real-time monitoring and alerting capabilities for fire and gas hazards.

## Overview

This project implements a comprehensive fire and gas detection system using multiple sensors for redundant safety monitoring. The system features visual alarm capabilities and is designed for reliable operation in safety-critical environments.

## Hardware Components

### Target Platform
- **BeagleBone Green**: Main processing unit and GPIO controller

### Sensors (5 total)
- Fire detection sensors
- Gas detection sensors
- Connected via GPIO ports for digital/analog input

### Display & Alerting
- **8x8 LED Matrix**: Visual alarm display connected via I2C interface
- Provides clear visual indication of alarm states

## Software Architecture

### Development Environment
- **Development OS**: Debian Linux
- **Programming Language**: C
- **Deployment**: Cross-compiled for ARM architecture (BeagleBone Green)

### Key Features
- Real-time sensor monitoring
- GPIO interface management
- I2C communication for LED matrix control
- Alarm state management
- Cross-platform compilation support

## System Design

### Sensor Integration
- Multiple sensor inputs for enhanced reliability
- GPIO-based sensor interfacing
- Configurable threshold detection

### Alert System
- 8x8 LED matrix for visual warnings
- I2C protocol for display communication
- Customizable alarm patterns

### Safety Features
- Multi-sensor redundancy
- Real-time monitoring
- Fail-safe alarm mechanisms

## Development Workflow

1. **Development**: Code written and tested on Debian Linux
2. **Cross-compilation**: Built for ARM target architecture
3. **Deployment**: Transferred and executed on BeagleBone Green
4. **Testing**: Hardware-in-the-loop verification

## Getting Started

### Prerequisites
- Linux development environment
- ARM cross-compilation toolchain
- BeagleBone Green with required sensors
- 8x8 LED matrix display

### Hardware Setup
1. Connect 5 sensors to designated GPIO ports
2. Wire 8x8 LED matrix to I2C interface
3. Ensure proper power supply for all components

### Software Deployment
1. Cross-compile the C source code for ARM architecture
2. Transfer executable to BeagleBone Green
3. Configure GPIO permissions and I2C access
4. Run the fire detection system

## Technical Specifications

- **Target Architecture**: ARM (BeagleBone Green)
- **Communication Protocols**: GPIO, I2C
- **Sensor Count**: 5 detection units
- **Display**: 8x8 LED matrix
- **Real-time Operation**: Continuous monitoring

This embedded system provides a robust foundation for fire and gas detection applications with reliable hardware interfacing and cross-platform development capabilities.
