// $Id$

#ifndef CIAO_SIMPLEPUBLISHER_EXEC_H
#define CIAO_SIMPLEPUBLISHER_EXEC_H

#include /**/ "ace/pre.h"

#include "SimplePublisherEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SimplePublisher_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Simple_SimplePublisher_Impl
{
  class SIMPLEPUBLISHER_EXEC_Export SimplePublisher_exec_i
    : public virtual SimplePublisher_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimplePublisher_exec_i (void);
    virtual ~SimplePublisher_exec_i (void);

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
    ::Simple::CCM_SimplePublisher_Context_var context_;
  };

  class SIMPLEPUBLISHER_EXEC_Export SimplePublisherHome_exec_i
    : public virtual SimplePublisherHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimplePublisherHome_exec_i (void);
    virtual ~SimplePublisherHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" SIMPLEPUBLISHER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimplePublisherHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_SIMPLEPUBLISHER_EXEC_H */

