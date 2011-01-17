// $Id$

#ifndef CIAO_SIMPLEUSER_EXEC_H
#define CIAO_SIMPLEUSER_EXEC_H

#include /**/ "ace/pre.h"

#include "SimpleLocalUserEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SimpleLocalUser_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Simple_SimpleLocalUser_Impl
{
  class SIMPLEUSER_EXEC_Export SimpleLocalUser_exec_i
    : public virtual SimpleLocalUser_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleLocalUser_exec_i (void);
    virtual ~SimpleLocalUser_exec_i (void);

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
    ::Simple::CCM_SimpleLocalUser_Context_var context_;
  };

  class SIMPLEUSER_EXEC_Export SimpleLocalUserHome_exec_i
    : public virtual SimpleLocalUserHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleLocalUserHome_exec_i (void);
    virtual ~SimpleLocalUserHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" SIMPLEUSER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleLocalUserHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_SIMPLEUSER_EXEC_H */

