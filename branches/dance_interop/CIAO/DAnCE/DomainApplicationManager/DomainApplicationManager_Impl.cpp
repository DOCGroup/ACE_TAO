// $Id$

#include "DomainApplicationManager_Impl.h"
#include "ExecutionManager/Execution_Manager_Impl.h"
#include "ciao/NodeApplicationManagerC.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_string.h"
#include "ace/SString.h"
#include "ace/Assert.h"
#include "ciao/Connection_Manager.h"
#include "DomainApplication/DomainApplication_Impl.h"

#if !defined (__ACE_INLINE__)
# include "DomainApplicationManager_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::DomainApplicationManager_Impl::
DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               Deployment::TargetManager_ptr manager,
                               const Deployment::DeploymentPlan & plan,
                               const char * deployment_file)
  throw (CORBA::SystemException)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    target_manager_ (Deployment::TargetManager::_duplicate (manager)), // object ref
    plan_ (plan),
    num_child_plans_ (0),
    // @@ (OO) The default size for an ACE_Hash_Map_Mapanger is quiet
    //         large.  The maximum size of an ACE_Hash_Map_Manager is
    //         also fixed, i.e. it does not grow dynamically on demand.
    //         Make sure the default size of artifact_map_ is
    //         appropriate for your needs.  You may also want to make
    //         the size configurable at compile-time, at least.
    //
    // Need to initialize chained artifacts here.
    //
    deployment_file_ (CORBA::string_dup (deployment_file)),
    deployment_config_ (orb)
{
  CIAO_TRACE("DomainApplicationManager_Impl::DomainApplicationManager_Impl");
}

CIAO::DomainApplicationManager_Impl::~DomainApplicationManager_Impl ()
{
  CIAO_TRACE("DomainApplicationManager_Impl::~DomainApplicationManager_Impl");
  CIAO_DEBUG(1, (LM_DEBUG, "DomainApplicationManager destroyed\n"));
}


void
CIAO::DomainApplicationManager_Impl::
init (::Deployment::ResourceCommitmentManager_ptr resourceCommitment)
  throw (CORBA::SystemException,
         Deployment::ResourceNotAvailable,
         Deployment::StartError,
         Deployment::PlanError)
{
  CIAO_TRACE("DomainApplicationManager_Impl::init");
  try
    {
      // (1) Call get_plan_info() method to get the total number
      //     of child plans and list of NodeManager names, and
      // (2) Check the validity of the global deployment plan.
      this->get_plan_info ();

      // Call split_plan()
      this->split_plan ();
      
      // Invoke preparePlan on each NodeManager by giving child plan.
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeManager object reference.
          ::Deployment::NodeManager_var my_node_manager =
	      this->deployment_config_.get_node_manager
              (this->node_manager_names_[i].c_str ());

          // Get the child deployment plan reference.
          Node_Level_Plan_Entry *entry;

          if (this->plan_table_.find (this->node_manager_names_[i],
                                      entry) != 0)
            {
              ACE_CString error ("Error while finding the node specific plan "
                                 "for the node ");
              error += this->node_manager_names_[i].c_str ();
              
              CIAO_ERROR (1, (LM_ERROR, "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                              "CIAO::DomainApplicationManager_Impl::init - %s\n",
                              error.c_str ()));
              
              throw Deployment::PlanError ("DomainApplicationManager::init",
                                           error.c_str ());
            }

          Node_Level_Plan &artifacts = entry->int_id_;

          // Call preparePlan() method on the NodeManager with the
          // corresponding child plan as input, which returns a
          // NodeApplicationManager object reference.
          Deployment::NodeApplicationManager_var app_manager =
            my_node_manager->preparePlan (artifacts.child_plan_.in (),
                                          resourceCommitment);

          if (CORBA::is_nil (app_manager.in ()))
            {
              ACE_CString error ("Nil object reference for NodeApplicationManager "
                                 "received from NodeManager::preparePlan on ");
              error += this->node_manager_names_[i].c_str ();
              
              CIAO_ERROR(1, (LM_ERROR,
                             "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                             "CIAO::DomainApplicationManager_Impl::init - %s \n",
                             error.c_str ()));

              throw Deployment::StartError ("DomainApplicationManager_Impl:init",
                                            error.c_str ());
            }
          
          // Cache the NodeApplicationManager object reference.
          artifacts.node_application_manager_ = app_manager._retn ();
        }
    }
  catch (Deployment::PlanError &ex)
    {
      // rethrow....
      throw;
    }
  catch (Deployment::StartError &ex)
    {
      // rethrow....
      throw;
    }
  catch (CORBA::SystemException &ex)
    {
      CIAO_PRINT_EXCEPTION (1, ex,
			    "DomainApplicationManager_Impl::init\t\n");
      throw;
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, "DomainApplicationManager_Impl::init caught unknown exception\n"));
      throw Deployment::PlanError ("DomainApplicationManager_Impl::init",
                                   "Unknown exception\n");
    }
}

void
CIAO::DomainApplicationManager_Impl::get_plan_info (void)
  throw (Deployment::PlanError)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::get_plan_info");

  // Read the deployment.dat file and get to know the different nodes
  // that are present in the deployment domain.
  // We should be able to replace this way, by parsing the domain
  // descriptor to get to know more on the domain.
  //
  if ( this->deployment_config_.init (this->deployment_file_) == -1 )
    {
      CIAO_ERROR (1, (LM_ERROR,
                      "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                      "CIAO::DomainApplicationManager_Impl::get_plan_info -"
                      "ERROR while trying to initialize after reading "
                      "deployment DAT file \n"));
      throw Deployment::PlanError ("DomainApplicationManager::get_plan_info", 
				   "DomainApplicationManager unable to parse deployment DAT file.");
    }

  // Error: If there are no nodes in the plan => No nodes to deploy the
  // components
  const CORBA::ULong length = this->plan_.instance.length ();

  if (length == 0)
    {
      CIAO_ERROR (1, (LM_ERROR,
                      "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                      "CIAO::DomainApplicationManager_Impl::get_plan_info -"
                      "ERROR while trying to get the total instances to "
                      "be deployed \n"));
      throw Deployment::PlanError ("DomainApplicationManager::get_plan_info",
				   "No MonolithicInstanciationDescriptions found in plan.");
    }

  // Copy the name of the node in the plan on to the node manager
  // array, Making sure that duplicates are not added.
  //
  // OPTIMIZATION:
  // We can parse the domain descriptor and the size of the node
  // data structure is the size of the num_plans and
  // the individual node names need to be entered into the
  // node_manager_map.
  //
  this->node_manager_names_.clear ();

  for (CORBA::ULong index = 0; index < length; index ++)
    {
      bool matched = false;

      const CORBA::ULong number_node_managers =
	this->node_manager_names_.size ();

      for (CORBA::ULong i = 0; i < number_node_managers; ++i)
        // If a match is found do not add it to the list of unique
        // node names
        if (ACE_OS::strcmp (this->plan_.instance [index].node.in (),
                            (this->node_manager_names_ [i]).c_str ()) == 0)
          {
            // Break out -- Duplicates found
            matched = true;
            break;
          }

      if (! matched)
        {
          // Check if there is a corresponding NodeManager instance existing
          // If not present return false
          ::Deployment::NodeManager_var mgr =
              this->deployment_config_.get_node_manager
	      (this->plan_.instance [index].node.in ());

          if (CORBA::is_nil (mgr.in ()))
            {
              ACE_CString error ("Node ");
              error += this->plan_.instance[index].node.in ();
              error += " is not present in the domain\n";
              
              throw Deployment::PlanError ("DomainApplicationManager::get_plan_info",
					   error.c_str ());
            }                     

          // Add this unique node_name to the list of NodeManager names
          this->node_manager_names_.push_back (this->plan_.instance [index].node.in ());

          // Increment the number of plans
          ++this->num_child_plans_;
        }
    }
}

void
CIAO::DomainApplicationManager_Impl::
split_plan (void) throw (CORBA::NO_MEMORY)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::split_plan");

  // Initialize the total number of child deployment plans specified
  // by the global plan.
  CORBA::ULong i;

  for ( i = 0; i < this->num_child_plans_; ++i)
    {
      ::Deployment::DeploymentPlan_var tmp_plan;
      ACE_NEW_THROW_EX (tmp_plan,
			::Deployment::DeploymentPlan,
			CORBA::NO_MEMORY ());
    
      // @@ (WO) - Is this really necessary? for the time being, I am thinking not.
      /*
      // Construct the UUID for the child plan.
      // Currently, this UUID consists of:
      // The global UUID for the parent plan + the name of the node the
      // child plan is to be installed on.
      ACE_CString child_uuid (this->plan_.UUID.in ());
      child_uuid += "@";
      child_uuid += this->node_manager_names_[i];
      */
    
      tmp_plan->UUID = CORBA::string_dup (this->plan_.UUID.in ());
      tmp_plan->implementation.length (0);
      tmp_plan->instance.length (0);
      tmp_plan->connection.length (0);
      tmp_plan->externalProperty.length (0);
      tmp_plan->dependsOn.length (0);
      tmp_plan->artifact.length (0);

      // @@ There is an optimization point here, since we really don't have to
      // pass the entire CIAOServerResources into each individual child plan.
      // @@ (WO) - In any event, the CSR is not the only thing in the infoProperties.
      tmp_plan->infoProperty = this->plan_.infoProperty;

      Node_Level_Plan artifacts;

      // Fill in the child_plan_ field, relinquishing ownership
      artifacts.child_plan_ = tmp_plan._retn ();

      // Fill in the node_manager_ field.
      artifacts.node_manager_ =
	this->deployment_config_.get_node_manager
	(this->node_manager_names_[i].c_str ());

      // In case we are doing redeployment, rebind will help replace the
      // old child plan with the new child plan.
      this->plan_table_.rebind (node_manager_names_[i], artifacts);
    }

  // (1) Iterate over the <instance> field of the global DeploymentPlan
  //     structure.
  // (2) Retrieve the necessary information to contruct the node-level
  //     plans one by one.
  for ( i = 0; i < (this->plan_.instance).length (); ++i)
    {
      // @@TODO Fill in the child deployment plan in the map.
      // If the component instance already exists in the child plan,
      // then we overwrite the existing instance, since the new instance
      // might have different resource usage requirements.



      // Get the instance deployment description
      const ::Deployment::InstanceDeploymentDescription & my_instance =
        (this->plan_.instance)[i];

      // Find the corresponding child deployment plan entry in
      // the hash map for this instance.
      Node_Level_Plan_Entry *entry = 0;

      if (this->plan_table_.find
          (ACE_CString (my_instance.node.in ()),
	   entry) != 0)
        {
          // no valid name found - I doubt this should ever happen
          ACE_CString error ("Node ");
          error += my_instance.node.in ();
          error += " is not present in the domain\n";
              
          throw Deployment::PlanError ("DomainApplicationManager::split_plan",
				       error.c_str ());
        }
      
      // Get the child plan.
      ::Deployment::DeploymentPlan_var &child_plan =
          (entry->int_id_).child_plan_;

      // Fill in the contents of the child plan entry.

      // Append the "MonolithicDeploymentDescriptions implementation"
      // field with a new "implementation", which is specified by the
      // <implementationRef> field of <my_instance> entry.  NOTE: The
      // <artifactRef> field needs to be changed accordingly.
      const ::Deployment::MonolithicDeploymentDescription & my_implementation =
        (this->plan_.implementation)[my_instance.implementationRef];

      //      Deployment::DnC_Dump::dump (this->plan_);
      //Deployment::DnC_Dump::dump ( (this->plan_.implementation)[my_instance.implementationRef]);
      //ACE_DEBUG ((LM_DEBUG, "My implementation"));
      //Deployment::DnC_Dump::dump (my_implementation);

      CORBA::ULong index_imp = child_plan->implementation.length ();
      child_plan->implementation.length (++index_imp);
      child_plan->implementation[index_imp-1] = my_implementation;

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
          CORBA::ULong index_art = child_plan->artifact.length ();
          child_plan->artifact.length (++index_art);
          child_plan->artifact[index_art-1] =
            (this->plan_.artifact)[artifact_ref];

          // Fill in the <artifactRef> field of the MonolithicDeploymentDescription
          ulong_seq[iter] = index_art-1;
        }

      // Change the <artifactRef> field of the "implementation".
      child_plan->implementation[index_imp-1].artifactRef = ulong_seq;

      // Append the "InstanceDeploymentDescription instance" field with
      // a new "instance", which is almost the same as the "instance" in
      // the global plan except the <implementationRef> field.
      // NOTE: The <implementationRef> field needs to be changed accordingly.
      CORBA::ULong index_ins = child_plan->instance.length ();
      child_plan->instance.length (++index_ins);
      child_plan->instance[index_ins-1] = my_instance;

      // Change the <implementationRef> field of the "instance".
      // @@ The implementationRef starts from 0.
      // @@TODO: Below logic needs to be changed once we want to avoid
      // duplicate <implementation> for the optimization.
      child_plan->instance[index_ins-1].implementationRef = index_ins-1;
    }
}

::Deployment::Application_ptr
CIAO::DomainApplicationManager_Impl::
startLaunch (const ::Deployment::Properties & configProperty,
             ::Deployment::Connections_out providedReference)
  throw (::CORBA::SystemException,
	 ::Deployment::ResourceNotAvailable,
	 ::Deployment::StartError,
	 ::Deployment::InvalidProperty,
	 ::Deployment::InvalidNodeExecParameter,
	 ::Deployment::InvalidComponentExecParameter)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::startLaunch");

  try
    {
      auto_ptr<Connection_Manager> connections;
      ACE_AUTO_NEW_THROW_EX (connections,
			     Connection_Manager (this->plan_.connection),
			     ::Deployment::StartError ("DomainApplicationManager_Impl::startLaunch",
						       "Insufficient memory to allocate Connection_Manager"));
      			
      auto_ptr<DomainApplication_Impl::Node_Infos> applications;
      ACE_AUTO_NEW_THROW_EX (applications,
			     DomainApplication_Impl::Node_Infos (this->num_child_plans_),
			     ::Deployment::StartError ("DomainApplicationManager_Impl::startLaunch",
						       "Insufficient memory to allocate Connection_Manager"));

      // Invoke startLaunch() operations on each cached NodeApplicationManager
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeApplicationManager object reference.
          Node_Level_Plan_Entry *entry = 0;

          if (this->plan_table_.find (this->node_manager_names_[i],
				      entry) != 0)
            {
              CIAO_ERROR (1, (LM_ERROR,
                              "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                              "CIAO::DomainApplicationManager_Impl::startLaunch -"
                              "ERROR while finding the node specific plan "
                              "for the node [%s] \n",
                              this->node_manager_names_[i].c_str ()));

	      ACE_CString error
		("Unable to resolve a reference to node manager: ");
              error += this->node_manager_names_[i];

              throw Deployment::StartError
                ("DomainApplicationManager_Impl:startLaunch",
                 error.c_str ());
            }

          ::Deployment::NodeApplicationManager_ptr my_nam =
              (entry->int_id_).node_application_manager_.in ();

          if (CORBA::is_nil (my_nam))
            {
              CIAO_ERROR (1, (LM_ERROR,
                              "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                              "CIAO::DomainApplicationManager_Impl::startLaunch -"
                              "ERROR while narrowing the NAM "
                              "for the node [%s] \n",
                              this->node_manager_names_[i].c_str ()));
              
              ACE_CString error
                ("While starting launch, the DomainApplicationManager\
                  has a nil reference for NodeApplicationManager\n");
                            
              throw Deployment::StartError
                ("DomainApplicationManager_Impl::startLaunch",
                 error.c_str ());
            }

          ::Deployment::Connections_var retn_connections;

          // Obtained the returned NodeApplication object reference
          // and the returned Connections variable.
          ::Deployment::Application_var temp_application;
          temp_application = my_nam->startLaunch (configProperty,
                                                  retn_connections.out ());

          // Narrow down to NodeApplication object reference
	  ::Deployment::NodeApplication_var app = 
	      ::Deployment::NodeApplication::_narrow (temp_application.in ());

          if (CORBA::is_nil (app.in ()))
            {
              CIAO_ERROR (1, (LM_ERROR,
                              "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                              "CIAO::DomainApplicationManager_Impl::startLaunch -"
                              "ERROR while narrowing the NAM "
                              "for the node [%s] \n",
                              this->node_manager_names_[i].c_str ()));

              ACE_CString error
                ("The DomainApplicationManager receives a nil "
                 "reference of NodeApplication after calling "
                 "startLaunch on NodeApplicationManager.\n");

              throw Deployment::StartError
                ("DomainApplicationManager_Impl::startLaunch",
                 error.c_str ());
            }
	  
	  DomainApplication_Impl::Node_Info info = {app._retn (), 
						    ::Deployment::NodeApplicationManager::_duplicate (my_nam)};

	  // Bind the application reference
	  applications->bind (this->node_manager_names_[i],
			      info);

          // Cache the returned set of connections into the list.
          connections->add_connections (retn_connections.in ());          
        }
      
      // Construct DomainApplication and return it.
      CIAO::DomainApplication_Impl *da_servant = 0;
      CIAO_DEBUG (5, (LM_DEBUG, "CIAO (%P|%t) DAM - Creating new DA instance\n"));
      
      ACE_NEW_THROW_EX (da_servant,
                        CIAO::DomainApplication_Impl (this->orb_.in (),
						      this->poa_.in (),
						      applications.release (),
						      connections.release ()),
                        Deployment::StartError ("DomainApplicationManager",
                                                "Insufficient memory to create DAM."));
      
      PortableServer::ServantBase_var owner_transfer (da_servant);
      
      Deployment::DomainApplication_var da = 
        da_servant->_this ();
      
      if (CORBA::is_nil (da.in ()))
        {
          throw Deployment::StartError ("DomainApplicationManager",
                                        "Implicit activation of DA instance failed\n");
        }
      
      // Cache the DA object reference
      this->applications_.insert_tail (::Deployment::DomainApplication::_duplicate (da.in ()));
      
      return da._retn ();
    }
  catch (Deployment::StartError &ex)
    {
      CIAO_ERROR (1, (LM_DEBUG, "DAM_Impl:StartError: %s, %s\n",
                      ex.name.in (),
                      ex.reason.in ()));
      throw;
    }
  catch (CORBA::SystemException &ex)
    {
      CIAO_PRINT_EXCEPTION (1, ex,
                            "DomainApplicationManager_Impl::startLaunch\t\n");
      throw;
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, "DAM_Impl::startLaunch - caught unknown C++ exception"
                      ", throwing as start error\n"));
      throw Deployment::StartError ("DomainApplicationManager_Impl::startLaunch",
				    "Unknown C++ exception");
    }  
  
}

void
CIAO::DomainApplicationManager_Impl::
destroyApplication (Deployment::Application*)
  throw (CORBA::SystemException,
         ::Deployment::StopError)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::destroyApplication");
  /*
  try
    {
      // Passivate all components associated with the plan
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> *entry = 0;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                          "CIAO::DomainApplicationManager_Impl::destroyApplication -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
		("Unable to resolve a reference to NodeManager: ");
              error += this->node_manager_names_[i];

              ACE_TRY_THROW
                (Deployment::StopError
		 ("DomainApplicationManager_Impl::destroyApplication",
		  error.c_str ()));
            }
	  ACE_DEBUG ((LM_DEBUG, "DAM_Impl: Invoking passivate on %s\n",
		      this->node_manager_names_[i].c_str ()));

          // Invoke ciao_passivate () operation on each cached NodeApplication object.
          ::Deployment::NodeApplication_ptr my_na =
              (entry->int_id_).node_application_.in ();

          my_na->ciao_passivate ();
        }

      // Remove all connections associated with the plan
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> *entry = 0;

          this->artifact_map_.find (this->node_manager_names_[i], entry);

          Deployment::Connections_var connections =
            this->get_outgoing_connections (
					    (entry->int_id_).child_plan_.in (),
					    true,  // yes, get *all* the connections
					    true,  // yes, we search the current plan
					    DomainApplicationManager_Impl::External_Connections
					    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Invoke finishLaunch() on NodeApplication to remove bindings.
          // If this NodeApplication is not within the control of this DAM,
          // then we should delegate the call to the correct DAM through EM.

          // Iterave over the returned connection list in <connections>,
          // (1) If this is a shared component, then we remove this connection
          //     from <connections> list, and then populate a "Component_Binding_Info"
          //     struct for it and send to EM for remove.
          // (2) For the rest of the connections, we send them to the NAs
          //     managed by the local DAM to remove.

          for (CORBA::ULong j = 0; j < connections->length (); ++j)
            {
              if (this->is_shared_component (connections[j].instanceName.in ()))
                {
                  // ask EM to remove the binding for us
                  ACE_CString inst_name = connections[j].instanceName.in ();
                  CIAO::Component_Binding_Info *
                    binding = this->populate_binding_info (inst_name.c_str ());

                  this->execution_manager_->finalize_global_binding (
								     *binding, false);

                  // Remove all the connections whose "source" component
                  // is this component instance from the <connections> list
                  this->purge_connections (connections,
                                           inst_name.c_str ());
                }
            }

          if (connections->length () > 0)
            {
              entry->int_id_.node_application_->finishLaunch
                (connections.in (),
                 true, // "true" ==> start the components
                 false // "false" => remove connections
                 ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

        }

      // After all the connections are removed, we actually destroy
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> *entry = 0;

          this->artifact_map_.find (this->node_manager_names_[i], entry);

          // Invoke destroyPlan() operation on the NodeManager
          Deployment::NodeManager_var
	    my_node_manager = (entry->int_id_).node_manager_;

          my_node_manager->destroyPlan ((entry->int_id_).child_plan_
                                        ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // ??
      // Shall we invoke destroyManager () operation on the NodeManager, since we need
      // to clean up all the NodeApplicationManagers associated with this deployment
      // plan (one NodeApplicationManager per Node per plan).

      // We should again activate those shared components
      this->activate_shared_components ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "DomainApplicationManager_Impl::destroyApplication\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  ACE_CHECK;
  */
}

// Returns the DeploymentPlan associated with this ApplicationManager.
::Deployment::DeploymentPlan *
CIAO::DomainApplicationManager_Impl::
getPlan (void)
  throw (CORBA::SystemException)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::getPlan");
  Deployment::DeploymentPlan_var plan = 0;
  // Make a deep copy of the Plan
  ACE_NEW_THROW_EX (plan,
                    Deployment::DeploymentPlan (this->plan_),
                    CORBA::NO_MEMORY ());

  // Transfer ownership
  return plan._retn ();
}
/*
void
CIAO::DomainApplicationManager_Impl::
dump_connections (const ::Deployment::Connections & connections)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::dump_connections");
  const CORBA::ULong conn_len = connections.length ();
  for (CORBA::ULong i = 0; i < conn_len; ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "instanceName: %s\n", connections[i].instanceName.in ()));

      ACE_DEBUG ((LM_DEBUG, "portName: %s\n", connections[i].portName.in ()));

      ACE_DEBUG ((LM_DEBUG, "portkind: "));

      switch (connections[i].kind)
        {
	case Deployment::Facet:

	  ACE_DEBUG ((LM_DEBUG, "Facet\n"));
	  break;

	case Deployment::SimplexReceptacle:

	  ACE_DEBUG ((LM_DEBUG, "SimplexReceptacle\n"));
	  break;

	case Deployment::MultiplexReceptacle:

	  ACE_DEBUG ((LM_DEBUG, "MultiplexReceptacle\n"));
	  break;

	case Deployment::EventEmitter:

	  ACE_DEBUG ((LM_DEBUG, "EventEmitter\n"));
	  break;

	case Deployment::EventPublisher:

	  ACE_DEBUG ((LM_DEBUG, "EventPublisher\n"));
	  break;

	case Deployment::EventConsumer:

	  ACE_DEBUG ((LM_DEBUG, "EventConsumer\n"));
	  break;

        default:
          ACE_DEBUG ((LM_DEBUG, "Unknown port kind.\n"));
        }

      ACE_DEBUG ((LM_DEBUG,
                  "endpointInstanceName: %s\n",
                  connections[i].endpointInstanceName.in ()));

      ACE_DEBUG ((LM_DEBUG,
		  "endpointPortName: %s\n",
		  connections[i].endpointPortName.in ()));
      ACE_DEBUG ((LM_DEBUG, "---------------------\n"));
    }
}
*/
