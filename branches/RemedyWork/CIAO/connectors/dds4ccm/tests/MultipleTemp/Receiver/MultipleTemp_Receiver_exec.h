// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "MultipleTemp_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"


namespace CIAO_MultipleTemp_Receiver_Impl
{
  class Receiver_exec_i;

  //============================================================
  // ListenOneByOneTest_Listener_exec_i one
  //============================================================
  class RECEIVER_EXEC_Export ListenOne_Listener_exec_i
  : public virtual ::MultipleTemp::MultipleTempConnector::T_Typed::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ListenOne_Listener_exec_i (
        ::MultipleTemp::CCM_Receiver_Context_ptr ctx);

    virtual ~ListenOne_Listener_exec_i (void);

    virtual void
    on_one_data (
      const TestTopicOne& an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const TestTopicOneSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    ::MultipleTemp::CCM_Receiver_Context_var ciao_context_;
  };
  //============================================================
  // ListenOneByOneTest_Listener_exec_i two
  //============================================================
  class RECEIVER_EXEC_Export ListenTwo_Listener_exec_i
  : public virtual ::MultipleTemp::MultipleTempConnector::Y_Typed::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ListenTwo_Listener_exec_i (
        ::MultipleTemp::CCM_Receiver_Context_ptr ctx);

    virtual ~ListenTwo_Listener_exec_i (void);

    virtual void
    on_one_data (
//      const TestTopicTwo& an_instance,
      const TestTopicOne& an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
      on_many_data (
 //       const TestTopicTwoSeq & an_instance,
        const TestTopicOneSeq & an_instance,
        const ::CCM_DDS::ReadInfoSeq & info);
  private:
    ::MultipleTemp::CCM_Receiver_Context_var ciao_context_;
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

    // Port operations.
    virtual ::MultipleTemp::MultipleTempConnector::T_Typed::CCM_Listener_ptr
    get_info_one_out_data_listener (void);
    virtual ::MultipleTemp::MultipleTempConnector::Y_Typed::CCM_Listener_ptr
    get_info_two_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_one_out_status (void);
    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_two_out_status (void);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::MultipleTemp::CCM_Receiver_Context_var ciao_context_;
    ::MultipleTemp::MultipleTempConnector::T_Typed::CCM_Listener_var ciao_info_out_one_data_listener_;
    ::MultipleTemp::MultipleTempConnector::Y_Typed::CCM_Listener_var ciao_info_out_two_data_listener_;
   };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_MultipleTemp_Receiver_Impl (void);
}

#endif /* ifndef */
