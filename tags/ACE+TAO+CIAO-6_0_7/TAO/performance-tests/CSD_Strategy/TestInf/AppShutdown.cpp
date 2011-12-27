// $Id$
#include "AppShutdown.h"
#include "TestAppExceptionC.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

AppShutdown::AppShutdown()
  : num_clients_(0),
    num_clients_shutdown_(0)
{
}


AppShutdown::~AppShutdown()
{
}


void
AppShutdown::init(CORBA::ORB_ptr orb,
                  unsigned num_clients)
{
  if ((!CORBA::is_nil(this->orb_.in())) ||
      (CORBA::is_nil(orb))              ||
      (num_clients == 0))
    {
      // Already init()'ed, or bad argument values.
      throw TestAppException();
    }
  else
    {
      this->orb_ = CORBA::ORB::_duplicate(orb);
      this->num_clients_ = num_clients;
      this->num_clients_shutdown_ = 0;
    }
}


void
AppShutdown::wait ()
{
  this->orb_shutdown_task_.wait();
}


void
AppShutdown::client_done()
{
  if ((this->num_clients_ == 0) || (CORBA::is_nil(this->orb_.in())))
    {
      ACE_ERROR((LM_ERROR, "(%P|%t) AppShutdown was never initialized.\n"));
      return;
    }

  unsigned cur_shutdown;

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
    cur_shutdown = ++this->num_clients_shutdown_;
  }

  if (cur_shutdown == this->num_clients_)
    {
      // Sleep for one second before shutting down the ORB.  This
      // is a poor-man version of "wait until the CSD request queue drains".
      ACE_OS::sleep(1);
      this->orb_shutdown_task_.orb(this->orb_.in());
      if (this->orb_shutdown_task_.open(0) != 0)
        {
          ACE_ERROR((LM_ERROR, "(%P|%t) AppShutdown failed to create orb "\
            "shutdown thread.\n"));
          return;
        }
    }
}


AppShutdown*
AppShutdown::instance ()
{
  static AppShutdown app_shutdown;
  return &app_shutdown;
}
