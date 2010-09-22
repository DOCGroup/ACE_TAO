// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "AmiDds_ReceiverEC.h"

#include /**/ "AmiDds_Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_AmiDds_Receiver_Impl
{
  class MyFoo_exec_i
      : public virtual ::AmiDds::CCM_MyFoo,
        public virtual ::CORBA::LocalObject
  {
    public:
      MyFoo_exec_i (void);
      virtual ~MyFoo_exec_i (void);

      virtual void hello (::CORBA::Long_out answer);
  };

  //============================================================
  // ListenOneByOneTest_Listener_exec_i
  //============================================================ 
  class AMIDDS_RECEIVER_EXEC_Export ListenOneByOneTest_Listener_exec_i
    : public virtual ::AmiDds::AmiDdsConnector::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ListenOneByOneTest_Listener_exec_i (void);

    virtual ~ListenOneByOneTest_Listener_exec_i (void);

    virtual void
    on_one_data (
      const TestTopic& an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const ::TestTopicSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  };

  //============================================================
  // Receiver_exec_i
  //============================================================
  class Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    // Port operations.
    virtual AmiDds::AmiDdsConnector::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    virtual ::AmiDds::CCM_MyFoo_ptr
    get_do_my_foo (void);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::AmiDds::CCM_Receiver_Context_var context_;
};

  extern "C" AMIDDS_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_AmiDds_AMI_Receiver_Impl (void);
}

#endif /* ifndef */
