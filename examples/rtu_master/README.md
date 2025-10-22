# Modbus RTU Master Example

This example demonstrates how to implement a Modbus RTU master device to read/write data from slave devices.

## Hardware Setup

- STM32 microcontroller (e.g., STM32F407)
- RS-485 transceiver (e.g., MAX485)
- One or more Modbus RTU slave devices

## Code Example

```c
#include "main.h"
#include "modbus_rtu.h"

/* Modbus handle */
modbus_rtu_t modbus_master;

void setup_modbus_master(void)
{
    /* Initialize Modbus RTU master */
    modbus_rtu_init(&modbus_master, &huart1, 0, 9600, MODBUS_PARITY_NONE);
    
    /* Set mode to master */
    modbus_rtu_set_mode(&modbus_master, MODBUS_MODE_MASTER);
    
    /* Set DE/RE control pin */
    modbus_rtu_set_de_pin(&modbus_master, GPIOA, GPIO_PIN_8);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    
    setup_modbus_master();
    
    uint16_t read_buffer[10];
    modbus_error_t error;
    
    while (1)
    {
        /* Read holding registers from slave ID 1 */
        error = modbus_rtu_read_holding_registers(&modbus_master, 1, 0, 2, read_buffer);
        
        if (error == MODBUS_OK) {
            printf("Register 0: %d\n", read_buffer[0]);
            printf("Register 1: %d\n", read_buffer[1]);
        }
        
        HAL_Delay(1000);
        
        /* Write to holding register */
        error = modbus_rtu_write_single_register(&modbus_master, 1, 0, 9999);
        
        if (error == MODBUS_OK) {
            printf("Write successful\n");
        }
        
        HAL_Delay(1000);
    }
}

/* UART interrupt callback */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1) {
        modbus_rtu_rx_callback(&modbus_master);
    }
}
```

## Function Usage

### Read Holding Registers
```c
uint16_t registers[10];
modbus_rtu_read_holding_registers(&modbus_master, slave_id, start_addr, count, registers);
```

### Read Input Registers
```c
uint16_t inputs[10];
modbus_rtu_read_input_registers(&modbus_master, slave_id, start_addr, count, inputs);
```

### Write Single Register
```c
modbus_rtu_write_single_register(&modbus_master, slave_id, address, value);
```

### Write Multiple Registers
```c
uint16_t values[] = {100, 200, 300};
modbus_rtu_write_multiple_registers(&modbus_master, slave_id, start_addr, 3, values);
```
