/**
 * @file modbus_config.h
 * @brief Modbus library configuration file
 * @author Nguyen Tien Dung
 * @date 2025
 */

#ifndef MODBUS_CONFIG_H
#define MODBUS_CONFIG_H

/* Enable/Disable Modbus protocols */
#define MODBUS_RTU_ENABLED              1
#define MODBUS_TCP_ENABLED              1

/* Buffer sizes */
#define MODBUS_MAX_FRAME_SIZE           256
#define MODBUS_RTU_MAX_ADU_LENGTH       256
#define MODBUS_TCP_MAX_ADU_LENGTH       260

/* Timeout settings (milliseconds) */
#define MODBUS_RESPONSE_TIMEOUT_MS      1000
#define MODBUS_INTER_FRAME_DELAY_MS     4
#define MODBUS_TURNAROUND_DELAY_MS      100

/* Retry settings */
#define MODBUS_MAX_RETRIES              3

/* Register limits */
#define MODBUS_MAX_READ_REGISTERS       125
#define MODBUS_MAX_WRITE_REGISTERS      123
#define MODBUS_MAX_READ_BITS            2000
#define MODBUS_MAX_WRITE_BITS           1968

/* TCP settings */
#define MODBUS_TCP_DEFAULT_PORT         502
#define MODBUS_TCP_MAX_CONNECTIONS      5

/* Debug settings */
#define MODBUS_DEBUG_ENABLED            0

#if MODBUS_DEBUG_ENABLED
    #define MODBUS_DEBUG_PRINT(...)     printf(__VA_ARGS__)
#else
    #define MODBUS_DEBUG_PRINT(...)
#endif

#endif /* MODBUS_CONFIG_H */
