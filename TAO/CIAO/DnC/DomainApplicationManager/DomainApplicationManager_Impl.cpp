// $Id$

#include "DomainApplicationManager_Impl.h"

//===============================================================
//============   DomainApplicationManager_Impl  =================
//===============================================================

CIAO::DomainApplicationManager_Impl::
DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
  	                           PortableServer::POA_ptr poa,
                               Deployment::TargetManager_ptr manager,
                               Deployment::DeploymentPlan & plan)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    target_manager_ (manager),
    plan_ (plan)
{
  // @@ 
}

CIAO::DomainApplicationManager_Impl::~DomainApplicationManager_Impl ()
{
  // @@ remove the DomainApplication?
}


int
CIAO::DomainApplicationManager_Impl::
init (int num_child_plans, ACE_CString * node_manager_names)
{
  this->num_child_plans_ = num_child_plans;
  this->node_manager_names_ = node_manager_names;

  return 1;
}


int 
CIAO::DomainApplicationManager_Impl::
split_plan ()
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

        // @@ The artifactRef starts from 0 or 1??
        ulong_seq[iter] = index_art; 
      }

      // Change the <artifactRef> field of the "implementation".
      entry->int_id_->implementation.artifactRef = ulong_seq;

      
      // Append the "InstanceDeploymentDescription instance" field with
      // a new "instance", which is almost the same as the "instance" in
      // the global plan except the <implementationRef> field.
      // NOTE: The <implementationRef> field needs to be changed accordingly.
      CORBA::ULong index_ins = entry->int_id_->instance.length ();
      entry->int_id_->instance.length (index_ins++);
      entry->int_id_->instance[index_ins] = my_instance;

      // Change the <implementationRef> field of the "instance".
      // @@ The implementationRef starts from 0 or 1??
      entry->int_id_->instance.implementationRef = index_ins;
    }

  return 1;
}





::Deployment::Application_ptr 
CIAO::DomainApplicationManager_Impl::
startLaunch (const Deployment::Properties & configProperty,
             Deployment::Connections_out providedReference,
             CORBA::Boolean start
             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::ResourceNotAvailable,
                   Deployment::StartError,
                   Deployment::InvalidProperty))
{
  /**
   *  1. First Map properties to TAO/CIAO specific property/configurations
   *  2. Necessary property checking (needed?)
   *  3. Call create_nade_application to spawn new process.
   *  4. Initialize the NodeApplication.
   *  5. get the provided connection endpoints back and return them.
   */
  ACE_TRY
    {

    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (0);

  return 0;
}


void
CIAO::DomainApplicationManager_Impl::
destroyApplication (Deployment::Application_ptr app
		                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException, 
                   Deployment::StopError))
{
  //ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  //if (this->cs_set_.object_in_set (app) == 0)
  //  ACE_THROW (Deployment::StopError ());

  //app->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
  //ACE_CHECK;

  // Should we remove the server still even if the previous call failed.

  //if (this->cs_set_.remove (app) == -1)
  //  ACE_THROW (Deployment::StopError ());
  return;
}
