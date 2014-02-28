// $Id$

#ifndef CIAO_SIMPLEEMITTER_EXEC_H
#define CIAO_SIMPLEEMITTER_EXEC_H

#include /**/ "ace/pre.h"

#include "SimpleEmitterEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SimpleEmitter_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Simple_SimpleEmitter_Impl
{
  class SIMPLEEMITTER_EXEC_Export SimpleEmitter_exec_i
    : public virtual SimpleEmitter_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleEmitter_exec_i (void);
    virtual ~SimpleEmitter_exec_i (void);

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
    ::Simple::CCM_SimpleEmitter_Context_var context_;
  };

  class SIMPLEEMITTER_EXEC_Export SimpleEmitterHome_exec_i
    : public virtual SimpleEmitterHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleEmitterHome_exec_i (void);
    virtual ~SimpleEmitterHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" SIMPLEEMITTER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleEmitterHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_SIMPLEEMITTER_EXEC_H */

