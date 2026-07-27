/*
 * protocol.h
 *
 *  Created on: Jul 18, 2026
 *      Author: Emagix
 */
#ifndef HIL_PROTOCOL_H
#define HIL_PROTOCOL_H

#include <stdbool.h>
#include <stdint.h>

#define HIL_SOF                 0xAAU
#define HIL_EOF                 0x55U
#define HIL_MAX_DATA_LENGTH     32U

/* Commands PC → MCU */
#define CMD_GPO_SET             0x10U
#define CMD_GPO_GET             0x11U
#define CMD_RELAY_SET           0x20U
#define CMD_RELAY_GET           0x21U
#define CMD_DC_RELAY_SET        0x30U
#define CMD_DC_RELAY_GET        0x31U
#define CMD_ADC_START           0x40U
#define CMD_ADC_STOP            0x41U
#define CMD_WGEN_SET            0x50U
#define CMD_PING                0xF0U

/* Responses MCU → PC */
#define RSP_OK                  0x01U
#define RSP_ADC_DATA            0x02U
#define RSP_GPIO_STATE          0x03U
#define RSP_BTN_STATE           0x04U
#define RSP_GPO_STATE           0x05U
#define RSP_RELAY_STATE         0x06U
#define RSP_DC_RELAY_STATE      0x07U
#define RSP_PONG                0xF1U
#define RSP_ERROR               0xFFU

/* Error codes */
#define HIL_ERROR_INVALID_COMMAND   0x01U
#define HIL_ERROR_INVALID_LENGTH    0x02U
#define HIL_ERROR_INVALID_CHANNEL   0x03U
#define HIL_ERROR_INVALID_DATA      0x04U
#define HIL_ERROR_EXECUTION_FAILED  0x05U

/*
 * Frame:
 *
 * [SOF][CMD][LEN][DATA...][EOF]
 */
typedef struct
{
    uint8_t cmd;
    uint8_t len;
    uint8_t data[HIL_MAX_DATA_LENGTH];

} HIL_Packet_t;

void HIL_Protocol_Init(void);

bool HIL_Protocol_ProcessByte(
    uint8_t receivedByte,
    HIL_Packet_t *completedPacket
);

void HIL_Protocol_Execute(
    const HIL_Packet_t *packet
);

bool HIL_Protocol_SendPacket(
    uint8_t response,
    const uint8_t *data,
    uint8_t length
);

#endif /* HIL_PROTOCOL_H */