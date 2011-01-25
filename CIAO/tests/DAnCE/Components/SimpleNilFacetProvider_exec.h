// $Id$

#ifndef CIAO_SIMPLENILFACETPROVIDER_EXEC_H
#define CIAO_SIMPLENILFACETPROVIDER_EXEC_H

#include /**/ "ace/pre.h"

#include "SimpleNilFacetProviderEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SimpleNilFacetProvider_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Simple_SimpleNilFacetProvider_Impl
{
  class SIMPLENILFACETPROVIDER_EXEC_Export Trigger_exec_i
    : public virtual ::Simple::CCM_Trigger,
      public virtual ::CORBA::LocalObject
  {
    public:
    Trigger_exec_i (void);
    virtual ~Trigger_exec_i (void);

    // Operations from ::Simple::Trigger

    virtual void
    hello (
      const char * hello);
  };

  class SIMPLENILFACETPROVIDER_EXEC_Export SimpleNilFacetProvider_exec_i
    : public virtual SimpleNilFacetProvider_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleNilFacetProvider_exec_i (void);
    virtual ~SimpleNilFacetProvider_exec_i (void);

    // Supported or inherited operations.

    // Attribute operations.

    // Port operations.

    virtual ::Simple::CCM_Trigger_ptr
    get_trig_provides ();

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete ();

    virtual void ccm_activate ();

    virtual void ccm_passivate ();

    virtual void ccm_remove ();

    private:
    ::Simple::CCM_SimpleNilFacetProvider_Context_var context_;
  };

  class SIMPLENILFACETPROVIDER_EXEC_Export SimpleNilFacetProviderHome_exec_i
    : public virtual SimpleNilFacetProviderHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleNilFacetProviderHome_exec_i (void);
    virtual ~SimpleNilFacetProviderHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" SIMPLENILFACETPROVIDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleNilFacetProviderHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_SIMPLENILFACETPROVIDER_EXEC_H */

