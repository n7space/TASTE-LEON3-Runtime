/**@file
 * This file is part of the TASTE Leon3 Runtime.
 *
 * @copyright 2022 N7 Space Sp. z o.o.
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

#pragma once

/**
 * @file    BrockerLock.h
 * @brief   Headers for lock functions required by Broker.
 */

/// @brief function used by Broker to acquire the lock.
void Broker_acquire_lock();
/// @brief function used by Broker to release the lock.
void Broker_release_lock();
