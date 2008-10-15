#include "HostMonitorImpl.h"
#include "Failure_Handler.h"
#include "Monitor_Thread.h"
#include "RM_Proxy.h"
#include "HMOptions.h"
#include "LinuxCPULoadCalculator.h"
#include "Utilization_Monitor.h"

#include "ace/Connector.h"
#include "ace/Service_Object.h"
#include "ace/Reactor.h"
#include "ace/INET_Addr.h"
#include "ace/Time_Value.h"
#include "ace/Synch_Options.h"
#include "ace/Thread_Mutex.h"


HostMonitorImpl::HostMonitorImpl (CORBA::ORB_ptr orb, Monitor_Thread *mt)
: monitor_thread_ (mt),
  connector_ (monitor_thread_->get_reactor ()),
  orb_ (CORBA::ORB::_duplicate(orb))
{
  this->create_rm_proxy ();
}

void
HostMonitorImpl::dump (void)
throw (CORBA::SystemException)
{
  //ACE_DEBUG ((LM_DEBUG, "inside dump method\n"));
}


::CORBA::Boolean
HostMonitorImpl::register_process (const char *process_id, const char * hostname, CORBA::Long port)
throw (CORBA::SystemException)
{
  //ACE_DEBUG ((LM_DEBUG, "Entering register process\n"));
  Failure_Handler *handler = 0;
  ACE_SOCK_Connector::PEER_ADDR serv_addr;
  serv_addr.set (port, hostname);
  ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT, ACE_Time_Value (1));

  if (connector_.connect (handler, serv_addr, options) < 0)
  {
    ACE_ERROR_RETURN((LM_ERROR,"Failed to open an connector socket.\n"), 1);
  }

  //ACE_DEBUG ((LM_DEBUG, "Entering register process\n"));

  handler->set_host_monitor (this);
  handler->watch_process (handler->get_handle(), process_id, hostname, port);
  process_map_.bind (process_id, handler);
  //this->create_rm_proxy ();
  ACE_DEBUG ((LM_DEBUG,"HostMonitorImpl::register_process process_id = %s, port = %d.\n",process_id, port));

  return true;
}

::CORBA::Boolean
HostMonitorImpl::unregister_process (const char *process_id)
throw (CORBA::SystemException)
{
  Failure_Handler *handler = 0;
  if (process_map_.find (process_id, handler) == 0)
  {
      ACE_DEBUG ((LM_DEBUG,"HostMonitorImpl::unregister_process %s.\n", process_id));
      if ((handler->drop_process (handler->get_handle ()) == 0))
      {
        if (remove_process (process_id) == 0)
          return true;
      }
      else
        ACE_DEBUG ((LM_DEBUG,"HostMonitorImpl::unregister_process Process %s can't be dropped!\n", process_id));
  }
  else
    ACE_DEBUG ((LM_DEBUG,"HostMonitorImpl::unregister_process Invalid process_id = %s.\n", process_id));

  return true;
}

int HostMonitorImpl::drop_process (const std::string &process_id)
{
  Failure_Handler *handler = 0;
  if (process_map_.find (process_id, handler) == 0)
  {
     rm_proxy_->proc_failure (process_id);
     return remove_process (process_id);
  }
  return -1;
}

RM_Proxy *HostMonitorImpl::create_rm_proxy (void)
{
  if (rm_proxy_.get() == 0)
  {
    std::auto_ptr <CPULoadCalculator> linux_load_calc (new LinuxCPULoadCalculator());
    //std::auto_ptr <CPULoadCalculator> linux_load_calc (new FCS_Monitor_Adapter ());
    std::auto_ptr <Utilization_Monitor> util_mon (
        new Utilization_Monitor (linux_load_calc.get()));
    linux_load_calc.release ();
    std::auto_ptr <RM_Proxy> rm_proxy (new RM_Proxy (orb_));

    rm_proxy->setCPULoadCalculator (util_mon.get());
    util_mon->setRM_Proxy (rm_proxy.get());

    rm_proxy->hertz (HMOptions::instance()->RM_update_freq());
    util_mon->hertz (HMOptions::instance()->load_monitor_freq ());

    util_mon->start ();
    rm_proxy->start ();

    /// Trasnfer of ownership.
    this->util_mon_ = util_mon;
    this->rm_proxy_ = rm_proxy;
  }
  return rm_proxy_.get();
}

void HostMonitorImpl::remove_rm_proxy (void)
{
  if (process_map_.current_size() == 0)
  {
    rm_proxy_->stop ();
    util_mon_->stop ();
    rm_proxy_.reset (0);
    util_mon_.reset (0);
  }
}

int HostMonitorImpl::remove_process (std::string const &process_id)
{
  if (process_map_.unbind (process_id) == 0)
  {
    remove_rm_proxy ();
    return 0;
  }
  return -1;
}

