// $Id$


#include "ace/streams.h"
#include "DAnCE/Logger/Log_Macros.h"

#include "NodeApplicationManager_Impl.h"
#include "NodeApplication/NodeApplication_Impl.h"
#include "DAnCE/DAnCE/DAnCE_PropertiesC.h"
#include "DAnCE/DAnCE/DAnCE_Utility.h"
#include "ace/Unbounded_Set.h"
#include "tao/ORB.h"
#include "tao/Object.h"

using namespace DAnCE;
using namespace DAnCE::Utility;

NodeApplicationManager_Impl::NodeApplicationManager_Impl (CORBA::ORB_ptr orb,
                                                          PortableServer::POA_ptr poa,
                                                          DAnCE::ArtifactInstallation_ptr installer,
                                                          const ACE_CString& node_name,
                                                          const PROPERTY_MAP &properties)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      poa_ (PortableServer::POA::_duplicate (poa)),
      installer_ (DAnCE::ArtifactInstallation::_duplicate (installer)),
      application_ (0),
      node_name_ (node_name),
      properties_ ()
{
  DANCE_TRACE ("NodeApplicationManager_Impl::NodeApplicationManager_Impl");

  DANCE_DEBUG (6, (LM_DEBUG, DLINFO
               ACE_TEXT(" NodeApplicationManager_Impl::NodeApplicationManager_Impl - ")
               ACE_TEXT("Initializing for node '%C' starting...\n"),
               node_name.c_str()));

  PROPERTY_MAP::const_iterator i = properties.begin ();
  while (!i.done ())
    {
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                    ACE_TEXT("NodeApplicationManager_Impl::NodeApplicationManager_Impl - ")
                    ACE_TEXT("Binding value for property %C\n"), i->key ().c_str ()));
      this->properties_.bind (i->key (), i->item ());
      i.advance ();
    }

  const ACE_TCHAR *ior = 0;

  if (get_property_value (DAnCE::INSTANCE_NC, this->properties_, ior) ||
      get_property_value (DAnCE::DOMAIN_NC, this->properties_, ior))
    {
      try
        {
          CORBA::Object_var obj = this->orb_->string_to_object (ior);
          this->instance_nc_ = CosNaming::NamingContext::_narrow (obj);
        }
      catch (const CORBA::Exception &e)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                        ACE_TEXT("NodeApplicationManager_Impl::NodeApplicationManager_Impl - ")
                        ACE_TEXT("Unable to resolve the instance naming context:%C\n"),
                        e._info ().c_str ()));
        }
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                    ACE_TEXT("NodeApplicationManager_Impl::NodeApplicationManager_Impl - ")
                    ACE_TEXT("Successfully resolved the instance naming context.\n")));
    }
  else DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                     ACE_TEXT("NodeApplicationManager_Impl::NodeApplicationManager_Impl - ")
                     ACE_TEXT("No instance NC was provided\n")));
}

NodeApplicationManager_Impl::~NodeApplicationManager_Impl()
{
  DANCE_TRACE ("NodeApplicationManager_Impl::~NodeApplicationManager_Impl");

  try
    {
      // initialize installer
      this->installer_->initialize ();

      // install artifacts for each sub plan and update path and libpath
      // properties for the locality manager instance of each sub plan
      // also pass the installer reference in a property
      for (LocalitySplitter::TSubPlanIterator itplan = this->split_plan_.plans ().begin ();
          itplan != this->split_plan_.plans ().end ();
          ++itplan)
        {
          // remove all artifacts for this sub plan
          this->installer_->remove_all ((*itplan).int_id_.UUID. in ());
        }

      // cleanup installer resources
      this->installer_->clear ();

      if (this->application_ != 0)
        {
          CORBA::Object_var app =
            this->poa_->servant_to_reference (this->application_);
          PortableServer::ObjectId_var id = this->poa_->reference_to_id (app);
          this->poa_->deactivate_object (id);

          delete this->application_;
          this->application_ = 0;
        }
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("NodeApplicationManager_Impl::~NodeApplicationManager_Impl - ")
                    ACE_TEXT("Caught exception in NodeApplicationManager destructor\n")));
    }
}

void
NodeApplicationManager_Impl::preparePlan (const Deployment::DeploymentPlan& plan)
{
  DANCE_TRACE ("NodeApplicationManager_Impl::preparePlan");

  DANCE_DEBUG (4, (LM_TRACE, DLINFO
                   ACE_TEXT("NodeApplicationManager_impl::preparePlan - ")
                   ACE_TEXT("Performing locality split on plan %C.\n"),
                   plan.UUID.in ()));

  this->split_plan_.split_plan (plan);

  DANCE_DEBUG (4, (LM_TRACE, DLINFO
                   ACE_TEXT("NodeApplicationManager_impl::preparePlan - ")
                   ACE_TEXT("Plan %C successfully split into %u localities.\n"),
                   plan.UUID.in (),
                   this->split_plan_.plans ().current_size ()));

  // initialize installer
  this->installer_->initialize ();

  // install artifacts for each sub plan and update path and libpath
  // properties for the locality manager instance of each sub plan
  // also pass the installer reference in a property
  for (LocalitySplitter::TSubPlanIterator itplan = this->split_plan_.plans ().begin ();
       itplan != this->split_plan_.plans ().end ();
       ++itplan)
    {
      ACE_Unbounded_Set<ACE_CString> artifact_paths;

      ::Deployment::DeploymentPlan &sub_plan = (*itplan).int_id_;
      LocalitySplitter::TSubPlanKey &sub_plan_key = (*itplan).ext_id_;

      DANCE_DEBUG (4, (LM_TRACE, DLINFO
                      ACE_TEXT("NodeApplicationManager_impl::preparePlan - ")
                      ACE_TEXT("Installing %u artifacts for locality %C.\n"),
                      sub_plan.artifact.length (),
                      sub_plan.UUID.in ()));

      // install artifacts
      for (CORBA::ULong i=0;
           i < sub_plan.artifact.length ();
           ++i)
        {
          this->installer_->install(sub_plan.UUID.in (),
                  sub_plan.artifact[i]);

          CORBA::String_var location =
            this->installer_->get_artifact_location (sub_plan.UUID.in (),
                                                     sub_plan.artifact[i].name.in ());

          // modify artifact descriptor location field to reflect the installed
          // artifact's location
          CORBA::ULong loc_last = sub_plan.artifact[i].location.length ();
          sub_plan.artifact[i].location.length (loc_last + 1);
          for (CORBA::ULong iloc = loc_last;
               iloc > 0;
               --iloc)
            {
              // shift all locations 1 position to the back to make room for the installed
              // location
              sub_plan.artifact[i].location[iloc] = sub_plan.artifact[i].location[iloc - 1];
            }
          // store installed location as first in list
          sub_plan.artifact[i].location[0] = location._retn ();

          // collect all unique paths
          ACE_CString path (sub_plan.artifact[i].location[0].in (),
                            0, false); // avoid copying
          ACE_CString::size_type pos = path.rfind (ACE_DIRECTORY_SEPARATOR_CHAR_A);
          if (pos != ACE_CString::npos)
            {
              artifact_paths.insert (path.substr (0, pos>0 ? pos : 1));
            }
        }

      // get locality manager deployment descriptors
      ::Deployment::InstanceDeploymentDescription& lm_idd =
          sub_plan.instance[sub_plan_key.locality_manager_instance ()];
      ::Deployment::MonolithicDeploymentDescription& lm_mdd =
          sub_plan.implementation[lm_idd.implementationRef];

      // update path and libpath properties of sub plan locality manager
      if (artifact_paths.size () > 0)
        {
          ACE_CString search_path;
          for (ACE_Unbounded_Set<ACE_CString>::iterator it = artifact_paths.begin ();
               it != artifact_paths.end ();
               ++it)
            {
              if (!search_path.empty ())
                search_path += ACE_TEXT_ALWAYS_CHAR (ACE_LD_SEARCH_PATH_SEPARATOR_STR);
              search_path += *it;
            }

          // update PATH
          ACE_CString new_path;
          const char* prop_val = 0;
          if (DAnCE::Utility::get_property_value (DAnCE::DANCE_LM_PATH,
                                                  lm_mdd.execParameter,
                                                  prop_val) && prop_val != 0)
            {
              new_path += prop_val;
              new_path += ACE_TEXT_ALWAYS_CHAR (ACE_LD_SEARCH_PATH_SEPARATOR_STR);
              new_path += search_path;
            }
          else
            {
              new_path.set (search_path.c_str (), false); // avoid copying
            }
          DAnCE::Utility::update_property_value (DAnCE::DANCE_LM_PATH,
                                                 lm_mdd.execParameter,
                                                 new_path.c_str ());

          // On windows these two search paths are the same
#if !defined (ACE_WIN32)
          // update LIBPATH
          new_path.clear ();
          prop_val = 0;
          if (DAnCE::Utility::get_property_value (DAnCE::DANCE_LM_LIBPATH,
                                                  lm_mdd.execParameter,
                                                  prop_val) && prop_val != 0)
            {
              new_path += prop_val;
              new_path += ACE_TEXT_ALWAYS_CHAR (ACE_LD_SEARCH_PATH_SEPARATOR_STR);
              new_path += search_path;
            }
          else
            {
              new_path.set (search_path.c_str (), false); // avoid copying
            }
          DAnCE::Utility::update_property_value (DAnCE::DANCE_LM_LIBPATH,
                                                 lm_mdd.execParameter,
                                                 new_path.c_str ());
        }
#endif
      // Need to add naming service reference to config properties.
      DAnCE::Utility::update_property_value<CosNaming::NamingContext_ptr> (
            DAnCE::LOCALITY_NAMINGCONTEXT,
            lm_idd.configProperty,
            this->instance_nc_. in ());

      // pass installer reference as an execParameter property
      DAnCE::Utility::update_property_value<DAnCE::ArtifactInstallation_ptr> (
            DAnCE::DANCE_LM_INSTALLER,
            lm_mdd.execParameter,
            this->installer_.in ());
    }

  // cleanup installer resources
  // (does *not* cleanup install registry; just signals end of install actions)
  this->installer_->clear ();

  DANCE_DEBUG (4, (LM_TRACE, DLINFO
                   ACE_TEXT("NodeApplicationManager_impl::preparePlan - ")
                   ACE_TEXT("Finished preparing plan %C.\n"),
                   plan.UUID.in ()));
}

Deployment::Application_ptr
NodeApplicationManager_Impl::startLaunch (const Deployment::Properties &prop,
                                          Deployment::Connections_out providedReference)
{
  DANCE_TRACE ("NodeApplicationManager_Impl::startLaunch");

  // Creating NodeApplication object
  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplicationManager_impl::startLaunch - ")
               ACE_TEXT("Initializing NodeApplication\n")));
  ACE_NEW_THROW_EX (this->application_,
                    NodeApplication_Impl (this->orb_.in(),
                                          this->poa_.in(),
                                          this->installer_.in (),
                                          this->node_name_,
                                          this->properties_),
                    CORBA::NO_MEMORY ());

  DANCE_DEBUG (9, (LM_TRACE, DLINFO
                   ACE_TEXT("NodeApplicationManager_impl::startLaunch - ")
                   ACE_TEXT("Instructing NodeApplication to prepare locality managers.\n")));

  this->application_->prepare_instances (this->split_plan_.plans ());

  DANCE_DEBUG (9, (LM_TRACE, DLINFO
                   ACE_TEXT("NodeApplicationManager_impl::startLaunch - ")
                   ACE_TEXT("Instructing NodeApplication to start launch localities.\n")));

  this->application_->start_launch_instances (prop, providedReference);

  DANCE_DEBUG (6, (LM_DEBUG, DLINFO
               ACE_TEXT("NodeApplicationManager_impl::startLaunch - ")
               ACE_TEXT("Activating NodeApplication servant\n")));

  PortableServer::ObjectId_var as_id =
    this->poa_->activate_object (this->application_);

  CORBA::Object_var as_obj = this->poa_->id_to_reference (as_id.in ());
  Deployment::Application_var app = Deployment::Application::_narrow (as_obj.in ());

  if (CORBA::is_nil (app))
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO,
                       "NodeApplicationManager_Impl::startLaunch - ",
                       "NodeApplication servant failed to activate\n"));
      throw ::Deployment::StartError ("NodeApplication",
                                      "Activation failure");
    }
  return app._retn ();
}

void
NodeApplicationManager_Impl::destroyApplication (Deployment::Application_ptr application)
{
  DANCE_TRACE ("NodeApplicationManager_Impl::destroyApplication");

  try
  {
    if (!application->_is_equivalent (this->poa_->servant_to_reference (this->application_)))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                     ACE_TEXT("NodeApplicationManager_Impl::destroyApplication - ")
                     ACE_TEXT("application is equivalent to current application\n")));
        throw ::Deployment::StopError("NodeApplicationManager",
                                      "Wrong application passed to destroyApplication");
      }

    this->application_->remove_instances ();

    PortableServer::ObjectId_var id = this->poa_->reference_to_id (application);
    this->poa_->deactivate_object (id);

    delete this->application_;
    this->application_ = 0;
  }
  catch (const CORBA::SystemException &)
    {
      throw;
    }
  catch (const Deployment::StopError &)
    {
      throw;
    }
  catch (const CORBA::UserException &e)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                   ACE_TEXT("NodeApplicationManager_Impl::destroyApplication failed with UserException %C(%C) \"%C\"\n"),
                   e._name(), e._rep_id(), e._info().c_str()));
      throw Deployment::StopError(e._name(), e._info().c_str());
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                       ACE_TEXT("NodeApplicationManager_Impl::destroyApplication failed with unknown exception.\n")));
      throw Deployment::StopError("NodeApplicatoinManager", "Unknown C++ exception in destroyApplication");
    }
}

