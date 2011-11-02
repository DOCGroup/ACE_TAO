// -*- C++ -*-
// $Id$

#ifndef CIAO_SUB_COMPONENT_EXEC_SXAYYT_H_
#define CIAO_SUB_COMPONENT_EXEC_SXAYYT_H_

#include /**/ "ace/pre.h"

#include "Sub_ComponentEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Sub_Component_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_CoPubSub_Sub_comp_Impl
{

  /**
   * Provider Executor Implementation Class: TestSub_data_listener_exec_i
   */

  class TestSub_data_listener_exec_i
    : public virtual ::CoPubSub::Test_conn::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    TestSub_data_listener_exec_i (
      ::CoPubSub::CCM_Sub_comp_Context_ptr ctx);
    virtual ~TestSub_data_listener_exec_i (void);

    /** @name Operations and attributes from CoPubSub::Test_conn::Listener */
    //@{

    virtual
    void on_one_data (
      const ::CoPubSub::Test_msg & datum,
      const ::CCM_DDS::ReadInfo & info);

    virtual
    void on_many_data (
      const ::CoPubSub::Test_msgSeq & data,
      const ::CCM_DDS::ReadInfoSeq & infos);
    //@}

  private:
    ::CoPubSub::CCM_Sub_comp_Context_var ciao_context_;
  };

  /**
   * Provider Executor Implementation Class: TestSub_status_exec_i
   */

  class TestSub_status_exec_i
    : public virtual ::CCM_DDS::CCM_PortStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    TestSub_status_exec_i (
      ::CoPubSub::CCM_Sub_comp_Context_ptr ctx);
    virtual ~TestSub_status_exec_i (void);

    /** @name Operations and attributes from CCM_DDS::PortStatusListener */
    //@{

    virtual
    void on_requested_deadline_missed (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::RequestedDeadlineMissedStatus & status);

    virtual
    void on_sample_lost (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::SampleLostStatus & status);
    //@}

  private:
    ::CoPubSub::CCM_Sub_comp_Context_var ciao_context_;
  };

  /**
   * Component Executor Implementation Class: Sub_comp_exec_i
   */

  class Sub_comp_exec_i
    : public virtual Sub_comp_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sub_comp_exec_i (void);
    virtual ~Sub_comp_exec_i (void);

    /** @name Supported operations and attributes. */
    //@{

    //@}

    /** @name Component attributes and port operations. */
    //@{

    virtual ::CoPubSub::Test_conn::CCM_Listener_ptr
    get_TestSub_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_TestSub_status (void);
    //@}

    /** @name Operations from Components::SessionComponent. */
    //@{
    virtual void set_session_context (::Components::SessionContext_ptr ctx);
    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}

    /** @name User defined public operations. */
    //@{

    //@}

  private:
    ::CoPubSub::CCM_Sub_comp_Context_var ciao_context_;

    /** @name Component attributes. */
    //@{
    ::CoPubSub::Test_conn::CCM_Listener_var ciao_TestSub_data_listener_;
    ::CCM_DDS::CCM_PortStatusListener_var ciao_TestSub_status_;
    //@}

    /** @name User defined members. */
    //@{

    //@}

    /** @name User defined private operations. */
    //@{

    //@}
  };

  extern "C" PUBSUB_COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CoPubSub_Sub_comp_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
