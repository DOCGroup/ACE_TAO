// $Id$
//Author: Stoyan Paunov


#include "LocationUpdater_exec.h"
#include "ciao/CIAO_common.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "Config_Handlers/Package_Handlers/PCD_Handler.h"
#include "Config_Handlers/Common.h"

#include "DAnCE/RepositoryManager/RepositoryManagerDaemonC.h"
#include "PlanUpdater.h"

#include <iostream>
using namespace std;


namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_LocationUpdater_Impl
    {
      //==================================================================
      // Component Executor Implementation Class:
      // LocationUpdater_exec_i
      //==================================================================

      LocationUpdater_exec_i::LocationUpdater_exec_i (void)
      {
      }

      LocationUpdater_exec_i::~LocationUpdater_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Attribute operations.

      // Port operations.

      void
      LocationUpdater_exec_i::push_deployment (
        ::CIAO::RACE::Deploy_Input * ev)
      ACE_THROW_SPEC ((CORBA::SystemException))
      {
        bool plan_exists = false;

        try
        {
          ACE_Auto_Ptr < ::Deployment::DeploymentPlan > dp;
          ACE_Auto_Ptr < ::Deployment::PackageConfiguration > pcd;

          if (ev->plan_uri () != 0)
          {
            ACE_DEBUG ((LM_DEBUG, "Opening the plan\n"));

            Config_Handlers::XML_File_Intf xfi (ev->plan_uri ());

            dp.reset ( xfi.get_plan ());

            plan_exists = true;
          }
          else
          {
             dp.reset ( new ::Deployment::DeploymentPlan );
          }


          pcd.reset (new ::Deployment::PackageConfiguration);

          if ((ACE_OS::strcmp (ev->package_uri (), "") != 0))
          {
            ACE_DEBUG ((LM_DEBUG, "Opening the package descriptor: :%s:\n",
                        ev->package_uri ()));

            Config_Handlers::Packaging::PCD_Handler::package_config
                  (ev->package_uri (), *pcd);
           }

           //Get the IOR of the RepoMan from the naming service

           //Obtain a ref to the orb
           CORBA::ORB_ptr orb = this->context_->_ciao_the_Container ()->the_ORB ();

           //form the path to the RepoMan IOR file
           ACE_CString RepoMan_ior = ("file://");
           RepoMan_ior += ACE_OS::getenv ("CIAO_ROOT");
           RepoMan_ior += "/DAnCE/RepositoryManager/RepositoryManagerDeamon.ior";

           cout << "RepoMan ior file: " << RepoMan_ior.c_str () << endl;

           CORBA::Object_var obj =
                  orb->string_to_object (RepoMan_ior.c_str ());//RM_ior);

           CIAO::RepositoryManagerDaemon_var rm =
                 CIAO::RepositoryManagerDaemon::_narrow (obj.in ());

           if (CORBA::is_nil (rm.in ()))
           {
             ACE_ERROR ((LM_ERROR,
                         "Unable to acquire RepositoryManagerDaemon's objref\n"));

             throw CORBA::INTERNAL ();
           }

           //now obtian the corresponding PackageConfiguration from
           //the RepositoryManager

           Deployment::PackageConfiguration_var rmpc;

           try
           {
             //retrieve the curresponding PackageConfiguration
             //from the RepoMan
             rmpc = rm->findPackageByUUID (pcd->UUID);

             ACE_DEBUG ((LM_INFO,
                        "The package was found!\nUUID: %s\n",
                         rmpc->UUID));
           }
           catch (Deployment::NoSuchName &)
           {
             ACE_ERROR ((LM_ERROR,
                 "Error! Package not found! Location update failed!\n"));

             throw CORBA::INTERNAL ();

           }
           catch (CORBA::Exception &)
           {
             ACE_ERROR ((LM_ERROR,
                 "Error! General exception! Location update failed!\n"));

             throw CORBA::INTERNAL ();
           }

          //now update the plan
          if (plan_exists)
          {
            cout << "Updating plan ..." << endl;

            PlanUpdater updater (*dp, *rmpc);
            if (!updater.Visit ())
               ACE_ERROR ((LM_ERROR,
                   "Partial location update error possible!\n"));

            cout << "Updating plan ... DONE!" << endl;

            size_t add_len = dp->artifact.length ();

            cout << "Updated location: \n";
            for (size_t i = 0; i < add_len; ++i)
            {
               Deployment::ArtifactDeploymentDescription& add = dp->artifact[i];
               if (ACE_OS::strstr (add.location[0], "http://"))
                  cout << add.location[0] << endl;
            }
          }
          //Done with the location updating

          //do not call the plan analyzer just yet
          return;

          //creat an action sequence and populate it
          ::CIAO::RACE::Plan_Actions plan_action_seq;

           /// Create a Plan_Action_seq structure of length 1.
           plan_action_seq.length (1);

           /// Now populate the Plan_Action_seq structure.
           plan_action_seq [0].command = ev->command ();
           plan_action_seq [0].plan = *dp;
           plan_action_seq [0].package = *pcd;

           /// Now invoke the plan_analyzer.
           this->context_->get_connection_ingress  ()
               ->analyze_plan (plan_action_seq);
        }
        catch (CIAO::Config_Handlers::Config_Error &ex)
        {
          ACE_ERROR ((LM_ERROR, "Config_Error exception caught in "
                      "LocationUpdater::push_deployment: %s, %s\n",
                      ex.error_.c_str (),
                      ex.name_.c_str ()));
        }
        catch (...)
        {
          ACE_ERROR ((LM_ERROR, "Exception caught in push_deployment\n"));
        }
      }

      // Operations from Components::SessionComponent

      void
      LocationUpdater_exec_i::set_session_context (
        ::Components::SessionContext_ptr ctx)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        this->context_ =
        LocationUpdater_Context::_narrow (
          ctx);

        if (this->context_ == 0)
        {
          throw CORBA::INTERNAL ();
        }
      }

      void
      LocationUpdater_exec_i::ciao_preactivate ()
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      LocationUpdater_exec_i::ciao_postactivate ()
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      LocationUpdater_exec_i::ccm_activate ()
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      LocationUpdater_exec_i::ccm_passivate ()
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      LocationUpdater_exec_i::ccm_remove ()
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      //==================================================================
      // Home Executor Implementation Class:   LocationUpdater_Home_exec_i
      //==================================================================

      LocationUpdater_Home_exec_i::LocationUpdater_Home_exec_i (void)
      {
      }

      LocationUpdater_Home_exec_i::~LocationUpdater_Home_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Home operations.

      // Factory and finder operations.

      // Attribute operations.

      // Implicit operations.

      ::Components::EnterpriseComponent_ptr
      LocationUpdater_Home_exec_i::create ()
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        ::Components::EnterpriseComponent_ptr retval =
        ::Components::EnterpriseComponent::_nil ();

        ACE_NEW_THROW_EX (
          retval,
          LocationUpdater_exec_i,
          CORBA::NO_MEMORY ());

        return retval;
      }

      extern "C" LOCATIONUPDATER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_LocationUpdater_Home_Impl (void)
      {
        ::Components::HomeExecutorBase_ptr retval =
        ::Components::HomeExecutorBase::_nil ();

        ACE_NEW_RETURN (
          retval,
          LocationUpdater_Home_exec_i,
          ::Components::HomeExecutorBase::_nil ());

        return retval;
      }
    }
  }
}
