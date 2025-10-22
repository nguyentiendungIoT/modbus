# RS-485 Interface Schematics

This directory contains hardware design files for RS-485 interface circuits.

## Available Designs

### MAX485 Basic Circuit

Simple RS-485 interface using MAX485 transceiver.

**Features:**
- Single 3.3V/5V supply
- DE/RE controlled by single GPIO
- Basic protection

**Use case:** Simple, low-cost applications

### SP3485 Enhanced Circuit

Industrial-grade RS-485 interface with enhanced protection.

**Features:**
- Built-in fail-safe
- TVS diode protection
- Isolated power option
- LED indicators

**Use case:** Industrial applications requiring robustness

## License

Hardware designs are shared under CERN Open Hardware License v2 - Permissive.
