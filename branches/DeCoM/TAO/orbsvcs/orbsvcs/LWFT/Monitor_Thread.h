// -*- C++ -*-
// $Id$

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

#include "host_monitor_export.h"

/**
 *  @class  Monitor_Thread
 *
 *  @brief Encapsulates Monitor_Thread
 */

class HostMonitor_Export Monitor_Thread : public ACE_Task_Base
{
public:
  Monitor_Thread (void);
  virtual int svc (void);
  ACE_Reactor * get_reactor (void);

private:
  ACE_Reactor reactor_;
};



#endif /// __MONITOR_THREAD_H_
