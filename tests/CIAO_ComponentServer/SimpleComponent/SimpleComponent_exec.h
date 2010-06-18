// $Id$

#ifndef CIAO_SIMPLECOMPONENT_EXEC_H
#define CIAO_SIMPLECOMPONENT_EXEC_H

#include /**/ "ace/pre.h"

#include "SimpleComponentEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SimpleComponent_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Foo_SimpleComponent_Impl
{
  class SIMPLECOMPONENT_EXEC_Export SimpleComponent_exec_i
    : public virtual SimpleComponent_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleComponent_exec_i (const char *CreateType);
    virtual ~SimpleComponent_exec_i (void);

    // Supported or inherited operations.

    virtual void
    trigger ();

    // Attribute operations.

    virtual char *
    creationtype_ ()
    ;

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
    ::Foo::CCM_SimpleComponent_Context_var context_;

    CORBA::String_var type_;

  };

  class SIMPLECOMPONENT_EXEC_Export SimpleHome_exec_i
    : public virtual SimpleHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleHome_exec_i (void);
    virtual ~SimpleHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" SIMPLECOMPONENT_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Foo_SimpleHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_SIMPLECOMPONENT_EXEC_H */

