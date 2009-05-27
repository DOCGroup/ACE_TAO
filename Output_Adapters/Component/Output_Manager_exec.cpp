#include "Output_Manager_exec.h"
#include "ciao/CIAO_common.h"
#include <map>

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Output_Manager_Impl
    {
      //==================================================================
      // Facet Executor Implementation Class:   PlanEgress_exec_i
      //==================================================================

      PlanEgress_exec_i::PlanEgress_exec_i (Output_Manager_exec_i &comp) :
        comp_ (comp)
      {
      }

      PlanEgress_exec_i::~PlanEgress_exec_i (void)
      {
      }

      // Operations from ::CIAO::RACE::PlanEgress

      ::CORBA::Boolean
      PlanEgress_exec_i::output_plan (
        const ::CIAO::RACE::Plan_Actions & plan_seq
        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
      {
	ACE_DEBUG ((LM_ERROR, "RACE:Output_Manager::output_plan\n"));
        CIAO::RACE::Output_Manager::adaptersConnections_var conn
            (comp_.context_->get_connections_adapters ());

        for (CORBA::ULong i = 0; i < conn->length (); ++i)
          {
            ACE_DEBUG ((LM_DEBUG, "In for loop\n"));
            try
              {
                conn[i].objref->output_plan (plan_seq);
              }
	    catch (...)
	      {
                ACE_ERROR ((LM_ERROR, "Output_Manager: Caught unknown "
                            "exception while pushing plan\n"));
	      }
          }

        return true;
      }

      //==================================================================
      // Component Executor Implementation Class:   Output_Manager_exec_i
      //==================================================================

      Output_Manager_exec_i::Output_Manager_exec_i (void)
      {
      }

      Output_Manager_exec_i::~Output_Manager_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Attribute operations.

      // Port operations.

      ::CIAO::RACE::CCM_PlanEgress_ptr
      Output_Manager_exec_i::get_output (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
      {
        return new PlanEgress_exec_i (*this);
      }

      // Operations from Components::SessionComponent

      void
      Output_Manager_exec_i::set_session_context (
        ::Components::SessionContext_ptr ctx
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        this->context_ =
        Output_Manager_Context::_narrow (
          ctx
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        if (this->context_ == 0)
        {
          ACE_THROW (CORBA::INTERNAL ());
        }
      }

      void
      Output_Manager_exec_i::ciao_preactivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
      }

      void
      Output_Manager_exec_i::configuration_complete (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      Output_Manager_exec_i::ccm_activate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      Output_Manager_exec_i::ccm_passivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      Output_Manager_exec_i::ccm_remove (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      //==================================================================
      // Home Executor Implementation Class:   Output_Manager_Home_exec_i
      //==================================================================

      Output_Manager_Home_exec_i::Output_Manager_Home_exec_i (void)
      {
      }

      Output_Manager_Home_exec_i::~Output_Manager_Home_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Home operations.

      // Factory and finder operations.

      // Attribute operations.

      // Implicit operations.

      ::Components::EnterpriseComponent_ptr
      Output_Manager_Home_exec_i::create (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        ::Components::EnterpriseComponent_ptr retval =
        ::Components::EnterpriseComponent::_nil ();

        ACE_NEW_THROW_EX (
          retval,
          Output_Manager_exec_i,
          CORBA::NO_MEMORY ());
        ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

        return retval;
      }

      extern "C" OUTPUT_MANAGER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Output_Manager_Home_Impl (void)
      {
        ::Components::HomeExecutorBase_ptr retval =
        ::Components::HomeExecutorBase::_nil ();

        ACE_NEW_RETURN (
          retval,
          Output_Manager_Home_exec_i,
          ::Components::HomeExecutorBase::_nil ());

        return retval;
      }
    }
  }
}
