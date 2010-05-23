// $Id$

#include "DomainApplicationManager_Impl.h"
#include "Deployment/Deployment_ConnectionC.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "Split_Plan/Split_Plan.h"
#include "Split_Plan/Node_Splitter.h"

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
}

DomainApplicationManager_Impl::~DomainApplicationManager_Impl()
{
  DANCE_TRACE( "DomainApplicationManager_Impl::~DomainApplicationManager_Impl()");

  DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                ACE_TEXT("DomainApplicationManager_Impl::~DomainApplicationManager_Impl - ")
                ACE_TEXT("Deleting %u applications for plan %C\n"),
                this->running_app_.size(),
                this->getPlanUUID ()));
  while (0 < this->running_app_.size())
    {
      DomainApplication_Impl* p = this->running_app_[this->running_app_.size()-1];
      Deployment::DomainApplication_var app =
        Deployment::DomainApplication::_narrow (this->poa_->servant_to_reference (p));
      PortableServer::ObjectId_var id = this->poa_->reference_to_id (app);
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplicationManager_impl::~DomainApplicationManager_impl - ")
                   ACE_TEXT("deactivating DomainApplication[%@] object...\n"),
                   p));
      this->poa_->deactivate_object (id);
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("DomainApplicationManager_impl::~DomainApplicationManager_impl - ")
                   ACE_TEXT("deleting DomainApplication.\n")));
      this->running_app_.pop_back();
      delete p;
    }

  this->sub_app_mgr_.unbind_all();
  this->node_ids_.unbind_all ();
}

void
DomainApplicationManager_Impl::startLaunch (
    ::Deployment::AMH_ApplicationManagerResponseHandler_ptr _tao_rh,
    const ::Deployment::Properties & configProperty)
{
  DANCE_TRACE( "DomainApplicationManager_impl::startLaunch");

  try
    {
      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                    ACE_TEXT("DomainApplicationManager_Impl::startLaunch - ")
                    ACE_TEXT("Creating DomainApplication for plan %C\n"),
                    this->getPlanUUID ()));

      DomainApplication_Impl* app = 0;
      ACE_NEW_THROW_EX (app,
                        DomainApplication_Impl (this->getPlanUUID (),
                                                this->poa_.in (),
                                                this->sub_app_mgr_,
                                                this->node_ids_),
                        CORBA::NO_MEMORY());

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                    ACE_TEXT("DomainApplicationManager_Impl::startLaunch - ")
                    ACE_TEXT("Successfully created DomainApplication for plan %C\n"),
                    this->getPlanUUID ()));

      StartLaunchCompletionHandler* slch = 0;
      ACE_NEW_THROW_EX (slch,
                        StartLaunchCompletionHandler (this,
                                                      _tao_rh),
                        CORBA::NO_MEMORY ());

      app->startLaunch (configProperty, slch);

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                    ACE_TEXT("DomainApplicationManager_Impl::startLaunch - ")
                    ACE_TEXT("DomainApplication startLaunch has been called\n")));
    }
  catch (CORBA::Exception& ex)
    {
      CORBA::Exception* local_ex = ex._tao_duplicate ();
      ::Deployment::AMH_ApplicationManagerExceptionHolder amh_exholder (local_ex);
      _tao_rh->startLaunch_excep (&amh_exholder);
    }
  catch (...)
    {
      CORBA::Exception* unknown_ex = new CORBA::UNKNOWN;
      ::Deployment::AMH_ApplicationManagerExceptionHolder amh_exholder (unknown_ex);
      _tao_rh->startLaunch_excep (&amh_exholder);
    }
}

void
DomainApplicationManager_Impl::finish_startLaunch (
  ::Deployment::AMH_ApplicationManagerResponseHandler_ptr _tao_rh,
  DomainApplication_Impl *da_servant,
  const ::Deployment::Connections & providedReference)
{
  DANCE_DEBUG (6, (LM_NOTICE, DLINFO
                   ACE_TEXT("DomainApplicationManager_Impl::finish_startLaunch - ")
                   ACE_TEXT("DomainApplication[%u] for plan %C created %u provided references.\n"),
                   this->running_app_.size (),
                   this->getPlanUUID (),
                   providedReference.length ()));

  PortableServer::ObjectId_var id = this->poa_->activate_object (da_servant);
  this->running_app_.push_back(da_servant);

  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplicationManager_Impl::finish_startLaunch - ")
              ACE_TEXT("DomainApplication was successfully activated.\n")));

  CORBA::Object_var ref = this->poa_->id_to_reference (id.in());
  _tao_rh->startLaunch (Deployment::DomainApplication::_narrow (ref.in ()), providedReference);
}

void
DomainApplicationManager_Impl::fail_startLaunch (
  ::Deployment::AMH_ApplicationManagerResponseHandler_ptr _tao_rh,
  CORBA::Exception* local_ex)
{
  DANCE_DEBUG (6, (LM_NOTICE, DLINFO
                   ACE_TEXT("DomainApplicationManager_Impl::fail_startLaunch for plan %C called\n"),
                   this->getPlanUUID ()));

  ::Deployment::AMH_ApplicationManagerExceptionHolder amh_exholder (local_ex);
  _tao_rh->startLaunch_excep (&amh_exholder);
}

void
DomainApplicationManager_Impl::destroyApplication (
    ::Deployment::AMH_ApplicationManagerResponseHandler_ptr _tao_rh,
    ::Deployment::Application_ptr application)
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
              DANCE_DEBUG (6, (LM_TRACE, DLINFO
                           ACE_TEXT("DomainApplicationManager_impl::destroyApplication - ")
                           ACE_TEXT("destroying DomainApplication[%u] for plan %C\n"),
                           i,
                           this->getPlanUUID ()));

              // create completion handler
              DestroyApplicationCompletionHandler* dach_ptr = 0;
              ACE_NEW_THROW_EX (dach_ptr,
                                DestroyApplicationCompletionHandler (this,
                                                                     _tao_rh,
                                                                     p),
                                CORBA::NO_MEMORY ());
              DA_CompletionHandler_AutoPtr dach (dach_ptr);

              // deactivate servant
              PortableServer::ObjectId_var id = this->poa_->reference_to_id (application);
              DANCE_DEBUG (9, (LM_TRACE, DLINFO
                           ACE_TEXT("DomainApplicationManager_impl::destroyApplication - ")
                           ACE_TEXT("deactivating application object\n")));
              this->poa_->deactivate_object (id);
              // remove registration
              for (TApplications::size_type j = i + 1; j < this->running_app_.size(); ++j)
                {
                  this->running_app_[j-1] = this->running_app_[j];
                }
              this->running_app_.pop_back();

              DANCE_DEBUG (9, (LM_TRACE, DLINFO
                           ACE_TEXT("DomainApplicationManager_impl::destroyApplication - ")
                           ACE_TEXT("deleting application object\n")));

              // remove default ref so servant will be deleted when
              // var in DestroyApplicationCompletionHandler gets destructed
              // at the finish of DomainApplication_Impl::destroyApplication
              p->_remove_ref ();

              // destruct
              p->destroyApplication (dach);

              return;
            }
        }
    }
  catch (CORBA::Exception& ex)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                  ACE_TEXT("DomainApplicationManager_impl::destroyApplication - ")
                  ACE_TEXT("Propagating StopError for CORBA exception caught here: %C\n"),
                  ex._info ().c_str ()));
      CORBA::Exception* local_ex = new Deployment::StopError();
      ::Deployment::AMH_ApplicationManagerExceptionHolder amh_exholder (local_ex);
      _tao_rh->destroyApplication_excep (&amh_exholder);
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                  ACE_TEXT("DomainApplicationManager_impl::destroyApplication - ")
                  ACE_TEXT("Propagating StopError for unknown exception caught here\n")));
      CORBA::Exception* stop_ex = new Deployment::StopError();
      ::Deployment::AMH_ApplicationManagerExceptionHolder amh_exholder (stop_ex);
      _tao_rh->destroyApplication_excep (&amh_exholder);
    }

  DANCE_ERROR (1, (LM_ERROR, DLINFO
              ACE_TEXT("DomainApplicationManager_impl::destroyApplication - ")
              ACE_TEXT("Provided application reference unknown\n")));
  CORBA::Exception* stop_ex = new Deployment::StopError();
  ::Deployment::AMH_ApplicationManagerExceptionHolder amh_exholder (stop_ex);
  _tao_rh->destroyApplication_excep (&amh_exholder);
}

void
DomainApplicationManager_Impl::getApplications (
    ::Deployment::AMH_DomainApplicationManagerResponseHandler_ptr _tao_rh)
{
  DANCE_TRACE ( "DomainApplicationManager_Impl::getApplications ()");

  try
    {
      Deployment::Applications* running_app_ptr = 0;
      ACE_NEW_THROW_EX (running_app_ptr,
                        Deployment::Applications(),
                        CORBA::NO_MEMORY());
      Deployment::Applications_var running_app (running_app_ptr);
      running_app->length (this->running_app_.size());
      CORBA::ULong index = 0;
      for (TApplications::size_type i = 0; i < this->running_app_.size(); ++i)
        {
          CORBA::Object_var ref = this->poa_->servant_to_reference (this->running_app_[i]);
          (*running_app) [index++] =
            Deployment::DomainApplication::_narrow (ref.in ());
        }

      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("DomainApplicationManager_impl::getApplications - ")
                  ACE_TEXT("Returning %u running applications for plan %C\n"),
                  running_app->length (),
                  this->getPlanUUID ()));
      _tao_rh->getApplications (running_app);
      return;
    }
  catch (CORBA::Exception& ex)
    {
      CORBA::Exception* local_ex = ex._tao_duplicate ();
      ::Deployment::AMH_DomainApplicationManagerExceptionHolder amh_exholder (local_ex);
      _tao_rh->getApplications_excep (&amh_exholder);
    }
  catch (...)
    {
      CORBA::Exception* unknown_ex = new CORBA::UNKNOWN;
      ::Deployment::AMH_DomainApplicationManagerExceptionHolder amh_exholder (unknown_ex);
      _tao_rh->getApplications_excep (&amh_exholder);
    }
}

void
DomainApplicationManager_Impl::getPlan (
    ::Deployment::AMH_DomainApplicationManagerResponseHandler_ptr _tao_rh)
{
  DANCE_TRACE ("DomainApplicationManager_Impl::getPlan ()");

  try
    {
      Deployment::DeploymentPlan* plan_ptr = 0;
      ACE_NEW_THROW_EX (plan_ptr,
                        Deployment::DeploymentPlan (this->plan_),
                        CORBA::NO_MEMORY());
      Deployment::DeploymentPlan_var plan (plan_ptr);

      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("DomainApplicationManager_impl::getApplications - ")
                  ACE_TEXT("Returning plan %C\n"),
                  this->getPlanUUID ()));

      _tao_rh->getPlan (plan);
      return;
    }
  catch (CORBA::Exception& ex)
    {
      CORBA::Exception* local_ex = ex._tao_duplicate ();
      ::Deployment::AMH_DomainApplicationManagerExceptionHolder amh_exholder (local_ex);
      _tao_rh->getApplications_excep (&amh_exholder);
    }
  catch (...)
    {
      CORBA::Exception* unknown_ex = new CORBA::UNKNOWN;
      ::Deployment::AMH_DomainApplicationManagerExceptionHolder amh_exholder (unknown_ex);
      _tao_rh->getApplications_excep (&amh_exholder);
    }
}

const char*
DomainApplicationManager_Impl::getPlanUUID (void)
{
  return this->plan_.UUID.in ();
}

void
DomainApplicationManager_Impl::preparePlan(DAM_CompletionHandler* completion_handler)
{
  DANCE_TRACE ("DomainApplicationManager_Impl::preparePlan()");

  DAM_CompletionHandler_AutoPtr _ch_ptr (completion_handler); // manage handler

  try
    {
      // Map of sub plans for each node
      Split_Plan<Node_Splitter> split_plan;
      split_plan.split_plan (this->plan_);
      Split_Plan<Node_Splitter>::TSubPlans &sub_plans = split_plan.plans ();

      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplicationManager_Impl::preparePlan - ")
                                        ACE_TEXT("Plan %C successfully split. %u nodes to prepare.\n"),
                                        this->getPlanUUID (),
                                        sub_plans.current_size ()));

      // create completion counter
      DAM_NM_ReplyHandlerImpl::Counter* _cp = 0;
      ACE_NEW_THROW_EX (_cp,
                        DAM_NM_ReplyHandlerImpl::Counter (sub_plans.current_size (),
                                                          this,
                                                          _ch_ptr),
                        CORBA::NO_MEMORY ());
      DAM_NM_ReplyHandlerImpl::Counter_AutoPtr _counter_ptr (_cp);

      // Executing preparePlan on each NodeManager described in DeploymentPlan
      for (Split_Plan<Node_Splitter>::TSubPlanIterator iter_plans = sub_plans.begin();
           iter_plans != sub_plans.end();
           ++iter_plans)
        {
          try
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

              // create and activate reply handler
              DAM_NM_ReplyHandlerImpl* dam_nm_rh_servant = 0;
              ACE_NEW_THROW_EX (dam_nm_rh_servant,
                                DAnCE::DAM_NM_ReplyHandlerImpl (this,
                                                                nm.in (),
                                                                (*iter_plans).ext_id_.c_str(),
                                                                _counter_ptr),
                                CORBA::NO_MEMORY ());

              PortableServer::ObjectId_var id = this->poa_->activate_object (dam_nm_rh_servant);

              CORBA::Object_var ref = this->poa_->id_to_reference (id.in());

              ::Deployment::AMI_NodeManagerHandler_var dam_nm_handler =
                  ::Deployment::AMI_NodeManagerHandler::_narrow (ref.in ());

              // Calling preparePlan for node, specified in current sub plan
              DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplicationManager_Impl::preparePlan - ")
                          ACE_TEXT("Calling preparePlan on node %C\n"),
                          (*iter_plans).ext_id_.c_str()));

              nm->sendc_preparePlan (dam_nm_handler.in (),
                                    (*iter_plans).int_id_,
                                    Deployment::ResourceCommitmentManager::_nil());

              DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplicationManager_Impl::preparePlan - ")
                          ACE_TEXT("preparePlan on node %C has been called\n"),
                          (*iter_plans).ext_id_.c_str()));
            }
          catch (CORBA::Exception &ex)
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                            ACE_TEXT("DomainApplicationManager_Impl::preparePlan - ")
                            ACE_TEXT("Caught a CORBA exception handling node %C : %C\n"),
                            (*iter_plans).ext_id_.c_str(),
                            ex._info ().c_str ()));
              // mark failure
              _counter_ptr->increment_fail_count ();
              // mark off node
              _counter_ptr->decrement_exec_count ();
              // continue for next node
            }
          catch (...)
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                            ACE_TEXT("DomainApplicationManager_Impl::preparePlan - ")
                            ACE_TEXT("Caught unknown exception handling node %C\n"),
                            (*iter_plans).ext_id_.c_str()));
              // mark failure
              _counter_ptr->increment_fail_count ();
              // mark off node
              _counter_ptr->decrement_exec_count ();
              // continue for next node
            }
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
      throw ::Deployment::StartError (this->plan_.UUID.in (),
                                      ex._info ().c_str ());
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DomainApplicationManager_Impl::preparePlan - ")
                    ACE_TEXT("Caught unknown exception.  Propagating StartError\n")));
      throw ::Deployment::StartError (this->plan_.UUID.in (),
				      "Unknown C++ exception");
    }
  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplicationManager_Impl::preparePlan - finished\n")));
}

void
DomainApplicationManager_Impl::destroyManager (DAM_CompletionHandler* completion_handler)
{
  DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                ACE_TEXT("DomainApplicationManager_Impl::destroyManager - ")
                ACE_TEXT("Destroying %u applications for plan %C\n"),
                this->running_app_.size (),
                this->getPlanUUID ()));

  DAM_CompletionHandler_AutoPtr _ch_ptr (completion_handler); // manage handler

  try
    {
      if (this->running_app_.size() > 0)
        {
          // create our completionhandler
          DestroyManagerCompletionHandler* dmch = 0;
          ACE_NEW_THROW_EX (dmch,
                            DestroyManagerCompletionHandler (this,
                                                             _ch_ptr,
                                                             running_app_.size ()),
                            CORBA::NO_MEMORY ());
          DA_CompletionHandler_AutoPtr da_ch_ptr (dmch);

          while (0 < this->running_app_.size())
            {
              DomainApplication_Impl* p = this->running_app_[this->running_app_.size()-1];
              try
                {
                  Deployment::DomainApplication_var app =
                    Deployment::DomainApplication::_narrow (this->poa_->servant_to_reference (p));
                  PortableServer::ObjectId_var id = this->poa_->reference_to_id (app);
                  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplicationManager_impl::destroyManager - ")
                              ACE_TEXT("deactivating DomainApplication[%@] object...\n"),
                              p));
                  this->poa_->deactivate_object (id);
                  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("DomainApplicationManager_impl::destroyManager - ")
                              ACE_TEXT("calling destroyApplication for DomainApplication[%@].\n"),
                              p));
                  p->destroyApplication (da_ch_ptr);
                }
              catch (CORBA::Exception &ex)
                {
                  DANCE_ERROR (1, (LM_ERROR, DLINFO
                                ACE_TEXT("DomainApplicationManager_Impl::destroyManager - ")
                                ACE_TEXT("Caught a CORBA exception for DomainApplication[%@]: %C\n"),
                                p,
                                ex._info ().c_str ()));
                  // mark failure
                  dmch->increment_fail_count ();
                  // mark off app
                  dmch->decrement_exec_count ();
                  // continue
                }
              catch (...)
                {
                  DANCE_ERROR (1, (LM_ERROR, DLINFO
                                ACE_TEXT("DomainApplicationManager_Impl::destroyManager - ")
                                ACE_TEXT("Caught unknown exception for DomainApplication[%@].\n"),
                                p));
                  // mark failure
                  dmch->increment_fail_count ();
                  // mark off app
                  dmch->decrement_exec_count ();
                  // continue
                }
              this->running_app_.pop_back();
            }
        }
      else
        {
          // nothing to destroy, complete
          _ch_ptr->handle_completion (this);
        }
    }
  catch (Deployment::StopError &e)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DomainApplicationManager_Impl::destroyManager - ")
                    ACE_TEXT("Propagating StopError exception caught here\n")));
      throw e;
    }
  catch (CORBA::Exception &ex)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DomainApplicationManager_Impl::destroyManager - ")
                    ACE_TEXT("Caught a CORBA exception, propagating StopError: %C\n"),
                    ex._info ().c_str ()));
      throw ::Deployment::StopError (this->plan_.UUID.in (),
                                     ex._info ().c_str ());
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DomainApplicationManager_Impl::destroyManager - ")
                    ACE_TEXT("Caught unknown exception.  Propagating StopError\n")));
      throw ::Deployment::StopError (this->plan_.UUID.in (),
                                     "Unknown C++ exception");
    }
  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplicationManager_Impl::destroyManager - finished\n")));
}

void
DomainApplicationManager_Impl::finishDestroyManager (const DAM_CompletionHandler_AutoPtr& ch_ptr)
{
  DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                ACE_TEXT("DomainApplicationManager_Impl::finishDestroyManager - ")
                ACE_TEXT("Destroying %u node managers for plan %C\n"),
                this->sub_app_mgr_.current_size (),
                this->getPlanUUID ()));

  try
    {
      // create counters
      DAM_NM_ReplyHandlerImpl::Counter* _cp = 0;
      ACE_NEW_THROW_EX (_cp,
                        DAM_NM_ReplyHandlerImpl::Counter (sub_app_mgr_.current_size (),
                                                          this,
                                                          ch_ptr),
                        CORBA::NO_MEMORY ());
      DAM_NM_ReplyHandlerImpl::Counter_AutoPtr _counter_ptr (_cp);

      for (DomainApplication_Impl::TNam2Nm::iterator iter = this->sub_app_mgr_.begin();
          iter != this->sub_app_mgr_.end();
          ++iter)
        {
          ACE_CString node_id;
          this->node_ids_.find ((*iter).int_id_, node_id);

          DAM_NM_ReplyHandlerImpl* dam_nm_rh_servant = 0;
          try
            {
              // create and activate reply handler
              ACE_NEW_THROW_EX (dam_nm_rh_servant,
                                DAnCE::DAM_NM_ReplyHandlerImpl (this,
                                                                (*iter).int_id_.in (),
                                                                node_id.c_str (),
                                                                _counter_ptr),
                                CORBA::NO_MEMORY ());

              PortableServer::ObjectId_var id = this->poa_->activate_object (dam_nm_rh_servant);

              CORBA::Object_var ref = this->poa_->id_to_reference (id.in());

              ::Deployment::AMI_NodeManagerHandler_var dam_nm_handler =
                  ::Deployment::AMI_NodeManagerHandler::_narrow (ref.in ());

              // Calling destroyManager for node
              DANCE_DEBUG (6, (LM_TRACE, DLINFO ACE_TEXT("DomainApplicationManager_Impl::finishDestroyManager - ")
                          ACE_TEXT("Calling destroyManager on node manager for node %C\n"),
                          node_id.c_str ()));

              (*iter).int_id_->sendc_destroyManager (dam_nm_handler.in (),
                                                    (*iter).ext_id_.in());
            }
            catch (CORBA::Exception &ex)
              {
                DANCE_ERROR (1, (LM_ERROR, DLINFO
                              ACE_TEXT("DomainApplicationManager_Impl::finishDestroyManager - ")
                              ACE_TEXT("Caught a CORBA exception attempting to call destroyManager on node %C: %C\n"),
                              node_id.c_str (),
                              ex._info ().c_str ()));
                // mark failure
                _counter_ptr->increment_fail_count ();
                // mark of node
                _counter_ptr->decrement_exec_count ();
                // continue to next node
              }
            catch (...)
              {
                DANCE_ERROR (1, (LM_ERROR, DLINFO
                              ACE_TEXT("DomainApplicationManager_Impl::finishDestroyManager - ")
                              ACE_TEXT("Caught unknown exception attempting to call destroyManager on node %C\n"),
                              node_id.c_str ()));
                // mark failure
                _counter_ptr->increment_fail_count ();
                // mark of node
                _counter_ptr->decrement_exec_count ();
                // continue to next node
              }

          DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplicationManager_Impl::finishDestroyManager - ")
                      ACE_TEXT("destroyManager has been called on node manager\n")));
        }
    }
  catch (Deployment::StopError &e)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DomainApplicationManager_Impl::finishDestroyManager - ")
                    ACE_TEXT("Propagating StopError exception caught here\n")));
      CORBA::Exception* local_ex = e._tao_duplicate ();
      ch_ptr->handle_exception (local_ex);
    }
  catch (CORBA::Exception &ex)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DomainApplicationManager_Impl::finishDestroyManager - ")
                    ACE_TEXT("Caught a CORBA exception, propagating StopError: %C\n"),
                    ex._info ().c_str ()));
      ch_ptr->handle_exception (new ::Deployment::StopError ());
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DomainApplicationManager_Impl::finishDestroyManager - ")
                    ACE_TEXT("Caught unknown exception.  Propagating StopError\n")));
      ch_ptr->handle_exception (new ::Deployment::StopError ());
    }
  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplicationManager_Impl::finishDestroyManager - finished\n")));
}

DAM_NM_ReplyHandlerImpl::Counter::Counter (
    unsigned int exec_count,
    DomainApplicationManager_Impl* dam_servant,
    const DAM_CompletionHandler_AutoPtr& em_completion_handler)
    : Completion_Counter_Base<ACE_Null_Mutex> (exec_count, 0),
      dam_servant_ (DomainApplicationManager_Impl_var::_duplicate (dam_servant)),
      em_ch_ptr_ (em_completion_handler)
{
}

void
DAM_NM_ReplyHandlerImpl::Counter::on_all_completed ()
{
  DANCE_DEBUG (6, (LM_DEBUG, DLINFO
              ACE_TEXT("DAM_NM_ReplyHandlerImpl::Counter::on_all_completed - ")
              ACE_TEXT("Successfully prepared plan: %C\n"),
              this->dam_servant_->getPlanUUID ()));

  this->em_ch_ptr_->handle_completion (dam_servant_.in ());
}

void
DAM_NM_ReplyHandlerImpl::Counter::on_all_completed_with_failure ()
{
  DANCE_ERROR (1, (LM_ERROR, DLINFO
              ACE_TEXT("DAM_NM_ReplyHandlerImpl::Counter::on_all_completed_with_failure - ")
              ACE_TEXT("%u errors preparing plan: %C\n"),
              this->fail_count (),
              this->dam_servant_->getPlanUUID ()));

  this->em_ch_ptr_->handle_exception (new ::Deployment::StartError());
}

DAM_NM_ReplyHandlerImpl::DAM_NM_ReplyHandlerImpl (
    DomainApplicationManager_Impl* dam_servant,
    ::Deployment::NodeManager_ptr nm,
    const char* node_id,
    const Counter_AutoPtr& counter)
    : dam_servant_ (DomainApplicationManager_Impl_var::_duplicate (dam_servant)),
      nm_ (::Deployment::NodeManager::_duplicate (nm)),
      node_id_ (node_id),
      counter_ (counter)
{
}

DAM_NM_ReplyHandlerImpl::~DAM_NM_ReplyHandlerImpl ()
{
}

void DAM_NM_ReplyHandlerImpl::joinDomain (void)
{
}
void DAM_NM_ReplyHandlerImpl::joinDomain_excep (
  ::Messaging::ExceptionHolder * /*excep_holder*/)
{
}

void DAM_NM_ReplyHandlerImpl::leaveDomain (void)
{
}
void DAM_NM_ReplyHandlerImpl::leaveDomain_excep (
  ::Messaging::ExceptionHolder * /*excep_holder*/)
{
}

void DAM_NM_ReplyHandlerImpl::preparePlan (
  ::Deployment::NodeApplicationManager_ptr nam)
{
  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DAM_NM_ReplyHandlerImpl::preparePlan - ")
                ACE_TEXT("Finished preparePlan on node %C\n"),
                this->node_id_.c_str()));

  if (CORBA::is_nil (nam))
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DAM_NM_ReplyHandlerImpl::preparePlan - ")
                    ACE_TEXT("PreparePlan failed for node %C, returning a nil ")
                    ACE_TEXT("NodeApplicationManager pointer.\n"),
                    this->node_id_.c_str()));
      // add failure
      this->counter_->increment_fail_count ();
    }
  else
    {
      DANCE_DEBUG (8, (LM_INFO, DLINFO
                    ACE_TEXT("DAM_NM_ReplyHandlerImpl::preparePlan - ")
                    ACE_TEXT("Sucessfully prepared node %C for deployment\n"),
                    this->node_id_.c_str()));

      // We save NAM reference ptr in TNodes vector were it places to var variable
      this->dam_servant_->sub_app_mgr_.bind (::Deployment::NodeApplicationManager::_duplicate (nam),
                                              ::Deployment::NodeManager::_duplicate (this->nm_.in ()));
      // nm to node Id table
      this->dam_servant_->node_ids_.bind (::Deployment::NodeManager::_duplicate (this->nm_.in ()),
                                          this->node_id_);

    }

  // mark off node
  this->counter_->decrement_exec_count ();

  // deactivate AMI response handler
  PortableServer::ObjectId_var oid = this->dam_servant_->poa_->servant_to_id (this);
  this->dam_servant_->poa_->deactivate_object (oid.in ());
}

void DAM_NM_ReplyHandlerImpl::preparePlan_excep (
  ::Messaging::ExceptionHolder * excep_holder)
{
  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DAM_NM_ReplyHandlerImpl::preparePlan_excep - ")
                ACE_TEXT("Finished preparePlan on node %C\n"),
                this->node_id_.c_str()));

  try
    {
      excep_holder->raise_exception ();
    }
  catch (Deployment::StartError &)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DAM_NM_ReplyHandlerImpl::preparePlan_excep - ")
                    ACE_TEXT("StartError exception caught for node %C.\n"),
                    this->node_id_.c_str()));
    }
  catch (CORBA::Exception &ex)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DAM_NM_ReplyHandlerImpl::preparePlan_excep - ")
                    ACE_TEXT("Caught a CORBA exception for node %C: %C\n"),
                    this->node_id_.c_str(),
                    ex._info ().c_str ()));
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DAM_NM_ReplyHandlerImpl::preparePlan_excep - ")
                    ACE_TEXT("Caught unknown exception for node %C.\n"),
                    this->node_id_.c_str()));
    }

  // add failure
  this->counter_->increment_fail_count ();

  // mark off node
  this->counter_->decrement_exec_count ();

  // deactivate AMI response handler
  PortableServer::ObjectId_var oid = this->dam_servant_->poa_->servant_to_id (this);
  this->dam_servant_->poa_->deactivate_object (oid.in ());
}

void DAM_NM_ReplyHandlerImpl::destroyManager (void)
{
  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DAM_NM_ReplyHandlerImpl::destroyApplication called.\n")));

  DANCE_DEBUG (9, (LM_TRACE, DLINFO
                ACE_TEXT("DAM_NM_ReplyHandlerImpl::destroyApplication - ")
                ACE_TEXT("Plan %C node %C node application manager destroy succeeded\n"),
                this->dam_servant_->getPlanUUID (),
                this->node_id_.c_str ()));

  // mark off one node
  this->counter_->decrement_exec_count ();

  // deactivate AMI response handler
  PortableServer::ObjectId_var oid = this->dam_servant_->poa_->servant_to_id (this);
  this->dam_servant_->poa_->deactivate_object (oid.in ());
}
void DAM_NM_ReplyHandlerImpl::destroyManager_excep (
  ::Messaging::ExceptionHolder * excep_holder)
{
  DANCE_DEBUG (9, (LM_TRACE, DLINFO
                   ACE_TEXT("DAM_NM_ReplyHandlerImpl::destroyApplication_excep called for plan %C node %C"),
                   this->dam_servant_->getPlanUUID (),
                   this->node_id_.c_str ()));

  try
    {
      excep_holder->raise_exception ();
    }
  catch (Deployment::StopError &)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DAM_NM_ReplyHandlerImpl::destroyApplication_excep - ")
                    ACE_TEXT("StopError exception caught.\n")));
    }
  catch (CORBA::Exception &ex)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DAM_NM_ReplyHandlerImpl::destroyApplication_excep - ")
                    ACE_TEXT("Caught a CORBA exception: %C\n"),
                    ex._info ().c_str ()));
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DAM_NM_ReplyHandlerImpl::destroyApplication_excep - ")
                    ACE_TEXT("Caught unknown exception.\n")));
    }

  // add failure
  this->counter_->increment_fail_count ();

  // mark off node
  this->counter_->decrement_exec_count ();

  // deactivate AMI response handler
  PortableServer::ObjectId_var oid = this->dam_servant_->poa_->servant_to_id (this);
  this->dam_servant_->poa_->deactivate_object (oid.in ());
}

void DAM_NM_ReplyHandlerImpl::getDynamicResources (
  const ::Deployment::Resources & /*ami_return_val*/)
{
}
void DAM_NM_ReplyHandlerImpl::getDynamicResources_excep (
  ::Messaging::ExceptionHolder * /*excep_holder*/)
{
}

DomainApplicationManager_Impl::StartLaunchCompletionHandler::StartLaunchCompletionHandler (
  DomainApplicationManager_Impl *dam_servant,
  ::Deployment::AMH_ApplicationManagerResponseHandler_ptr _tao_rh)
    : dam_servant_ (DomainApplicationManager_Impl_var::_duplicate (dam_servant)),
      dam_rh_ (::Deployment::AMH_ApplicationManagerResponseHandler::_duplicate (_tao_rh))
{
}

DomainApplicationManager_Impl::StartLaunchCompletionHandler::~StartLaunchCompletionHandler ()
{
}

void
DomainApplicationManager_Impl::StartLaunchCompletionHandler::handle_completion (DomainApplication_Impl *da_servant)
{
  try
  {
    ::Deployment::Connections_var connections;
    ACE_NEW_THROW_EX (connections,
                      Deployment::Connections (),
                      CORBA::NO_MEMORY ());

    da_servant->getConnections (connections.inout ());

    this->dam_servant_->finish_startLaunch (
          this->dam_rh_.in (),
          da_servant,
          connections);
  }
  catch (CORBA::Exception& ex)
    {
      CORBA::Exception* local_ex = ex._tao_duplicate ();
      this->dam_servant_->fail_startLaunch (this->dam_rh_.in (),
                                            local_ex);
    }
  catch (...)
    {
      CORBA::Exception* unknown_ex = new CORBA::UNKNOWN;
      this->dam_servant_->fail_startLaunch (this->dam_rh_.in (),
                                            unknown_ex);
    }
}

void
DomainApplicationManager_Impl::StartLaunchCompletionHandler::handle_exception (CORBA::Exception* local_ex)
{
  this->dam_servant_->fail_startLaunch (this->dam_rh_.in (),
                                        local_ex);
}

DomainApplicationManager_Impl::DestroyManagerCompletionHandler::DestroyManagerCompletionHandler (
    DomainApplicationManager_Impl *dam_servant,
    const DAM_CompletionHandler_AutoPtr& ch_ptr,
    unsigned int exec_count)
  : Completion_Counter_Base<ACE_Null_Mutex> (exec_count, 0),
    dam_servant_ (DomainApplicationManager_Impl_var::_duplicate (dam_servant)),
    dam_ch_ (ch_ptr)
{
}

void
DomainApplicationManager_Impl::DestroyManagerCompletionHandler::handle_completion (
    DomainApplication_Impl *da_servant)
{
  DANCE_DEBUG (9, (LM_TRACE, DLINFO
                   ACE_TEXT("DomainApplicationManager_Impl::DestroyManagerCompletionHandler::handle_completion - ")
                   ACE_TEXT("Finished destroyApplication on DomainApplication[%@] for plan %C\n"),
                   da_servant,
                   this->dam_servant_->getPlanUUID ()));

  try
    {
      da_servant->_remove_ref ();
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                  ACE_TEXT("DomainApplicationManager_Impl::DestroyManagerCompletionHandler::handle_completion - ")
                  ACE_TEXT("UNEXPECTED exception while decrementing servant reference\n")));
    }

  // mark of application
  this->decrement_exec_count ();
}

void
DomainApplicationManager_Impl::DestroyManagerCompletionHandler::handle_exception (
    CORBA::Exception* local_ex)
{
  DANCE_ERROR (1, (LM_ERROR, DLINFO
                ACE_TEXT("DomainApplicationManager_Impl::DestroyManagerCompletionHandler::handle_exception - ")
                ACE_TEXT("CORBA exception %C for destroyApplication on domain application for plan %C\n"),
                local_ex->_info ().c_str (),
                this->dam_servant_->getPlanUUID ()));

  delete local_ex; // clean up

  // add failure
  this->increment_fail_count ();

  // mark of application
  this->decrement_exec_count ();
}

void
DomainApplicationManager_Impl::DestroyManagerCompletionHandler::on_all_completed ()
{
  // all applications gone, now finish it
  this->dam_servant_->finishDestroyManager (dam_ch_);
}

void
DomainApplicationManager_Impl::DestroyManagerCompletionHandler::on_all_completed_with_failure ()
{
  DANCE_ERROR (1, (LM_ERROR, DLINFO
              ACE_TEXT("DomainApplicationManager_Impl::DestroyManagerCompletionHandler::on_all_completed_with_failure - ")
              ACE_TEXT("%u errors destroying domain applications\n"),
              this->fail_count_i ()));

  this->dam_ch_->handle_exception (new ::Deployment::StopError());
}

DomainApplicationManager_Impl::DestroyApplicationCompletionHandler::DestroyApplicationCompletionHandler (
    DomainApplicationManager_Impl *dam_servant,
    ::Deployment::AMH_ApplicationManagerResponseHandler_ptr _tao_rh,
    DomainApplication_Impl *da_servant)
  : dam_servant_ (DomainApplicationManager_Impl_var::_duplicate (dam_servant)),
    dam_rh_ (::Deployment::AMH_ApplicationManagerResponseHandler::_duplicate (_tao_rh)),
    da_servant_ (DomainApplication_Impl_var::_duplicate (da_servant))
{
}

void DomainApplicationManager_Impl::DestroyApplicationCompletionHandler::handle_completion (
    DomainApplication_Impl * /*da_servant*/)
{
  DANCE_DEBUG (8, (LM_INFO, DLINFO
      ACE_TEXT("DomainApplicationManager_Impl::DestroyApplicationCompletionHandler::handle_completion - ")
      ACE_TEXT("finished\n")));
  dam_rh_->destroyApplication ();
}

void DomainApplicationManager_Impl::DestroyApplicationCompletionHandler::handle_exception (
    CORBA::Exception* local_ex)
{
  DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("ExecutionManager_Impl::DestroyManagerCompletionHandler::handle_exception - ")
                ACE_TEXT("propagating exception\n")));
  ::Deployment::AMH_ApplicationManagerExceptionHolder amh_exholder (local_ex);
  dam_rh_->destroyApplication_excep (&amh_exholder);
}
