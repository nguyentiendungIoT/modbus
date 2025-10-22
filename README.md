# STM32 Modbus Communication Library

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![STM32](https://img.shields.io/badge/STM32-HAL-blue.svg)](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html)
[![Modbus](https://img.shields.io/badge/Modbus-RTU%20%7C%20TCP-green.svg)](https://modbus.org/)

A professional, production-ready Modbus communication library for STM32 microcontrollers supporting both Modbus RTU (serial) and Modbus TCP (Ethernet) protocols.

## 📋 Table of Contents

- [Features](#features)
- [Supported Protocols](#supported-protocols)
- [Hardware Requirements](#hardware-requirements)
- [Directory Structure](#directory-structure)
- [Getting Started](#getting-started)
- [Usage Examples](#usage-examples)
- [API Documentation](#api-documentation)
- [Contributing](#contributing)
- [License](#license)

## ✨ Features

- ✅ **Modbus RTU** (RS-485/RS-232) implementation
- ✅ **Modbus TCP** (Ethernet) implementation
- ✅ Master and Slave modes support
- ✅ Standard Modbus functions:
  - 0x01: Read Coils
  - 0x02: Read Discrete Inputs
  - 0x03: Read Holding Registers
  - 0x04: Read Input Registers
  - 0x05: Write Single Coil
  - 0x06: Write Single Register
  - 0x0F: Write Multiple Coils
  - 0x10: Write Multiple Registers
- ✅ CRC16 error checking (RTU)
- ✅ TCP checksum validation
- ✅ Timeout handling
- ✅ Non-blocking operation support
- ✅ Thread-safe implementation
- ✅ Low memory footprint
- ✅ HAL library integration
- ✅ RTOS compatible

## 🔌 Supported Protocols

### Modbus RTU
- Serial communication over RS-485/RS-232
- Baud rates: 9600, 19200, 38400, 57600, 115200
- Data bits: 8
- Parity: None, Even, Odd
- Stop bits: 1, 2

### Modbus TCP
- Ethernet communication
- Port: 502 (default)
- Multiple simultaneous connections support

## 🔧 Hardware Requirements

### Minimum Requirements
- **MCU**: STM32F1/F4/F7/H7 series
- **Flash**: 32KB minimum
- **RAM**: 8KB minimum
- **Peripherals**: 
  - UART/USART (for Modbus RTU)
  - Ethernet MAC (for Modbus TCP)

### Recommended Development Boards
- STM32F103C8T6 (Blue Pill) - for RTU
- STM32F407VGT6 (Discovery) - for RTU/TCP
- STM32F429ZI (Nucleo-144) - for RTU/TCP
- STM32H743ZI (Nucleo-144) - for RTU/TCP

### External Components
- RS-485 transceiver (e.g., MAX485, SP3485) for Modbus RTU
- Ethernet PHY (e.g., LAN8720, DP83848) for Modbus TCP

## 📁 Directory Structure

```
modbus/
├── src/                    # Source files
│   ├── modbus_rtu.c       # Modbus RTU implementation
│   ├── modbus_tcp.c       # Modbus TCP implementation
│   ├── modbus_common.c    # Common Modbus functions
│   └── modbus_crc.c       # CRC16 calculation
├── inc/                    # Header files
│   ├── modbus_rtu.h       # Modbus RTU API
│   ├── modbus_tcp.h       # Modbus TCP API
│   ├── modbus_common.h    # Common definitions
│   └── modbus_config.h    # Configuration options
├── examples/              # Example implementations
│   ├── rtu_master/        # RTU master example
│   ├── rtu_slave/         # RTU slave example
│   ├── tcp_master/        # TCP master example
│   └── tcp_slave/         # TCP slave example
├── docs/                  # Documentation
│   ├── API.md            # API documentation
│   ├── SETUP.md          # Setup guide
│   └── HARDWARE.md       # Hardware configuration
├── hardware/             # Hardware schematics
│   ├── rs485_interface/  # RS-485 interface designs
│   └── ethernet/         # Ethernet interface designs
├── tests/                # Unit tests
└── README.md            # This file
```

## 🚀 Getting Started

### Prerequisites

1. **STM32CubeIDE** or **Keil MDK** or **IAR EWARM**
2. **STM32CubeMX** (for peripheral configuration)
3. **Git** (for version control)
4. **ARM GCC toolchain** (if using command line)

### Installation

1. Clone the repository:
```bash
git clone https://github.com/nguyentiendungIoT/modbus.git
cd modbus
```

2. Add the library to your STM32 project:
   - Copy `src/` and `inc/` folders to your project
   - Add include path to `inc/` directory
   - Add source files to your build configuration

3. Configure your hardware using STM32CubeMX:
   - Enable required UART/USART for Modbus RTU
   - Enable Ethernet for Modbus TCP
   - Configure GPIO pins for RS-485 DE/RE control

### Quick Start Example (Modbus RTU Slave)

```c
#include "modbus_rtu.h"

// Initialize Modbus RTU
modbus_rtu_t mb;
modbus_rtu_init(&mb, &huart1, 1, 9600, MODBUS_PARITY_NONE);

// Register data
uint16_t holding_registers[100];
uint16_t input_registers[100];
uint8_t coils[100];
uint8_t discrete_inputs[100];

// Main loop
while(1) {
    modbus_rtu_poll(&mb);
}
```

## 📖 Usage Examples

Detailed examples can be found in the `examples/` directory:

- **[RTU Master](examples/rtu_master/)** - Read/write data from RTU slave devices
- **[RTU Slave](examples/rtu_slave/)** - Respond to RTU master requests
- **[TCP Master](examples/tcp_master/)** - Control devices over Ethernet
- **[TCP Slave](examples/tcp_slave/)** - Serve data over TCP/IP

## 📚 API Documentation

### Initialization

```c
// RTU initialization
modbus_rtu_init(modbus_rtu_t *mb, UART_HandleTypeDef *huart, 
                uint8_t slave_id, uint32_t baudrate, 
                modbus_parity_t parity);

// TCP initialization  
modbus_tcp_init(modbus_tcp_t *mb, uint16_t port);
```

### Reading Data

```c
// Read holding registers
modbus_read_holding_registers(mb, slave_id, start_addr, count, buffer);

// Read input registers
modbus_read_input_registers(mb, slave_id, start_addr, count, buffer);

// Read coils
modbus_read_coils(mb, slave_id, start_addr, count, buffer);
```

### Writing Data

```c
// Write single register
modbus_write_single_register(mb, slave_id, address, value);

// Write multiple registers
modbus_write_multiple_registers(mb, slave_id, start_addr, count, buffer);

// Write single coil
modbus_write_single_coil(mb, slave_id, address, value);
```

For complete API documentation, see [docs/API.md](docs/API.md)

## 🔧 Configuration

Edit `inc/modbus_config.h` to customize:

```c
#define MODBUS_RTU_ENABLED        1    // Enable/disable RTU
#define MODBUS_TCP_ENABLED        1    // Enable/disable TCP
#define MODBUS_MAX_FRAME_SIZE     256  // Maximum frame size
#define MODBUS_TIMEOUT_MS         1000 // Response timeout
#define MODBUS_MAX_RETRIES        3    // Maximum retries
```

## 🧪 Testing

The library includes unit tests in the `tests/` directory. To run tests:

```bash
cd tests
make test
```

## 🤝 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for details on:
- Code style guidelines
- Pull request process
- Development workflow
- Testing requirements

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🌟 Acknowledgments

- Modbus Organization for protocol specification
- STMicroelectronics for STM32 HAL library
- FreeModbus project for inspiration
- Community contributors

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/nguyentiendungIoT/modbus/issues)
- **Discussions**: [GitHub Discussions](https://github.com/nguyentiendungIoT/modbus/discussions)
- **Email**: support@example.com

## 🗺️ Roadmap

- [ ] Add Modbus ASCII support
- [ ] Implement Modbus Gateway functionality
- [ ] Add diagnostic functions
- [ ] Create Python test tools
- [ ] Add more hardware examples
- [ ] Create GUI configuration tool

---

**Made with ❤️ for the embedded community**