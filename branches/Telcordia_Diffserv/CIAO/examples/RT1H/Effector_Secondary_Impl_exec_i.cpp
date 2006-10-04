// $Id$

#include "Effector_Secondary_Impl_exec_i.h"
#include "cuts/ActivationRecord.h"

namespace CIDL_MonolithicImplementation
{
  // Default constructor.
  Effector_Secondary::Effector_Secondary (void)
  {
    this->push_command_handler_.name ("command");
    this->push_command_handler_.bind (this, &Effector_Secondary::push_command_handler);
    this->benchmark_agent_->register_port_agent (&this->push_command_handler_.port_agent ());
  }

  // Destructor.
  Effector_Secondary::~Effector_Secondary (void)
  {
    this->benchmark_agent_->unregister_port_agent (&this->push_command_handler_.port_agent ());
  }

  // EventSink: command
  void Effector_Secondary::push_command (
    TSCE::Command_Event * ev
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException))
  {
    this->push_command_handler_.handle_event (ev);
  }

  void Effector_Secondary::push_command_handler (
    TSCE::Command_Event * ev, 
    CUTS_Activation_Record * record) 
  {
    record->perform_action_no_logging (
      CUTS_Memory_Worker::Allocate_Memory (this->memory_, 1));

    record->perform_action_no_logging (
      CUTS_CPU_Worker::Run_Processor (this->cpu_));

    record->perform_action_no_logging (
      CUTS_Memory_Worker::Deallocate_Memory (this->memory_, 1));

    record->record_exit_point (
      "status",
      Event_Producer::Push_Event <OBV_TSCE::Status_Event> (
        this->producer_, &_coworker_type::_ctx_type::push_status));
  }

  // SessionComponent: set_session_context
  void Effector_Secondary::set_session_context (
    ::Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::set_session_context (ctx);

    // Event: TSCE::Command_Event
    CIAO_REGISTER_OBV_FACTORY (TSCE::Command_Event_init,
                               TSCE::Command_Event);
  }

  // SessionComponent: ciao_preactivate
  void Effector_Secondary::ciao_preactivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ciao_preactivate ();

    this->memory_.parent (this->registration_id_);
    this->cpu_.parent (this->registration_id_);

    this->push_command_handler_.activate ();
  }

  // Environment: activate
  void Effector_Secondary::ccm_activate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
  }

  // SessionComponent: ciao_postactivate
  void Effector_Secondary::ciao_postactivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ciao_postactivate ();
  }

  // Environment: passivate
  void Effector_Secondary::ccm_passivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ccm_passivate ();


    this->push_command_handler_.deactivate ();
  }

  // Environment: remove
  void Effector_Secondary::ccm_remove (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
  }

  //
  // Effector_Secondary_Factory
  //
  Effector_Secondary_Factory::Effector_Secondary_Factory (void) 
  {
     
  }

  //
  // ~Effector_Secondary_Factory
  //
  Effector_Secondary_Factory::~Effector_Secondary_Factory (void) 
  {
     
  }

  CUTS_FACTORY_EXPORT_IMPLEMENT (Effector_Secondary_Factory,
                                 create_TSCE_Effector_Secondary_Factory_Impl);
}

