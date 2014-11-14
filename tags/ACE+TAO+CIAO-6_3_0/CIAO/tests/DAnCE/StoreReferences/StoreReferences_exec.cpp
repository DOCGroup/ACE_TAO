// $Id$

#include "StoreReferences_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"

namespace CIAO_store_references_test_testComp_Impl
{
  Component_exec_i::Component_exec_i  (void)
  {
  }

  Component_exec_i::~Component_exec_i  (void)
  {
  }

  // Operations from Components::SessionComponent.
  void
  Component_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::store_references_test::CCM_testComp_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Component_exec_i::configuration_complete (void)
  {
  }

  void
  Component_exec_i::ccm_activate (void)
  {
  }

  void
  Component_exec_i::ccm_passivate (void)
  {
  }

  void
  Component_exec_i::ccm_remove (void)
  {
  }

  extern "C" STOREREFERENCES_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Component_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Component_exec_i );

    return retval;
  }

  Component_home_exec_i::Component_home_exec_i (void)
  {
  }

  Component_home_exec_i::~Component_home_exec_i (void)
  {
  }

  ::Components::EnterpriseComponent_ptr Component_home_exec_i::create (void)
  {
      ::Components::EnterpriseComponent_ptr retval =
          ::Components::EnterpriseComponent::_nil ();

      ACE_NEW_THROW_EX (
              retval,
              Component_exec_i,
              ::CORBA::NO_MEMORY ());

      return retval;
  }

  extern "C" STOREREFERENCES_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_ComponentHome_Impl (void)
  {
      ::Components::HomeExecutorBase_ptr retval =
          ::Components::HomeExecutorBase::_nil ();

      ACE_NEW_NORETURN (
              retval,
              Component_home_exec_i);

      return retval;
  }

}

