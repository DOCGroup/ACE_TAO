// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "Getter_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_Getter_Test_Receiver_Impl
{
  class Receiver_exec_i;

  //============================================================
  // GetOneHandler
  //============================================================
  class GetOneHandler
    : public ACE_Event_Handler
  {
  public:
    GetOneHandler (Receiver_exec_i &,
                   const char * key,
                   CORBA::Long fixed_key,
                   CORBA::Long iteration);
    virtual ~GetOneHandler ();

    virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);

  private:
    Receiver_exec_i &callback_;
    const char * key_;
    CORBA::Long fixed_key_;
    CORBA::Long iteration_;
  };

  //============================================================
  // GetManyHandler
  //============================================================
  class GetManyHandler
    : public ACE_Event_Handler
  {
  public:
    GetManyHandler (Receiver_exec_i &,
                   CORBA::UShort keys,
                   CORBA::Long iterations);
    virtual ~GetManyHandler ();

    virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);

  private:
    Receiver_exec_i &callback_;
    CORBA::UShort keys_;
    CORBA::Long   iterations_;
  };

  //============================================================
  // Invoker_exec_i
  //============================================================
  class Invoker_exec_i
    : public virtual ::CCM_GetInvoker,
      public virtual ::CORBA::LocalObject
  {
  public:
    Invoker_exec_i (Receiver_exec_i & callback);
    virtual ~Invoker_exec_i (void);

    virtual void start_timeout_get_one ();
    virtual void start_timeout_get_many ();

    virtual void start_get_one (const char * key,
                                ::CORBA::Long fixed_key,
                                ::CORBA::Long iteration);
    virtual void start_get_many (::CORBA::Short keys , ::CORBA::Long iterations);

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

    virtual ::Getter_Test::GetterTestConnector::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_get_status (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_fixed_status (void);

    virtual ::CCM_GetInvoker_ptr
    get_getter_invoke ();


    void start_get_one (const char * key,
                        CORBA::Long fixed_key,
                        CORBA::Long iteration);
    void timeout_get_one ();
    void timeout_get_many ();
    void get_one (const char * key,
                  CORBA::Long fixed_key,
                  CORBA::Long iteration);

    void start_get_many (CORBA::Short keys,
                         CORBA::Long iterations);

    void get_many (CORBA::Short keys,
                         CORBA::Long iterations);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Getter_Test::CCM_Receiver_Context_var context_;
    ::Getter_Test::GetterTestConnector::Getter_var getter_;
    ::Getter_Test::GetterFixedConnector::Getter_var fixed_;

    void timeout_get_one_fixed ();
    void timeout_get_one_variable ();

    void get_one_fixed (CORBA::Long fixed_key,
                        CORBA::Long iteration);
    void get_one_variable (const char * key,
                           CORBA::Long iteration);
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Getter_Test_Receiver_Impl (void);
}

#endif /* ifndef */
