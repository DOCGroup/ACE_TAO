// $Id$

#include "Config_Op_Impl_exec_i.h"
#include "cuts/ActivationRecord.h"

namespace CIDL_Config_Op
{
  // Default constructor.
  Config_Op::Config_Op (void)
  {
    this->push_status_handler_.name ("status");
    this->push_status_handler_.bind (this, &Config_Op::push_status_handler);
    this->benchmark_agent_->register_port_agent (&this->push_status_handler_.port_agent ());

    this->push_assessment_handler_.name ("assessment");
    this->push_assessment_handler_.bind (this, &Config_Op::push_assessment_handler);
    this->benchmark_agent_->register_port_agent (&this->push_assessment_handler_.port_agent ());
  }

  // Destructor.
  Config_Op::~Config_Op (void)
  {
    this->benchmark_agent_->unregister_port_agent (&this->push_status_handler_.port_agent ());
    this->benchmark_agent_->unregister_port_agent (&this->push_assessment_handler_.port_agent ());
  }

  // EventSink: status
  void Config_Op::push_status (
    TSCE::Status_Event * ev
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException))
  {
    this->push_status_handler_.handle_event (ev);
  }

  void Config_Op::push_status_handler (
    TSCE::Status_Event * ev, 
    CUTS_Activation_Record * record) 
  {
    record->perform_action_no_logging (
      2, CUTS_CPU_Worker::Run_Processor (this->cpu_));
  }

  // EventSink: assessment
  void Config_Op::push_assessment (
    TSCE::Assessment_Event * ev
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException))
  {
    this->push_assessment_handler_.handle_event (ev);
  }

  void Config_Op::push_assessment_handler (
    TSCE::Assessment_Event * ev, 
    CUTS_Activation_Record * record) 
  {
    record->perform_action_no_logging (
      CUTS_Memory_Worker::Allocate_Memory (this->memory_, 4));

    record->perform_action_no_logging (
      10, CUTS_CPU_Worker::Run_Processor (this->cpu_));

    record->perform_action_no_logging (
      15, CUTS_CPU_Worker::Run_Processor (this->cpu_));

    record->record_exit_point (
      "command",
      Event_Producer::Push_Event <OBV_TSCE::Command_Event> (
        this->producer_, &_coworker_type::_ctx_type::push_command));

    record->perform_action_no_logging (
      CUTS_Memory_Worker::Deallocate_Memory (this->memory_, 4));
  }

  // SessionComponent: set_session_context
  void Config_Op::set_session_context (
    ::Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::set_session_context (ctx);

    // Event: TSCE::Assessment_Event
    CIAO_REGISTER_OBV_FACTORY (TSCE::Assessment_Event_init,
                               TSCE::Assessment_Event);

    // Event: TSCE::Status_Event
    CIAO_REGISTER_OBV_FACTORY (TSCE::Status_Event_init,
                               TSCE::Status_Event);
  }

  // SessionComponent: ciao_preactivate
  void Config_Op::ciao_preactivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ciao_preactivate ();

    this->memory_.parent (this->registration_id_);
    this->cpu_.parent (this->registration_id_);

    this->push_assessment_handler_.activate ();
    this->push_status_handler_.activate ();
  }

  // Environment: activate
  void Config_Op::ccm_activate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
  }

  // SessionComponent: ciao_postactivate
  void Config_Op::ciao_postactivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ciao_postactivate ();
  }

  // Environment: passivate
  void Config_Op::ccm_passivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ccm_passivate ();


    this->push_assessment_handler_.deactivate ();
    this->push_status_handler_.deactivate ();
  }

  // Environment: remove
  void Config_Op::ccm_remove (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
  }

  //
  // Config_Op_Factory
  //
  Config_Op_Factory::Config_Op_Factory (void) 
  {
     
  }

  //
  // ~Config_Op_Factory
  //
  Config_Op_Factory::~Config_Op_Factory (void) 
  {
     
  }

  CUTS_FACTORY_EXPORT_IMPLEMENT (Config_Op_Factory,
                                 create_TSCE_Config_Op_Factory_Impl);
}

