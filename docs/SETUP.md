# Setup Guide

## Prerequisites

### Hardware
- STM32 microcontroller (F1/F4/F7/H7 series)
- RS-485 transceiver (for Modbus RTU)
- Ethernet PHY (for Modbus TCP)
- USB-UART adapter (for development/debugging)

### Software
- STM32CubeIDE or Keil MDK or IAR EWARM
- STM32CubeMX (for peripheral configuration)
- Git
- Serial terminal (PuTTY, Tera Term, etc.)
- Modbus testing tools (ModbusPoll, QModMaster, etc.)

## Installation Steps

### 1. Clone Repository

```bash
git clone https://github.com/nguyentiendungIoT/modbus.git
cd modbus
```

### 2. Create New STM32 Project

Open STM32CubeMX and create a new project for your target MCU.

### 3. Configure Peripherals

#### For Modbus RTU:

**UART Configuration:**
1. Select UART peripheral (e.g., USART1)
2. Mode: Asynchronous
3. Baud Rate: 9600 (or desired)
4. Word Length: 8 Bits
5. Parity: None
6. Stop Bits: 1
7. Enable global interrupt

**GPIO Configuration (DE/RE control):**
1. Select a GPIO pin (e.g., PA8)
2. Set as GPIO_Output
3. GPIO output level: Low (receive mode)
4. Pull-up/Pull-down: No pull

#### For Modbus TCP:

**Ethernet Configuration:**
1. Enable ETH peripheral
2. Configure PHY type (RMII/MII)
3. Set MAC address

**lwIP Middleware:**
1. Enable lwIP
2. Configure IP address (static or DHCP)
3. Enable TCP protocol
4. Adjust buffer sizes if needed

### 4. Generate Code

Generate initialization code in STM32CubeMX.

### 5. Add Modbus Library

Copy library files to your project:

```bash
# Copy source files
cp -r modbus/src/* YourProject/Core/Src/
cp -r modbus/inc/* YourProject/Core/Inc/
```

Or add as Git submodule:

```bash
cd YourProject
git submodule add https://github.com/nguyentiendungIoT/modbus.git Middlewares/modbus
```

### 6. Configure Include Paths

In your IDE:
- Add `modbus/inc` to include paths
- Ensure STM32 HAL headers are accessible

### 7. Update HAL Include

Edit `modbus_rtu.h` to match your STM32 series:

```c
#include "stm32f4xx_hal.h"  // Change f4 to your series (f1, f7, h7, etc.)
```

### 8. Build Project

Compile the project and fix any errors.

## Hardware Setup

### Modbus RTU Wiring

**RS-485 Connection (MAX485 example):**

```
STM32          MAX485
------         ------
UART_TX   ---> DI
UART_RX   <--- RO
PA8       ---> DE
PA8       ---> RE
GND       ---> GND
3.3V/5V   ---> VCC
```

**Bus Termination:**
- Add 120Ω resistor between A and B at both ends of the bus
- Connect A to A and B to B on all devices

### Modbus TCP Wiring

**Ethernet Connection:**

```
STM32           PHY (LAN8720)
------          --------------
ETH_RMII_TXD0   ---> TXD0
ETH_RMII_TXD1   ---> TXD1
ETH_RMII_TX_EN  ---> TXEN
ETH_RMII_RXD0   <--- RXD0
ETH_RMII_RXD1   <--- RXD1
ETH_RMII_CRS_DV <--- CRS_DV
ETH_MDC         ---> MDC
ETH_MDIO        <--> MDIO
ETH_RMII_REF_CLK<--- REFCLK (50MHz)
```

## Testing

### Test Modbus RTU

1. **Connect Hardware:**
   - Connect STM32 RS-485 to a USB-RS485 adapter
   - Or connect to another Modbus RTU device

2. **Configure Master Software:**
   - Tool: ModbusPoll, QModMaster
   - Port: COM port of USB-RS485
   - Baud: 9600 (or configured rate)
   - Parity: None
   - Slave ID: 1 (or configured)

3. **Test Operations:**
   - Read holding registers 0-1
   - Write to holding register 0
   - Verify responses

### Test Modbus TCP

1. **Connect to Network:**
   - Connect STM32 Ethernet to router/switch
   - Verify link LED

2. **Find IP Address:**
   - Use serial console to print IP
   - Or check DHCP lease table

3. **Configure Master Software:**
   - Tool: ModbusPoll, QModMaster
   - Protocol: Modbus TCP/IP
   - IP Address: STM32's IP
   - Port: 502
   - Unit ID: 1

4. **Test Operations:**
   - Read holding registers
   - Write registers
   - Monitor traffic with Wireshark

## Troubleshooting

### Modbus RTU Issues

**No Communication:**
- Check UART configuration matches (baud, parity, stop bits)
- Verify wiring (TX->RO, RX->DI)
- Check DE/RE pin toggles correctly
- Measure voltage levels on bus

**CRC Errors:**
- Check data integrity
- Verify bus termination (120Ω resistors)
- Reduce baud rate
- Check for noise/interference

**Timeout Errors:**
- Increase timeout value
- Check slave ID matches
- Verify slave is responding

### Modbus TCP Issues

**No Network Connection:**
- Check Ethernet cable
- Verify PHY configuration (RMII/MII)
- Check link LED
- Verify IP configuration

**Cannot Connect:**
- Ping device IP
- Check firewall settings
- Verify port 502 is open
- Check lwIP configuration

**Slow Response:**
- Increase TCP buffers
- Check network latency
- Optimize lwIP settings

## Performance Optimization

### RTU Optimization

1. **Increase Baud Rate:** Use 115200 for faster communication
2. **Minimize Processing:** Keep interrupt handlers short
3. **Use DMA:** Enable UART DMA for large transfers
4. **Adjust Timeouts:** Fine-tune inter-frame delays

### TCP Optimization

1. **Buffer Sizing:** Increase TCP buffers for throughput
2. **Connection Pooling:** Reuse connections
3. **Polling Frequency:** Optimize MX_LWIP_Process() calls
4. **Zero Copy:** Use lwIP zero-copy features

## Next Steps

- Review [API Documentation](API.md)
- Check [Hardware Schematics](HARDWARE.md)
- Try [Example Projects](../examples/)
- Read [Contributing Guide](../CONTRIBUTING.md)
