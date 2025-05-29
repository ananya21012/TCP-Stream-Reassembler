# TCP Stream Reassembler

A C++ implementation of a simplified TCP receiver that reassembles byte streams using sequence numbers and handles wrapping and out-of-order data segments.

📁 [GitHub Repository](https://github.com/ananya21012/TCP-Stream-Reassembler)

## 📌 Overview

This project was implemented as part of a **Computer Networks** assignment at IIIT Delhi. It simulates core TCP receiver logic including:

- Stream reassembly
- Sequence number wrapping using 32-bit integers
- Byte stream management with capacity limits

## 🚀 Components

- **`ByteStream`**: Stores incoming bytes with limited capacity and supports reading/peeking.
- **`StreamReassembler`**: Buffers out-of-order segments and assembles them in-order.
- **`TCPReceiver`**: Manages TCP state and interacts with the reassembler.
- **`WrappingInt32`**: Implements logic for 32-bit wrapping sequence numbers.


## 🛠️ Build Instructions

### Prerequisites

- C++17 compatible compiler
- CMake ≥ 3.10

### Steps

```bash
git clone https://github.com/ananya21012/TCP-Stream-Reassembler.git
cd TCP-Stream-Reassembler
mkdir build && cd build
cmake ..
make
```
To Run Tests:
```bash
ctest
```

📈 Use Cases
- Educational simulation of TCP internals
- Byte stream assembly and reliability analysis
- Testbed for implementing TCP-like behaviors

📚 Related Topics
- TCP Segment Reordering
- Stream-based Data Transmission
- Network Protocol Simulation
- Byte-wise Buffer Management

📄 License
MIT License
