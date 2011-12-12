// -*- C++ -*-
// $Id$

#ifndef CIAO_RG_RESETTOPIC_RECEIVER_EXEC_MMKH3G_H_
#define CIAO_RG_RESETTOPIC_RECEIVER_EXEC_MMKH3G_H_

#include /**/ "ace/pre.h"

#include "RG_ResetTopic_ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Receiver_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_RG_ResetTopic_Receiver_Impl
{
  class Receiver_exec_i;
  class RG_ResetTopic_Receiver_impl;
  /**
   * Provider Executor Implementation Class: info_get1_status_exec_i
   */

  class info_get_status_exec_i
    : public virtual ::CCM_DDS::CCM_PortStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    info_get_status_exec_i (
      ::RG_ResetTopic::CCM_Receiver_Context_ptr ctx);
    virtual ~info_get_status_exec_i (void);

    /** @name Operations and attributes from CCM_DDS::PortStatusListener */
    //@{

    virtual
    void on_requested_deadline_missed (::DDS::DataReader_ptr the_reader,
    const ::DDS::RequestedDeadlineMissedStatus & status);

    virtual
    void on_sample_lost (::DDS::DataReader_ptr the_reader,
    const ::DDS::SampleLostStatus & status);
    //@}

  private:
    ::RG_ResetTopic::CCM_Receiver_Context_var ciao_context_;
  };

  /**
   * Provider Executor Implementation Class: info_read1_status_exec_i
   */

  class info_read_status_exec_i
    : public virtual ::CCM_DDS::CCM_PortStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    info_read_status_exec_i (
      ::RG_ResetTopic::CCM_Receiver_Context_ptr ctx);
    virtual ~info_read_status_exec_i (void);

    /** @name Operations and attributes from CCM_DDS::PortStatusListener */
    //@{

    virtual
    void on_requested_deadline_missed (::DDS::DataReader_ptr the_reader,
    const ::DDS::RequestedDeadlineMissedStatus & status);

    virtual
    void on_sample_lost (::DDS::DataReader_ptr the_reader,
    const ::DDS::SampleLostStatus & status);
    //@}

  private:
    ::RG_ResetTopic::CCM_Receiver_Context_var ciao_context_;
  };

  /**
   * Provider Executor Implementation Class: start_reading_exec_i
   */

  class start_reading_exec_i
    : public virtual ::CCM_ReaderStarter,
      public virtual ::CORBA::LocalObject
  {
  public:
    start_reading_exec_i (
      ::RG_ResetTopic::CCM_Receiver_Context_ptr ctx,
      Receiver_exec_i &callback_);
    virtual ~start_reading_exec_i (void);

    /** @name Operations and attributes from ReaderStarter */
    //@{

    virtual
    void start_read (const char * topic_name);

    virtual
    void set_reader_properties (::CORBA::UShort nr_keys,
    ::CORBA::UShort nr_iterations);
    //@}

  private:
    ::RG_ResetTopic::CCM_Receiver_Context_var ciao_context_;
    Receiver_exec_i &callback_;
  };

  /**
   * Component Executor Implementation Class: Receiver_exec_i
   */

  class Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    /** @name Supported operations and attributes. */
    //@{

    //@}

    /** @name Component attributes and port operations. */
    //@{

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_get_status (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_read_status (void);

    virtual ::CCM_ReaderStarter_ptr
    get_start_reading (void);

    virtual ::CORBA::UShort iterations (void);

    virtual void iterations (::CORBA::UShort iterations);

    virtual ::CORBA::UShort keys (void);

    virtual void keys (::CORBA::UShort keys);
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
    void
    start_read (const char * topic_name);
    //@}

  private:
    ::RG_ResetTopic::CCM_Receiver_Context_var ciao_context_;

    /** @name Component attributes. */
    //@{
    ::CCM_DDS::CCM_PortStatusListener_var ciao_info_get_status_;
    ::CCM_DDS::CCM_PortStatusListener_var ciao_info_read_status_;
    ::CCM_ReaderStarter_var ciao_start_reading_;

    ::CORBA::UShort iterations_;

    ::CORBA::UShort keys_;
    //@}

    /** @name User defined members. */
    //@{
    RG_ResetTopic_Receiver_impl *impl_;
    //@}

    /** @name User defined private operations. */
    //@{

    //@}
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_RG_ResetTopic_Receiver_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
