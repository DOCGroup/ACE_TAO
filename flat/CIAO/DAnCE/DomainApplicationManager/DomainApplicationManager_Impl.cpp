// $Id$

#include "DomainApplicationManager_Impl.h"
#include "Deployment/Deployment_ConnectionC.h"
#include "DAnCE/Logger/Log_Macros.h"

using namespace DAnCE;

DomainApplicationManager_Impl::DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
                                                              PortableServer::POA_ptr poa,
                                                              const Deployment::DeploymentPlan& plan,
                                                              Node_Locator & nodes)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , poa_ (PortableServer::POA::_duplicate (poa))
  , plan_ (plan)
  , nodes_ (nodes)
{
  DANCE_TRACE(DLINFO "DomainApplicationManager_Impl::DomainApplicationManager_Impl");
  this->preparePlan();
  DANCE_DEBUG((LM_DEBUG, DLINFO "DomainApplicationManager_Impl::DomainApplicationManager_Impl - "
               "Successfully prepared plan : %C\n", this->plan_.UUID.in()));
}

DomainApplicationManager_Impl::~DomainApplicationManager_Impl()
{
  DANCE_TRACE( "DomainApplicationManager_Impl::~DomainApplicationManager_Impl()");

  DANCE_DEBUG ((LM_DEBUG, DLINFO "DomainApplicationManager_Impl::~DomainApplicationManager_Impl - "
                "Destroying %u applications\n", this->running_app_.size()));
  while (0 < this->running_app_.size())
    {
      DomainApplication_Impl* p = this->running_app_[this->running_app_.size()-1];
      Deployment::DomainApplication_var app =
        Deployment::DomainApplication::_narrow (this->poa_->servant_to_reference (p));
      PortableServer::ObjectId_var id = this->poa_->reference_to_id (app);
      DANCE_DEBUG((LM_TRACE, DLINFO "DomainApplicationManager_impl::~DomainApplicationManager_impl - "
                   "deactivating DomainApplication object...\n"));
      this->poa_->deactivate_object (id);
      DANCE_DEBUG((LM_DEBUG, DLINFO "DomainApplicationManager_impl::~DomainApplicationManager_impl - "
                   "deleting DomainApplication.\n"));
      this->running_app_.pop_back();
      delete p;
    }

  DANCE_DEBUG ((LM_DEBUG, DLINFO "DomainApplicationManager_Impl::~DomainApplicationManager_Impl - "
                "Destroying %u managers\n", this->sub_app_mgr_.current_size()));
  for (DomainApplication_Impl::TNam2Nm::iterator iter = this->sub_app_mgr_.begin();
       iter != this->sub_app_mgr_.end();
       ++iter)
    {
      (*iter).int_id_->destroyManager ( (*iter).ext_id_.in());
    }
  this->sub_app_mgr_.unbind_all();
}

Deployment::Application_ptr
DomainApplicationManager_Impl::startLaunch (const Deployment::Properties & configProperty,
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

  DANCE_DEBUG ((LM_TRACE, "DomainApplicationManager_Impl::startLaunch - "
                "Successfully created DomainApplication\n"));
  DANCE_DEBUG ((LM_TRACE, "DomainApplicationManager_Impl::startLaunch - "
                "Created %u provided references\n",
                providedReference->length ()));

  CORBA::Object_var ref = this->poa_->id_to_reference (id);
  return Deployment::DomainApplication::_narrow (ref.in ());
}

void
DomainApplicationManager_Impl::destroyApplication (Deployment::Application_ptr application)
{
  DANCE_TRACE ( "DomainApplicationManager_Impl::destroyApplication");

  try
    {
      for (size_t i = 0; i < this->running_app_.size(); ++i)
        {
          DomainApplication_Impl* p = this->running_app_[i];
          Deployment::DomainApplication_var app =
            Deployment::DomainApplication::_narrow (this->poa_->servant_to_reference (p));
          if (application->_is_equivalent (app.in()))
            {
              PortableServer::ObjectId_var id = this->poa_->reference_to_id (application);
              DANCE_DEBUG((LM_TRACE, DLINFO "DomainApplicationManager_impl::destroyApplication - "
                           "deactivating application object\n"));
              this->poa_->deactivate_object (id);
              DANCE_DEBUG((LM_TRACE, DLINFO "DomainApplicationManager_impl::destroyApplication - "
                           "deleting application object\n"));
              delete p;
              for (size_t j = i + 1; j < this->running_app_.size(); ++j)
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
      ACE_ERROR ((LM_ERROR, DLINFO "DomainApplicationManager_impl::destroyApplication - "
                  "Propagating a received StopError exception\n"));
      throw e;
    }
  catch (...)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "DomainApplicationManager_impl::destroyApplication - "
                    "Caught unknown exception."));
      throw Deployment::StopError();
    }

  ACE_ERROR ((LM_ERROR, DLINFO "DomainApplicationManager_impl::destroyApplication - "
              "Provided application reference unknown\n"));
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
  unsigned int index = 0;
  for (size_t i = 0; i < this->running_app_.size(); ++i)
    {
      CORBA::Object_var ref = this->poa_->servant_to_reference (this->running_app_[i]);
      (*running_app) [index++] =
        Deployment::DomainApplication::_narrow (ref.in ());
    }

  DANCE_DEBUG((LM_DEBUG, DLINFO "DomainApplicationManager_impl::getApplications - "
               "Returning %u running applications\n",
               running_app->length ()));
  return running_app;
}

::Deployment::DeploymentPlan *
DomainApplicationManager_Impl::getPlan ()
{
  DANCE_TRACE ( DLINFO "DomainApplicationManager_Impl::getPlan ()");

  Deployment::DeploymentPlan* plan = 0;

  ACE_NEW_THROW_EX (plan,
                    Deployment::DeploymentPlan (this->plan_),
                    CORBA::NO_MEMORY());

  return plan;
}

// This methhod is commented because it requred rework for processing connections
// sequence together with correspondent DeploymentPlan
void
DomainApplicationManager_Impl::
dump_connections (const ::Deployment::Connections & /*connections*/) const
{
  DANCE_DEBUG((LM_DEBUG, DLINFO "DomainApplicationManager_impl::dump_connections - started\n"));

  //  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::dump_connections");
  //  const CORBA::ULong conn_len = connections.length ();
  //  for (CORBA::ULong i = 0; i < conn_len; ++i)
  //    {
  //      DANCE_DEBUG((LM_DEBUG, "[%M] instanceName: %C\n", connections[i].instanceName.in ()));
  //
  //      DANCE_DEBUG((LM_DEBUG, "[%M] portName: %C\n", connections[i].portName.in ()));
  //
  //      DANCE_DEBUG((LM_DEBUG, "[%M] portkind: "));
  //
  //      switch (connections[i].kind)
  //        {
  //          case Deployment::Facet:
  //
  //            DANCE_DEBUG((LM_DEBUG, "[%M] Facet\n"));
  //            break;
  //
  //          case Deployment::SimplexReceptacle:
  //
  //            DANCE_DEBUG((LM_DEBUG, "[%M] SimplexReceptacle\n"));
  //            break;
  //
  //          case Deployment::MultiplexReceptacle:
  //
  //            DANCE_DEBUG((LM_DEBUG, "[%M] MultiplexReceptacle\n"));
  //            break;
  //
  //          case Deployment::EventEmitter:
  //
  //            DANCE_DEBUG((LM_DEBUG, "[%M] EventEmitter\n"));
  //            break;
  //
  //          case Deployment::EventPublisher:
  //
  //            DANCE_DEBUG((LM_DEBUG, "[%M] EventPublisher\n"));
  //            break;
  //
  //          case Deployment::EventConsumer:
  //
  //            DANCE_DEBUG((LM_DEBUG, "[%M] EventConsumer\n"));
  //            break;
  //
  //        default:
  //          DANCE_DEBUG((LM_DEBUG, "[%M] Unknown port kind.\n"));
  //        }
  //
  //      DANCE_DEBUG((LM_DEBUG, "[%M] endpointInstanceName: %C\n",
  //                  connections[i].endpointInstanceName.in ()));
  //
  //      DANCE_DEBUG((LM_DEBUG, "[%M] endpointPortName: %C\n",
  //                 connections[i].endpointPortName.in ()));
  //      DANCE_DEBUG((LM_DEBUG, "[%M] ---------------------\n"));
  //    }

  DANCE_DEBUG((LM_DEBUG, DLINFO "DomainApplicationManager_impl::dump_connections - finished\n"));
}

void
DomainApplicationManager_Impl::split_plan (const Deployment::DeploymentPlan & plan,
                                           TNodePlans & sub_plans)
{
  DANCE_TRACE ( "DomainApplicationManager_Impl::split_plan");

  DANCE_DEBUG ((LM_TRACE, "DomainApplicationManager_Impl::split_plan - "
                "Creating sub-plans\n"));
  // Create empty sub-plans
  for (CORBA::ULong i = 0; i < plan.instance.length(); ++i)
    {
      ACE_CString node = plan.instance[i].node.in();
      if (0 == sub_plans.find (node)) continue;

      ::Deployment::DeploymentPlan tmp_plan;

      // Construct the UUID for the child plan.
      // Currently, this UUID consists of:
      // The global UUID for the parent plan + the name of the node the
      // child plan is to be installed on.
      ACE_CString child_uuid (plan.UUID.in ());
      //      child_uuid += "@"; CCM#61
      //      child_uuid += node;

      tmp_plan.UUID = CORBA::string_dup (child_uuid.c_str());
      tmp_plan.implementation.length (0);
      tmp_plan.instance.length (0);
      tmp_plan.connection.length (0);
      tmp_plan.externalProperty.length (0);
      tmp_plan.dependsOn.length (0);
      tmp_plan.artifact.length (0);

      // @@ There is an optimization point here, since we really don't have to
      // pass the entire CIAOServerResources into each individual child plan.
      tmp_plan.infoProperty = plan.infoProperty;

      sub_plans.bind (node, tmp_plan);

      DANCE_DEBUG ((LM_TRACE, DLINFO "DomainApplicationManager_Impl::split_plan - "
                    "Created sub-plan for node %C with UUID %C\n",
                    node.c_str (), child_uuid.c_str ()));
    }

  DANCE_DEBUG ((LM_DEBUG, DLINFO "DomainApplicationManager_Impl::split_plan - "
                "First phase produced %u child plans, proceeding to second\n"));

  // (1) Iterate over the <instance> field of the global DeploymentPlan
  //     structure.
  // (2) Retrieve the necessary information to contruct the node-level
  //     plans one by one.
  for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
    {
      DANCE_DEBUG ((LM_TRACE, DLINFO "DomainApplicationManager_Impl::split_plan - "
                    "Processing instance : %C\n", plan.instance[i].name.in()));
      // @@TODO Fill in the child deployment plan in the map.
      // If the component instance already exists in the child plan,
      // then we overwrite the existing instance, since the new instance
      // might have different resource usage requirements.

      // Get the instance deployment description
      const ::Deployment::InstanceDeploymentDescription & my_instance = plan.instance[i];

      // Get the child plan.
      ::Deployment::DeploymentPlan child_plan;

      if (0 != sub_plans.find (ACE_CString (my_instance.node.in()), child_plan))
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "DomainApplicationManager_Impl::split_plan - "
                        "ERROR: Unable to find sub-plan for instance %C\n",
                        my_instance.node.in ()));
        }

      // Fill in the contents of the child plan entry.

      // Append the "MonolithicDeploymentDescriptions implementation"
      // field with a new "implementation", which is specified by the
      // <implementationRef> field of <my_instance> entry.  NOTE: The
      // <artifactRef> field needs to be changed accordingly.
      const ::Deployment::MonolithicDeploymentDescription & my_implementation
        = plan.implementation[my_instance.implementationRef];

      CORBA::ULong index_imp = child_plan.implementation.length ();
      child_plan.implementation.length (++index_imp);
      child_plan.implementation[index_imp-1] = my_implementation;

      // Initialize with the correct sequence length.
      CORBA::ULongSeq ulong_seq;
      ulong_seq.length (my_implementation.artifactRef.length ());

      // Append the "ArtifactDeploymentDescriptions artifact" field
      // with some new "artifacts", which is specified by the
      // <artifactRef> sequence of <my_implementation> entry.
      const CORBA::ULong impl_length = my_implementation.artifactRef.length ();
      for (CORBA::ULong iter = 0;
           iter < impl_length;
           iter ++)
        {
          CORBA::ULong artifact_ref = my_implementation.artifactRef[iter];

          // Fill in the <artifact> field of the child plan
          CORBA::ULong index_art = child_plan.artifact.length ();
          child_plan.artifact.length (++index_art);
          child_plan.artifact[index_art-1] = plan.artifact[artifact_ref];

          // Fill in the <artifactRef> field of the MonolithicDeploymentDescription
          ulong_seq[iter] = index_art - 1;
        }

      // Change the <artifactRef> field of the "implementation".
      child_plan.implementation[index_imp-1].artifactRef = ulong_seq;

      // Append the "InstanceDeploymentDescription instance" field with
      // a new "instance", which is almost the same as the "instance" in
      // the global plan except the <implementationRef> field.
      // NOTE: The <implementationRef> field needs to be changed accordingly.
      CORBA::ULong index_ins = child_plan.instance.length ();
      child_plan.instance.length (++index_ins);
      child_plan.instance[index_ins-1] = my_instance;

      // Change the <implementationRef> field of the "instance".
      // @@ The implementationRef starts from 0.
      // @@TODO: Below logic needs to be changed once we want to avoid
      // duplicate <implementation> for the optimization.
      child_plan.instance[index_ins-1].implementationRef = index_imp - 1;

      DANCE_DEBUG ((LM_TRACE, DLINFO "DomainApplicationManager_Impl::split_plan - "
                    "Processing connections.\n"));
      // Copy connections
      for (CORBA::ULong j = 0; j < plan.connection.length(); ++j)
        {
          DANCE_DEBUG ((LM_TRACE, DLINFO "DomainApplicationManager_Impl::split_plan - "
                        "For connection : %C\n", plan.connection[j].name.in()));
          for (CORBA::ULong k = 0; k < plan.connection[j].internalEndpoint.length(); ++k)
            {
              DANCE_DEBUG ((LM_TRACE, DLINFO "DomainApplicationManager_Impl::split_plan - "
                            "For endpoint : %C(%C)\n",
                            plan.connection[j].internalEndpoint[k].portName.in(),
                            plan.connection[j].internalEndpoint[k].provider ? "provider" : "client"));
              if (i == plan.connection[j].internalEndpoint[k].instanceRef)   // the instance (i) is referenced by the connection
                {
                  Deployment::PlanConnectionDescription * connection_copied = 0;
                  for (CORBA::ULong m = 0; m < child_plan.connection.length(); ++m)
                    {
                      if (plan.connection[j].name == child_plan.connection[m].name)
                        {
                          connection_copied = &child_plan.connection[m];
                          break;
                        }
                    }

                  if (0 == connection_copied)
                    {
                      // Copy the connection
                      CORBA::ULong index_con = child_plan.connection.length();
                      child_plan.connection.length (index_con + 1);
                      child_plan.connection[index_con] = plan.connection[j];
                      connection_copied = &child_plan.connection[index_con];
                      connection_copied->internalEndpoint.length (0);
                    }

                  // Copy the endpoint
                  CORBA::ULong index_ep = connection_copied->internalEndpoint.length();
                  connection_copied->internalEndpoint.length (index_ep + 1);
                  connection_copied->internalEndpoint[index_ep] = plan.connection[j].internalEndpoint[k];
                  connection_copied->internalEndpoint[index_ep].instanceRef = index_ins - 1;
                }
            }
        }
      sub_plans.rebind (ACE_CString (my_instance.node.in()), child_plan);
    }

  //Debug
  DANCE_DEBUG ((LM_DEBUG, DLINFO "DomainApplicationManager_Impl::split_plan - "
                "Original plan connection count : %u\n", plan.connection.length()));
  CORBA::ULong cnt = 0;
  for (TNodePlans::iterator it = sub_plans.begin(); it != sub_plans.end(); ++it)
    {
      cnt += (*it).int_id_.connection.length();
      //        DANCE_DEBUG((LM_DEBUG, "[%M] Dumping deployment plan #%C:\n", (*it).ext_id_.c_str()));
      //        Deployment::DnC_Dump::dump((*it).int_id_);
    }
  DANCE_DEBUG ((LM_DEBUG, DLINFO "DomainApplicationManager_Impl::split_plan - "
                "Child plans connection count : %u\n", cnt));
}

void
DomainApplicationManager_Impl::preparePlan()
{
  DANCE_TRACE ("DomainApplicationManager_Impl::preparePlan()");

  try
    {
      // Map of sub plans for each node
      TNodePlans sub_plans;
      // Splitting deployment plan on sub plans for each node
      DomainApplicationManager_Impl::split_plan (this->plan_, sub_plans);
      DANCE_DEBUG((LM_TRACE, DLINFO "DomainApplicationManager_Impl::preparePlan - Plan successfully split\n"));
      // Executing preparePlan on each NodeManager described in DeploymentPlan
      for (TNodePlans::iterator iter_plans = sub_plans.begin();
           iter_plans != sub_plans.end();
           ++iter_plans)
        {
          // Finding correspondent NodeManager
          Deployment::NodeManager_var nm = this->nodes_.locate_node ((*iter_plans).ext_id_.c_str ());;
          // If NodeManager not found throw StartError exception
          if (CORBA::is_nil (nm.in ()))
            {
              DANCE_ERROR ((LM_ERROR, DLINFO "DomainApplicationManager_Impl::preparePlan - "
                            "Deployment::StartError exception. NodeManager %C cannot be found\n",
                            (*iter_plans).ext_id_.c_str()));
              throw Deployment::StartError ( (*iter_plans).ext_id_.c_str(), "NodeManager not found");
            }

          // Calling preparePlan for node, specified in current sub plan
          DANCE_DEBUG((LM_TRACE, DLINFO "DomainApplicationManager_Impl::preparePlan - "
                       "Calling preparePlan on node %C\n",
                       (*iter_plans).ext_id_.c_str()));

          Deployment::NodeApplicationManager_ptr nam
            = nm->preparePlan ( (*iter_plans).int_id_,
                                Deployment::ResourceCommitmentManager::_nil());

          if (CORBA::is_nil (nam))
            {
              DANCE_ERROR ((LM_ERROR, DLINFO "DomainApplicationManager_Impl::preparePlan - "
                            "PreparePlan failed for node %C, returning a nill "
                            "NodeApplicationManager poiniter.\n",
                            (*iter_plans).ext_id_.c_str()));
              throw ::Deployment::StartError();
            }
          // We save NAM reference ptr in TNodes vector were it places to var variable
          this->sub_app_mgr_.bind (nam, nm);

          DANCE_DEBUG ((LM_INFO, DLINFO "DomainApplicationManager_Impl::preparePlan - "
                        "Sucessfully prepared node %C for deployment\n",
                        (*iter_plans).ext_id_.c_str()));
        }
    }
  catch (Deployment::StartError &e)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "DomainApplicationManager_Impl::preparePlan - "
                    "Propagating StartError exception caught here\n"));
      throw e;
    }
  catch (CORBA::Exception &ex)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "DomainApplicationManager_Impl::preparePlan - "
                    "Caught a CORBA exception, propagating StartError: %s\n",
                    ex._info ().c_str ()));
      throw ::Deployment::StartError ();
    }
  catch (...)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "DomainApplicationManager_Impl::preparePlan - "
                    "Caught unknown exception.  Propagating StartError\n"));
      throw ::Deployment::StartError ();
    }
}




