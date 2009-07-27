// $Id$
#include "SystemSnapshot.h"

namespace CIAO_CIAO_RACE_TM_Proxy_TM_Proxy_Component_Impl
{
  SystemSnapshot_exec_i::SystemSnapshot_exec_i 
  (::CIAO::TM_Daemon::Daemon_ptr TM,
   Logger &logger)
    : TM_Daemon_ (::CIAO::TM_Daemon::Daemon::_duplicate (TM)),
      logger_ (logger)
  {
  }

  SystemSnapshot_exec_i::~SystemSnapshot_exec_i (void)
  {
  }

  ::Deployment::Domain* 
  SystemSnapshot_exec_i::getSnapshot ()
  {
    std::string msg = "Entering SystemSnapshot_exec_i::getSnapshot ()\n";
    msg += "Leaving SystemSnapshot_exec_i::getSnapshot ()\n";
    this->logger_.log (msg);          
    return this->TM_Daemon_->getSnapshot ();
  }
}
