// -*- C++ -*-
//
// $Id$

#ifndef CIAO_DISTRIBUTOR_EXEC_H_
#define CIAO_DISTRIBUTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "DistributorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Distributor_exec_export.h"
#include "tao/LocalObject.h"
#include "ace/Thread_Mutex.h"
#include "ace/Task.h"
#include "ace/Reactor.h"

#include <map>

namespace CIAO_Quoter_Distributor_Impl
{
  class Distributor_exec_i;

  /**
  * @class pulse generator
  *
  * @brief an active object used by StockDistributor to perform periodical pulse generation
  *
  */
  class pulse_Generator : public ACE_Task_Base
  {
  public:
    pulse_Generator (Distributor_exec_i &callback);

    ~pulse_Generator ();

    /// Hook method that performs application-defined initialization activities
    int open_h (void);

    /// Hook method that performs application-defined destruction activites
    int close_h (void);

    /// appliation-defined method for starting the pulse-generation service
    int start (CORBA::ULong hertz);

    /// application-defined method for stopping the pulse-generation service
    int stop (void);

    /// Indicate whether the current object is active
    int active (void);

    /// Handle the timeout.
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);

    /// Called when timer handler is removed.
    virtual int handle_close (ACE_HANDLE handle,
                              ACE_Reactor_Mask close_mask);

    /// Hook methods implemnting the task's service processing,
    /// invoked by all threads activated by activate () method
    virtual int svc (void);

  private:
    /// Tracking whether we are actively generating pulse or not.
    int active_;

    /// Maintains a handle that actually process the event
    Distributor_exec_i &pulse_callback_;

  };

class DISTRIBUTOR_EXEC_Export ConnectorStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_exec_i (void);
    virtual ~ConnectorStatusListener_exec_i (void);

    virtual
      void on_inconsistent_topic( ::DDS::Topic_ptr the_topic,
                                      const DDS::InconsistentTopicStatus & status);
    virtual
    void on_requested_incompatible_qos( ::DDS::DataReader_ptr the_reader,
                                      const DDS::RequestedIncompatibleQosStatus & status);
    virtual
    void on_sample_rejected( ::DDS::DataReader_ptr the_reader,
                                      const DDS::SampleRejectedStatus & status);
    virtual
      void on_offered_deadline_missed( ::DDS::DataWriter_ptr the_writer,
                                      const DDS::OfferedDeadlineMissedStatus & status);
    virtual
    void on_offered_incompatible_qos( ::DDS::DataWriter_ptr the_writer,
                                      const DDS::OfferedIncompatibleQosStatus & status);
    virtual
    void on_unexpected_status( ::DDS::Entity_ptr the_entity,
                                      ::DDS::StatusKind  status_kind);
    virtual
    void on_publication_matched ( ::DDS::DataWriter_ptr the_writer,
                                      const DDS::PublicationMatchedStatus & status);

  };
  class DISTRIBUTOR_EXEC_Export Distributor_exec_i
    : public virtual Distributor_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Distributor_exec_i (void);
    virtual ~Distributor_exec_i (void);

    void tick (void);

    // Supported operations and attributes.

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46

    virtual void
    add_stock (
      const char * stock);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46

    virtual void
    del_stock (
      const char * stock);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46

    virtual void
    start (void);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46

    virtual void
    stop (void);

    // Component attributes.

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46

    virtual ::CORBA::ULong
    rate (void);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46

    virtual void
    rate (
      ::CORBA::ULong rate);

    // Port operations.
    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_info_out_connector_status (void);

    // Operations from Components::SessionComponent.

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
  
  private:
    pulse_Generator * ticker_;
    ::Quoter::CCM_Distributor_Context_var context_;
    TAO_SYNCH_MUTEX mutex_;
    typedef std::map<ACE_CString, Quoter::Stock_Info_var> Stock_Table;
    Stock_Table stocks_;
    CORBA::ULong rate_;
    CCM_DDS::Quoter::Writer_var writer_;
    CCM_DDS::Quoter::Updater_var updater_;
  };
  
  extern "C" DISTRIBUTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Quoter_Distributor_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

