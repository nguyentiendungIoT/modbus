# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Initial Modbus RTU implementation
  - Master and Slave modes
  - Standard function codes (0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x0F, 0x10)
  - CRC16 error checking
  - RS-485 DE/RE control
  - Interrupt-based UART communication
- Initial Modbus TCP implementation (stub)
  - Basic structure for TCP/IP communication
  - MBAP header handling
  - Integration points for lwIP stack
- Comprehensive documentation
  - API reference
  - Setup guide
  - Hardware configuration guide
  - Code examples for RTU master/slave
  - Code examples for TCP master/slave
- Project infrastructure
  - MIT License
  - Contributing guidelines
  - Code of Conduct
  - .gitignore for embedded development
- Example implementations
  - RTU Master example
  - RTU Slave example
  - TCP Master example (stub)
  - TCP Slave example (stub)

## [1.0.0] - TBD

### Planned Features
- Complete Modbus TCP implementation with lwIP
- Additional function codes support
- DMA support for UART
- RTOS integration examples
- Unit test framework
- Python test tools
- More hardware examples

---

## Version History

### Legend
- `Added` - New features
- `Changed` - Changes in existing functionality
- `Deprecated` - Soon-to-be removed features
- `Removed` - Removed features
- `Fixed` - Bug fixes
- `Security` - Vulnerability fixes
