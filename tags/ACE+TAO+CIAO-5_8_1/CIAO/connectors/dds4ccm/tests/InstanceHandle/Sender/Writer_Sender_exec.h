// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_


#include "Writer_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

class WriterTestDataWriter;

namespace CIAO_Writer_Sender_Impl
{
  class Sender_exec_i;

  //============================================================
  // StartHandler
  //============================================================
  class StartHandler :
    public ACE_Event_Handler
  {
  public:
    StartHandler (Sender_exec_i &callback);
    virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
  private:
    Sender_exec_i &callback_;
  };

  //============================================================
  // Sender_exec_i
  //============================================================
  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    virtual ::CORBA::UShort keys (void);

    virtual void keys (::CORBA::UShort keys);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    void run ();

  private:
    void start (void);

    WriterTestDataWriter *dds_writer_;

    ::Writer::CCM_Sender_Context_var context_;
    CORBA::UShort keys_;

    void register_handles ();
    void unregister_handles ();
    void test_equality ();
    void test_non_equality ();

    TAO_SYNCH_MUTEX mutex_;
    typedef std::map<ACE_CString, WriterTest_var> Writer_Table;
    Writer_Table ktests_;

    typedef std::map<ACE_CString, ::DDS::InstanceHandle_t> CCM_Handles;
    CCM_Handles handles_;
  };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Writer_Sender_Impl (void);
}

#endif /* ifndef */

