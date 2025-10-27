/**@file
 * This file is part of the TASTE Leon3 Runtime.
 *
 * @copyright 2025 N7 Space Sp. z o.o.
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

#include <Monitor.h>

Monitor_MessageQueueOverflow Monitor_MessageQueueOverflowCallback;

bool Monitor_Init() { return true; }

bool Monitor_MonitoringTick(void) {}

bool Monitor_GetUsageData(const enum interfaces_enum interface,
			  struct Monitor_InterfaceUsageData *const usage_data)
{
	return true;
}

bool Monitor_GetIdleCPUUsageData(
    struct Monitor_CPUUsageData *const cpu_usage_data)
{
	return true;
}

int32_t Monitor_GetMaximumStackUsage(const enum interfaces_enum interface)
{
	return -1;
}

bool Monitor_SetMessageQueueOverflowCallback(
    Monitor_MessageQueueOverflow overflow_callback)
{
	return true;
}

int32_t Monitor_GetQueuedItemsCount(const enum interfaces_enum interface)
{
	return 0;
}

int32_t Monitor_GetMaximumQueuedItemsCount(const enum interfaces_enum interface)
{
	return 0;
}

bool Monitor_IndicateInterfaceActivated(const enum interfaces_enum interface)
{
	return true;
}

bool Monitor_IndicateInterfaceDeactivated(const enum interfaces_enum interface)
{
	return true;
}

bool Monitor_GetInterfaceActivationEntryLog(
    struct Monitor_InterfaceActivationEntry **activation_log,
    uint32_t *out_latest_activation_entry_index,
    uint32_t *out_size_of_activation_log)
{
	return true;
}

bool Monitor_FreezeInterfaceActivationLogging() { return true; }

bool Monitor_UnfreezeInterfaceActivationLogging() { return true; }

bool Monitor_ClearInterfaceActivationLog() { return true; }
