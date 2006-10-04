// $Id$

#include "Planner_Three_Impl_exec_i.h"
#include "cuts/ActivationRecord.h"

namespace CIDL_MonolithicImplementation
{
  // Default constructor.
  Planner_Three::Planner_Three (void)
  {
    this->push_track_handler_.name ("track");
    this->push_track_handler_.bind (this, &Planner_Three::push_track_handler);
    this->benchmark_agent_->register_port_agent (&this->push_track_handler_.port_agent ());

    // Initialize all periodic actions
    this->periodic_send_track_event_.init (this, &This_Component::periodic_send_track_event);
  }

  // Destructor.
  Planner_Three::~Planner_Three (void)
  {
    this->benchmark_agent_->unregister_port_agent (&this->push_track_handler_.port_agent ());
  }

  // EventSink: track
  void Planner_Three::push_track (
    TSCE::Track_Event * ev
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException))
  {
    this->push_track_handler_.handle_event (ev);
  }

  void Planner_Three::push_track_handler (
    TSCE::Track_Event * ev, 
    CUTS_Activation_Record * record) 
  {
    record->perform_action_no_logging (
      CUTS_Memory_Worker::Allocate_Memory (this->memory_, 1));

    record->perform_action_no_logging (
      10, CUTS_CPU_Worker::Run_Processor (this->cpu_));

    record->perform_action_no_logging (
      30, CUTS_CPU_Worker::Run_Processor (this->cpu_));

    record->record_exit_point (
      "situation",
      Event_Producer::Push_Event <OBV_TSCE::Situation_Event> (
        this->producer_, &_coworker_type::_ctx_type::push_situation));

    record->perform_action_no_logging (
      CUTS_Memory_Worker::Deallocate_Memory (this->memory_, 1));
  }

  // Periodic event: send_track_event
  void Planner_Three::periodic_send_track_event (void) 
  {
    CUTS_Activation_Record dummy_record;
    CUTS_Activation_Record * record = &dummy_record;

    record->record_exit_point (
      "command",
      Event_Producer::Push_Event <OBV_TSCE::Command_Event> (
        this->producer_, &_coworker_type::_ctx_type::push_command));
  }

  // SessionComponent: set_session_context
  void Planner_Three::set_session_context (
    ::Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::set_session_context (ctx);

    // Event: TSCE::Track_Event
    CIAO_REGISTER_OBV_FACTORY (TSCE::Track_Event_init,
                               TSCE::Track_Event);
  }

  // SessionComponent: ciao_preactivate
  void Planner_Three::ciao_preactivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ciao_preactivate ();

    this->memory_.parent (this->registration_id_);
    this->cpu_.parent (this->registration_id_);

    this->push_track_handler_.activate ();
  }

  // Environment: activate
  void Planner_Three::ccm_activate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
  }

  // SessionComponent: ciao_postactivate
  void Planner_Three::ciao_postactivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ciao_postactivate ();

    this->periodic_send_track_event_.activate (1000);
  }

  // Environment: passivate
  void Planner_Three::ccm_passivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ccm_passivate ();

    this->periodic_send_track_event_.deactivate ();

    this->push_track_handler_.deactivate ();
  }

  // Environment: remove
  void Planner_Three::ccm_remove (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
  }

  //
  // Planner_Three_Factory
  //
  Planner_Three_Factory::Planner_Three_Factory (void) 
  {
     
  }

  //
  // ~Planner_Three_Factory
  //
  Planner_Three_Factory::~Planner_Three_Factory (void) 
  {
     
  }

  CUTS_FACTORY_EXPORT_IMPLEMENT (Planner_Three_Factory,
                                 create_TSCE_Planner_Three_Factory_Impl);
}

