// $Id$

#include "ciao/Logger/Log_Macros.h"
#include "SimpleNilFacetUser_exec.h"

namespace CIAO_Simple_SimpleNilFacetUser_Impl
{
  //==================================================================
  // Component Executor Implementation Class:   SimpleNilFacetUser_exec_i
  //==================================================================

  SimpleNilFacetUser_exec_i::SimpleNilFacetUser_exec_i (void)
  {
    CIAO_TRACE ("SimpleNilFacetUser_exec_i::SimpleNilFacetUser_exec_i (void)");
  }

  SimpleNilFacetUser_exec_i::~SimpleNilFacetUser_exec_i (void)
  {
    CIAO_TRACE ("SimpleNilFacetUser_exec_i::~SimpleNilFacetUser_exec_i (void)");
  }

  // Supported or inherited operations.

  // Attribute operations.

  // Port operations.

  // Operations from Components::SessionComponent

  void
  SimpleNilFacetUser_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    CIAO_TRACE ("SimpleNilFacetUser_exec_i::set_session_context ()");
    this->context_ =
      ::Simple::CCM_SimpleNilFacetUser_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  SimpleNilFacetUser_exec_i::configuration_complete ()
  {
    CIAO_TRACE ("SimpleNilFacetUser_exec_i::configuration_complete ()");
    // Your code here.
  }

  void
  SimpleNilFacetUser_exec_i::ccm_activate ()
  {
    CIAO_TRACE ("SimpleNilFacetUser_exec_i::ccm_activate ()");
    bool caught_exception = false;
    ::Simple::Trigger_var trig (this->context_->get_connection_trig ());

    if (CORBA::is_nil (trig.in ()))
      {
        ACE_ERROR ((LM_ERROR, "SimpleNilFacetUser_exec_i::ccm_activate () - "
                     "Error: Reference nil for port trig\n"));
        return;
      }

    try
      {
        trig->hello ("Test successful; hello from SimpleNilFacetUser.");
      }
    catch (const ::CORBA::INV_OBJREF &)
      {
        ACE_DEBUG ((LM_DEBUG, "SimpleNilFacetUser_exec_i::ccm_activate () - "
                     "Caught correct CORBA exception\n"));
        caught_exception = true;
      }
    catch (const CORBA::Exception &ex)
      {
        ACE_ERROR ((LM_ERROR, "SimpleNilFacetUser_exec_i::ccm_activate () - "
                     "Caught CORBA exception, details follow:\n"));
        ex._tao_print_exception ("SimpleNilFacetUser_exec_i::ccm_activate () - ");
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "SimpleNilFacetUser_exec_i::ccm_activate () - "
                     "Error: Caught unknown exception whilst invoking reference for port trig.\n"));
      }
    if (!caught_exception)
      {
        ACE_ERROR ((LM_ERROR, "SimpleNilFacetUser_exec_i::ccm_activate () - "
                     "Error: Didn't catch correct exception\n"));
      }
  }

  void
  SimpleNilFacetUser_exec_i::ccm_passivate ()
  {
    CIAO_TRACE ("SimpleNilFacetUser_exec_i::ccm_passivate ()");
  }

  void
  SimpleNilFacetUser_exec_i::ccm_remove ()
  {
    CIAO_TRACE ("SimpleNilFacetUser_exec_i::ccm_remove ()");
  }

  //==================================================================
  // Home Executor Implementation Class:   SimpleNilFacetUserHome_exec_i
  //==================================================================

  SimpleNilFacetUserHome_exec_i::SimpleNilFacetUserHome_exec_i (void)
  {
    CIAO_TRACE ("SimpleNilFacetUserHome_exec_i::SimpleNilFacetUserHome_exec_i (void)");
  }

  SimpleNilFacetUserHome_exec_i::~SimpleNilFacetUserHome_exec_i (void)
  {
    CIAO_TRACE ("SimpleNilFacetUserHome_exec_i::~SimpleNilFacetUserHome_exec_i (void)");
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  SimpleNilFacetUserHome_exec_i::create ()
  {
    CIAO_TRACE ("SimpleNilFacetUserHome_exec_i::create ()");
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      SimpleNilFacetUser_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SIMPLENILFACETUSER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleNilFacetUserHome_Impl (void)
  {
    CIAO_TRACE ("create_Simple_SimpleNilFacetUserHome_Impl (void)");
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      SimpleNilFacetUserHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

