// -*- C++ -*-
// $Id$

/**
 *  @file  C++ Interface: Failure_Handler
 *
 *  @brief Declares interface for Failure_Handler.
 *
 */

#ifndef __FAILURE_HANDLER_H_
#define __FAILURE_HANDLER_H_

#include <string>

#include "ace/Svc_Handler.h"
#include "ace/Map_Manager.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"

#include "host_monitor_export.h"

struct ProcessInfo
{
  ProcessInfo () {}
  ProcessInfo (const std::string &pid,
               const std::string &hn,
               size_t port);

  std::string process_id;
  std::string hostname;
  size_t port;
};

/**
 *  @class  Failure_Handler
 *
 *  @brief Encapsulates Failure_Handler
 */

class HostMonitorImpl;

class HostMonitor_Export Failure_Handler
  : public ACE_Svc_Handler <ACE_SOCK_Connector::PEER_STREAM, ACE_NULL_SYNCH>
{
public:
  typedef ACE_Map_Manager <ACE_HANDLE, ProcessInfo,
                           ACE_Thread_Mutex>
    ProcessInfoMap;
    
  typedef ACE_Svc_Handler <ACE_SOCK_Connector::PEER_STREAM,
                           ACE_NULL_SYNCH>
    super;
    
  typedef ACE_Connector <super, ACE_SOCK_Connector> FactoryConnector;

  Failure_Handler (void);

  virtual int handle_input (ACE_HANDLE fd);
  virtual int open (void *factory);

  void set_host_monitor (HostMonitorImpl *hm);
  int watch_process (ACE_HANDLE fd,
                     const std::string &process_id,
                     const std::string &hostname,
                     size_t port);
  int drop_process (ACE_HANDLE fd);

private:
  static ProcessInfoMap process_map_;
  FactoryConnector *connector_factory_;
  HostMonitorImpl *host_monitor_;
};




#endif /// __FAILURE_HANDLER_H_
