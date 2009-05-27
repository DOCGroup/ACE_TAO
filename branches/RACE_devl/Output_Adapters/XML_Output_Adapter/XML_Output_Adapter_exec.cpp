#include "XML_Output_Adapter_exec.h"
#include "ciao/CIAO_common.h"
#include "Config_Handlers/DP_Handler.h"
#include "Config_Handlers/Utils/XML_Helper.h"
#include "Config_Handlers/Deployment.hpp"
#include <string>

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_XML_Output_Adapter_Impl
    {
      //==================================================================
      // Facet Executor Implementation Class:   PlanEgress_exec_i
      //==================================================================

      PlanEgress_exec_i::PlanEgress_exec_i (XML_Output_Adapter_exec_i &comp)
        : comp_ (comp)
      {
      }

      PlanEgress_exec_i::~PlanEgress_exec_i (void)
      {
      }

      // Operations from ::CIAO::RACE::PlanEgress

      ::CORBA::Boolean
      PlanEgress_exec_i::output_plan (const ::CIAO::RACE::Plan_Actions &plan_seq)
        ACE_THROW_SPEC ((CORBA::SystemException))
      {
        CIAO_TRACE ("XML_Output_Adapter_exec::PlanEgres_exec::output_plan");
        for (size_t i = 0;i < plan_seq.length(); ++i)
          {
            if (!this->print_plan (plan_seq[i].plan))
              {
                return false;
              }
          }
        return true;
      }

      bool
      PlanEgress_exec_i::print_plan (const ::Deployment::DeploymentPlan &plan)
      {
        using namespace CIAO::Config_Handlers;

        DP_Handler reverse_handler (plan);
        xercesc::DOMDocument *the_xsc (XML_HELPER->create_dom (0));

        deploymentPlan (*reverse_handler.xsc (), the_xsc);

        std::string dom_out (plan.UUID.in ());
        dom_out += ".cdp";
        XML_HELPER->write_DOM (the_xsc, dom_out.c_str());

        delete the_xsc;
	ACE_DEBUG ((LM_DEBUG, "6\n"));
        return true;
      }

      //==================================================================
      // Component Executor Implementation Class:   XML_Output_Adapter_exec_i
      //==================================================================

      XML_Output_Adapter_exec_i::XML_Output_Adapter_exec_i (void)
      {
      }

      XML_Output_Adapter_exec_i::~XML_Output_Adapter_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Attribute operations.

      // Port operations.

      ::CIAO::RACE::CCM_PlanEgress_ptr
      XML_Output_Adapter_exec_i::get_plan_egress (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
      {
        return new PlanEgress_exec_i (*this);
      }

      // Operations from Components::SessionComponent

      void
      XML_Output_Adapter_exec_i::set_session_context (
        ::Components::SessionContext_ptr ctx
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        this->context_ =
        XML_Output_Adapter_Context::_narrow (
          ctx
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        if (this->context_ == 0)
        {
          ACE_THROW (CORBA::INTERNAL ());
        }
      }

      void
      XML_Output_Adapter_exec_i::ciao_preactivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      XML_Output_Adapter_exec_i::configuration_complete (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      XML_Output_Adapter_exec_i::ccm_activate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      XML_Output_Adapter_exec_i::ccm_passivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      XML_Output_Adapter_exec_i::ccm_remove (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      //==================================================================
      // Home Executor Implementation Class:   XML_Output_Adapter_Home_exec_i
      //==================================================================

      XML_Output_Adapter_Home_exec_i::XML_Output_Adapter_Home_exec_i (void)
      {
      }

      XML_Output_Adapter_Home_exec_i::~XML_Output_Adapter_Home_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Home operations.

      // Factory and finder operations.

      // Attribute operations.

      // Implicit operations.

      ::Components::EnterpriseComponent_ptr
      XML_Output_Adapter_Home_exec_i::create (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        ::Components::EnterpriseComponent_ptr retval =
        ::Components::EnterpriseComponent::_nil ();

        ACE_NEW_THROW_EX (
          retval,
          XML_Output_Adapter_exec_i,
          CORBA::NO_MEMORY ());
        ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

        return retval;
      }

      extern "C" XML_OUTPUT_ADAPTER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_XML_Output_Adapter_Home_Impl (void)
      {
        ::Components::HomeExecutorBase_ptr retval =
        ::Components::HomeExecutorBase::_nil ();

        ACE_NEW_RETURN (
          retval,
          XML_Output_Adapter_Home_exec_i,
          ::Components::HomeExecutorBase::_nil ());

        return retval;
      }
    }
  }
}
