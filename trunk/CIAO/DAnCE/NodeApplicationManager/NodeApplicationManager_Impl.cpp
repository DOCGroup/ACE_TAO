// $Id$


#include "ace/streams.h"
#include "DAnCE/Logger/Log_Macros.h"

#include "NodeApplicationManager_Impl.h"
#include "NodeApplication/NodeApplication_Impl.h"

#include "DAnCE/DAnCE/DAnCE_PropertiesC.h"

using namespace DAnCE;

NodeApplicationManager_Impl::NodeApplicationManager_Impl (CORBA::ORB_ptr orb,
                                                          PortableServer::POA_ptr poa,
                                                          const ACE_CString& node_name,
                                                          const PROPERTY_MAP &properties)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      poa_ (PortableServer::POA::_duplicate (poa)),
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
}

NodeApplicationManager_Impl::~NodeApplicationManager_Impl()
{
  DANCE_TRACE ("NodeApplicationManager_Impl::~NodeApplicationManager_Impl");

  try
    {
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

  DANCE_DEBUG (9, (LM_TRACE, DLINFO
                   ACE_TEXT("NodeApplicationManager_impl::preparePlan - ")
                   ACE_TEXT("Performing locality split on plan %C.\n"),
                   plan.UUID.in ()));

  this->split_plan_.split_plan (plan);

  DANCE_DEBUG (9, (LM_TRACE, DLINFO
                   ACE_TEXT("NodeApplicationManager_impl::preparePlan - ")
                   ACE_TEXT("Plan %C successfully split into %u localities.\n"),
                   plan.UUID.in (),
                   this->split_plan_.plans ().current_size ()));
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

