// $Id$

#ifndef CIAO_RECEIVER_EXEC_H
#define CIAO_RECEIVER_EXEC_H

#include /**/ "ace/pre.h"

#include "ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Receiver_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Minimum_Receiver_Impl
{
  class RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

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
    ::Minimum::CCM_Receiver_Context_var context_;
  };

  class RECEIVER_EXEC_Export ReceiverHome_exec_i
    : public virtual ReceiverHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    ReceiverHome_exec_i (void);
    virtual ~ReceiverHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" RECEIVER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Minimum_ReceiverHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_RECEIVER_EXEC_H */

