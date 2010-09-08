// $Id$

#ifndef CIAO_SIMPLEFAILURE_EXEC_H
#define CIAO_SIMPLEFAILURE_EXEC_H

#include /**/ "ace/pre.h"

#include "SimpleFailureEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SimpleFailure_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Simple_SimpleFailure_Impl
{
  class SIMPLEATTRIBUTE_EXEC_Export SimpleFailure_exec_i
    : public virtual SimpleFailure_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleFailure_exec_i (void);
    virtual ~SimpleFailure_exec_i (void);

    // Supported or inherited operations.

    // Attribute operations.

    virtual ::Simple::FailureType
    type_attr (void);

    virtual void
    type_attr (
      ::Simple::FailureType type_attr);

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
    ::Simple::CCM_SimpleFailure_Context_var context_;
    ::Simple::FailureType type_;
  };

  class SIMPLEATTRIBUTE_EXEC_Export SimpleFailureHome_exec_i
    : public virtual SimpleFailureHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleFailureHome_exec_i (void);
    virtual ~SimpleFailureHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  private:
  };

  extern "C" SIMPLEATTRIBUTE_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleFailureHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_SIMPLEFAILURE_EXEC_H */

