// $Id$

#include "Planner_One_Impl_exec_i.h"
#include "cuts/ActivationRecord.h"

namespace CIDL_MonolithicImplementation
{
  // Default constructor.
  Planner_One::Planner_One (void)
  {
    this->push_situation_handler_.name ("situation");
    this->push_situation_handler_.bind (this, &Planner_One::push_situation_handler);
    this->benchmark_agent_->register_port_agent (&this->push_situation_handler_.port_agent ());
  }

  // Destructor.
  Planner_One::~Planner_One (void)
  {
    this->benchmark_agent_->unregister_port_agent (&this->push_situation_handler_.port_agent ());
  }

  // EventSink: situation
  void Planner_One::push_situation (
    TSCE::Situation_Event * ev
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException))
  {
    this->push_situation_handler_.handle_event (ev);
  }

  void Planner_One::push_situation_handler (
    TSCE::Situation_Event * ev, 
    CUTS_Activation_Record * record) 
  {
    record->perform_action_no_logging (
      CUTS_Memory_Worker::Allocate_Memory (this->memory_, 1));

    record->perform_action_no_logging (
      10, CUTS_CPU_Worker::Run_Processor (this->cpu_));

    record->perform_action_no_logging (
      30, CUTS_CPU_Worker::Run_Processor (this->cpu_));

    record->record_exit_point (
      "assessment",
      Event_Producer::Push_Event <OBV_TSCE::Assessment_Event> (
        this->producer_, &_coworker_type::_ctx_type::push_assessment));

    record->perform_action_no_logging (
      CUTS_Memory_Worker::Deallocate_Memory (this->memory_, 1));
  }

  // SessionComponent: set_session_context
  void Planner_One::set_session_context (
    ::Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::set_session_context (ctx);

    // Event: TSCE::Situation_Event
    CIAO_REGISTER_OBV_FACTORY (TSCE::Situation_Event_init,
                               TSCE::Situation_Event);
  }

  // SessionComponent: ciao_preactivate
  void Planner_One::ciao_preactivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ciao_preactivate ();

    this->memory_.parent (this->registration_id_);
    this->cpu_.parent (this->registration_id_);

    this->push_situation_handler_.activate ();
  }

  // Environment: activate
  void Planner_One::ccm_activate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
  }

  // SessionComponent: ciao_postactivate
  void Planner_One::ciao_postactivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ciao_postactivate ();
  }

  // Environment: passivate
  void Planner_One::ccm_passivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
    this->_coworker_type::ccm_passivate ();


    this->push_situation_handler_.deactivate ();
  }

  // Environment: remove
  void Planner_One::ccm_remove (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException))
  {
  }

  //
  // Planner_One_Factory
  //
  Planner_One_Factory::Planner_One_Factory (void) 
  {
     
  }

  //
  // ~Planner_One_Factory
  //
  Planner_One_Factory::~Planner_One_Factory (void) 
  {
     
  }

  CUTS_FACTORY_EXPORT_IMPLEMENT (Planner_One_Factory,
                                 create_TSCE_Planner_One_Factory_Impl);
}

