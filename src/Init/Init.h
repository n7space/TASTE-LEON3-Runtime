/**@file
 * This file is part of the TASTE Runtime Leon3.
 *
 * @copyright 2022 N7 Space Sp. z o.o.
 *
 * Test Environment was developed under a programme of,
 * and funded by, the European Space Agency (the "ESA").
 *
 *
 * Licensed under the ESA Public License (ESA-PL) Permissive,
 * Version 2.3 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://essr.esa.int/license/list
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef INIT_H
#define INIT_H

/**
 * @file    Init.h
 * @brief   Init - Leon3FT GR712RC board initialization
 *
 *  The Init component used by TASTE runtime to initialize Leon3FT GR712RC board,
 *  the following sequence is used:
 *
 *  Configure RTEMS settings
 *  Configure Timer
 *  Configure Uart
 *
 */

void Init_setup_hardware(void);

#endif
