#if !defined (_REACTOR_TIMER_QUEUE_TEST_H_)
#define _REACTOR_TIMER_QUEUE_TEST_H_

#include "ace/Timer_Heap.h"
#include "Driver.h"

class Reactor_Timer_Queue_Test_Driver;

class Input_Handler : public ACE_Event_Handler
// @@ Please add comments.
{
public:
  Input_Handler (ACE_Timer_Queue *tq, Reactor_Timer_Queue_Test_Driver &timer_queue_driver);
  int handle_input (ACE_HANDLE);

  int done (void);
  int schedule_timer (void *argument);
  int cancel_timer (void *argument);
  int list_timer (void *argument);
  int shutdown_timer (void *argument);

private:
  ACE_Timer_Queue *tq_;
  // Keep a pointer to the timer queue we are using so we can
  // traverse the queue.

  int done_;
  // Flag used to close down program.

  Reactor_Timer_Queue_Test_Driver &driver_;
};

class Reactor_Timer_Queue_Test_Driver : public Timer_Queue_Test_Driver <ACE_Timer_Heap, Input_Handler>
// @@ Please add comments.
{
public:
  Reactor_Timer_Queue_Test_Driver (void);

  virtual int display_menu (void);
  virtual int init (void);
  virtual int run_test (void);
  
private:
  // pick a timer queue implementation
  // which happens to be the one that ACE is using.
  //  ACE_Timer_Heap private_queue;

  // This is the stdin handler.
  Input_Handler thandler;
};

class Timer_Handler : public ACE_Event_Handler
// @@ Please add comments.
{
public:
  //  Timer_Handler (Reactor_Queue_Test_Driver &driver);
  Timer_Handler (void);

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *);

  void set_timer_id (long tid);
  
private:
  long tid_;
};

#endif /* _REACTOR_TIMER_QUEUE_TEST_H_ */
