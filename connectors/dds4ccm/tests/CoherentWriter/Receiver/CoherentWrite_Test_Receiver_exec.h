// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "CoherentWrite_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"



namespace CIAO_CoherentWrite_Test_Receiver_Impl
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
    : public virtual ::CCM_CoherentWriteStarter,
      public virtual ::CORBA::LocalObject
  {
  public:
    Starter_exec_i (Receiver_exec_i & callback);
    virtual ~Starter_exec_i (void);

    virtual void set_reader_properties (CORBA::UShort nr_iterations);
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
    // Component attributes.

    // Port operations.
    virtual ::CoherentWriteTestConnector::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    virtual ::CCM_CoherentWriteStarter_ptr
    get_reader_start ();

    bool check_last ();
    void start_read (CORBA::UShort run);
    void run (CORBA::UShort run);

    ::CORBA::UShort iterations (void);
    void iterations (::CORBA::UShort iterations);

    ::CORBA::UShort nr_runs (void);
    void nr_runs (::CORBA::UShort nr_runs);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::CoherentWrite_Test::CCM_Receiver_Context_var context_;

    CORBA::UShort   iterations_;
    CORBA::UShort   run_;
    CORBA::UShort   nr_runs_;
    CORBA::Long     last_iter_;

    read_action_Generator *ticker_;

    void read_all (void);

  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CoherentWrite_Test_Receiver_Impl (void);
}

#endif /* ifndef */

