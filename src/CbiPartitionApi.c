/**@file
 * This file is part of the TASTE Leon3 Runtime.
 *
 * @copyright 2023 N7 Space Sp. z o.o.
 *
 * TASTE Leon3 Runtime was developed under the project AURORA.
 * This project has received funding from the European Union’s Horizon 2020
 * research and innovation programme under grant agreement No 101004291
 *
 * Leon3 Runtime is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with Leon3 Runtime. If not, see <http://www.gnu.org/licenses/>.
 */

#include <CbiPartitionApi.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Offset of the reset canary from the end of the used RAM
 *
 */
#define RESET_CANARY_BUFFER_OFFSET 32
/**
 * @brief Size of the reset canary buffer
 *
 */
#define RESET_CANARY_BUFFER_SIZE 4
/**
 * @brief Reset canary buffer pattern indicating proper system initialization
 *
 */
#define RESET_CANARY_PATTERN 0x5A

/**
 * @brief Symbol indicating the end of the used RAM
 *
 */
extern int __end;

/**
 * @brief Variable holding the reset reason, initialized with the API
 *
 */
Reset_Reason g_reset_reason = Reset_Reason_Unknown;

/**
 * @brief Get the Reset Canary address
 *
 * @return Reset canary address
 */
static unsigned char *getResetCanaryAddress()
{
	return (unsigned char *)((uintptr_t)(&__end) +
				 RESET_CANARY_BUFFER_OFFSET +
				 RESET_CANARY_BUFFER_SIZE);
}

/**
 * @brief Returns whether Reset Canary is set
 *
 * @return Whether Reset Canary is set
 */
static bool isResetCanarySet()
{
	volatile unsigned char *buffer = getResetCanaryAddress();
	for (int i = 0; i < RESET_CANARY_BUFFER_SIZE; i++) {
		if (buffer[i] != RESET_CANARY_PATTERN) {
			return false;
		}
	}
	return true;
}

/**
 * @brief Set the Reset Canary
 *
 */
static void setResetCanary()
{
	volatile unsigned char *buffer = getResetCanaryAddress();
	for (int i = 0; i < RESET_CANARY_BUFFER_SIZE; i++) {
		buffer[i] = RESET_CANARY_PATTERN;
	}
}

void Cbi_Partition_Api_initialize()
{
	if (isResetCanarySet()) {
		// Memory is properly initialized, so reset is assumed
		g_reset_reason = Reset_Reason_Reset;
	} else {
		// Memory is not properly initialized, power-up is assumed
		g_reset_reason = Reset_Reason_Power;
	}
	setResetCanary();
}

Reset_Reason Cbi_Partition_Api_getResetReason() { return g_reset_reason; }
