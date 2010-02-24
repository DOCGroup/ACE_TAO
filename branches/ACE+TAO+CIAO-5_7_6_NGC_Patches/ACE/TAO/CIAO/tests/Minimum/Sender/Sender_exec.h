// $Id$

#ifndef CIAO_SENDER_EXEC_H
#define CIAO_SENDER_EXEC_H

#include /**/ "ace/pre.h"

#include "SenderEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Sender_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Minimum_Sender_Impl
{
  class SENDER_EXEC_Export Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    // Supported or inherited operations.

    virtual void
    bar ();

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
    ::Minimum::CCM_Sender_Context_var context_;
  };

  class SENDER_EXEC_Export SenderHome_exec_i
    : public virtual SenderHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SenderHome_exec_i (void);
    virtual ~SenderHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Minimum_SenderHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_SENDER_EXEC_H */

