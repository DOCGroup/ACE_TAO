// $Id$

#ifndef CIAO_SIMPLENILFACETUSER_EXEC_H
#define CIAO_SIMPLENILFACETUSER_EXEC_H

#include /**/ "ace/pre.h"

#include "SimpleNilFacetUserEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SimpleNilFacetUser_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Simple_SimpleNilFacetUser_Impl
{
  class SIMPLENILFACETUSER_EXEC_Export SimpleNilFacetUser_exec_i
    : public virtual SimpleNilFacetUser_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleNilFacetUser_exec_i (void);
    virtual ~SimpleNilFacetUser_exec_i (void);

    // Supported or inherited operations.

    // Attribute operations.

    // Port operations.

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete ();

    virtual void ccm_activate ();

    virtual void ccm_passivate ();

    virtual void ccm_remove ();

    private:
    ::Simple::CCM_SimpleNilFacetUser_Context_var context_;
  };

  class SIMPLENILFACETUSER_EXEC_Export SimpleNilFacetUserHome_exec_i
    : public virtual SimpleNilFacetUserHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleNilFacetUserHome_exec_i (void);
    virtual ~SimpleNilFacetUserHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" SIMPLENILFACETUSER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleNilFacetUserHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_SIMPLENILFACETUSER_EXEC_H */

