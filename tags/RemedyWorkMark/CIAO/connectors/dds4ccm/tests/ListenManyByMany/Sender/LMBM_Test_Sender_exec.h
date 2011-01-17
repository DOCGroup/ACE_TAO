// -*- C++ -*-
// $Id$

#ifndef CIAO_LMBM_TEST_SENDER_EXEC_HRZ3NN_H_
#define CIAO_LMBM_TEST_SENDER_EXEC_HRZ3NN_H_

#include /**/ "ace/pre.h"

#include "LMBM_Test_SenderEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Sender_exec_export.h"
#include "tao/LocalObject.h"

#include <map>

namespace CIAO_LMBM_Test_Sender_Impl
{
  class Sender_exec_i;

  //============================================================
  // WriteTicker
  //============================================================
  class WriteTicker :
    public ACE_Event_Handler
  {
  public:
    WriteTicker (Sender_exec_i &callback);
    int handle_timeout (const ACE_Time_Value &, const void *);
  private:
    Sender_exec_i &callback_;
  };

  //============================================================
  // WriteManyHandler
  //============================================================
  class WriteManyHandler :
    public ACE_Event_Handler
  {
  public:
    WriteManyHandler (Sender_exec_i &callback);
    virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
  private:
    Sender_exec_i &callback_;
  };

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    //@{
    /** Supported operations and attributes. */

    //@}

    //@{
    /** Component attributes and port operations. */

    virtual ::CORBA::UShort keys (void);

    virtual void keys (::CORBA::UShort keys);

    virtual ::CORBA::UShort iterations (void);

    virtual void iterations (::CORBA::UShort iterations);
    //@}

    //@{
    /** Operations from Components::SessionComponent. */
    virtual void set_session_context (::Components::SessionContext_ptr ctx);
    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}

    //@{
    /** User defined public operations. */
    void start (void);
    void write_one (void);
    //@}

  private:
    ::LMBM_Test::CCM_Sender_Context_var ciao_context_;

    //@{
    /** Component attributes and port operations. */

    ::CORBA::UShort keys_;

    ::CORBA::UShort iterations_;
    //@}

    //@{
    /** User defined members. */
    WriteTicker *ticker_;
    TAO_SYNCH_MUTEX mutex_;
    typedef std::map<ACE_CString, ListenManyByManyTest_var> Writer_Table;
    Writer_Table samples_;
    Writer_Table::iterator last_key_;
    //@}

    //@{
    /** User defined private operations. */

    //@}

    /// Get the ACE_Reactor
    ACE_Reactor* reactor (void);
  };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LMBM_Test_Sender_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
