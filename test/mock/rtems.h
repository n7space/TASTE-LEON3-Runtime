#ifndef THREADIMPL_H
#define THREADIMPL_H

#include <stdint.h>
#include <stddef.h>

#define RTEMS_BINARY_SEMAPHORE 0x00000010
#define RTEMS_SUCCESSFUL 1
#define RTEMS_WAIT 0x00000000
#define RTEMS_NO_TIMEOUT 0x00000001

typedef int rtems_status_code;
typedef int rtems_id;
typedef char* rtems_name;
typedef uint32_t rtems_attribute;
typedef uint32_t rtems_task_priority;
typedef uint32_t rtems_option;
typedef uint32_t rtems_interval;

rtems_name rtems_build_name( char c1, char c2, char c3, char c4 );

#define  _Objects_Build_name( _C1, _C2, _C3, _C4 ) \
  ( (uint32_t) (uint8_t) (_C1) << 24 | \
    (uint32_t) (uint8_t) (_C2) << 16 | \
    (uint32_t) (uint8_t) (_C3) << 8 | \
    (uint8_t) (_C4) )

#define rtems_build_name( _c1, _c2, _c3, _c4 ) \
  _Objects_Build_name( _c1, _c2, _c3, _c4 )


#define RTEMS_MILLISECONDS_TO_TICKS( _ms ) 100

  rtems_status_code rtems_semaphore_create(
  rtems_name          name,
  uint32_t            count,
  rtems_attribute     attribute_set,
  rtems_task_priority priority_ceiling,
  rtems_id           *id
){}

rtems_status_code rtems_semaphore_obtain(
  rtems_id       id,
  rtems_option   option_set,
  rtems_interval timeout
){}

typedef void rtems_timer_service_routine;

typedef rtems_timer_service_routine ( *rtems_timer_service_routine_entry )( rtems_id, void * );

rtems_status_code rtems_semaphore_release( rtems_id id ){}

rtems_interval rtems_clock_get_ticks_per_second(){}

rtems_interval rtems_clock_get_ticks_since_boot(){}

rtems_status_code rtems_task_wake_after( rtems_interval ticks ){}

rtems_status_code rtems_timer_fire_after(
  rtems_id                          id,
  rtems_interval                    ticks,
  rtems_timer_service_routine_entry routine,
  void                             *user_data
)
{}

rtems_status_code rtems_message_queue_send(
  rtems_id    id,
  const void *buffer,
  size_t      size
){}

rtems_status_code rtems_timer_create( rtems_name name, rtems_id *id ){}

#endif