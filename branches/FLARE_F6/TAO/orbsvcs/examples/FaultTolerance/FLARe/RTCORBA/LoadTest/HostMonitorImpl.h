#ifndef __HOSTMONITORIMPL_H
#define __HOSTMONITORIMPL_H

#include "monitorS.h"
#include "LWFTS.h"

#include "ace/Map_Manager.h"
#include "ace/Event_Handler.h"
#include "ace/Thread_Mutex.h"
#include "ace/SOCK_Connector.h"

#include "Failure_Handler.h"
#include "Utilization_Monitor.h"
#include "RM_Proxy.h"
#include <string>

class Monitor_Thread;

class HostMonitorImpl : public virtual POA_HostMonitor
{
public:

  typedef ACE_Map_Manager <std::string, Failure_Handler *, ACE_Thread_Mutex> ProcessHandlerMap;

  explicit HostMonitorImpl (CORBA::ORB_ptr, Monitor_Thread *);
  int drop_process (const std::string &process_id);

  ::CORBA::Boolean
  register_process (const char *process_id, const char * hostname, CORBA::Long port)
  throw (CORBA::SystemException);

  ::CORBA::Boolean
  unregister_process (const char *process_id)
  throw (CORBA::SystemException);

  void dump (void)
  throw (CORBA::SystemException);

protected:
  RM_Proxy *create_rm_proxy (void);
  void remove_rm_proxy (void);
  int remove_process (std::string const &process_id);

private:
  Monitor_Thread *monitor_thread_;
  std::auto_ptr <RM_Proxy> rm_proxy_;
  std::auto_ptr <Utilization_Monitor> util_mon_;
  ProcessHandlerMap process_map_;
  ACE_Connector <Failure_Handler, ACE_SOCK_Connector> connector_;
  CORBA::ORB_var orb_;
};


#endif

