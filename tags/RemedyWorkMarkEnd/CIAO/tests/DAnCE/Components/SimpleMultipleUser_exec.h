// $Id$

#ifndef CIAO_SIMPLEMULTIPLEUSER_EXEC_H
#define CIAO_SIMPLEMULTIPLEUSER_EXEC_H

#include /**/ "ace/pre.h"

#include "SimpleMultipleUserEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SimpleMultipleUser_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Simple_SimpleMultipleUser_Impl
{
  class SIMPLEMULTIPLEUSER_EXEC_Export SimpleMultipleUser_exec_i
    : public virtual SimpleMultipleUser_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleMultipleUser_exec_i (void);
    virtual ~SimpleMultipleUser_exec_i (void);

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
    ::Simple::CCM_SimpleMultipleUser_Context_var context_;
  };

  class SIMPLEMULTIPLEUSER_EXEC_Export SimpleMultipleUserHome_exec_i
    : public virtual SimpleMultipleUserHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleMultipleUserHome_exec_i (void);
    virtual ~SimpleMultipleUserHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" SIMPLEMULTIPLEUSER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleMultipleUserHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_SIMPLEMULTIPLEUSER_EXEC_H */

