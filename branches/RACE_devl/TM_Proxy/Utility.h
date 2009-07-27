// $Id$
#ifndef UTILITY_H
#define UTILITY_H

#include /**/ "ace/pre.h"

#include "TM_ProxyEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TM_Proxy_exec_export.h"
#include "tao/LocalObject.h"

#include "TM_Daemon/TM_DaemonC.h"
#include "utils/Logger.h"

using namespace CIAO::RACE;

namespace CIAO_CIAO_RACE_TM_Proxy_TM_Proxy_Component_Impl
{
  class TM_PROXY_EXEC_Export Utility_exec_i
    : public virtual ::CIAO::RACE::TM_Proxy::CCM_Utility,
      public virtual TAO_Local_RefCounted_Object
  {
  public:
    Utility_exec_i (::CIAO::TM_Daemon::Daemon_ptr TM, Logger &logger);

    virtual ~Utility_exec_i (void);

    virtual ::Deployment::Domain* getInitialDomain ();

  private:
    ::CIAO::TM_Daemon::Daemon_var TM_Daemon_;

    Logger &logger_;

  };
}

#include /**/ "ace/post.h"

#endif /* UTILITY_H */
