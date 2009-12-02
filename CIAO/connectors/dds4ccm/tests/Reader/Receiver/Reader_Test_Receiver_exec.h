// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "Reader_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Reactor.h"

#include <map>

namespace CIAO_Reader_Test_Receiver_Impl
{
  class Receiver_exec_i;

  class Starter_exec_i
    : public virtual ::CCM_ReaderStarter,
      public virtual ::CORBA::LocalObject
  {
  public:
    Starter_exec_i (Receiver_exec_i & callback);
    virtual ~Starter_exec_i (void);

    virtual void
    start_reader (CORBA::UShort nr_keys,
                  CORBA::UShort nr_iterations);

  private:
    Receiver_exec_i &callback_;
  };

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
    virtual ::CCM_DDS::ReaderTest::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    virtual ::CCM_ReaderStarter_ptr
    get_reader_start ();

    void
    start (CORBA::UShort nr_keys, CORBA::UShort nr_iterations);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Reader_Test::CCM_Receiver_Context_var context_;
    ::CCM_DDS::ReaderTest::Reader_var       reader_;
    CORBA::UShort   iterations_;
    CORBA::UShort   keys_;
    bool            timer_started_;

    void run (void);

    void read_all ();
    void read_last ();
    void read_one_all (bool test_handles = false);
    void read_one_last (bool test_handles = false);
    void test_exception ();
    void test_exception_with_handles ();

    typedef std::map<ACE_CString, DDS::InstanceHandle_t> Handle_Table;
    Handle_Table handles_;

  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Reader_Test_Receiver_Impl (void);
}

#endif /* ifndef */

