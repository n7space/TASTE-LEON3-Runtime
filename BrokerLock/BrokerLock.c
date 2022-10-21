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

#include <BrokerLock.h>

#include <rtems.h>

extern rtems_id broker_Semaphore;

void
Broker_acquire_lock()
{
    while (rtems_semaphore_obtain(broker_Semaphore, RTEMS_WAIT, RTEMS_NO_WAIT) != RTEMS_SUCCESSFUL);
}

void
Broker_release_lock()
{
    rtems_semaphore_release(broker_Semaphore);
}