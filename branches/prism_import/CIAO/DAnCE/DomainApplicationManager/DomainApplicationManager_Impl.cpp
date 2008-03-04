// $Id$

#include "DomainApplicationManager_Impl.h"
#include "Deployment/Deployment_ConnectionC.h"

using namespace DAnCE;

DomainApplicationManager_Impl::DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
    PortableServer::POA_ptr poa,
    const Deployment::DeploymentPlan& plan,
    TNodeManagers & nodes)
    : orb_ (CORBA::ORB::_duplicate (orb))
    , poa_ (PortableServer::POA::_duplicate (poa))
    , plan_ (plan)
    , nodes_ (nodes)
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_Impl constructor\n"));
  this->preparePlan();
  ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl was created for plan : %s\n", this->plan_.UUID.in()));
  //ACE_DEBUG((LM_DEBUG, "[%M] DomainApplicationManager_Impl received the deployment plan :\n"));
  //Deployment::DnC_Dump::dump(this->plan_);
}

DomainApplicationManager_Impl::~DomainApplicationManager_Impl()
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl destructor started\n"));
  while (0 < this->runningApp_.size())
    {
      DomainApplication_Impl* p = this->runningApp_[this->runningApp_.size()-1];
      Deployment::DomainApplication_var app =
        Deployment::DomainApplication::_narrow (this->poa_->servant_to_reference (p));
      PortableServer::ObjectId_var id = this->poa_->reference_to_id (app);
      ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl::~DomainApplicationManager_impl - deactivating object...\n"));
      this->poa_->deactivate_object (id);
      ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl::~DomainApplicationManager_impl - deleting application...\n"));
      this->runningApp_.pop_back();
      delete p;
    }

  for (DomainApplication_Impl::TNam2Nm::iterator iter = this->subAppMgr_.begin();
       iter != this->subAppMgr_.end();
       ++iter)
    {
      (*iter).int_id_->destroyManager ( (*iter).ext_id_.in());
    }
  this->subAppMgr_.unbind_all();
  ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl destructor finished\n"));
}

Deployment::Application_ptr
DomainApplicationManager_Impl::startLaunch (const Deployment::Properties & configProperty,
                                            Deployment::Connections_out providedReference)
ACE_THROW_SPEC ( (
                   ::CORBA::SystemException
                   , ::Deployment::ResourceNotAvailable
                   , ::Deployment::StartError
                   , ::Deployment::InvalidProperty
                   , ::Deployment::InvalidNodeExecParameter
                   , ::Deployment::InvalidComponentExecParameter
                 ))
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl::startLaunch - started\n"));

  ::Deployment::Connections_var connections;
  ACE_NEW_THROW_EX (connections,
                    Deployment::Connections (),
                    CORBA::NO_MEMORY ());
  DomainApplication_Impl* app;
  ACE_NEW_THROW_EX (app,
                    DomainApplication_Impl (this->subAppMgr_
                                            , configProperty
                                            , connections.inout()),
                    CORBA::NO_MEMORY());

  providedReference = connections._retn();

  PortableServer::ObjectId_var id = this->poa_->activate_object (app);
  this->runningApp_.push_back(app);
  
  ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl::startLaunch - finished\n"));
  CORBA::Object_var ref = this->poa_->id_to_reference (id);
  return Deployment::DomainApplication::_narrow (ref.in ());
}

void
DomainApplicationManager_Impl::destroyApplication (Deployment::Application_ptr application)
ACE_THROW_SPEC ( (
                   ::CORBA::SystemException,
                   ::Deployment::StopError
                 ))
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl::destroyApplication - started\n"));

  for (size_t i = 0; i < this->runningApp_.size(); ++i)
    {
      DomainApplication_Impl* p = this->runningApp_[i];
      Deployment::DomainApplication_var app =
        Deployment::DomainApplication::_narrow (this->poa_->servant_to_reference (p));
      if (application->_is_equivalent (app.in()))
        {
          PortableServer::ObjectId_var id = this->poa_->reference_to_id (application);
          ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl::destroyApplication - deactivating object\n"));
          this->poa_->deactivate_object (id);
          ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl::destroyApplication - deleting application\n"));
          delete p;
          for (size_t j = i + 1; j < this->runningApp_.size(); ++j)
            {
              this->runningApp_[j-1] = this->runningApp_[j];
            }
          this->runningApp_.pop_back();
          ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl::destroyApplication - finished\n"));
          return;
        }
    }
  ACE_ERROR ( (LM_ERROR, "[%M] DomainApplicationManager_impl::destroyApplication - Deployment::StopError exception occurred. Current application cannot be found \n"));
  throw Deployment::StopError();
}

::Deployment::Applications *
DomainApplicationManager_Impl::getApplications (
  ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
)
ACE_THROW_SPEC ( (
                   ::CORBA::SystemException
                 ))
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl::getApplications - started\n"));

  Deployment::Applications* runningApp;
  ACE_NEW_THROW_EX (runningApp,
                    Deployment::Applications(),
                    CORBA::NO_MEMORY());
  runningApp->length (this->runningApp_.size());
  unsigned int index = 0;
  for (size_t i = 0; i < this->runningApp_.size(); ++i)
    {
      CORBA::Object_var ref = this->poa_->servant_to_reference (this->runningApp_[i]);
      (*runningApp) [index++] =
        Deployment::DomainApplication::_narrow (ref.in ());
    }

  ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl::getApplications - finished\n"));
  return runningApp;
}

::Deployment::DeploymentPlan *
DomainApplicationManager_Impl::getPlan (
  ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
)
ACE_THROW_SPEC ( (
                   ::CORBA::SystemException
                 ))
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl::getPlan - started\n"));

  Deployment::DeploymentPlan* plan;

  ACE_NEW_THROW_EX (plan,
                    Deployment::DeploymentPlan (this->plan_),
                    CORBA::NO_MEMORY());

  ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl::getPlan - finished\n"));
  return plan;
}

// This methhod is commented because it requred rework for processing connections
// sequence together with correspondent DeploymentPlan
void
DomainApplicationManager_Impl::
dump_connections (const ::Deployment::Connections & /*connections*/) const
  {
    ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl::dump_connections - started\n"));

//  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::dump_connections");
//  const CORBA::ULong conn_len = connections.length ();
//  for (CORBA::ULong i = 0; i < conn_len; ++i)
//    {
//      ACE_DEBUG((LM_DEBUG, "[%M] instanceName: %s\n", connections[i].instanceName.in ()));
//
//      ACE_DEBUG((LM_DEBUG, "[%M] portName: %s\n", connections[i].portName.in ()));
//
//      ACE_DEBUG((LM_DEBUG, "[%M] portkind: "));
//
//      switch (connections[i].kind)
//        {
//          case Deployment::Facet:
//
//            ACE_DEBUG((LM_DEBUG, "[%M] Facet\n"));
//            break;
//
//          case Deployment::SimplexReceptacle:
//
//            ACE_DEBUG((LM_DEBUG, "[%M] SimplexReceptacle\n"));
//            break;
//
//          case Deployment::MultiplexReceptacle:
//
//            ACE_DEBUG((LM_DEBUG, "[%M] MultiplexReceptacle\n"));
//            break;
//
//          case Deployment::EventEmitter:
//
//            ACE_DEBUG((LM_DEBUG, "[%M] EventEmitter\n"));
//            break;
//
//          case Deployment::EventPublisher:
//
//            ACE_DEBUG((LM_DEBUG, "[%M] EventPublisher\n"));
//            break;
//
//          case Deployment::EventConsumer:
//
//            ACE_DEBUG((LM_DEBUG, "[%M] EventConsumer\n"));
//            break;
//
//        default:
//          ACE_DEBUG((LM_DEBUG, "[%M] Unknown port kind.\n"));
//        }
//
//      ACE_DEBUG((LM_DEBUG, "[%M] endpointInstanceName: %s\n",
//                  connections[i].endpointInstanceName.in ()));
//
//      ACE_DEBUG((LM_DEBUG, "[%M] endpointPortName: %s\n",
//                 connections[i].endpointPortName.in ()));
//      ACE_DEBUG((LM_DEBUG, "[%M] ---------------------\n"));
//    }

    ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_impl::dump_connections - finished\n"));
  }

void
DomainApplicationManager_Impl::split_plan (
  const Deployment::DeploymentPlan & plan,
  TNodePlans & sub_plans)
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] ExecutionManager_Impl::split_plan - started\n"));

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
    }

  ACE_DEBUG ( (LM_DEBUG, "[%M] ExecutionManager_Impl::split_plan - second phase\n"));
  // (1) Iterate over the <instance> field of the global DeploymentPlan
  //     structure.
  // (2) Retrieve the necessary information to contruct the node-level
  //     plans one by one.
  for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
    {
      ACE_DEBUG ( (LM_DEBUG, "[%M] Processing instance : %s\n", plan.instance[i].name.in()));
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
          ACE_ERROR ( (LM_ERROR, "[%M] Splitting plan failed"));
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

      ACE_DEBUG ( (LM_DEBUG, "[%M] Connections.\n"));
      // Copy connections
      for (CORBA::ULong j = 0; j < plan.connection.length(); ++j)
        {
          ACE_DEBUG ( (LM_DEBUG, "[%M] For connection : %s\n", plan.connection[j].name.in()));
          for (CORBA::ULong k = 0; k < plan.connection[j].internalEndpoint.length(); ++k)
            {
              ACE_DEBUG ( (LM_DEBUG, "[%M] For endpoint : %s(%s)\n"
                           , plan.connection[j].internalEndpoint[k].portName.in()
                           , plan.connection[j].internalEndpoint[k].provider ? "provider" : "client"));
              if (i == plan.connection[j].internalEndpoint[k].instanceRef)   // the instance (i) is referenced by the connection
                {
                  ACE_DEBUG ( (LM_DEBUG, "[%M] Taking.\n"));
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
                      ACE_DEBUG ( (LM_DEBUG, "[%M] Copying connection.\n"));
                      CORBA::ULong index_con = child_plan.connection.length();
                      child_plan.connection.length (index_con + 1);
                      child_plan.connection[index_con] = plan.connection[j];
                      connection_copied = &child_plan.connection[index_con];
                      connection_copied->internalEndpoint.length (0);
                    }

                  // Copy the endpoint
                  ACE_DEBUG ( (LM_DEBUG, "[%M] Copying endpoint.\n"));
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
  ACE_DEBUG ( (LM_DEBUG, "[%M] Original plan connection count : %u\n", plan.connection.length()));
  CORBA::ULong cnt = 0;
  for (TNodePlans::iterator it = sub_plans.begin(); it != sub_plans.end(); ++it)
    {
      cnt += (*it).int_id_.connection.length();
//        ACE_DEBUG((LM_DEBUG, "[%M] Dumping deployment plan #%s:\n", (*it).ext_id_.c_str()));
//        Deployment::DnC_Dump::dump((*it).int_id_);
    }
  ACE_DEBUG ( (LM_DEBUG, "[%M] Child plans connection count : %u\n", cnt));

  ACE_DEBUG ( (LM_DEBUG, "[%M] ExecutionManager_Impl::split_plan - finished\n"));

}

void
DomainApplicationManager_Impl::preparePlan()
{
  // Map of sub plans for each node
  TNodePlans sub_plans;
  // Splitting deployment plan on sub plans for each node
  DomainApplicationManager_Impl::split_plan (this->plan_, sub_plans);
  ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_Impl::preparePlan - plan have been splitted\n"));
  // Executing preparePlan on each NodeManager described in DeploymentPlan
  for (TNodePlans::iterator iter_plans = sub_plans.begin();
       iter_plans != sub_plans.end();
       ++iter_plans)
    {
      // Finding correspondent NodeManager
      Deployment::NodeManager_var nm;
      // If NodeManager not found throw StartError exception
      if (0 != this->nodes_.find ( (*iter_plans).ext_id_, nm))
        {
          ACE_ERROR ( (LM_ERROR, "[%M] DomainApplicationManager_Impl::preparePlan - Deployment::StartError exception. NodeManager cannot be found\n"));
          throw Deployment::StartError ( (*iter_plans).ext_id_.c_str(), "NodeManager not found");
        }
      // Calling preparePlan for node, specified in current sub plan
      ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_Impl::preparePlan - before preparePlan call\n"));

      Deployment::NodeApplicationManager_ptr nam
      = nm->preparePlan ( (*iter_plans).int_id_,
                          Deployment::ResourceCommitmentManager::_nil());
      ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplicationManager_Impl::preparePlan - after preparePlan call\n"));
      if (CORBA::is_nil (nam))
        {
          ACE_ERROR ( (LM_ERROR, "[%M] DomainApplicationManager_Impl::preparePlan - calling preparePlan for node, specified in current sub plan returned incorrect NodeApplicationManager poiniter. It is nil.\n"));
          throw ::Deployment::StartError();
        }
      // We save NAM reference ptr in TNodes vector were it places to var variable
      this->subAppMgr_.bind (nam, nm);
    }
}




