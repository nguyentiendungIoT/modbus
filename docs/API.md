# API Documentation

## Table of Contents

- [Initialization](#initialization)
- [Modbus RTU API](#modbus-rtu-api)
- [Modbus TCP API](#modbus-tcp-api)
- [Common Functions](#common-functions)
- [Data Structures](#data-structures)
- [Error Codes](#error-codes)

## Initialization

### modbus_rtu_init

```c
modbus_error_t modbus_rtu_init(
    modbus_rtu_t *mb,
    UART_HandleTypeDef *huart,
    uint8_t slave_id,
    uint32_t baudrate,
    modbus_parity_t parity
);
```

**Description:** Initialize Modbus RTU interface

**Parameters:**
- `mb` - Pointer to Modbus RTU handle
- `huart` - Pointer to STM32 UART handle
- `slave_id` - Slave ID (1-247, 0 for master/broadcast)
- `baudrate` - Baud rate (9600, 19200, 38400, 57600, 115200)
- `parity` - Parity setting (MODBUS_PARITY_NONE, MODBUS_PARITY_EVEN, MODBUS_PARITY_ODD)

**Returns:** MODBUS_OK on success

---

### modbus_tcp_init

```c
modbus_error_t modbus_tcp_init(
    modbus_tcp_t *mb,
    uint16_t port
);
```

**Description:** Initialize Modbus TCP interface

**Parameters:**
- `mb` - Pointer to Modbus TCP handle
- `port` - TCP port number (default: 502)

**Returns:** MODBUS_OK on success

---

## Modbus RTU API

### modbus_rtu_set_mode

```c
void modbus_rtu_set_mode(modbus_rtu_t *mb, modbus_mode_t mode);
```

**Description:** Set operation mode (Master or Slave)

**Parameters:**
- `mb` - Modbus RTU handle
- `mode` - MODBUS_MODE_MASTER or MODBUS_MODE_SLAVE

---

### modbus_rtu_set_de_pin

```c
void modbus_rtu_set_de_pin(
    modbus_rtu_t *mb,
    GPIO_TypeDef *port,
    uint16_t pin
);
```

**Description:** Configure DE/RE control pin for RS-485 transceiver

**Parameters:**
- `mb` - Modbus RTU handle
- `port` - GPIO port (e.g., GPIOA)
- `pin` - GPIO pin (e.g., GPIO_PIN_8)

---

### modbus_rtu_poll

```c
void modbus_rtu_poll(modbus_rtu_t *mb);
```

**Description:** Poll for incoming Modbus frames. Must be called regularly in main loop for slave mode.

**Parameters:**
- `mb` - Modbus RTU handle

---

### modbus_rtu_read_holding_registers

```c
modbus_error_t modbus_rtu_read_holding_registers(
    modbus_rtu_t *mb,
    uint8_t slave_id,
    uint16_t start_addr,
    uint16_t count,
    uint16_t *dest
);
```

**Description:** Read holding registers from a slave device (Function code 0x03)

**Parameters:**
- `mb` - Modbus RTU handle
- `slave_id` - Target slave address (1-247)
- `start_addr` - Starting register address
- `count` - Number of registers to read (1-125)
- `dest` - Destination buffer for register values

**Returns:** MODBUS_OK on success, error code otherwise

---

### modbus_rtu_read_input_registers

```c
modbus_error_t modbus_rtu_read_input_registers(
    modbus_rtu_t *mb,
    uint8_t slave_id,
    uint16_t start_addr,
    uint16_t count,
    uint16_t *dest
);
```

**Description:** Read input registers from a slave device (Function code 0x04)

**Parameters:**
- `mb` - Modbus RTU handle
- `slave_id` - Target slave address (1-247)
- `start_addr` - Starting register address
- `count` - Number of registers to read (1-125)
- `dest` - Destination buffer for register values

**Returns:** MODBUS_OK on success, error code otherwise

---

### modbus_rtu_read_coils

```c
modbus_error_t modbus_rtu_read_coils(
    modbus_rtu_t *mb,
    uint8_t slave_id,
    uint16_t start_addr,
    uint16_t count,
    uint8_t *dest
);
```

**Description:** Read coil status from a slave device (Function code 0x01)

**Parameters:**
- `mb` - Modbus RTU handle
- `slave_id` - Target slave address (1-247)
- `start_addr` - Starting coil address
- `count` - Number of coils to read (1-2000)
- `dest` - Destination buffer for coil values (packed bits)

**Returns:** MODBUS_OK on success, error code otherwise

---

### modbus_rtu_write_single_register

```c
modbus_error_t modbus_rtu_write_single_register(
    modbus_rtu_t *mb,
    uint8_t slave_id,
    uint16_t address,
    uint16_t value
);
```

**Description:** Write single holding register to a slave device (Function code 0x06)

**Parameters:**
- `mb` - Modbus RTU handle
- `slave_id` - Target slave address (1-247)
- `address` - Register address
- `value` - Register value to write

**Returns:** MODBUS_OK on success, error code otherwise

---

### modbus_rtu_write_multiple_registers

```c
modbus_error_t modbus_rtu_write_multiple_registers(
    modbus_rtu_t *mb,
    uint8_t slave_id,
    uint16_t start_addr,
    uint16_t count,
    const uint16_t *src
);
```

**Description:** Write multiple holding registers to a slave device (Function code 0x10)

**Parameters:**
- `mb` - Modbus RTU handle
- `slave_id` - Target slave address (1-247)
- `start_addr` - Starting register address
- `count` - Number of registers to write (1-123)
- `src` - Source buffer with register values

**Returns:** MODBUS_OK on success, error code otherwise

---

### modbus_rtu_write_single_coil

```c
modbus_error_t modbus_rtu_write_single_coil(
    modbus_rtu_t *mb,
    uint8_t slave_id,
    uint16_t address,
    uint16_t value
);
```

**Description:** Write single coil to a slave device (Function code 0x05)

**Parameters:**
- `mb` - Modbus RTU handle
- `slave_id` - Target slave address (1-247)
- `address` - Coil address
- `value` - Coil value (0x0000 = OFF, 0xFF00 = ON)

**Returns:** MODBUS_OK on success, error code otherwise

---

## Modbus TCP API

### modbus_tcp_set_mode

```c
void modbus_tcp_set_mode(modbus_tcp_t *mb, modbus_mode_t mode);
```

**Description:** Set operation mode (Master or Slave)

**Parameters:**
- `mb` - Modbus TCP handle
- `mode` - MODBUS_MODE_MASTER or MODBUS_MODE_SLAVE

---

### modbus_tcp_start_server

```c
modbus_error_t modbus_tcp_start_server(modbus_tcp_t *mb);
```

**Description:** Start Modbus TCP server (slave mode)

**Parameters:**
- `mb` - Modbus TCP handle

**Returns:** MODBUS_OK on success

---

### modbus_tcp_connect

```c
modbus_error_t modbus_tcp_connect(
    modbus_tcp_t *mb,
    const char *ip_addr,
    uint16_t port
);
```

**Description:** Connect to Modbus TCP server (master mode)

**Parameters:**
- `mb` - Modbus TCP handle
- `ip_addr` - IP address string (e.g., "192.168.1.100")
- `port` - TCP port number

**Returns:** MODBUS_OK on success

---

## Common Functions

### modbus_crc16

```c
uint16_t modbus_crc16(const uint8_t *buffer, uint16_t length);
```

**Description:** Calculate CRC16 checksum for Modbus RTU

**Parameters:**
- `buffer` - Data buffer
- `length` - Buffer length

**Returns:** CRC16 value

---

### modbus_set_bit

```c
void modbus_set_bit(uint8_t *byte_array, uint16_t bit_index, uint8_t value);
```

**Description:** Set bit in byte array

**Parameters:**
- `byte_array` - Byte array
- `bit_index` - Bit index
- `value` - Bit value (0 or 1)

---

### modbus_get_bit

```c
uint8_t modbus_get_bit(const uint8_t *byte_array, uint16_t bit_index);
```

**Description:** Get bit from byte array

**Parameters:**
- `byte_array` - Byte array
- `bit_index` - Bit index

**Returns:** Bit value (0 or 1)

---

## Data Structures

### modbus_data_model_t

```c
typedef struct {
    uint8_t *coils;
    uint16_t coils_count;
    uint8_t *discrete_inputs;
    uint16_t discrete_inputs_count;
    uint16_t *holding_registers;
    uint16_t holding_registers_count;
    uint16_t *input_registers;
    uint16_t input_registers_count;
} modbus_data_model_t;
```

**Description:** Modbus data model for slave devices

**Fields:**
- `coils` - Pointer to coil array (read/write)
- `coils_count` - Number of coils
- `discrete_inputs` - Pointer to discrete input array (read-only)
- `discrete_inputs_count` - Number of discrete inputs
- `holding_registers` - Pointer to holding register array (read/write)
- `holding_registers_count` - Number of holding registers
- `input_registers` - Pointer to input register array (read-only)
- `input_registers_count` - Number of input registers

---

## Error Codes

```c
typedef enum {
    MODBUS_OK = 0,                      // Success
    MODBUS_ERROR_TIMEOUT,               // Communication timeout
    MODBUS_ERROR_CRC,                   // CRC error
    MODBUS_ERROR_INVALID_SLAVE,         // Invalid slave ID
    MODBUS_ERROR_INVALID_FUNCTION,      // Invalid function code
    MODBUS_ERROR_INVALID_DATA,          // Invalid data
    MODBUS_ERROR_EXCEPTION,             // Modbus exception received
    MODBUS_ERROR_BUSY,                  // Device busy
    MODBUS_ERROR_INVALID_PARAM,         // Invalid parameter
    MODBUS_ERROR_NOT_INITIALIZED        // Not initialized
} modbus_error_t;
```

---

## Function Codes

| Code | Name | Description |
|------|------|-------------|
| 0x01 | Read Coils | Read coil status (discrete outputs) |
| 0x02 | Read Discrete Inputs | Read discrete input status |
| 0x03 | Read Holding Registers | Read holding registers |
| 0x04 | Read Input Registers | Read input registers |
| 0x05 | Write Single Coil | Write single coil value |
| 0x06 | Write Single Register | Write single holding register |
| 0x0F | Write Multiple Coils | Write multiple coils |
| 0x10 | Write Multiple Registers | Write multiple holding registers |
