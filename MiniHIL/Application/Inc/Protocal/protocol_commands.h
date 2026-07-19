/*
 * protocol_commands.h
 *
 *  Created on: Jul 18, 2026
 *      Author: Emagix
 */

#ifndef INC_PROTOCAL_PROTOCOL_COMMANDS_H_
#define INC_PROTOCAL_PROTOCOL_COMMANDS_H_

/* App/Inc/hil_protocol.h */

#define HIL_SOF          0xAA

/*
+------+--------+--------+--------+--------+------+------+
| SOF  | CMD ID | LEN    | DATA   | ...    | CRC  | EOF  |
+------+--------+--------+--------+--------+------+------+
+------+--------+--------+--------+--------+------+
| SOF  | CMD ID | LEN    | DATA   | ...    | EOF  |
+------+--------+--------+--------+--------+------+

 */

/* Commands PC → MCU */
#define CMD_GPIO_SET     0x10   /* CH=pin  DATA=state(1byte) */
#define CMD_RELAY_SET    0x20   /* CH=relay DATA=state(1byte) */
#define CMD_ADC_START    0x30   /* CH=mask  DATA=rate_hz(2bytes) */
#define CMD_ADC_STOP     0x31   /* CH=0     DATA=none */
#define CMD_WGEN_SET     0x40   /* CH=0     DATA=mode(1)+freq(2)+amp(2) */
#define CMD_PING         0xF0   /* CH=0     DATA=none */

/* Responses MCU → PC */
#define RSP_OK           0x01   /* ACK for any set command */
#define RSP_ADC_DATA     0x02   /* CH=channel DATA=samples */
#define RSP_GPIO_STATE   0x03   /* CH=pin   DATA=state */
#define RSP_BTN_STATE    0x04   /* CH=btn   DATA=state */
#define RSP_PONG         0xF1   /* DATA=fw_major,fw_minor */
#define RSP_ERROR        0xFF   /* DATA=err_code */

/* Frame: [SOF][CMD][CH][LEN][DATA...][CRC8] */
typedef struct {
    uint8_t cmd;
    uint8_t channel;
    uint8_t len;
    uint8_t data[32];
} HIL_Packet_t;

#endif /* INC_PROTOCAL_PROTOCOL_COMMANDS_H_ */
