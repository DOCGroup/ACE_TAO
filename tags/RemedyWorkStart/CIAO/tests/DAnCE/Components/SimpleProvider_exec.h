// $Id$

#ifndef CIAO_SIMPLEPROVIDER_EXEC_H
#define CIAO_SIMPLEPROVIDER_EXEC_H

#include /**/ "ace/pre.h"

#include "SimpleProviderEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SimpleProvider_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Simple_SimpleProvider_Impl
{
  class SIMPLEPROVIDER_EXEC_Export Trigger_exec_i
    : public virtual ::Simple::CCM_Trigger,
      public virtual ::CORBA::LocalObject
  {
    public:
    Trigger_exec_i (bool &);
    virtual ~Trigger_exec_i (void);

    // Operations from ::Simple::Trigger

    virtual void
    hello (
      const char * hello);

  private:
    bool &triggered_;
  };

  class SIMPLEPROVIDER_EXEC_Export SimpleProvider_exec_i
    : public virtual SimpleProvider_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleProvider_exec_i (void);
    virtual ~SimpleProvider_exec_i (void);

    // Supported or inherited operations.

    // Attribute operations.

    // Port operations.

    virtual ::Simple::CCM_Trigger_ptr
    get_trig ();

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete ();

    virtual void ccm_activate ();

    virtual void ccm_passivate ();

    virtual void ccm_remove ();

    private:
    ::Simple::CCM_SimpleProvider_Context_var context_;
    bool triggered_;
  };

  class SIMPLEPROVIDER_EXEC_Export SimpleProviderHome_exec_i
    : public virtual SimpleProviderHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleProviderHome_exec_i (void);
    virtual ~SimpleProviderHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" SIMPLEPROVIDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleProviderHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_SIMPLEPROVIDER_EXEC_H */

