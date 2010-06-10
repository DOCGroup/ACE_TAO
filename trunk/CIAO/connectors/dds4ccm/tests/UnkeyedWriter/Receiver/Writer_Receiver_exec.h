// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "Writer_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include "ace/Reactor.h"

namespace CIAO_Writer_Receiver_Impl
{
  class RECEIVER_EXEC_Export WriterTest_Listener_exec_i
    : public virtual ::Writer::WriterTestConnector::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    WriterTest_Listener_exec_i (CORBA::UShort iterations);
    virtual ~WriterTest_Listener_exec_i (void);

    virtual void
    on_one_data (
      const WriterTest & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const WriterTestSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    CORBA::UShort iterations_;
  };

  class RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    // Component attributes.
    virtual ::CORBA::UShort iterations (void);

    virtual void iterations (::CORBA::UShort iterations);

    // Port operations.
    virtual ::Writer::WriterTestConnector::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Writer::CCM_Receiver_Context_var context_;
    CORBA::UShort iterations_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Writer_Receiver_Impl (void);
}

#endif /* ifndef */

