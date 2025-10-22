# Hardware Configuration Guide

## Overview

This guide provides detailed hardware setup information for Modbus RTU and Modbus TCP implementations.

## Modbus RTU Hardware

### RS-485 Transceiver Options

#### MAX485 (3.3V/5V)

**Features:**
- Low power consumption
- Half-duplex communication
- Single 5V or 3.3V supply
- 2.5 Mbps maximum data rate

**Pinout:**
```
Pin  | Name | Description
-----|------|------------
1    | RO   | Receiver Output (to MCU RX)
2    | RE   | Receiver Enable (active low)
3    | DE   | Driver Enable (active high)
4    | DI   | Driver Input (from MCU TX)
5    | GND  | Ground
6    | A    | Non-inverting bus line
7    | B    | Inverting bus line
8    | VCC  | Power supply (3.3V or 5V)
```

**Connection Schematic:**
```
                    +3.3V/5V
                       |
                       +----[VCC]
                               |
STM32                      MAX485
-----                      ------
PA2 (TX)  ----------->  4  DI
PA3 (RX)  <-----------  1  RO
PA8       ----------->  2  RE
PA8       ----------->  3  DE
GND       ----------->  5  GND
                        6  A  ----+---- RS-485 Bus A
                        7  B  ----+---- RS-485 Bus B
                               |
                             [120Ω] (termination resistor)
                               |
                              GND
```

**Recommended Circuit:**
```
                +3.3V
                  |
                  R1 (10kΩ) Pull-up for RE/DE
                  |
     MCU GPIO ----+---- RE/DE
                  |
                  |
     Tx ------------------- DI
     Rx ------------------- RO
     
     A ----[120Ω]---- B (at bus ends only)
```

#### SP3485 (3.3V)

**Features:**
- True 3.3V operation
- Full fail-safe protection
- Extended common mode range
- 10 Mbps maximum data rate

**Pin Configuration:** Similar to MAX485

#### SN65HVD72 (3.3V, Automotive)

**Features:**
- Enhanced ESD protection
- Built-in fail-safe biasing
- -7V to +12V bus common-mode range
- Ideal for industrial applications

### Bus Topology

#### Linear Bus (Recommended)
```
[Master/Slave 1] ---120Ω--- [Slave 2] --- [Slave 3] ---120Ω
      |                          |              |
      A/B                       A/B            A/B
```

**Rules:**
- Maximum 32 devices (with standard transceivers)
- Maximum cable length: 1200m at 9600 baud
- Terminate both ends with 120Ω resistors
- Use twisted pair cable (recommended: Belden 3105A or equivalent)

#### Star Topology (Not Recommended)
- Causes signal reflections
- Reduces maximum distance
- Can cause communication errors

### Cable Specifications

**Recommended Cable:**
- Type: Twisted pair, shielded
- Impedance: 120Ω
- Wire gauge: 22-24 AWG
- Examples: Belden 3105A, Alpha Wire 6450

**Maximum Distances:**
| Baud Rate | Maximum Distance |
|-----------|------------------|
| 9600      | 1200m           |
| 19200     | 1200m           |
| 38400     | 1000m           |
| 57600     | 750m            |
| 115200    | 500m            |

### Termination Resistors

- Value: 120Ω, 1/4W
- Location: Both ends of bus
- Purpose: Prevent signal reflections

```
     120Ω
A ---[===]--- B
```

### Biasing (Optional for Long Lines)

```
A ---- [680Ω] ---- VCC (pull-up)
B ---- [680Ω] ---- GND (pull-down)
```

## Modbus TCP Hardware

### Ethernet PHY Options

#### LAN8720A

**Features:**
- RMII interface
- 10/100 Mbps
- Low power consumption
- Small footprint

**Connection to STM32:**
```
STM32F4/F7/H7              LAN8720A
-------------              --------
PA1  - ETH_RMII_REF_CLK    REFCLK0 (50MHz)
PA2  - ETH_MDIO            MDIO
PA7  - ETH_RMII_CRS_DV     CRS_DV
PC1  - ETH_MDC             MDC
PC4  - ETH_RMII_RXD0       RXD0
PC5  - ETH_RMII_RXD1       RXD1
PG11 - ETH_RMII_TX_EN      TXEN
PG13 - ETH_RMII_TXD0       TXD0
PG14 - ETH_RMII_TXD1       TXD1

Crystal: 25MHz
Output: 50MHz for REFCLK
```

**Power Supply:**
- 3.3V for I/O
- 1.2V for core (use LDO or internal regulator)

#### DP83848

**Features:**
- MII/RMII configurable
- 10/100 Mbps
- Robust ESD protection
- Auto-MDIX support

**Similar pinout to LAN8720A**

### Ethernet Connector

**RJ45 with Integrated Magnetics (Recommended):**

**Example: HanRun HR911105A**
```
RJ45 Pin | Function    | PHY Pin
---------|-------------|--------
1        | TX+         | TXP
2        | TX-         | TXN
3        | RX+         | RXP
4        | -           | -
5        | -           | -
6        | RX-         | RXN
7        | -           | -
8        | -           | -

Shield   | GND through | Chassis
         | capacitor   | Ground
```

**LED Indicators:**
- Link LED: Shows network connection
- Activity LED: Shows data traffic

### PCB Design Guidelines

**Ethernet PHY Layout:**
1. Keep crystal close to PHY (< 5mm)
2. Use ground plane under signals
3. Route RMII signals as matched-length traces
4. Add test points for debugging
5. Place bypass capacitors close to power pins

**Crystal Circuit:**
```
PHY               25MHz Crystal
---               -------------
XTAL1 ---[22pF]--- |         | ---[22pF]--- GND
XTAL2 ------------ |         |
                   -----------
```

**Power Supply Decoupling:**
- 100nF ceramic cap per power pin
- 10µF tantalum/electrolytic per supply
- Place close to IC

## Power Considerations

### Current Requirements

**Modbus RTU:**
- STM32: 50-200mA (depending on series and clock)
- RS-485 transceiver: 1-10mA
- Total: ~100-250mA

**Modbus TCP:**
- STM32 with Ethernet: 100-300mA
- PHY: 100-200mA
- Total: ~300-500mA

### Power Supply Options

1. **USB-UART Adapter** (development): 5V -> LDO -> 3.3V
2. **DC Power Jack**: 9-12V -> LDO -> 3.3V
3. **PoE (Power over Ethernet)**: Use PoE module

## Protection Circuits

### RS-485 Bus Protection

**TVS Diodes:**
```
A ----+---- [TVS] ---- GND
      |
B ----+---- [TVS] ---- GND
```

Recommended: PESD2CAN (bidirectional TVS)

### Ethernet Protection

**RJ45 with integrated magnetics** provides:
- Isolation (1500V)
- Common-mode filtering
- ESD protection

**Additional protection:**
- Add TVS diodes on each pair
- Use proper grounding

## Recommended Development Boards

### For Modbus RTU

1. **STM32F103C8T6 (Blue Pill)**
   - Low cost
   - Good for learning
   - Add external RS-485 module

2. **STM32F407VG Discovery**
   - More features
   - Built-in debugger
   - Add RS-485 module

### For Modbus TCP

1. **STM32F407VG Discovery + DP83848 Board**
   - Discovery board + external PHY module

2. **STM32F429I Discovery**
   - Has LCD for debugging
   - Add PHY module

3. **NUCLEO-F429ZI**
   - Built-in Ethernet
   - Professional development board

## Bill of Materials (BOM)

### Minimal RTU Setup
| Item | Part Number | Quantity |
|------|-------------|----------|
| MCU | STM32F103C8T6 | 1 |
| RS-485 | MAX485 | 1 |
| Resistor | 120Ω 1/4W | 2 |
| Capacitor | 100nF | 2 |
| Terminal Block | 2-pin | 1 |

### Minimal TCP Setup
| Item | Part Number | Quantity |
|------|-------------|----------|
| MCU | STM32F407VG | 1 |
| PHY | LAN8720A | 1 |
| Crystal | 25MHz | 1 |
| RJ45 | HR911105A | 1 |
| Capacitor | 100nF | 5 |
| Capacitor | 22pF | 2 |
| Capacitor | 10µF | 2 |

## Testing Equipment

### Essential
- Multimeter
- USB-UART adapter (for debugging)
- USB-RS485 adapter (for RTU testing)

### Advanced
- Oscilloscope (for signal analysis)
- Logic analyzer (for protocol debugging)
- Network analyzer (Wireshark for TCP)

## Safety Considerations

1. **ESD Protection**: Use wrist strap when handling boards
2. **Power Supply**: Verify voltage before connecting
3. **Bus Wiring**: Double-check A/B polarity
4. **Isolation**: Consider optoisolated RS-485 for high-voltage environments
5. **Grounding**: Proper ground connection to prevent noise

## References

- [Modbus Organization](https://modbus.org/)
- [STM32 Application Notes](https://www.st.com/en/applications/connectivity.html)
- [RS-485 Standard (TIA/EIA-485-A)](https://en.wikipedia.org/wiki/RS-485)
- [Ethernet IEEE 802.3](https://standards.ieee.org/standard/802_3-2018.html)
