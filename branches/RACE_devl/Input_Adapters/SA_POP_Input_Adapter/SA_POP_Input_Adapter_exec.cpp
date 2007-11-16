#include "SA_POP_Input_Adapter_exec.h"
#include "ciao/CIAO_common.h"

#include "Admin_exec.h"

#include "Config_Handlers/XML_File_Intf.h"
#include "Config_Handlers/Package_Handlers/PCD_Handler.h"
#include "Config_Handlers/Common.h"
#include <string>

namespace CIAO
{
	namespace RACE
	{
		namespace SA_POP_IA
		{
			namespace CIDL_SA_POP_IA_Component_Impl
			{
				SA_POP_IA_Component_exec_i::SA_POP_IA_Component_exec_i (void)
					: logger_ ("SA_POP_IA.log")
				{
				}

				SA_POP_IA_Component_exec_i::~SA_POP_IA_Component_exec_i (void)
				{
				}

				::CORBA::Boolean
					SA_POP_IA_Component_exec_i::deploy_plan (const char * uri)
				{

					std::string msg = "SA_POP_IA_Component_exec_i::Entering deploy_plan.";
					this->logger_.log (msg);
	
					auto_ptr < ::Deployment::DeploymentPlan > dp;
					Config_Handlers::XML_File_Intf xfi (uri);
					dp.reset (xfi.get_plan ());
					try
					{
						DAnCE_OA::Admin_var OA = this->context_->get_connection_OA ();
						OA->deploy_plan (*dp);
						msg = "SA_POP_IA_Component_exec_i::Leaving deploy_plan.";
						this->logger_.log (msg);
						return true;
					}
					catch (CORBA::Exception &ex)
					{
						msg = "Exception caught::SA_POP_IA_Component_exec_i::deploy_plan.";
						msg += ex._info ().c_str();
						this->logger_.log (msg);
						return false;
					}
				}

				::CORBA::Boolean
					SA_POP_IA_Component_exec_i::redeploy_plan (const char * uri)
				{
					std::string msg = "SA_POP_IA_Component_exec_i::Entering redeploy_plan.";
					this->logger_.log (msg);

					auto_ptr < ::Deployment::DeploymentPlan > dp;
					Config_Handlers::XML_File_Intf xfi (uri);
					dp.reset (xfi.get_plan ());
					try
					{
						DAnCE_OA::Admin_var OA = this->context_->get_connection_OA ();
						OA->redeploy_plan (*dp);
						std::string msg = "SA_POP_IA_Component_exec_i::Leaving redeploy_plan.";
						this->logger_.log (msg);
						return true;
					}
					catch (CORBA::Exception &ex)
					{
						std::string msg = "Exception caught::SA_POP_IA_Component_exec_i::redeploy_plan.";
						msg += ex._info ().c_str();
						this->logger_.log (msg);
						return false;
					}

				}

				::CIAO::RACE::SA_POP_IA::CCM_Admin_ptr
					SA_POP_IA_Component_exec_i::get_admin ()
				{
					std::string msg = "SA_POP_IA_Component_exec_i::Creating the admin object.\n";
					this->logger_.log (msg);
					return (new Admin_exec_i (this->context_, this->logger_));
				}

				void
					SA_POP_IA_Component_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
				{
					this->context_ =
						SA_POP_IA_Component_Context::_narrow (ctx);

					if (this->context_ == 0)
					{
						throw ::CORBA::INTERNAL ();
					}
				}

				void
					SA_POP_IA_Component_exec_i::ciao_preactivate ()
				{
				}

				void
					SA_POP_IA_Component_exec_i::ciao_postactivate ()
				{
				}

				void
					SA_POP_IA_Component_exec_i::ccm_activate ()
				{
				}

				void
					SA_POP_IA_Component_exec_i::ccm_passivate ()
				{
				}

				void
					SA_POP_IA_Component_exec_i::ccm_remove ()
				{
				}

				//==================================================================
				// Home Executor Implementation Class:   SA_POP_IA_Component_Home_exec_i
				//==================================================================

				SA_POP_IA_Component_Home_exec_i::SA_POP_IA_Component_Home_exec_i (void)
				{
				}

				SA_POP_IA_Component_Home_exec_i::~SA_POP_IA_Component_Home_exec_i (void)
				{
				}

				// Supported or inherited operations.

				// Home operations.

				// Factory and finder operations.

				// Attribute operations.

				// Implicit operations.

				::Components::EnterpriseComponent_ptr
					SA_POP_IA_Component_Home_exec_i::create ()
				{
					::Components::EnterpriseComponent_ptr retval =
						::Components::EnterpriseComponent::_nil ();

					ACE_NEW_THROW_EX (
						retval,
						SA_POP_IA_Component_exec_i,
						::CORBA::NO_MEMORY ());

					return retval;
				}

				extern "C" SA_POP_INPUT_ADAPTER_EXEC_Export ::Components::HomeExecutorBase_ptr
					create_CIAO_RACE_SA_POP_IA_SA_POP_IA_Component_Home_Impl (void)
				{
					::Components::HomeExecutorBase_ptr retval =
						::Components::HomeExecutorBase::_nil ();

					ACE_NEW_RETURN (
						retval,
						SA_POP_IA_Component_Home_exec_i,
						::Components::HomeExecutorBase::_nil ());

					return retval;
				}
			}
		}
	}
}
