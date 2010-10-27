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

class WriterTestDataReader;

namespace CIAO_Writer_Receiver_Impl
{
  //============================================================
  // WriterTest_Listener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export WriterTest_Listener_exec_i
    : public virtual ::WriterTestConnector::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    WriterTest_Listener_exec_i ();
    virtual ~WriterTest_Listener_exec_i (void);

    virtual void
    on_one_data (
      const WriterTest & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const WriterTestSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  };

  //============================================================
  // Receiver_exec_i
  //============================================================
  class RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    virtual ::WriterTestConnector::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    WriterTestDataReader * reader_;
    ::Writer::CCM_Receiver_Context_var context_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Writer_Receiver_Impl (void);
}

#endif /* ifndef */
