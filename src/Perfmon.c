#include <Perfmon.h>


static thread_info Perf_Mon_getThreadInfo(Perf_Mon *perf_mon_ctx, uint64_t threadId)
{
    for (int i = 0; i < RUNTIME_THREAD_COUNT; i++)
    {
        if(threadId == threads_info[i].id)
        {
            return threads_info[i];
        }
    }
}

static bool Perf_Mon_cpuUsageVisitor( Thread_Control *the_thread, void *arg )
{
    Perf_Mon           *ctx;
    char               name[MAX_THREAD_NAME_SIZE];
    float              val;
    float              meanVal;
    uint32_t           ival;
    uint32_t           fval;
    Timestamp_Control  uptime;
    Timestamp_Control  used;

    ctx = arg;
    _Thread_Get_name( the_thread, name, sizeof(name) );

    used = _Thread_Get_CPU_time_used_after_last_reset(the_thread);
    _TOD_Get_uptime( &uptime );
    _Timestamp_Subtract(&ctx->uptime_at_last_reset, &uptime, &ctx->total_usage_time);
    _Timestamp_Divide(&used, &ctx->total_usage_time, &ival, &fval);

    val = (float)fval / TOD_NANOSECONDS_PER_MICROSECOND;
    val += (float)ival;

    if(val < ctx->usage_data[ctx->task_iterator].cpu_usage_min_val)
    {
        ctx->usage_data[ctx->task_iterator].cpu_usage_min_val = val;
    }

    if(val > ctx->usage_data[ctx->task_iterator].cpu_usage_max_val)
    {
        ctx->usage_data[ctx->task_iterator].cpu_usage_max_val = val;
    }

    meanVal = ctx->usage_data[ctx->task_iterator].cpu_usage_mean_val +
            (val - ctx->usage_data[ctx->task_iterator].cpu_usage_mean_val) /
            (ctx->benchmarking_ticks + 1);

    ctx->usage_data[ctx->task_iterator].cpu_usage_mean_val = meanVal;
    ctx->usage_data[ctx->task_iterator].system_ticks_spent_by_interface_min_val =
            Perf_Mon_getThreadInfo(ctx, the_thread->Object.id).min_thread_execution_time;
    ctx->usage_data[ctx->task_iterator].system_ticks_spent_by_interface_max_val =
            Perf_Mon_getThreadInfo(ctx, the_thread->Object.id).max_thread_execution_time;
    ctx->usage_data[ctx->task_iterator].system_ticks_spent_by_interface_mean_val =
            Perf_Mon_getThreadInfo(ctx, the_thread->Object.id).mean_thread_execution_time;

    uint32_t microse = rtems_clock_get_ticks_per_second();

    ctx->task_iterator++;
    return false;
}

static bool Perf_Mon_dataInitVisitor(Thread_Control *the_thread, void *arg)
{
   Perf_Mon   *ctx;
   char       name[MAX_THREAD_NAME_SIZE];

   ctx = arg;
   _Thread_Get_name( the_thread, name, sizeof( name ) );

   ctx->usage_data[ctx->task_iterator].id = the_thread->Object.id;
   strncpy(ctx->usage_data[ctx->task_iterator].thread_name, name, sizeof(name));
   ctx->usage_data[ctx->task_iterator].thread_name[sizeof(name) - 1] = '\0';
   ctx->usage_data[ctx->task_iterator].cpu_usage_min_val = FLT_MAX;
   ctx->usage_data[ctx->task_iterator].system_ticks_spent_by_interface_min_val = (uint64_t)(-1);

   ctx->task_iterator++;
   ctx->task_count++;
   return false;
}

void Perf_Mon_init(Perf_Mon *perf_mon_ctx)
{
    rtems_cpu_usage_reset();
    _TOD_Get_uptime( &perf_mon_ctx->CPU_usage_Uptime_at_last_reset );
    perf_mon_ctx->isInit = false;
}

void Perf_Mon_tick(Perf_Mon *perf_mon_ctx)
{
    if(!perf_mon_ctx->isInit)
    {
        perf_mon_ctx->task_count = 0;
        perf_mon_ctx->task_iterator = 0;
        perf_mon_ctx->benchmarking_ticks = 0;

        rtems_task_iterate(Perf_Mon_dataInitVisitor, perf_mon_ctx);

        perf_mon_ctx->isInit = true;
    }

    /*
     *  When not using nanosecond CPU usage resolution, we have to count
     *  the number of "ticks" we gave credit for to give the user a rough
     *  guideline as to what each number means proportionally.
     */
    _Timestamp_Set_to_zero(&perf_mon_ctx->total_usage_time);
    perf_mon_ctx->uptime_at_last_reset = perf_mon_ctx->CPU_usage_Uptime_at_last_reset;
    perf_mon_ctx->task_iterator = 0;

    rtems_task_iterate(Perf_Mon_cpuUsageVisitor, perf_mon_ctx);
    perf_mon_ctx->benchmarking_ticks++;
}

Interface_Usage_Data Perf_Mon_getUsageData(Perf_Mon *perf_mon_ctx, enum interfaces_enum interface, double system_frequency)
{
    Interface_Usage_Data data;
    rtems_id id = threads_info[interface].id;

    for (int i = 0; i < perf_mon_ctx->task_count; i++)
    {
        if(id == perf_mon_ctx->usage_data[i].id)
        {
            data = perf_mon_ctx->usage_data[i];
            strncpy(data.interface_name, threads_info[interface].name, sizeof(data.interface_name));
            data.interface_name[sizeof(data.interface_name) - 1] = '\0';

            data.microseconds_spent_by_interface_min_val = (double)(data.system_ticks_spent_by_interface_min_val) *
                    (double)(TIMER_SCALER_VALUE) * (double)(TOD_MICROSECONDS_PER_SECOND) / system_frequency;

            data.microseconds_spent_by_interface_max_val = (double)(data.system_ticks_spent_by_interface_max_val) *
                    (double)(TIMER_SCALER_VALUE) * (double)(TOD_MICROSECONDS_PER_SECOND) / system_frequency;

            data.microseconds_spent_by_interface_mean_val = (double)(data.system_ticks_spent_by_interface_mean_val) *
                    (double)(TIMER_SCALER_VALUE) * (double)(TOD_MICROSECONDS_PER_SECOND) / system_frequency;
            break;
        }
    }

    return data;
}

Summarized_Usage_Data Perf_Mon_getIdleUsageData(Perf_Mon *perf_mon_ctx)
{
    Summarized_Usage_Data   data;
    Interface_Usage_Data    usageData;

    usageData = perf_mon_ctx->usage_data[0];

    strncpy(data.name, "IDLE", sizeof(data.name));
    data.cpu_usage_min_val = usageData.cpu_usage_min_val;
    data.cpu_usage_max_val = usageData.cpu_usage_max_val;
    data.cpu_usage_mean_val = usageData.cpu_usage_mean_val;

    return data;
}

Summarized_Usage_Data Perf_Mon_getThreadsSummarizedUsageData(Perf_Mon *perf_mon_ctx)
{
    Summarized_Usage_Data   data;
    Interface_Usage_Data    usageData;

    usageData = perf_mon_ctx->usage_data[0];

    strncpy(data.name, "SUMMARIZED", sizeof(data.name));
    data.cpu_usage_min_val = 100.0 - usageData.cpu_usage_min_val;
    data.cpu_usage_max_val = 100.0 - usageData.cpu_usage_max_val;
    data.cpu_usage_mean_val = 100.0 - usageData.cpu_usage_mean_val;

    return data;
}