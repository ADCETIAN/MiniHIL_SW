/*
 * protocol.c
 *
 *  Created on: Jul 18, 2026
 *      Author: Emagix
 */

#include "hil_protocol.h"
#include "bsp_uart2.h"
#include "bsp_uart1.h"
#include "bsp_relay.h"
#include "bsp_gpo.h"
#include <stddef.h>
#include <string.h>

typedef enum
{
    HIL_RX_WAIT_SOF = 0,
    HIL_RX_READ_CMD,
    HIL_RX_READ_LENGTH,
    HIL_RX_READ_DATA,
    HIL_RX_READ_EOF

} HIL_RxState_t;

static HIL_RxState_t hilRxState;
static HIL_Packet_t hilRxPacket;
static uint8_t hilDataIndex;


static void HIL_Protocol_ResetParser(void)
{
    hilRxState = HIL_RX_WAIT_SOF;
    hilDataIndex = 0U;

    memset(
        &hilRxPacket,
        0,
        sizeof(hilRxPacket)
    );
}


void HIL_Protocol_Init(void)
{
    HIL_Protocol_ResetParser();
}


bool HIL_Protocol_ProcessByte(
    uint8_t receivedByte,
    HIL_Packet_t *completedPacket
)
{
    if (completedPacket == NULL)
    {
        return false;
    }

    switch (hilRxState)
    {
        case HIL_RX_WAIT_SOF:
        {
            if (receivedByte == HIL_SOF)
            {
                hilRxState = HIL_RX_READ_CMD;
            }

            break;
        }

        case HIL_RX_READ_CMD:
        {
            hilRxPacket.cmd = receivedByte;
            hilRxState = HIL_RX_READ_LENGTH;
            break;
        }

        case HIL_RX_READ_LENGTH:
        {
            hilRxPacket.len = receivedByte;
            hilDataIndex = 0U;

            if (hilRxPacket.len > HIL_MAX_DATA_LENGTH)
            {
                HIL_Protocol_ResetParser();
            }
            else if (hilRxPacket.len == 0U)
            {
                hilRxState = HIL_RX_READ_EOF;
            }
            else
            {
                hilRxState = HIL_RX_READ_DATA;
            }

            break;
        }

        case HIL_RX_READ_DATA:
        {
            hilRxPacket.data[hilDataIndex] = receivedByte;
            hilDataIndex++;

            if (hilDataIndex >= hilRxPacket.len)
            {
                hilRxState = HIL_RX_READ_EOF;
            }

            break;
        }

        case HIL_RX_READ_EOF:
        {
            if (receivedByte == HIL_EOF)
            {
                *completedPacket = hilRxPacket;

                HIL_Protocol_ResetParser();

                return true;
            }

            /*
             * Invalid EOF: reset parser.
             */
            HIL_Protocol_ResetParser();

            /*
             * Treat a new SOF as the beginning of another frame.
             */
            if (receivedByte == HIL_SOF)
            {
                hilRxState = HIL_RX_READ_CMD;
            }

            break;
        }

        default:
        {
            HIL_Protocol_ResetParser();
            break;
        }
    }

    return false;
}

bool HIL_Protocol_SendPacket(
    uint8_t response,
    const uint8_t *data,
    uint8_t length
)
{
    uint8_t txBuffer[HIL_MAX_DATA_LENGTH + 4U];
    uint16_t txLength;
    uint8_t index;

    if (length > HIL_MAX_DATA_LENGTH)
    {
        return false;
    }

    if ((length > 0U) && (data == NULL))
    {
        return false;
    }

    txBuffer[0] = HIL_SOF;
    txBuffer[1] = response;
    txBuffer[2] = length;

    for (index = 0U; index < length; index++)
    {
        txBuffer[3U + index] = data[index];
    }

    txBuffer[3U + length] = HIL_EOF;

    /*
     * SOF + response + length + data + EOF
     */
    txLength = (uint16_t)length + 4U;

    return BSP_UART1_SendData(
        txBuffer,
        txLength
    );
}

static void HIL_Protocol_SendOK(void)
{
    (void)HIL_Protocol_SendPacket(
        RSP_OK,
        NULL,
        0U
    );
}


static void HIL_Protocol_SendError(uint8_t errorCode)
{
    (void)HIL_Protocol_SendPacket(
        RSP_ERROR,
        &errorCode,
        1U
    );
}

static void HIL_Protocol_SendCommandOK(uint8_t command)
{
    uint8_t responseData[1];

    responseData[0] = command;

    (void)HIL_Protocol_SendPacket(
        RSP_OK,
        responseData,
        sizeof(responseData)
    );
}

void HIL_Protocol_Execute(
    const HIL_Packet_t *packet
)
{
    if (packet == NULL)
    {
        return;
    }

    switch (packet->cmd)
    {
    case CMD_GPO_GET:
    {
        uint8_t protocolGpoId;
        uint8_t responseData[2];
        BSP_GpoId_t bspGpoId;
        bool logicalState;

        if (packet->len != 1U)
        {
            HIL_Protocol_SendError(
                HIL_ERROR_INVALID_LENGTH
            );
            break;
        }

        protocolGpoId = packet->data[0];

        if ((protocolGpoId < 1U) ||
            (protocolGpoId > (uint8_t)BSP_GPO_COUNT))
        {
            HIL_Protocol_SendError(
                HIL_ERROR_INVALID_CHANNEL
            );
            break;
        }

        bspGpoId =
            (BSP_GpoId_t)(protocolGpoId - 1U);

        logicalState =
            BSP_GPO_GetState(bspGpoId);

        /*
        * Send ID and logical state.
        *
        * responseData[0] = protocol GPO ID, 1–8
        * responseData[1] = logical state, 0 or 1
        */
        responseData[0] = protocolGpoId;
        responseData[1] = logicalState ? 1U : 0U;

        (void)HIL_Protocol_SendPacket(
            RSP_GPO_STATE,
            responseData,
            sizeof(responseData)
        );

        break;
    }
        case CMD_GPO_SET:
        {
            uint8_t protocolGpoId;
            uint8_t requestedState;
            BSP_GpoId_t bspGpoId;

            if (packet->len != 2U)
            {
                HIL_Protocol_SendError(
                    HIL_ERROR_INVALID_LENGTH
                );
                break;
            }

            protocolGpoId = packet->data[0];
            requestedState = packet->data[1];

            /*
            * UART protocol uses GPO IDs 1 to 8.
            */
            if ((protocolGpoId < 1U) ||
                (protocolGpoId > (uint8_t)BSP_GPO_COUNT))
            {
                HIL_Protocol_SendError(
                    HIL_ERROR_INVALID_CHANNEL
                );
                break;
            }

            if (requestedState > 1U)
            {
                HIL_Protocol_SendError(
                    HIL_ERROR_INVALID_DATA
                );
                break;
            }

            /*
            * Convert protocol ID 1–8 into array index 0–7.
            */
            bspGpoId =
                (BSP_GpoId_t)(protocolGpoId - 1U);

            if (!BSP_GPO_Set(
                    bspGpoId,
                    requestedState != 0U))
            {
                HIL_Protocol_SendError(
                    HIL_ERROR_EXECUTION_FAILED
                );
                break;
            }

            HIL_Protocol_SendCommandOK(
                CMD_GPO_SET
            );

            break;
        }
        case CMD_RELAY_GET:
        {
            uint8_t relayId;
            uint8_t responseData;
            bool relayState;

            if (packet->len != 1U)
            {
                HIL_Protocol_SendError(
                    HIL_ERROR_INVALID_LENGTH
                );
                break;
            }

            relayId = packet->data[0];

            if (relayId >= BSP_RELAY_COUNT)
            {
                HIL_Protocol_SendError(
                    HIL_ERROR_INVALID_CHANNEL
                );
                break;
            }

            relayState = BSP_Relay_GetState(
                (BSP_RelayId_t)relayId
            );

            responseData = relayState ? 1U : 0U;

            (void)HIL_Protocol_SendPacket(
                RSP_RELAY_STATE,
                &responseData,
                1U
            );

            break;
        }
        case CMD_PING:
        {
            uint8_t firmwareVersion[2];

            if (packet->len != 0U)
            {
                HIL_Protocol_SendError(
                    HIL_ERROR_INVALID_LENGTH
                );
                break;
            }

            firmwareVersion[0] = 1U;
            firmwareVersion[1] = 0U;

            (void)HIL_Protocol_SendPacket(
                RSP_PONG,
                firmwareVersion,
                sizeof(firmwareVersion)
            );

            break;
        }

        default:
        {
            HIL_Protocol_SendError(
                HIL_ERROR_INVALID_COMMAND
            );

            break;
        }
    }
}
