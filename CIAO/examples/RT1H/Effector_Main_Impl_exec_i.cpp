// $Id$

#include "Effector_Main_Impl_exec_i.h"
#include "cuts/ActivationRecord.h"

namespace CIDL_MonolithicImplementation
{
  // Default constructor.
  Effector_Main::Effector_Main (void)
  {
    this->push_status_handler_.name ("status");
    this->push_status_handler_.bind (this, &Effector_Main::push_status_handler);
    this->benchmark_agent_->register_port_agent (&this->push_status_handler_.port_agent ());

    this->push_command_handler_.name ("command");
    this->push_command_handler_.bind (this, &Effector_Main::push_command_handler);
    this->benchmark_agent_->register_port_agent (&this->push_command_handler_.port_agent ());
  }

  // Destructor.
  Effector_Main::~Effector_Main (void)
  {
    this->benchmark_agent_->unregister_port_agent (&this->push_status_handler_.port_agent ());
    this->benchmark_agent_->unregister_port_agent (&this->push_command_handler_.port_agent ());
  }

  // EventSink: status
  void Effector_Main::push_status (
    TSCE::Status_Event * ev
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException))
  {
    this->push_status_handler_.handle_event (ev);
  }

  void Effector_Main::push_status_handler (
    TSCE::Status_Event * ev, 
    CUTS_Activation_Record * record) 
  {
    record->perform_action_no_logging (
      CUTS_CPU_Worker::Run_Processor (this->cpu_));

    record->record_exit_point (
      "status",
      Event_Producer::Push_Event <OBV_TSCE::Status_Event> (
        this->producer_, &_coworker_type::_ctx_type::push_status));
  }

  // EventSink: command
  void Effector_Main::push_command (
    TSCE::Command_Event * ev
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException))
  {
    this->push_command_handler_.handle_event (ev);
  }

  void Effector_Main::push_command_handler (
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
      "command",
      Event_Producer::Push_Event <OBV_TSCE::Command_Event> (
        this->producer_, &_coworker_type::_ctx_type::push_command));

    record->record_exit_point (
      "status",
      Event_Producer::Push_Event <OBV_TSCE::Status_Event> (
        this->producer_, &_coworker_type::_ctx_type::push_status));
  }

  // SessionComponent: set_session_context
  void Effector_Main::set_session_context (
    ::Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::set_session_context (ctx);

    // Event: TSCE::Command_Event
    CIAO_REGISTER_OBV_FACTORY (TSCE::Command_Event_init,
                               TSCE::Command_Event);

    // Event: TSCE::Status_Event
    CIAO_REGISTER_OBV_FACTORY (TSCE::Status_Event_init,
                               TSCE::Status_Event);
  }

  // SessionComponent: ciao_preactivate
  void Effector_Main::ciao_preactivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ciao_preactivate ();

    this->cpu_.parent (this->registration_id_);
    this->memory_.parent (this->registration_id_);

    this->push_command_handler_.activate ();
    this->push_status_handler_.activate ();
  }

  // Environment: activate
  void Effector_Main::ccm_activate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
  }

  // SessionComponent: ciao_postactivate
  void Effector_Main::ciao_postactivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ciao_postactivate ();
  }

  // Environment: passivate
  void Effector_Main::ccm_passivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ccm_passivate ();


    this->push_command_handler_.deactivate ();
    this->push_status_handler_.deactivate ();
  }

  // Environment: remove
  void Effector_Main::ccm_remove (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
  }

  //
  // Effector_Main_Factory
  //
  Effector_Main_Factory::Effector_Main_Factory (void) 
  {
     
  }

  //
  // ~Effector_Main_Factory
  //
  Effector_Main_Factory::~Effector_Main_Factory (void) 
  {
     
  }

  CUTS_FACTORY_EXPORT_IMPLEMENT (Effector_Main_Factory,
                                 create_TSCE_Effector_Main_Factory_Impl);
}

