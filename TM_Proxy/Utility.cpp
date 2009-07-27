// $Id$
#include "Utility.h"

namespace CIAO_CIAO_RACE_TM_Proxy_TM_Proxy_Component_Impl
{
  Utility_exec_i::Utility_exec_i (::CIAO::TM_Daemon::Daemon_ptr TM,
                                  Logger &logger)
    : TM_Daemon_ (::CIAO::TM_Daemon::Daemon::_duplicate (TM)),
      logger_ (logger)
  {
  }

  Utility_exec_i::~Utility_exec_i (void)
  {
  }
  ::Deployment::Domain* 
  Utility_exec_i::getInitialDomain ()
  {
    std::string msg = "Entering Utility_exec_i::getInitialDomain ()\n";
    msg += "Leaving Utility_exec_i::getInitialDomain ()\n";
    this->logger_.log (msg);          
    return this->TM_Daemon_->getInitialDomain ();
  }
}
