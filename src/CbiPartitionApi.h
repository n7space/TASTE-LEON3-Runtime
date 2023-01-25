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

#ifndef CBIPARTITIONAPI_H
#define CBIPARTITIONAPI_H

/**
 * @file    CbiPartitionApi.h
 * @brief   Header for CBI Partition API module
 */

/**
 * @brief Enumeration listing possible reset reasons
 * 
 */
typedef enum {
    Reset_Reason_Unknown, //< Unknown reset reason
    Reset_Reason_Power,   //< Reset due to a power up
    Reset_Reason_Reset    //< Software reset
} Reset_Reason;

/**
 * @brief Initializes the CBI Partition API module
 * 
 */
void Cbi_Partition_Api_initialize();

/**
 * @brief Returns the reset reason
 * 
 * @return Reset reason 
 */
Reset_Reason Cbi_Partition_Api_getResetReason();

#endif // CBIPARTITIONAPI
