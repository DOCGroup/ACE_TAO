// $Id$
#ifndef SYSTEMSNAPSHOT_H
#define SYSTEMSNAPSHOT_H

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
  
  class TM_PROXY_EXEC_Export SystemSnapshot_exec_i
    : public virtual ::CIAO::RACE::TM_Proxy::CCM_SystemSnapshot,
      public virtual TAO_Local_RefCounted_Object
  {
  public:
    SystemSnapshot_exec_i 
      (::CIAO::TM_Daemon::Daemon_ptr TM,
       Logger &logger);

    virtual ~SystemSnapshot_exec_i (void);

    virtual ::Deployment::Domain* getSnapshot ();

  private:

    ::CIAO::TM_Daemon::Daemon_var TM_Daemon_;

    Logger &logger_;

  };
}
#include /**/ "ace/post.h"

#endif /* SYSTEMSNAPSHOT_H */
