#include "Backup_Gizmo_Low_exec.h"
#include "ciao/CIAO_common.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/Time_Value.h"
#include "ace/Thread.h"

namespace CIDL_Backup_Gizmo_Low
{
  //==================================================================
  // Component Executor Implementation Class:   Backup_Gizmo_Low_exec_i
  //==================================================================

  Backup_Gizmo_Low_exec_i::Backup_Gizmo_Low_exec_i (void)
  {
  }

  Backup_Gizmo_Low_exec_i::~Backup_Gizmo_Low_exec_i (void)
  {
  }

  int
  Backup_Gizmo_Low_exec_i::handle_timeout (const ACE_Time_Value &,
                                           const void *)
  {

    ::SPACE::Gizmo_Data_var ev = new OBV_SPACE::Gizmo_Data;
    ev->sender ("Secondary-Gizmo-Low");
    this->context_->push_observation_data (ev);
    return 0;

  }

  void
  Backup_Gizmo_Low_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC (( ::CORBA::SystemException,
                   ::Components::CCMException))
  {
    this->context_ =
      Backup_Gizmo_Low_Context::_narrow (
        ctx
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (this->context_ == 0)
    {
      ACE_THROW ( ::CORBA::INTERNAL ());
    }
  }

  void
  Backup_Gizmo_Low_exec_i::ciao_preactivate (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (( ::CORBA::SystemException,
                   ::Components::CCMException))
  {
    // Your code here.
  }

  void
  Backup_Gizmo_Low_exec_i::configuration_complete (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (( ::CORBA::SystemException,
                   ::Components::CCMException))
  {
    ACE_Reactor *reactor =
      this->context_->_ciao_the_Container ()->the_ORB ()->orb_core ()->reactor ();
    ACE_Time_Value interval (5);

    reactor->owner (ACE_Thread::self ());
    if (reactor->schedule_timer (this, this, interval, interval) < 0)
      {
        ACE_ERROR ((LM_ERROR, "Exec-Agent: Error will registering "
                    "periodic task!\n"));
      }
  }

  void
  Backup_Gizmo_Low_exec_i::ccm_activate (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (( ::CORBA::SystemException,
                   ::Components::CCMException))
  {
    // Your code here.
  }

  void
  Backup_Gizmo_Low_exec_i::ccm_passivate (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (( ::CORBA::SystemException,
                   ::Components::CCMException))
  {
    // Your code here.
  }

  void
  Backup_Gizmo_Low_exec_i::ccm_remove (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (( ::CORBA::SystemException,
                   ::Components::CCMException))
  {
    // Your code here.
  }

  //==================================================================
  // Home Executor Implementation Class:   Backup_Gizmo_Low_Factory_exec_i
  //==================================================================

  Backup_Gizmo_Low_Factory_exec_i::Backup_Gizmo_Low_Factory_exec_i (void)
  {
  }

  Backup_Gizmo_Low_Factory_exec_i::~Backup_Gizmo_Low_Factory_exec_i (void)
  {
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  Backup_Gizmo_Low_Factory_exec_i::create (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC (( ::CORBA::SystemException,
                   ::Components::CCMException))
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      Backup_Gizmo_Low_exec_i,
      ::CORBA::NO_MEMORY ());
    ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

    return retval;
  }

  extern "C" BACKUP_GIZMO_LOW_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_SPACE_Backup_Gizmo_Low_Factory_Impl (void)
  {
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      Backup_Gizmo_Low_Factory_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}
