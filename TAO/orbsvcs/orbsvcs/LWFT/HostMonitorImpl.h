// -*- C++ -*-
// $Id$

#ifndef __HOSTMONITORIMPL_H
#define __HOSTMONITORIMPL_H

#include <string>

#include "ace/Map_Manager.h"
#include "ace/Event_Handler.h"
#include "ace/Thread_Mutex.h"
#include "ace/SOCK_Connector.h"

#include "HostMonitorS.h"

#include "Failure_Handler.h"
#include "Utilization_Monitor.h"
#include "RM_Proxy.h"

class Monitor_Thread;

class HostMonitor_Export HostMonitorImpl
  : public virtual POA_HostMonitor
{
public:

  typedef ACE_Map_Manager <std::string,
                           Failure_Handler *,
                           ACE_Thread_Mutex>
    ProcessHandlerMap;

  HostMonitorImpl (CORBA::ORB_ptr, Monitor_Thread *);
  
  int
  drop_process (const std::string &process_id);

  virtual
  ~HostMonitorImpl ();

  ::CORBA::Boolean
  register_process (const char *process_id,
                    const char * hostname,
                    CORBA::Long port);

  ::CORBA::Boolean
  unregister_process (const char *process_id);

  virtual ::CORBA::Short
  heartbeat_port (void);

  void
  dump (void);

protected:
  RM_Proxy *
  create_RM_Proxy (void);
  
  void
  remove_RM_Proxy (void);
  
  int
  remove_process (std::string const &process_id);

private:
  Monitor_Thread *monitor_thread_;
  CORBA::Short port_counter_;
  std::auto_ptr <RM_Proxy> rm_proxy_;
  std::auto_ptr <Utilization_Monitor> util_mon_;
  ProcessHandlerMap process_map_;
  ACE_Connector <Failure_Handler, ACE_SOCK_Connector> connector_;
  CORBA::ORB_var orb_;
};

#endif // __HOSTMONITORIMPL_H
