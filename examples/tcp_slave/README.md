# Modbus TCP Slave Example

This example demonstrates how to implement a Modbus TCP slave device using STM32 with Ethernet.

## Hardware Requirements

- STM32 with Ethernet peripheral (e.g., STM32F407, STM32F429, STM32H743)
- Ethernet PHY (e.g., LAN8720, DP83848)
- Network connection

## Software Requirements

- lwIP TCP/IP stack (included in STM32Cube)
- STM32CubeMX for configuration

## Configuration

1. Enable Ethernet in STM32CubeMX
2. Enable lwIP middleware
3. Configure IP address (static or DHCP)

## Code Example

```c
#include "main.h"
#include "modbus_tcp.h"
#include "lwip.h"

/* Modbus TCP handle */
modbus_tcp_t modbus_tcp_slave;

/* Data model */
uint16_t holding_registers[100] = {0};
uint16_t input_registers[100] = {0};

modbus_data_model_t data_model = {
    .holding_registers = holding_registers,
    .holding_registers_count = 100,
    .input_registers = input_registers,
    .input_registers_count = 100
};

void setup_modbus_tcp_slave(void)
{
    /* Initialize Modbus TCP on port 502 */
    modbus_tcp_init(&modbus_tcp_slave, 502);
    
    /* Register data model */
    modbus_tcp_register_data_model(&modbus_tcp_slave, &data_model);
    
    /* Start TCP server */
    modbus_tcp_start_server(&modbus_tcp_slave);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_LWIP_Init();
    
    setup_modbus_tcp_slave();
    
    /* Initialize test data */
    holding_registers[0] = 1234;
    holding_registers[1] = 5678;
    
    while (1)
    {
        /* Process lwIP */
        MX_LWIP_Process();
        
        /* Poll Modbus TCP */
        modbus_tcp_poll(&modbus_tcp_slave);
        
        /* Update sensor data */
        input_registers[0] = read_temperature();
        input_registers[1] = read_humidity();
    }
}
```

## Testing

Use a Modbus TCP client tool:
1. Connect to the device IP address on port 502
2. Read/write holding registers
3. Read input registers

## Network Configuration

Static IP example in lwIP:
```c
IP_ADDRESS0 = 192;
IP_ADDRESS1 = 168;
IP_ADDRESS2 = 1;
IP_ADDRESS3 = 100;
```
