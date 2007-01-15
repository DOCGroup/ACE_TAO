// $Id$
// Author: Stoyan Paunov

#include "PlanGenerator_exec.h"
#include "ciao/CIAO_common.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "Config_Handlers/Package_Handlers/PCD_Handler.h"
#include "Config_Handlers/Common.h"
#include "Config_Handlers/DnC_Dump.h"

#include "DAnCE/RepositoryManager/RepositoryManagerDaemonC.h"
#include "PlanGenerator.h"

#include <iostream>
using namespace std;


namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_PlanGenerator_Impl
    {
      //==================================================================
      // Component Executor Implementation Class:
      // PlanGenerator_exec_i
      //==================================================================

      PlanGenerator_exec_i::PlanGenerator_exec_i (void)
      {
      }

      PlanGenerator_exec_i::~PlanGenerator_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Attribute operations.

      // Port operations.

      void
      PlanGenerator_exec_i::push_deployment (
        ::CIAO::RACE::Deploy_Input * ev
        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
      {
        try
        {

          //Get the IOR of the RepoMan from the naming service

          //Obtain a ref to the orb
          CORBA::ORB_ptr orb = this->context_->_ciao_the_Container ()->the_ORB ();

          //form the path to the RepoMan IOR file
          ACE_CString RepoMan_ior = ("file://");
          RepoMan_ior += ACE_OS::getenv ("CIAO_ROOT");
          RepoMan_ior += "/DAnCE/RepositoryManager/RepositoryManagerDeamon.ior";

          cout << "RepoMan ior file: " << RepoMan_ior.c_str () << endl;

          CORBA::Object_var obj =
                    orb->string_to_object (RepoMan_ior.c_str ());

          CIAO::RepositoryManagerDaemon_var rm =
                   CIAO::RepositoryManagerDaemon::_narrow (obj.in ());

          if (CORBA::is_nil (rm.in ()))
          {
            ACE_ERROR ((LM_ERROR,
                        "Unable to acquire RepositoryManagerDaemon's objref\n"));

            ACE_THROW (CORBA::INTERNAL ());
          }

          //now obtian the corresponding PackageConfiguration from
          //the RepositoryManager

          Deployment::PackageConfiguration* rmpc;

          try
          {
            //retrieve the curresponding PackageConfiguration
            //from the RepoMan
            rmpc = rm->findPackageByName (ev->package_name ());

            ACE_DEBUG ((LM_INFO,
                       "The package was found!\nUUID: %s\n",
                        rmpc->UUID));
          }
          catch (Deployment::NoSuchName &)
          {
            ACE_ERROR ((LM_ERROR,
                       "Error! Package not found! Location update failed!\n"));

            ACE_THROW (CORBA::INTERNAL ());

          }
          catch (CORBA::Exception &)
          {
            ACE_ERROR ((LM_ERROR,
                       "Error! General exception! Location update failed!\n"));

            ACE_THROW (CORBA::INTERNAL ());
          }

          //now create the plan

          Deployment::DeploymentPlan* plan = 0;
          ACE_NEW_THROW_EX (plan,
                            Deployment::DeploymentPlan (),
                            CORBA::INTERNAL ());

          Plan_Generator pgen (*plan, *rmpc);

          //start the generation
          pgen.Visit ();

          //So now what?
          ::Deployment::DnC_Dump::dump (*plan);

          //do not call the plan analyzer just yet
          return;

          //creat an action sequence and populate it
          ::CIAO::RACE::Plan_Actions plan_action_seq;

           /// Create a Plan_Action_seq structure of length 1.
           plan_action_seq.length (1);

           /// Now populate the Plan_Action_seq structure.
           plan_action_seq [0].command = ev->command ();
           plan_action_seq [0].plan = *plan;
           plan_action_seq [0].package = *rmpc;

           /// Now invoke the plan_analyzer.
           this->context_->get_connection_ingress  ()
               ->analyze_plan (plan_action_seq);
        }
        catch (...)
        {
          ACE_ERROR ((LM_ERROR, "Exception caught in push_deployment\n"));
        }
      }

      // Operations from Components::SessionComponent

      void
      PlanGenerator_exec_i::set_session_context (
        ::Components::SessionContext_ptr ctx
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        this->context_ =
        PlanGenerator_Context::_narrow (
          ctx
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        if (this->context_ == 0)
        {
          ACE_THROW (CORBA::INTERNAL ());
        }
      }

      void
      PlanGenerator_exec_i::ciao_preactivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      PlanGenerator_exec_i::ciao_postactivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      PlanGenerator_exec_i::ccm_activate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      PlanGenerator_exec_i::ccm_passivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      PlanGenerator_exec_i::ccm_remove (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      //==================================================================
      // Home Executor Implementation Class:   PlanGenerator_Home_exec_i
      //==================================================================

      PlanGenerator_Home_exec_i::PlanGenerator_Home_exec_i (void)
      {
      }

      PlanGenerator_Home_exec_i::~PlanGenerator_Home_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Home operations.

      // Factory and finder operations.

      // Attribute operations.

      // Implicit operations.

      ::Components::EnterpriseComponent_ptr
      PlanGenerator_Home_exec_i::create (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        ::Components::EnterpriseComponent_ptr retval =
        ::Components::EnterpriseComponent::_nil ();

        ACE_NEW_THROW_EX (
          retval,
          PlanGenerator_exec_i,
          CORBA::NO_MEMORY ());
        ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

        return retval;
      }

      extern "C" PLANGENERATOR_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_PlanGenerator_Home_Impl (void)
      {
        ::Components::HomeExecutorBase_ptr retval =
        ::Components::HomeExecutorBase::_nil ();

        ACE_NEW_RETURN (
          retval,
          PlanGenerator_Home_exec_i,
          ::Components::HomeExecutorBase::_nil ());

        return retval;
      }
    }
  }
}
