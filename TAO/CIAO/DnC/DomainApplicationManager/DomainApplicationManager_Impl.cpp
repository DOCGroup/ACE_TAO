// $Id$

#include "DomainApplicationManager_Impl.h"

CIAO::DomainApplicationManager_Impl::
DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               Deployment::TargetManager_ptr manager,
                               const Deployment::DeploymentPlan & plan,
                               char * deployment_file)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    target_manager_ (Deployment::TargetManager::_duplicate (manager)),
    plan_ (plan),
    deployment_file_ (CORBA::string_dup (deployment_file)),
    deployment_config_ (orb)
{
  // @@
}

CIAO::DomainApplicationManager_Impl::~DomainApplicationManager_Impl ()
{

}

void
CIAO::DomainApplicationManager_Impl::
init (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((Deployment::ResourceNotAvailable,
                   Deployment::StartError,
                   Deployment::PlanError))
{
  //@@ Not implemented yet.

  ACE_TRY
    {
      // Call get_plan_info() method to get the total number
      // of child plans and list of NodeManager names.
      if ( this->get_plan_info () == false)
        ACE_THROW (Deployment::PlanError ());

      // Check the validity of the global deployment plan.
      if ( this->check_validity () == false)
        ACE_THROW (Deployment::PlanError ());

      // Invoke preparePlan for each child deployment plan.
      for (int i = 0; i < this->num_child_plans_; i++)
      {
        // Get the NodeManager reference.
        ::Deployment::NodeManager_var my_node_manager =
          this->deployment_config_.get_node_manager
            (this->node_manager_names_[i].c_str ());

        // Get the child deployment plan reference.
        ACE_Hash_Map_Entry
          <ACE_CString,
          ::Deployment::DeploymentPlan_var> *entry;

        if (this->child_plans_info_.find (this->node_manager_names_[i],
                                          entry) != 0)
           ACE_THROW (Deployment::PlanError ());

        Deployment::DeploymentPlan_var my_child_plan = entry->int_id_;

        // Call preparePlan() method on the NodeManager, which returns
        // a node application manager.
        // @@TODO: Does preparePlan take a _var type variable?
        ::Deployment::NodeApplicationManager_var my_nam =
          my_node_manager->preparePlan (my_child_plan
                                        ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        // Cache the node_application_manager reference
        this->node_application_manager_set_.add (my_nam.in ());
      }
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  return;
}

const char *
CIAO::DomainApplicationManager_Impl::get_uuid ()
{
  return this->uuid_;
}

void
CIAO::DomainApplicationManager_Impl::set_uuid (const char * uuid)
{
  // Copy this uuid reference
  this->uuid_ = CORBA::string_dup (uuid);
}


bool
CIAO::DomainApplicationManager_Impl::
get_plan_info (void)
{
  size_t length = this->plan_.instance.length ();

  // Error: If there are no nodes in the plan => No nodes to deploy the
  // components
  if (length == 0)
    return 0;

  // Copy the name of the node in the plan on to the node manager
  // array
  for (CORBA::ULong index = 0; index < length; index ++)
    this->node_manager_names_.
      push_back (CORBA::string_dup ((this->plan_.instance [index]).node));

  // Set the length of the Node Managers
  this->num_child_plans_ = length;

  // Indicate success
  return 1;
}

bool
CIAO::DomainApplicationManager_Impl::
check_validity (void)
{
  // (1) Read the deployment information data file through the
  //     Deployment_Configuration helper class,  which builds the hash
  //     table.
  // (1) For each NodeManager name, check whether the name is contained
  //     in the hash table, if not, then return <false>.
  if ( this->deployment_config_.init (this->deployment_file_) == -1 )
    return false;

  for (int i = 0; i < this->num_child_plans_; i++)
    {
      if (this->deployment_config_.get_node_manager
            (this->node_manager_names_[i].c_str ()) == NULL) // invalid name
        {
          return false;
        }
    }
  return true;
}

Deployment::Applications *
getApplications (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

int
CIAO::DomainApplicationManager_Impl::
split_plan (void)
{
  // Initialize the total number of node-level deployment plans specified
  // by the global plan.

 for (int i = 0; i < this->num_child_plans_; i++)
  {
    ::Deployment::DeploymentPlan * c_plan;
    ACE_NEW_RETURN (c_plan, ::Deployment::DeploymentPlan, 0);
    ::Deployment::DeploymentPlan_var tmp_plan(c_plan);

    tmp_plan->implementation.length (0);
    tmp_plan->instance.length (0);
    tmp_plan->connection.length (0);
    tmp_plan->externalProperty.length (0);
    tmp_plan->dependsOn.length (0);
    tmp_plan->artifact.length (0);
    tmp_plan->infoProperty.length (0);

    this->child_plans_info_.bind (node_manager_names_[i], tmp_plan);
  }

  // (1) Iterate over the <instance> field of the global DeploymentPlan
  //     variabl.
  // (2) Retrieve the necessary information to contruct the node-level
  //     plans one by one.

  for (CORBA::ULong i = 0; i < (this->plan_.instance).length (); i++)
    {
      // Fill in the child deployment plan in the map.

      // Get the instance deployment description
      ::Deployment::InstanceDeploymentDescription my_instance =
        (this->plan_.instance)[i];

      // Find the corresponding child deployment plan entry in
      // the hash map for this instance.
      ACE_Hash_Map_Entry
        <ACE_CString,
        ::Deployment::DeploymentPlan_var> *entry;

      if (this->child_plans_info_.find (ACE_CString (my_instance.node), //is this parameter correct?
                                        entry) != 0)
        return 0;                   // no valid name found.

      // Fill in the contents of the child deployment plan entry

      // Append the "MonolithicDeploymentDescriptions implementation" field with
      // a new "implementation", which is specified by the <implementationRef> field
      // of <my_instance> entry.
      // NOTE: The <artifactRef> field needs to be changed accordingly.
      ::Deployment::MonolithicDeploymentDescription my_implementation =
        (this->plan_.implementation)[my_instance.implementationRef];

      CORBA::ULong index_imp = entry->int_id_->implementation.length ();
      entry->int_id_->implementation.length (index_imp++);
      entry->int_id_->implementation[index_imp] = my_implementation;

      // @@TODO: Create a ULong sequence of artifactRef, which will be
      // as the new artifactRef field for the implementation struct.

      // Initialize with length.
      CORBA::ULongSeq ulong_seq (my_implementation.artifactRef.length ());

      // Append the "ArtifactDeploymentDescriptions artifact" field with
      // some new "artifacts", which is specified by the <artifactRef> sequence
      // of <my_implementation> entry.
      for (CORBA::ULong iter = 0;
           iter < my_implementation.artifactRef.length ();
           iter ++)
      {
        CORBA::ULong artifact_ref = my_implementation.artifactRef[iter];

        CORBA::ULong index_art = entry->int_id_->artifact.length ();
        entry->int_id_->artifact.length (index_art++);
        entry->int_id_->artifact[index_art] =
          (this->plan_.artifact)[artifact_ref];

        // @@ The artifactRef starts from 0.
        ulong_seq[iter] = index_art;
      }

      // Change the <artifactRef> field of the "implementation".
      entry->int_id_->implementation[index_imp].artifactRef = ulong_seq;


      // Append the "InstanceDeploymentDescription instance" field with
      // a new "instance", which is almost the same as the "instance" in
      // the global plan except the <implementationRef> field.
      // NOTE: The <implementationRef> field needs to be changed accordingly.
      CORBA::ULong index_ins = entry->int_id_->instance.length ();
      entry->int_id_->instance.length (index_ins++);
      entry->int_id_->instance[index_ins] = my_instance;

      // Change the <implementationRef> field of the "instance".
      // @@ The implementationRef starts from 0.
      entry->int_id_->instance[index_ins].implementationRef = index_ins;
    }

  return 1;
}

void
CIAO::DomainApplicationManager_Impl::
startLaunch (const ::Deployment::Properties & configProperty,
             ::CORBA::Boolean start
             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Deployment::ResourceNotAvailable,
                   ::Deployment::StartError,
                   ::Deployment::InvalidProperty))
{
  ACE_TRY
    {
      // Invoke startLaunch() operations on each cached NodeApplicationManager
      CORBA::ULong len = this->node_application_manager_set_.size ();
      for (CORBA::ULong i = 0; i < len; ++i)
        {
          ::Deployment::NodeApplicationManager_var my_nam =
            this->node_application_manager_set_.at (i);

          ::Deployment::Connections_var out_connections;

          // Obtained the returned NodeApplication object reference
          ::Deployment::Application_var temp =
            my_nam->startLaunch (configProperty,
                                 out_connections.out (),
                                 start);

          // Narrow down to NodeApplication object reference
          ::Deployment::NodeApplication_var my_na =
            ::Deployment::NodeApplication::_narrow (temp.in ()
                                                    ACE_ENV_ARG_PARAMETER);

          // Cache the NodeApplication object reference and Connections pair
          Node_Application_Para node_app;
          node_app.node_application_ = my_na;
          node_app.connections_ = out_connections;
          this->node_application_vec_.push_back (node_app);
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "DomainApplicationManager_Impl::startLaunch\t\n");
      ACE_RE_THROW;
      return;
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (0);
}


void
CIAO::DomainApplicationManager_Impl::
finishLaunch (::CORBA::Boolean start
              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Deployment::StartError))
{
  //@@
}


void
CIAO::DomainApplicationManager_Impl::
start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Deployment::StartError))
{
  // @@
}

void
CIAO::DomainApplicationManager_Impl::
destroyApplication ()
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Deployment::StopError))
{
  //ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  //if (this->cs_set_.object_in_set (app) == 0)
  //  ACE_THROW (Deployment::StopError ());

  //app->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
  //ACE_CHECK;

  // Should we remove the server still even if the previous call failed.

  //if (this->cs_set_.remove (app) == -1)
  //  ACE_THROW (Deployment::StopError ());
  //return;
}


/// Returns the DeploymentPlan associated with this ApplicationManager.
::Deployment::DeploymentPlan *
CIAO::DomainApplicationManager_Impl::
getPlan (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Deployment::DeploymentPlan_var plan = 0;
  // Make a deep copy of the Plan
  ACE_NEW_THROW_EX (plan,
                    Deployment::DeploymentPlan (this->plan_),
                    CORBA::NO_MEMORY ());

  // Transfer ownership
  return plan._retn ();
}

Deployment::Applications *
CIAO::DomainApplicationManager_Impl::
getApplications (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;

}
