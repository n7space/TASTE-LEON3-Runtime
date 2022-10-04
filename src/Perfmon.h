/**@file
 * This file is part of the TASTE Leon3 Runtime.
 *
 * @copyright 2022 N7 Space Sp. z o.o.
 *
 * TASTE Leon3 Runtime was developed under a programme of,
 * and funded by, the European Space Agency (the "ESA").
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

#ifndef PERFMON_H
#define PERFMON_H

/**
 * @file    Perfmon.h
 * @brief   Header for performance monitoring of TASTE interfaces
 */

#include "interfaces_info.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <rtems.h>
#include <rtems/cpuuse.h>
#include <rtems/score/threadimpl.h>
#include <rtems/score/todimpl.h>

/**
 * @brief   Struct representing usage and benchmarking data for the given interface
 */
typedef struct 
{
    uint32_t          id;
    char              thread_name[MAX_THREAD_NAME_SIZE];
    char              interface_name[MAX_THREAD_NAME_SIZE];
    float             cpu_usage_min_val;
    float             cpu_usage_max_val;
    float             cpu_usage_mean_val;
    uint64_t          system_ticks_spent_by_interface_min_val;
    uint64_t          system_ticks_spent_by_interface_max_val;
    double            system_ticks_spent_by_interface_mean_val;
    uint64_t          microseconds_spent_by_interface_min_val;
    uint64_t          microseconds_spent_by_interface_max_val;
    double            microseconds_spent_by_interface_mean_val;
} Interface_Usage_Data;

/**
 * @brief   Struct representing usage data for summarized states
 */
typedef struct 
{
    char              name[MAX_THREAD_NAME_SIZE];
    float             cpu_usage_min_val;
    float             cpu_usage_max_val;
    float             cpu_usage_mean_val;
} Summarized_Usage_Data;

/**
 * @brief   Struct representing Perfmon context
 */
typedef struct 
{
    Timestamp_Control        CPU_usage_Uptime_at_last_reset;
    Interface_Usage_Data     usage_data[RUNTIME_THREAD_COUNT];
    uint32_t                 task_iterator;
    uint32_t                 task_count;
    uint64_t                 benchmarking_ticks;
    bool                     isInit;
    Timestamp_Control        total_usage_time;
    Timestamp_Control        uptime_at_last_reset;
} Perf_Mon;

/**
 * @brief   Returns thread information object
 *
 * @param[in] perf_mon_ctx     Perfmon context
 * @param[in] threadId         Id of the thread
 *
 * @return  Thread information object
 */
static thread_info Perf_Mon_getThreadInfo(Perf_Mon *perf_mon_ctx, uint64_t threadId);

/**
 * @brief   Visitor, updates benchmarking data of the thread,
 *          check rtems_task_iterate
 *
 * @param[in] the_thread       Thread for benchmarking data extraction
 * @param[in] arg              Argument pointer, should be Perfmon context
 *
 * @return  Returns false
 */
static bool Perf_Mon_cpuUsageVisitor(Thread_Control *the_thread, void *arg);

/**
 * @brief   Visitor, initialize benchmarking data of the thread,
 *          check rtems_task_iterate
 *
 * @param[in] the_thread       Thread for benchmarking data extraction
 * @param[in] arg              Argument pointer, should be Perfmon context
 *
 * @return  Returns false
 */
static bool Perf_Mon_dataInitVisitor(Thread_Control *the_thread, void *arg);

/**
 * @brief   Perfom initializer, should be invoked during benchmarking interface startup
 *
 * @param[in] perf_mon_ctx     Perfmon context
 */
void Perf_Mon_init(Perf_Mon *perf_mon_ctx);

/**
 * @brief   Perfom tick, updates interfaces benchmarking information data
 *          should be invoked in benchmarking thread loop
 *
 * @param[in] perf_mon_ctx     Perfmon context
 */
void Perf_Mon_tick(Perf_Mon *perf_mon_ctx);

/**
 * @brief   Gets benchmarking information data for a given thread
 *
 * @param[in] perf_mon_ctx       Perfmon context
 * @param[in] interface          Enum representing given interface, check interfaces_info.h
 * @param[in] system_frequency   system frequency given in Hz
 * 
 * @return  Returns benchmarking information data
 */
Interface_Usage_Data Perf_Mon_getUsageData(Perf_Mon *perf_mon_ctx, enum interfaces_enum interface, double system_frequency);

/**
 * @brief   Gets benchmarking information data for Idle state
 *
 * @param[in] perf_mon_ctx       Perfmon context
 * 
 * @return  Returns benchmarking information data for Idle state
 */
Summarized_Usage_Data Perf_Mon_getIdleUsageData(Perf_Mon *perf_mon_ctx);

/**
 * @brief   Gets benchmarking information data for all interfaces summarized
 *
 * @param[in] perf_mon_ctx       Perfmon context
 * 
 * @return  Returns benchmarking information data for all interfaces
 */
Summarized_Usage_Data Perf_Mon_getThreadsSummarizedUsageData(Perf_Mon *perf_mon_ctx);

#endif