// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "Different_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_Different_Test_Receiver_Impl
{
  class Receiver_exec_i;

  //============================================================
  // read_action_Generator
  //============================================================
  class read_action_Generator
    : public ACE_Event_Handler
  {
  public:
    read_action_Generator (Receiver_exec_i &callback);

    ~read_action_Generator ();

    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
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
    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_get_port_status (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_read_port_status (void);

    bool check_last ();
    void start_read ();
    void run ();

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
    ::Different_Test::CCM_Receiver_Context_var context_;

    CORBA::UShort   iterations_;
    CORBA::UShort   keys_;

    read_action_Generator *ticker_;

    void test_all (void);
    void read_all (void);
    void get_all (void);

    void check_iter_reader (const QueryConditionTest & sample);
    void check_iter_getter (const QueryConditionTest & sample);

    void check_filters (void);
    void check_filter (
      ::Different_Test::QueryConditionTestConnector::Reader_ptr reader,
      bool check_getter);

    void set_filters (void);
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Different_Test_Receiver_Impl (void);
}

#endif /* ifndef */

