// $Id$

#ifndef CIAO_SIMPLENULL_EXEC_H
#define CIAO_SIMPLENULL_EXEC_H

#include /**/ "ace/pre.h"

#include "SimpleNullEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SimpleNull_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Simple_SimpleNull_Impl
{
  class SIMPLENULL_EXEC_Export SimpleNull_exec_i
    : public virtual SimpleNull_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleNull_exec_i (void);
    virtual ~SimpleNull_exec_i (void);

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
    ::Simple::CCM_SimpleNull_Context_var context_;
  };

  extern "C" SIMPLENULL_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Simple_SimpleNull_Impl (void);

  class SIMPLENULL_EXEC_Export SimpleNullHome_exec_i
    : public virtual SimpleNullHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleNullHome_exec_i (void);
    virtual ~SimpleNullHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" SIMPLENULL_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleNullHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_SIMPLENULL_EXEC_H */

