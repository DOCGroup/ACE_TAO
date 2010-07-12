// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "QCTQ_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_QCTQ_Test_Receiver_Impl
{
  class Receiver_exec_i;

  //============================================================
  // read_action_Generator
  //============================================================
  class read_action_Generator
    : public ACE_Event_Handler
  {
  public:
    read_action_Generator (Receiver_exec_i &callback,
                           int run);

    ~read_action_Generator ();

    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
  private:
    Receiver_exec_i &callback_;
    int run_;

  };

  //============================================================
  // Starter_exec_i
  //============================================================
  class Starter_exec_i
    : public virtual ::CCM_TwoQueriesStarter,
      public virtual ::CORBA::LocalObject
  {
  public:
    Starter_exec_i (Receiver_exec_i & callback);
    virtual ~Starter_exec_i (void);

    virtual void set_reader_properties (CORBA::UShort nr_keys,
                  CORBA::UShort nr_iterations);
    virtual void start_read (CORBA::UShort run);

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
    void iterations (CORBA::UShort iterations);
    void keys (::CORBA::UShort keys);

    // Component attributes.

    // Port operations.
    virtual ::CCM_TwoQueriesStarter_ptr
    get_reader_start ();

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_get_port_status (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_check_port_status (void);

    bool check_last ();
    void start_read (CORBA::UShort run);
    void run (CORBA::UShort run);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::QCTQ_Test::CCM_Receiver_Context_var context_;

    TwoQueriesRestarter_var restarter_;
    CORBA::UShort   iterations_;
    CORBA::UShort   keys_;

    int current_min_iteration_;
    int current_max_iteration_;

    read_action_Generator *ticker_;

    CORBA::ULong samples_expected_;
    CORBA::ULong samples_received_;

    CORBA::ULong test_all (void);
    CORBA::ULong read_all (void);
    void check_iter (const QueryConditionTest & sample,
                     const char * test);
    void check_filter (CORBA::UShort run);
    void set_filter (CORBA::UShort run);
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_QCTQ_Test_Receiver_Impl (void);
}

#endif /* ifndef */

