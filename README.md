# Socket Programming in WSL

A comprehensive educational repository for understanding the fundamentals of network communication and socket programming using Linux system calls.

## Overview

This repository is designed to help learners understand the **basics of network communication** through practical implementations and simulations. It covers essential networking concepts from IP addressing to transport layer protocols, providing hands-on experience with socket programming on Windows Subsystem for Linux (WSL).

## Contents

### 1. **IP Address Fundamentals**

This section covers the mathematical and practical aspects of IP addressing:

- **Subnet Mask Calculations**: Mathematical implementation for calculating the number of possible IP addresses within a subnet using various subnet masks
- **IP Address Validation**: Finding valid and invalid configurations of IP addresses based on subnet requirements
- **Point-to-Point Communication**: Understanding direct communication between IP addresses, including dedicated links and their configurations

### 2. **Dijkstra's Algorithm Implementation**

An implementation of Dijkstra's shortest path algorithm specifically applied to network routing:

- Finds the shortest distances between IP routers in a network
- Useful for understanding fundamental routing concepts
- Demonstrates how network protocols determine optimal paths for data transmission

### 3. **TCP and UDP Simulations**

Educational simulations of Transport Layer protocols using Linux system calls:

#### TCP Simulation
- Demonstrates the **three-way handshake** mechanism (SYN, SYN-ACK, ACK)
- Shows how TCP establishes reliable connections
- Useful for understanding connection-oriented communication

**Note**: This simulation is educational in nature and not production-ready. It lacks some crucial TCP functionality including:
  - Buffer management
  - Segment and sequence number handling
  - Flow control mechanisms
  - Congestion control algorithms

#### UDP Simulation
- Implements the connectionless datagram protocol
- Demonstrates unreliable but faster communication
- Useful for understanding connectionless communication patterns

## Learning Objectives

By exploring this repository, you will gain understanding of:

- ✅ How IP addresses work and their mathematical properties
- ✅ Subnet calculations and address space planning
- ✅ Network routing fundamentals using shortest path algorithms
- ✅ How TCP establishes connections through handshakes
- ✅ Differences between TCP and UDP protocols
- ✅ Linux socket programming using system calls

## Technologies Used

- **Language**: C++
- **Platform**: Windows Subsystem for Linux (WSL)
- **API**: POSIX socket programming (Linux system calls)

## Getting Started

### Prerequisites

- Windows Subsystem for Linux (WSL) or any Linux environment
- C++ compiler (g++, clang)
- Basic understanding of networking concepts

### Building and Running

Refer to individual directories for specific compilation and execution instructions.

## Author

[ShubhamAbhayDeshpande](https://github.com/ShubhamAbhayDeshpande)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

**Happy Learning!** 🚀
