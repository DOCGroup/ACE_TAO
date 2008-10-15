/**
 *  @file  C++ Interface: Monitor_Thread
 *
 *  @brief Declares interface for Monitor_Thread.
 *
 */

#ifndef __MONITOR_THREAD_H_
#define __MONITOR_THREAD_H_

#include "ace/Reactor.h"
#include "ace/Task.h"

/**
 *  @class  Monitor_Thread
 *
 *  @brief Encapsulates Monitor_Thread
 */

class Monitor_Thread : public ACE_Task_Base
{
  public:
    Monitor_Thread ();
    virtual int svc (void);
    //int  register_handler (ACE_Event_Handler *event_handler, ACE_Reactor_Mask mask);
    ACE_Reactor * get_reactor ();

  private:
    ACE_Reactor reactor_;
};



#endif /// __MONITOR_THREAD_H_
