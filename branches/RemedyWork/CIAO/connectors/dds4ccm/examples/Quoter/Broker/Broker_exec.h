// -*- C++ -*-
// $Id$

#ifndef CIAO_BROKER_EXEC_H_
#define CIAO_BROKER_EXEC_H_

#include /**/ "ace/pre.h"

#include "BrokerEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Broker_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Quoter_Broker_Impl
{

  class Broker_exec_i;

  class BROKER_EXEC_Export Stock_Info_Listener_exec_i
    : public virtual ::Quoter::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    Stock_Info_Listener_exec_i (void);
    virtual ~Stock_Info_Listener_exec_i (void);

    virtual void
    on_one_data (
      const ::Quoter::Stock_Info & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const ::Quoter::Stock_InfoSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  };

  class BROKER_EXEC_Export PortStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_PortStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    PortStatusListener_exec_i (void);
    virtual ~PortStatusListener_exec_i (void);

    virtual void
    on_requested_deadline_missed (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::RequestedDeadlineMissedStatus & status);

    virtual void
    on_sample_lost (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::SampleLostStatus & status);
  };


  class BROKER_EXEC_Export Broker_exec_i
    : public virtual Broker_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Broker_exec_i (void);
    virtual ~Broker_exec_i (void);

    // Supported operations and attributes.

    // Component attributes.

    // Port operations.

    virtual ::Quoter::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);
    void read_one (void);
    void read_one_history (void);
    void read_all(void);
    void read_all_history(void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    virtual void start (void);
    virtual void stop (void);

  private:
    ::Quoter::CCM_Broker_Context_var context_;
  };

  extern "C" BROKER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Quoter_Broker_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

