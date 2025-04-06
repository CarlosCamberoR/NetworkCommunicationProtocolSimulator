# Network Communication Protocol Simulator

## Overview
This project implements a communication protocol simulator between master and slave stations over a network interface. It was developed as part of the "Fundamentos de Redes de Comunicaciones" (Network Communication Fundamentals) course.

## Features
The application simulates communication between stations with the following features:

### Station Modes
- **Master Mode**: Initiates communication and controls the flow
- **Slave Mode**: Responds to master requests

### Communication Functions
- **F1**: Interactive character sending between stations
- **F2**: File transfer functionality
- **F3**: Stop-and-wait protocol implementation with:
  - Selection mode
  - Polling mode
- **F4**: Error introduction during protocol execution for testing

## Compilation and Execution

### Prerequisites
- GCC compiler
- The provided linkLayer library

### Compilation
Compile the project using:

### Execution
Run the application with:

## Usage
When started, the application asks to choose a mode:

- [1] Master
- [2] Slave

Based on the selected mode, a menu of functions will be presented:

- For Master: F1, F2, F3, F4 options and ESC to exit
- For Slave: F1, F3, F4 options and ESC to exit

The stations will establish communication and allow data transmission according to the selected function.

## File Structure
- **Sesion0.cpp**: Main program entry point
- **estacion.cpp/h**: Network interface and station mode handling
- **enviar.cpp/h**: Sending functionality implementation
- **recibir.cpp/h**: Receiving functionality implementation
- **linkLayer.h**: External library for network communication
- **Protocol files**: EProtoc.txt, RProtoc.txt for protocol testing
- **envio.txt**: Sample file for sending

## Protocol Details
The project implements custom communication protocols with proper frame construction, error checking, and ACK/NACK responses. The stop-and-wait protocol ensures reliable delivery with retransmission capability when errors are detected.
