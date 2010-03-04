// $Id$

#include "DomainApplicationManager_Impl.h"
#include "Deployment/Deployment_ConnectionC.h"
#include "DAnCE/Logger/Log_Macros.h"

using namespace DAnCE;

DomainApplicationManager_Impl::DomainApplicationManager_Impl (
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr poa,
  const Deployment::DeploymentPlan& plan,
  Node_Locator & nodes)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , poa_ (PortableServer::POA::_duplicate (poa))
  , plan_ (plan)
  , nodes_ (nodes)
{
  DANCE_TRACE( "DomainApplicationManager_Impl::DomainApplicationManager_Impl");

  this->preparePlan();

  DANCE_DEBUG (6, (LM_DEBUG, DLINFO
               ACE_TEXT("DomainApplicationManager_Impl::DomainApplicationManager_Impl - ")
               ACE_TEXT("Successfully prepared plan: %C\n"), this->plan_.UUID.in()));
}

DomainApplicationManager_Impl::~DomainApplicationManager_Impl()
{
  DANCE_TRACE( "DomainApplicationManager_Impl::~DomainApplicationManager_Impl()");

  DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                ACE_TEXT("DomainApplicationManager_Impl::~DomainApplicationManager_Impl - ")
                ACE_TEXT("Destroying %u applications\n"), this->running_app_.size()));
  while (0 < this->running_app_.size())
    {
      DomainApplication_Impl* p = this->running_app_[this->running_app_.size()-1];
      Deployment::DomainApplication_var app =
        Deployment::DomainApplication::_narrow (this->poa_->servant_to_reference (p));
      PortableServer::ObjectId_var id = this->poa_->reference_to_id (app);
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplicationManager_impl::~DomainApplicationManager_impl - ")
                   ACE_TEXT("deactivating DomainApplication object...\n")));
      this->poa_->deactivate_object (id);
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("DomainApplicationManager_impl::~DomainApplicationManager_impl - ")
                   ACE_TEXT("deleting DomainApplication.\n")));
      this->running_app_.pop_back();
      delete p;
    }

  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("DomainApplicationManager_Impl::~DomainApplicationManager_Impl - ")
                ACE_TEXT("Destroying %u managers\n"), this->sub_app_mgr_.current_size()));
  for (DomainApplication_Impl::TNam2Nm::iterator iter = this->sub_app_mgr_.begin();
       iter != this->sub_app_mgr_.end();
       ++iter)
    {
      (*iter).int_id_->destroyManager ( (*iter).ext_id_.in());
    }
  this->sub_app_mgr_.unbind_all();
}

Deployment::Application_ptr
DomainApplicationManager_Impl::startLaunch (
  const Deployment::Properties & configProperty,
  Deployment::Connections_out providedReference)
{
  DANCE_TRACE( "DomainApplicationManager_impl::startLaunch");

  ::Deployment::Connections_var connections;
  ACE_NEW_THROW_EX (connections,
                    Deployment::Connections (),
                    CORBA::NO_MEMORY ());
  DomainApplication_Impl* app = 0;
  ACE_NEW_THROW_EX (app,
                    DomainApplication_Impl (this->sub_app_mgr_,
                                            configProperty,
                                            connections.inout()),
                    CORBA::NO_MEMORY());

  providedReference = connections._retn();

  PortableServer::ObjectId_var id = this->poa_->activate_object (app);
  this->running_app_.push_back(app);

  DANCE_DEBUG (9, (LM_TRACE, DLINFO
                ACE_TEXT("DomainApplicationManager_Impl::startLaunch - ")
                ACE_TEXT("Successfully created DomainApplication\n")));
  DANCE_DEBUG (9, (LM_TRACE, DLINFO
                ACE_TEXT("DomainApplicationManager_Impl::startLaunch - ")
                ACE_TEXT("Created %u provided references\n"),
                providedReference->length ()));

  CORBA::Object_var ref = this->poa_->id_to_reference (id);
  return Deployment::DomainApplication::_narrow (ref.in ());
}

void
DomainApplicationManager_Impl::destroyApplication (
  Deployment::Application_ptr application)
{
  DANCE_TRACE ( "DomainApplicationManager_Impl::destroyApplication");

  try
    {
      for (TApplications::size_type i = 0; i < this->running_app_.size(); ++i)
        {
          DomainApplication_Impl* p = this->running_app_[i];
          Deployment::DomainApplication_var app =
            Deployment::DomainApplication::_narrow (this->poa_->servant_to_reference (p));
          if (application->_is_equivalent (app.in()))
            {
              PortableServer::ObjectId_var id = this->poa_->reference_to_id (application);
              DANCE_DEBUG (9, (LM_TRACE, DLINFO
                           ACE_TEXT("DomainApplicationManager_impl::destroyApplication - ")
                           ACE_TEXT("deactivating application object\n")));
              this->poa_->deactivate_object (id);
              DANCE_DEBUG (9, (LM_TRACE, DLINFO
                           ACE_TEXT("DomainApplicationManager_impl::destroyApplication - ")
                           ACE_TEXT("deleting application object\n")));
              delete p;
              for (TApplications::size_type j = i + 1; j < this->running_app_.size(); ++j)
                {
                  this->running_app_[j-1] = this->running_app_[j];
                }
              this->running_app_.pop_back();
              return;
            }
        }
    }
  catch (const Deployment::StopError &e)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                  ACE_TEXT("DomainApplicationManager_impl::destroyApplication - ")
                  ACE_TEXT("Propagating a received StopError exception\n")));
      throw e;
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DomainApplicationManager_impl::destroyApplication - ")
                    ACE_TEXT("Caught unknown exception\n")));
      throw Deployment::StopError();
    }

  DANCE_ERROR (1, (LM_ERROR, DLINFO
              ACE_TEXT("DomainApplicationManager_impl::destroyApplication - ")
              ACE_TEXT("Provided application reference unknown\n")));
  throw Deployment::StopError();
}

::Deployment::Applications *
DomainApplicationManager_Impl::getApplications ()
{
  DANCE_TRACE ( "DomainApplicationManager_Impl::getApplications ()");

  Deployment::Applications* running_app = 0;
  ACE_NEW_THROW_EX (running_app,
                    Deployment::Applications(),
                    CORBA::NO_MEMORY());
  running_app->length (this->running_app_.size());
  CORBA::ULong index = 0;
  for (TApplications::size_type i = 0; i < this->running_app_.size(); ++i)
    {
      CORBA::Object_var ref = this->poa_->servant_to_reference (this->running_app_[i]);
      (*running_app) [index++] =
        Deployment::DomainApplication::_narrow (ref.in ());
    }

  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("DomainApplicationManager_impl::getApplications - ")
               ACE_TEXT("Returning %u running applications\n"),
               running_app->length ()));
  return running_app;
}

::Deployment::DeploymentPlan *
DomainApplicationManager_Impl::getPlan ()
{
  DANCE_TRACE ("DomainApplicationManager_Impl::getPlan ()");

  Deployment::DeploymentPlan* plan = 0;

  ACE_NEW_THROW_EX (plan,
                    Deployment::DeploymentPlan (this->plan_),
                    CORBA::NO_MEMORY());

  return plan;
}

void
DomainApplicationManager_Impl::preparePlan()
{
  DANCE_TRACE ("DomainApplicationManager_Impl::preparePlan()");

  try
    {
      // Map of sub plans for each node
      Split_Plan::TNodePlans sub_plans;
      Split_Plan split_plan (this->plan_, sub_plans);
      split_plan.split_plan ();

      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplicationManager_Impl::preparePlan - Plan successfully split\n")));
      // Executing preparePlan on each NodeManager described in DeploymentPlan
      for (Split_Plan::TNodePlans::iterator iter_plans = sub_plans.begin();
           iter_plans != sub_plans.end();
           ++iter_plans)
        {
          // Finding correspondent NodeManager
          Deployment::NodeManager_var nm = this->nodes_.locate_node ((*iter_plans).ext_id_.c_str ());;
          // If NodeManager not found throw StartError exception
          if (CORBA::is_nil (nm.in ()))
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                            ACE_TEXT("DomainApplicationManager_Impl::preparePlan - ")
                            ACE_TEXT("Deployment::StartError exception. NodeManager %C cannot be found\n"),
                            (*iter_plans).ext_id_.c_str()));
              throw Deployment::StartError ( (*iter_plans).ext_id_.c_str(),
                                             "NodeManager not found");
            }

          // Calling preparePlan for node, specified in current sub plan
          DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplicationManager_Impl::preparePlan - ")
                       ACE_TEXT("Calling preparePlan on node %C\n"),
                       (*iter_plans).ext_id_.c_str()));

          Deployment::NodeApplicationManager_ptr nam
            = nm->preparePlan ( (*iter_plans).int_id_,
                                Deployment::ResourceCommitmentManager::_nil());

          if (CORBA::is_nil (nam))
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                            ACE_TEXT("DomainApplicationManager_Impl::preparePlan - ")
                            ACE_TEXT("PreparePlan failed for node %C, returning a nil ")
                            ACE_TEXT("NodeApplicationManager pointer.\n"),
                            (*iter_plans).ext_id_.c_str()));
              throw ::Deployment::StartError();
            }
          // We save NAM reference ptr in TNodes vector were it places to var variable
          this->sub_app_mgr_.bind (nam, nm);

          DANCE_DEBUG (8, (LM_INFO, DLINFO
                        ACE_TEXT("DomainApplicationManager_Impl::preparePlan - ")
                        ACE_TEXT("Sucessfully prepared node %C for deployment\n"),
                        (*iter_plans).ext_id_.c_str()));
        }
    }
  catch (Deployment::StartError &e)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DomainApplicationManager_Impl::preparePlan - ")
                    ACE_TEXT("Propagating StartError exception caught here\n")));
      throw e;
    }
  catch (CORBA::Exception &ex)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DomainApplicationManager_Impl::preparePlan - ")
                    ACE_TEXT("Caught a CORBA exception, propagating StartError: %C\n"),
                    ex._info ().c_str ()));
      throw ::Deployment::StartError ();
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DomainApplicationManager_Impl::preparePlan - ")
                    ACE_TEXT("Caught unknown exception.  Propagating StartError\n")));
      throw ::Deployment::StartError ();
    }
}




