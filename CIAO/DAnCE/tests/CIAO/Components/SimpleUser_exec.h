// $Id$

#ifndef CIAO_SIMPLEUSER_EXEC_H
#define CIAO_SIMPLEUSER_EXEC_H

#include /**/ "ace/pre.h"

#include "SimpleUserEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SimpleUser_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Simple_SimpleUser_Impl
{
  class SIMPLEUSER_EXEC_Export SimpleUser_exec_i
    : public virtual SimpleUser_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleUser_exec_i (void);
    virtual ~SimpleUser_exec_i (void);

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
    ::Simple::CCM_SimpleUser_Context_var context_;
  };

  class SIMPLEUSER_EXEC_Export SimpleUserHome_exec_i
    : public virtual SimpleUserHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleUserHome_exec_i (void);
    virtual ~SimpleUserHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" SIMPLEUSER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleUserHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_SIMPLEUSER_EXEC_H */

