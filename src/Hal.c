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

#include <Hal.h>

#define RT_MAX_HAL_SEMAPHORES 1

// zmokować rtems

static Timer_Apbctrl1 timer_1;
static Timer_Apbctrl1 timer_2;
static uint32_t created_semaphores_count = 0;
static rtems_id hal_semaphore_ids[RT_MAX_HAL_SEMAPHORES];

rtems_name generate_new_hal_semaphore_name();

rtems_name generate_new_hal_semaphore_name()
{
	static rtems_name name = rtems_build_name('H', 0, 0, 0);
	return name++;
}

bool Hal_Init(void)
{
	Timer_Config config_1;
	config_1.isEnabled = true;
	config_1.isAutoReloaded = false;
	config_1.isInterruptEnabled = false;
	config_1.isChained = false;
	config_1.reloadValue = TIMER_START_VALUE;

	Timer_Config config_2;
	config_2.isEnabled = true;
	config_2.isAutoReloaded = false;
	config_2.isInterruptEnabled = false;
	config_2.isChained = true;
	config_2.reloadValue = TIMER_START_VALUE;

	Timer_Apbctrl1_init(Timer_Id_3, &timer_1, defaultInterruptHandler);
	Timer_Apbctrl1_setBaseScalerReloadValue(&timer_1, TIMER_SCALER_VALUE);
	Timer_Apbctrl1_setConfigRegisters(&timer_1, &config_1);
	Timer_Apbctrl1_start(&timer_1);

	Timer_Apbctrl1_init(Timer_Id_4, &timer_2, defaultInterruptHandler);
	Timer_Apbctrl1_setBaseScalerReloadValue(&timer_2, TIMER_SCALER_VALUE);
	Timer_Apbctrl1_setConfigRegisters(&timer_2, &config_2);
	Timer_Apbctrl1_start(&timer_2);

	return true;
}

uint64_t Hal_GetElapsedTimeInNs(void)
{
	return (uint64_t)Timer_Apbctrl1_getCounterValue(&timer_2) << 32 |
	       Timer_Apbctrl1_getCounterValue(&timer_1);
}

bool Hal_SleepNs(uint64_t time_ns) { return true; }

int32_t Hal_SemaphoreCreate(void)
{
	if (created_semaphores_count >= RT_MAX_HAL_SEMAPHORES) {
		return 0;
	}

	rtems_status_code status_code = rtems_semaphore_create(
	    generate_new_hal_semaphore_name(),
	    1, // Initial value, unlocked
	    RTEMS_BINARY_SEMAPHORE,
	    0, // Priority ceiling
	    &hal_semaphore_ids[created_semaphores_count]);

	if (status_code == RTEMS_SUCCESSFUL) {
		return hal_semaphore_ids[created_semaphores_count++];
	}

	return 0;
}

bool Hal_SemaphoreObtain(int32_t id)
{
	rtems_status_code status_code;
	do {
		status_code =
		    rtems_semaphore_obtain(id, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
	} while (status_code != RTEMS_SUCCESSFUL);

	return true;
}

bool Hal_SemaphoreRelease(int32_t id)
{
	if (rtems_semaphore_release(id) == RTEMS_SUCCESSFUL) {
		return true;
	}

	return false;
}