/** CAN.h
 * CAN driver
 * @author Sijin Woo
 * @lastRevised 2/16/2019
 */

#ifdef CAN_H
#define CAN_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

// Generic BPS ID
#define CAN_ID_BPS										(0x01 << 7)
// Specific BPS IDs
#define CAN_ID_BPS_TRIP								0x002
#define CAN_ID_BPS_ALL_CLEAR						(CAN_ID_BPS | 0x01)
#define CAN_ID_BPS_OFF								(CAN_ID_BPS | 0x02)
#define CAN_ID_CURRENT_DATA						(CAN_ID_BPS | 0x03)
#define CAN_ID_TOTAL_VOLTAGE_DATA			(CAN_ID_BPS | 0x04)
#define CAN_ID_AVG_TEMPERATURE_DATA		(CAN_ID_BPS | 0x05)
#define CAN_ID_SOC_DATA								(CAN_ID_BPS | 0x06)
#define CAN_ID_WDOG_TRIGGERED					(CAN_ID_BPS | 0x07)
#define CAN_ID_ERROR									(CAN_ID_BPS | 0x08)

/** CAN_Init
 * Initializes CAN bus for pins:
 *		PB8 : RX
 *		PB9 : TX
 * @param freq of can bus line
 */
void CAN_Init(uint32_t freq);

bool

#endif
