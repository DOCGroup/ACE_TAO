/**
 *  @file  C++ Implementation: Failure_Handler
 *
 *  @brief Defines implementation of Failure_Handler.
 *
 */

#include "Failure_Handler.h"
#include "HostMonitorImpl.h"

ProcessInfo::ProcessInfo (const std::string &pid, const std::string &hn, size_t p)
: process_id (pid),
  hostname (hn),
  port (p)
{}


Failure_Handler::ProcessInfoMap Failure_Handler::process_map_;

Failure_Handler::Failure_Handler ()
: connector_factory_ (0),
  host_monitor_ (0)
{}

int Failure_Handler::handle_input (ACE_HANDLE fd)
{
  ProcessInfo pinfo;
  if (process_map_.find (fd, pinfo) == 0) /// if found
  {
    ACE_DEBUG ((LM_DEBUG,"It looks like process %s has failed.\n", pinfo.process_id.c_str()));
    process_map_.unbind (fd);
    if (host_monitor_->drop_process (pinfo.process_id) == 0)
      /// release the handler.
      return -1;
    else
      ACE_DEBUG ((LM_DEBUG,"drop_process failed process_id = %s.\n", pinfo.process_id.c_str()));
  }

  return -1;
}

void Failure_Handler::set_host_monitor (HostMonitorImpl *hm)
{
  host_monitor_ = hm;
}

int Failure_Handler::open (void *factory)
{
  connector_factory_ = static_cast <FactoryConnector *> (factory);
  return super::open (factory);
}

int Failure_Handler::drop_process (ACE_HANDLE fd)
{
  ProcessInfo pinfo;
  if ((process_map_.find (fd, pinfo) == 0) &&
      (process_map_.unbind (fd) == 0))
  {
    connector_factory_->reactor ()->remove_handler (this, super::ALL_EVENTS_MASK);
    ACE_DEBUG ((LM_DEBUG,"Record for process %s removed from the map.\n", pinfo.process_id.c_str()));
    return 0;
  }
  else
    return -1;
}

int Failure_Handler::watch_process (ACE_HANDLE fd,
                                    const std::string &process_id,
                                    const std::string &hostname,
                                    size_t port)
{
  return process_map_.bind (fd, ProcessInfo (process_id, hostname, port));
}
