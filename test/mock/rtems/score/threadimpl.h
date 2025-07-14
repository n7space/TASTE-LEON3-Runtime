#ifndef THREADIMPL_H
#define THREADIMPL_H

typedef int rtems_tcb;

typedef void (*rtems_task_visitor)(rtems_tcb *, void *);

void rtems_task_iterate(rtems_task_visitor visitor, void *arg) {}

#endif
