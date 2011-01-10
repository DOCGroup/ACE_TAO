// -*- C++ -*-
// $Id$

#ifndef CIAO_COHERENTUPDATE_TEST_RECEIVER_EXEC_94WCEL_H_
#define CIAO_COHERENTUPDATE_TEST_RECEIVER_EXEC_94WCEL_H_

#include /**/ "ace/pre.h"

#include "CoherentUpdate_Test_ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Receiver_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_CoherentUpdate_Test_Receiver_Impl
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

  class RECEIVER_EXEC_Export reader_start_exec_i
    : public virtual ::CCM_CoherentUpdateStarter,
      public virtual ::CORBA::LocalObject
  {
  public:
    reader_start_exec_i (
      ::CoherentUpdate_Test::CCM_Receiver_Context_ptr ctx,
      Receiver_exec_i & callback);
    virtual ~reader_start_exec_i (void);

    // Operations and attributes from CoherentUpdateStarter

    virtual
    void set_reader_properties (
      ::CORBA::UShort nr_iterations);

    virtual
    void start_read (
      ::CORBA::UShort run);

  private:
    ::CoherentUpdate_Test::CCM_Receiver_Context_var ciao_context_;
    Receiver_exec_i &callback_;
  };


  class RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    //@{
    /** Supported operations and attributes. */

    //@}

    //@{
    /** Component attributes and port operations. */

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    virtual ::CCM_CoherentUpdateStarter_ptr
    get_reader_start (void);

    virtual ::CORBA::UShort
    nr_runs (void);

    virtual void
    nr_runs (
      ::CORBA::UShort nr_runs);

    ::CORBA::UShort iterations (void);
    void iterations (::CORBA::UShort iterations);
    //@}

    //@{
    /** Operations from Components::SessionComponent. */



    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}

    bool check_last ();

    void start_read (CORBA::UShort run);
    void run (CORBA::UShort run);

  private:
    ::CoherentUpdate_Test::CCM_Receiver_Context_var ciao_context_;
    ::CCM_CoherentUpdateStarter_var ciao_reader_start_;

    ::CORBA::UShort nr_runs_;

    ::CORBA::UShort iterations_;
    ::CORBA::UShort run_;
    ::CORBA::Long last_iter_;

    read_action_Generator *ticker_;

    void read_all (
      ::CoherentUpdate_Test::Reader_ptr reader);

    ACE_Reactor* reactor (void);
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CoherentUpdate_Test_Receiver_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
