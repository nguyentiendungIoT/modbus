# Modbus RTU Slave Example

This example demonstrates how to implement a Modbus RTU slave device using the STM32 microcontroller.

## Hardware Setup

- STM32 microcontroller (e.g., STM32F407)
- RS-485 transceiver (e.g., MAX485)
- Connection: UART TX -> DI, UART RX -> RO, GPIO -> DE/RE

## Configuration

1. Configure UART in STM32CubeMX:
   - Baud rate: 9600
   - Word length: 8 bits
   - Parity: None
   - Stop bits: 1
   - Enable UART interrupt

2. Configure GPIO for DE/RE control

## Code Example

```c
#include "main.h"
#include "modbus_rtu.h"

/* Modbus handle */
modbus_rtu_t modbus_slave;

/* Data model */
uint16_t holding_registers[100] = {0};
uint16_t input_registers[100] = {0};
uint8_t coils[100] = {0};
uint8_t discrete_inputs[100] = {0};

modbus_data_model_t data_model = {
    .holding_registers = holding_registers,
    .holding_registers_count = 100,
    .input_registers = input_registers,
    .input_registers_count = 100,
    .coils = coils,
    .coils_count = 100,
    .discrete_inputs = discrete_inputs,
    .discrete_inputs_count = 100
};

void setup_modbus_slave(void)
{
    /* Initialize Modbus RTU slave with ID 1 */
    modbus_rtu_init(&modbus_slave, &huart1, 1, 9600, MODBUS_PARITY_NONE);
    
    /* Set DE/RE control pin (optional for RS-485) */
    modbus_rtu_set_de_pin(&modbus_slave, GPIOA, GPIO_PIN_8);
    
    /* Register data model */
    modbus_rtu_register_data_model(&modbus_slave, &data_model);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    
    /* Setup Modbus slave */
    setup_modbus_slave();
    
    /* Initialize some test data */
    holding_registers[0] = 1234;
    holding_registers[1] = 5678;
    input_registers[0] = 100;
    input_registers[1] = 200;
    
    while (1)
    {
        /* Poll for Modbus frames */
        modbus_rtu_poll(&modbus_slave);
        
        /* Update input registers with sensor data */
        input_registers[0] = read_sensor1();
        input_registers[1] = read_sensor2();
        
        HAL_Delay(10);
    }
}

/* UART interrupt callback */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1) {
        modbus_rtu_rx_callback(&modbus_slave);
    }
}
```

## Testing

Use a Modbus master software (e.g., ModbusPoll, QModMaster) to:

1. Read holding registers at address 0-1
2. Write to holding registers
3. Read input registers

Expected results:
- Holding register 0 should return 1234
- Holding register 1 should return 5678
- Writing to holding registers should succeed
