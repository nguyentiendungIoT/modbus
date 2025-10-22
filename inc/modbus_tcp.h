/**
 * @file modbus_tcp.h
 * @brief Modbus TCP protocol implementation for STM32
 * @author Nguyen Tien Dung
 * @date 2025
 */

#ifndef MODBUS_TCP_H
#define MODBUS_TCP_H

#include "modbus_common.h"

/* MBAP Header structure */
typedef struct {
    uint16_t transaction_id;    /* Transaction identifier */
    uint16_t protocol_id;       /* Protocol identifier (0 for Modbus) */
    uint16_t length;            /* Number of following bytes */
    uint8_t unit_id;            /* Unit identifier (slave ID) */
} modbus_mbap_header_t;

/* Modbus TCP connection */
typedef struct {
    int socket_fd;              /* Socket file descriptor */
    bool active;                /* Connection active flag */
    uint32_t last_activity;     /* Last activity timestamp */
} modbus_tcp_connection_t;

/* Modbus TCP structure */
typedef struct {
    uint16_t port;                                      /* TCP port */
    modbus_mode_t mode;                                 /* Master or Slave mode */
    
    modbus_tcp_connection_t connections[MODBUS_TCP_MAX_CONNECTIONS];
    uint16_t num_connections;
    
    uint8_t tx_buffer[MODBUS_TCP_MAX_ADU_LENGTH];
    uint8_t rx_buffer[MODBUS_TCP_MAX_ADU_LENGTH];
    
    uint16_t transaction_id;                            /* Transaction counter */
    
    modbus_data_model_t data_model;                     /* Data model for slave */
} modbus_tcp_t;

/* Function prototypes */

/**
 * @brief Initialize Modbus TCP
 * @param mb Modbus TCP handle
 * @param port TCP port number
 * @return MODBUS_OK on success
 */
modbus_error_t modbus_tcp_init(modbus_tcp_t *mb, uint16_t port);

/**
 * @brief Set Modbus mode (Master/Slave)
 * @param mb Modbus TCP handle
 * @param mode Mode setting
 */
void modbus_tcp_set_mode(modbus_tcp_t *mb, modbus_mode_t mode);

/**
 * @brief Register data model for slave mode
 * @param mb Modbus TCP handle
 * @param data_model Data model structure
 */
void modbus_tcp_register_data_model(modbus_tcp_t *mb, modbus_data_model_t *data_model);

/**
 * @brief Start TCP server (slave mode)
 * @param mb Modbus TCP handle
 * @return MODBUS_OK on success
 */
modbus_error_t modbus_tcp_start_server(modbus_tcp_t *mb);

/**
 * @brief Connect to TCP server (master mode)
 * @param mb Modbus TCP handle
 * @param ip_addr IP address string
 * @param port Port number
 * @return MODBUS_OK on success
 */
modbus_error_t modbus_tcp_connect(modbus_tcp_t *mb, const char *ip_addr, uint16_t port);

/**
 * @brief Poll for incoming connections/data
 * @param mb Modbus TCP handle
 */
void modbus_tcp_poll(modbus_tcp_t *mb);

/* Master functions */

/**
 * @brief Read holding registers (TCP)
 * @param mb Modbus TCP handle
 * @param unit_id Unit identifier
 * @param start_addr Starting address
 * @param count Number of registers
 * @param dest Destination buffer
 * @return MODBUS_OK on success
 */
modbus_error_t modbus_tcp_read_holding_registers(modbus_tcp_t *mb, uint8_t unit_id,
                                                  uint16_t start_addr, uint16_t count,
                                                  uint16_t *dest);

/**
 * @brief Read input registers (TCP)
 * @param mb Modbus TCP handle
 * @param unit_id Unit identifier
 * @param start_addr Starting address
 * @param count Number of registers
 * @param dest Destination buffer
 * @return MODBUS_OK on success
 */
modbus_error_t modbus_tcp_read_input_registers(modbus_tcp_t *mb, uint8_t unit_id,
                                                uint16_t start_addr, uint16_t count,
                                                uint16_t *dest);

/**
 * @brief Write single register (TCP)
 * @param mb Modbus TCP handle
 * @param unit_id Unit identifier
 * @param address Register address
 * @param value Register value
 * @return MODBUS_OK on success
 */
modbus_error_t modbus_tcp_write_single_register(modbus_tcp_t *mb, uint8_t unit_id,
                                                 uint16_t address, uint16_t value);

/**
 * @brief Write multiple registers (TCP)
 * @param mb Modbus TCP handle
 * @param unit_id Unit identifier
 * @param start_addr Starting address
 * @param count Number of registers
 * @param src Source buffer
 * @return MODBUS_OK on success
 */
modbus_error_t modbus_tcp_write_multiple_registers(modbus_tcp_t *mb, uint8_t unit_id,
                                                    uint16_t start_addr, uint16_t count,
                                                    const uint16_t *src);

#endif /* MODBUS_TCP_H */
