/**@file
 * This file is part of the TASTE Leon3 Runtime.
 *
 * @copyright 2025 N7 Space Sp. z o.o.
 *
 * TODO project description
 *
 * Leon3 Runtime is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with Leon3 Runtime. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PERFMON_H
#define PERFMON_H

/**
 * @file    Hal.h
 * @brief   Header for Hal
 */

#include <Timer.h>
#include <rtems.h>
#include <stdbool.h>
#include <stdlib.h>

bool Hal_Init(void);

uint64_t Hal_GetElapsedTimeInNs(void);

bool Hal_SleepNs(uint64_t time_ns);

int32_t Hal_SemaphoreCreate(void);

bool Hal_SemaphoreObtain(int32_t id);

bool Hal_SemaphoreRelease(int32_t id);

#endif