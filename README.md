# Secure Car Key Fob System Resistant to Replay and Relay Attacks

## Overview
This project presents the design and implementation of a secure wireless vehicle access system that demonstrates vulnerabilities in traditional Remote Keyless Entry (RKE) systems and provides practical countermeasures against replay and relay attacks.

The system was developed as a Senior Design Project at Qatar University and follows a three stage architecture:

1. Baseline wireless communication system  
2. Replay and relay attack demonstration  
3. Secure rolling code authentication system  

The project demonstrates how insecure RF communication can be exploited and how lightweight embedded security mechanisms can improve wireless system protection.

---

## Features

- Wireless lock and unlock functionality
- RF communication using 433 MHz modules
- Replay attack demonstration
- Relay attack demonstration
- Rolling code authentication mechanism
- Dynamic code generation using counters and XOR transformation
- Replay attack prevention
- LED and buzzer feedback system
- Real time embedded implementation using ESP32

---

## System Architecture

### Stage 1 – Baseline System
Implements a basic wireless communication system using static RF signals for lock and unlock operations.

### Stage 2 – Attack Demonstration
Demonstrates:
- Replay attacks
- Relay attacks

to highlight vulnerabilities in static communication systems.

### Stage 3 – Secure System
Implements a rolling code based authentication mechanism using:
- synchronized counters
- shared secret key
- transformation function

to ensure every transmitted signal is unique and resistant to replay attacks.

---

## Hardware Components

- ESP32 Microcontrollers
- 433 MHz RF Transmitter (FS1000A)
- 433 MHz RF Receiver (XY-MK-5V)
- Push Buttons
- LEDs
- Buzzer
- Breadboard and Jumper Wires

---

## Software and Tools

- Arduino IDE
- Embedded C/C++
- ESP NOW Protocol
- RF Communication Libraries
- Serial Monitor for debugging

---

## Security Mechanism

The secure mode uses a rolling code authentication mechanism where:
1. The key increments an internal counter
2. The counter is transformed using a shared secret key
3. A unique encoded signal is transmitted
4. The receiver validates the received signal within a synchronization window

This prevents reuse of previously captured RF signals.

---

## Testing and Evaluation

The system was tested across multiple stages:

### Functional Testing
- Lock and unlock operations verified
- RF transmission reliability validated

### Replay Attack Testing
- Captured signals successfully replayed in insecure mode

### Relay Attack Testing
- Real time signal forwarding demonstrated

### Secure System Testing
- Replayed signals rejected successfully
- Stable communication maintained

---

## Results

- Communication reliability achieved approximately 97%
- Replay attacks successfully mitigated in secure mode
- Stable low latency operation achieved
- Successful implementation of rolling code authentication

---

## Team Members

- Amna Abdulaziz AlObaidli
- Al-Dana Khalid AlTamimi
- Latifa Ali Albuenain
- Muneera Abdulla Alkubaisi

Supervisor:
Dr. Qutaibah Malluhi

Department of Computer Science and Engineering  
Qatar University

---

## Future Improvements

- AES based encryption
- Anti relay protection mechanisms
- PCB implementation
- Mobile application integration
- Cloud connectivity
- Enhanced synchronization recovery

---

## License

This project was developed for educational and research purposes only.

Attack demonstrations were conducted strictly in controlled environments on project owned prototype systems.
