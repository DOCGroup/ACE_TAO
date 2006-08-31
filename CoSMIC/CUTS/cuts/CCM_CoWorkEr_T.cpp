// $Id$

#if !defined (__CUTS_INLINE__)
# include "cuts/CCM_CoWorkEr_T.inl"
#endif

#include "ciao/Container_Base.h"
#include "ace/Log_Msg.h"
#include "ace/SString.h"

//
// CUTS_CCM_CoWorkEr_T
//
template <typename COMPONENT, typename COMPONENT_CONTEXT>
CUTS_CCM_CoWorkEr_T
<COMPONENT, COMPONENT_CONTEXT>::CUTS_CCM_CoWorkEr_T (void)
{
  CUTS_CCM_CoWorkEr::init ();
}

//
// ~CUTS_CCM_CoWorkEr_T
//
template <typename COMPONENT, typename COMPONENT_CONTEXT>
CUTS_CCM_CoWorkEr_T
<COMPONENT, COMPONENT_CONTEXT>::~CUTS_CCM_CoWorkEr_T (void)
{

}

//
// set_session_context
//
template <typename COMPONENT, typename COMPONENT_CONTEXT>
void CUTS_CCM_CoWorkEr_T
<COMPONENT, COMPONENT_CONTEXT>::set_session_context (
  ::Components::SessionContext_ptr ctx
  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((::CORBA::SystemException,
                   ::Components::CCMException))
{
  // Narrow the context to the one for this component type.
  this->context_ =
    Component_Context::_narrow (ctx ACE_ENV_ARG_PARAMETER);

  // Check the environment variable.
  ACE_CHECK;

  // Verify the context was properly narrowed.
  if (this->context_ == 0)
  {
    ACE_THROW (CORBA::INTERNAL ());
  }

  //// Get the instance name of the component from the context.
  //::CORBA::String_var instance_name (this->context_->_ciao_instance_id ());

  //::CIAO::Session_Container * container =
  //  this->context_->_ciao_the_Container ();

  //if (container != 0)
  //{
  //  CORBA::ORB_var the_ORB = container->the_ORB ();
  //  PortableServer::POA_var the_POA = container->the_POA ();

  //  if (CORBA::is_nil (the_ORB.in ()))
  //  {
  //    ACE_ERROR ((LM_ERROR,
  //                "[%M] -%T - container ORB for %s is NIL\n",
  //                instance_name.in ()));
  //  }
  //  else if (CORBA::is_nil (the_POA.in ()))
  //  {
  //    ACE_ERROR ((LM_ERROR,
  //                "[%M] -%T - container POA for %s is NIL\n",
  //                instance_name.in ()));
  //  }
  //  else
  //  {
  //    ACE_DEBUG ((LM_DEBUG,
  //                "[%M] -%T - initializing real-time ORB for %s\n",
  //                instance_name.in ()));

  //    try
  //    {
  //      CUTS_CCM_CoWorkEr::init ();
  //      // CUTS_CCM_CoWorkEr::init_realtime (the_ORB.in (), the_POA.in ());
  //    }
  //    catch (const CORBA::Exception & ex)
  //    {
  //      // Initialize the component with the default configuration
  //      // for the Benchmark_Agent component.
  //      ACE_ERROR ((LM_ERROR,
  //                  "[%M] -%T - %s\n",
  //                  ex._info ().c_str ()));

  //      ACE_ERROR ((LM_WARNING,
  //                  "[%M] -%T - real-time ORB initialization failed for %s\n",
  //                  instance_name.in ()));

  //      CUTS_CCM_CoWorkEr::init ();
  //    }
  //    catch (...)
  //    {
  //      // Initialize the component with the default configuration
  //      // for the Benchmark_Agent component.
  //      ACE_ERROR ((LM_ERROR,
  //                  "[%M] -%T - unknown exception caught\n"));

  //      ACE_ERROR ((LM_WARNING,
  //                  "[%M] -%T - real-time ORB initialization failed for %s\n",
  //                  instance_name.in ()));

  //      CUTS_CCM_CoWorkEr::init ();
  //    }
  //  }
  //}
  //else
  //{
  //  ACE_ERROR ((LM_ERROR,
  //              "[%M] -%T - cannot access container of %s\n",
  //              instance_name.in ()));

  //  // Initialize the component with the default configuration
  //  // for the Benchmark_Agent component.
  //  CUTS_CCM_CoWorkEr::init ();
  //}
}

//
// ciao_preactivate
//
template <typename COMPONENT, typename COMPONENT_CONTEXT>
void CUTS_CCM_CoWorkEr_T
<COMPONENT, COMPONENT_CONTEXT>::ciao_preactivate (
  ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((::CORBA::SystemException, ::Components::CCMException))
{
  ::CUTS::Testing_Service_var tsvc =
    this->context_->get_connection_cuts_testing_service ();

  // Get the instance name of the component from the context.
  ::CORBA::String_var instance_id (this->context_->_ciao_instance_id ());

  // Create a duplicate of the <benchmark_agent_servant_> before
  // passing it over the wire.
  ::CUTS::Benchmark_Agent_var agent =
    ::CUTS::Benchmark_Agent::_duplicate (this->benchmark_agent_->_this ());

  this->registration_id_ = CUTS_CCM_CoWorkEr::register_i (tsvc.in (),
                                                          agent.in (),
                                                          instance_id.in ());

  // Set the parent () of the <benchmark_agent_>.
  this->benchmark_agent_->parent (this->registration_id_);
}

//
// ccm_passivate
//
template <typename COMPONENT, typename COMPONENT_CONTEXT>
void CUTS_CCM_CoWorkEr_T
<COMPONENT, COMPONENT_CONTEXT>::ccm_passivate (
  ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((::CORBA::SystemException, ::Components::CCMException))
{
  ::CUTS::Testing_Service_var testing_service =
    this->context_->get_connection_cuts_testing_service ();

  // Get the instance name of the component from the context.
  ::CORBA::String_var instance_id (this->context_->_ciao_instance_id ());

  if (!::CORBA::is_nil (testing_service.in ()))
  {
    try
    {
      // Initialize the <Component_Registration> data structure and
      // unregister the component.
      ::CUTS::Component_Registration creg;
      creg.name = this->context_->_ciao_instance_id ();
      creg.agent =
        ::CUTS::Benchmark_Agent::_duplicate (
        this->benchmark_agent_->_this ());

      testing_service->unregister_component (creg);
    }
    catch (const ::CUTS::ID_Not_Found &)
    {
      ACE_ERROR ((
        LM_ERROR,
        "[%M] -%T - ID not found for %s; component was not registered\n",
        instance_id.in ()));

    }
    catch (const ::CORBA::Exception & ex)
    {
      ACE_ERROR ((LM_ERROR,
                  "[%M] -%T - %s\n",
                  ex._info ().c_str ()));
    }
    catch (...)
    {
      ACE_ERROR ((LM_ERROR,
                  "[%M] -%T - unknown exception has occurred\n"));
    }
  }
  else
  {
    ACE_DEBUG ((LM_ERROR,
                "[%M] -%T - %s not connected to Benchmark Data Collector\n",
                instance_id.in ()));
  }

  // Reset the <registration_id_>.
  this->registration_id_ = -1;
}
