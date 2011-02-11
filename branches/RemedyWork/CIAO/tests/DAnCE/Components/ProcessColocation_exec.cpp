// -*- C++ -*-
// $Id$

#include "ProcessColocation_exec.h"
#include "ace/Log_Msg.h"
#include "ace/Singleton.h"

namespace
{
  class Colocation_Tester
  {
  public:
    Colocation_Tester (void)
      : value_ ("")
    {
    }

    ACE_CString value (void)
    {
      return value_;
    }

    void value (const ACE_CString &s)
    {
      value_ = s;
    }

  private:
    ACE_CString value_;
  };

  typedef ACE_Singleton <Colocation_Tester,
                         ACE_SYNCH_RECURSIVE_MUTEX> Colocation_Tester_Singleton;

#define TESTER Colocation_Tester_Singleton::instance ()
}

namespace CIAO_Simple_ProcessColocation_Impl
{
  //============================================================
  // Component Executor Implementation Class: ProcessColocation_exec_i
  //============================================================

  ProcessColocation_exec_i::ProcessColocation_exec_i (void)
  {
  }

  ProcessColocation_exec_i::~ProcessColocation_exec_i (void)
  {
  }

  // Supported operations and attributes.

  // Component attributes.

  char *
  ProcessColocation_exec_i::process_name (void)
  {
    return CORBA::string_dup (this->process_name_.c_str ());
  }

  void
  ProcessColocation_exec_i::process_name (
    const char * process_name )
  {
    ACE_DEBUG ((LM_EMERGENCY, "Setting attr process name:%C\n",
                 process_name));

    this->process_name_ = process_name;
    TESTER->value (process_name);
  }

  // Port operations.

  // Operations from Components::SessionComponent.

  void
  ProcessColocation_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Simple::CCM_ProcessColocation_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  ProcessColocation_exec_i::configuration_complete (void)
  {
    /* Your code here. */
  }

  void
  ProcessColocation_exec_i::ccm_activate (void)
  {
    ACE_CString value = TESTER->value ();

    if (this->process_name_ != value)
      {
        ACE_DEBUG ((LM_EMERGENCY, "ProcessColocation_exec_i::ccm_activate - "
                     "Error: I am colocated with someone I shouldn't be.  "
                     "My process value is %C, expected %C\n",
                     value.c_str (),
                     this->process_name_.c_str ()));
        //        throw CORBA::BAD_PARAM ();
      }
  }

  void
  ProcessColocation_exec_i::ccm_passivate (void)
  {
    /* Your code here. */
  }

  void
  ProcessColocation_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" PROCESSCOLOCATION_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Simple_ProcessColocation_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_RETURN (
      retval,
      ProcessColocation_exec_i,
      ::Components::EnterpriseComponent::_nil ());

    return retval;
  }
}

namespace CIAO_Simple_ProcessColocation_Impl
{
  //============================================================
  // Home Executor Implementation Class: ProcessColocationHome_exec_i
  //============================================================

  ProcessColocationHome_exec_i::ProcessColocationHome_exec_i (void)
  {
  }

  ProcessColocationHome_exec_i::~ProcessColocationHome_exec_i (void)
  {
  }

  // All operations and attributes.

  // Factory operations.

  // Finder operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  ProcessColocationHome_exec_i::create (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      ProcessColocation_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" PROCESSCOLOCATION_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_ProcessColocationHome_Impl (void)
  {
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      ProcessColocationHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

