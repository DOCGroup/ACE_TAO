// $Id$
#ifndef CIAO_TM_PROXY_EXEC_H
#define CIAO_TM_PROXY_EXEC_H

#include /**/ "ace/pre.h"

// #include "TM_ProxyEC.h"
#include "TM_Proxy_svnt.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TM_Proxy_exec_export.h"
#include "tao/LocalObject.h"

#include "TM_Daemon/TM_DaemonC.h"

#include "utils/Logger.h"

#include "Utility.h"
#include "ResourceManager.h"
#include "SystemSnapshot.h"


using namespace CIAO::RACE;

namespace CIAO_CIAO_RACE_TM_Proxy_TM_Proxy_Component_Impl
{
  class TM_PROXY_EXEC_Export TM_Proxy_Component_exec_i
    : public virtual TM_Proxy_Component_Exec,
      public virtual TAO_Local_RefCounted_Object
  {
  public:
    TM_Proxy_Component_exec_i (void);
    virtual ~TM_Proxy_Component_exec_i (void);

    // Attribute operations.
    virtual char*
      TM_Name ();

    void
      TM_Name (const char * name);

    // Port operations.

    virtual ::CIAO::RACE::TM_Proxy::CCM_SystemSnapshot_ptr
      get_snapshot ();

    virtual ::CIAO::RACE::TM_Proxy::CCM_Utility_ptr
      get_utils ();

    virtual ::CIAO::RACE::TM_Proxy::CCM_ResourceManager_ptr
      get_manager ();

    // Operations from Components::SessionComponent

    virtual void
      set_session_context (
                           ::Components::SessionContext_ptr ctx);

    virtual void ciao_preactivate ();

    virtual void configuration_complete ();

    virtual void ccm_activate ();

    virtual void ccm_passivate ();

    virtual void ccm_remove ();

  private:

    TM_Proxy_Component_Context *context_;

    ::CIAO::TM_Daemon::Daemon_var TM_Daemon_;

    ::CORBA::String_var TM_Name_;

    Logger logger_;
  };

  class TM_PROXY_EXEC_Export TM_Proxy_Component_Home_exec_i
    : public virtual TM_Proxy_Component_Home_Exec,
      public virtual TAO_Local_RefCounted_Object
  {
  public:
    TM_Proxy_Component_Home_exec_i (void);
    virtual ~TM_Proxy_Component_Home_exec_i (void);

    virtual ::Components::EnterpriseComponent_ptr
      create ();
  };

  extern "C" TM_PROXY_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_CIAO_RACE_TM_Proxy_TM_Proxy_Component_Home_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_TM_PROXY_EXEC_H */
