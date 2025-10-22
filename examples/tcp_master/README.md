# Modbus TCP Master Example

This example shows how to read data from Modbus TCP slave devices over Ethernet.

## Code Example

```c
#include "main.h"
#include "modbus_tcp.h"
#include "lwip.h"

modbus_tcp_t modbus_tcp_master;

void setup_modbus_tcp_master(void)
{
    modbus_tcp_init(&modbus_tcp_master, 502);
    modbus_tcp_set_mode(&modbus_tcp_master, MODBUS_MODE_MASTER);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_LWIP_Init();
    
    setup_modbus_tcp_master();
    
    /* Connect to slave device */
    modbus_tcp_connect(&modbus_tcp_master, "192.168.1.100", 502);
    
    uint16_t registers[10];
    
    while (1)
    {
        MX_LWIP_Process();
        
        /* Read from unit ID 1 */
        if (modbus_tcp_read_holding_registers(&modbus_tcp_master, 1, 0, 2, registers) == MODBUS_OK) {
            printf("Reg 0: %d, Reg 1: %d\n", registers[0], registers[1]);
        }
        
        HAL_Delay(1000);
    }
}
```
