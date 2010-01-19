// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "QueryFilter_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

namespace CIAO_QueryFilter_Test_Receiver_Impl
{
  class Receiver_exec_i;

  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export ConnectorStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_exec_i (Receiver_exec_i &);
    virtual ~ConnectorStatusListener_exec_i (void);

    virtual
    void on_inconsistent_topic( ::DDS::Topic_ptr ,
                              const DDS::InconsistentTopicStatus & );
    virtual
    void on_requested_incompatible_qos( ::DDS::DataReader_ptr ,
                              const DDS::RequestedIncompatibleQosStatus & );
    virtual
    void on_sample_rejected( ::DDS::DataReader_ptr ,
                              const DDS::SampleRejectedStatus & );
    virtual
    void on_offered_deadline_missed( ::DDS::DataWriter_ptr ,
                              const DDS::OfferedDeadlineMissedStatus & );
    virtual
    void on_offered_incompatible_qos( ::DDS::DataWriter_ptr ,
                              const DDS::OfferedIncompatibleQosStatus & );
    virtual
    void on_unexpected_status( ::DDS::Entity_ptr ,
                              ::DDS::StatusKind );
  private:
    /// Maintains a handle that actually process the event
    Receiver_exec_i &callback_;
    bool has_run_;
  };

  //============================================================
  // Starter_exec_i
  //============================================================
  class Starter_exec_i
    : public virtual ::CCM_QueryFilterStarter,
      public virtual ::CORBA::LocalObject
  {
  public:
    Starter_exec_i (Receiver_exec_i & callback);
    virtual ~Starter_exec_i (void);

    virtual void set_reader_properties (CORBA::UShort nr_keys,
                  CORBA::UShort nr_iterations);

  private:
    Receiver_exec_i &callback_;
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

    // Supported operations and attributes.
    // Component attributes.

    // Port operations.
    virtual ::CCM_DDS::QueryFilterTest::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    virtual ::CCM_QueryFilterStarter_ptr
    get_reader_start ();

    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_info_out_connector_status (void);

    bool check_last ();
    void run (void);

    ::CORBA::UShort iterations (void);

    void iterations (::CORBA::UShort iterations);

    ::CORBA::UShort keys (void);

    void keys (::CORBA::UShort keys);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::QueryFilter_Test::CCM_Receiver_Context_var context_;
    ::CCM_DDS::QueryFilterTest::Reader_var       reader_;
    QueryFilterRestarter_var restarter_;
    CORBA::UShort   iterations_;
    CORBA::UShort   keys_;
    bool            has_run_;

    int current_max_iteration_;

    void read_all ();
    void check_filter ();
    void test_exception ();
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_QueryFilter_Test_Receiver_Impl (void);
}

#endif /* ifndef */

