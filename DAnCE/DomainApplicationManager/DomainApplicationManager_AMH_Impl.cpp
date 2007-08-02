// $Id$

#include "DomainApplicationManager_AMH_Impl.h"
#include "ExecutionManager/Execution_Manager_Impl.h"
#include "DAnCE/Deployment/Deployment_NodeApplicationManagerC.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_string.h"
#include "ace/SString.h"
#include "ace/Assert.h"

#if !defined (__ACE_INLINE__)
# include "DomainApplicationManager_AMH_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::DomainApplicationManager_AMH_Impl::
DomainApplicationManager_AMH_Impl (CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               Deployment::TargetManager_ptr manager,
                               Execution_Manager::Execution_Manager_Impl * em,
                               const Deployment::DeploymentPlan & plan,
                               const char * deployment_file)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    execution_manager_ (em), // a plain C++ pointer
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
    deployment_config_ (orb),
    uuid_ (plan.UUID),
    is_redeployment_ (false),
    esd_ (0)
{
  ACE_NEW_THROW_EX (this->all_connections_,
                    Deployment::Connections (),
                    CORBA::NO_MEMORY ());

  ACE_NEW_THROW_EX (this->shared_,
                    Deployment::ComponentPlans (),
                    CORBA::NO_MEMORY ());

  ACE_NEW_THROW_EX (this->esd_,
                    CIAO::DAnCE::EventServiceDeploymentDescriptions (),
                    CORBA::NO_MEMORY ());

  for (CORBA::ULong i = 0; i < this->plan_.infoProperty.length (); ++i)
    {
      if (ACE_OS::strcmp (this->plan_.infoProperty[i].name.in (),
                          "CIAOEvents") != 0)
        continue;

      // Note, we should do a sanity check here to make
      // sure we didn't pick up the wrong infoProperty!!
      this->plan_.infoProperty[0].value >>= this->esd_;
      break;
    }

  this->init ();
}

CIAO::DomainApplicationManager_AMH_Impl::~DomainApplicationManager_AMH_Impl ()
{
  if (CIAO::debug_level () > 1)
    {
      ACE_DEBUG ((LM_DEBUG, "DomainApplicationManager destroyed\n"));
    }
}

void
CIAO::DomainApplicationManager_AMH_Impl::get_node_app (
  Deployment::AMH_DomainApplicationManagerResponseHandler_ptr _tao_rh,
  const char * node_name)
{
  // Get the NodeApplication object reference.
  ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> *entry = 0;

  if (this->artifact_map_.find (node_name, entry) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                  "CIAO::DomainApplicationManager_AMH_Impl::get_node_app -"
                  "ERROR while finding the node application "
                  "for the node [%s] \n",
                  node_name));
      throw Deployment::NoSuchName ();
    }

  _tao_rh->get_node_app (
    Deployment::NodeApplication::_duplicate (
      entry->int_id_.node_application_.in ()));
}

void
CIAO::DomainApplicationManager_AMH_Impl::init ()
{
  try
    {
      // (1) Call get_plan_info() method to get the total number
      //     of child plans and list of NodeManager names, and
      // (2) Check the validity of the global deployment plan.
      if (! this->get_plan_info ())
        {
          ACE_ERROR ((LM_ERROR,
                      "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                      "CIAO::DomainApplicationManager_AMH_Impl::init -"
                      "ERROR while calling get_plan_info () \n"));
          throw Deployment::PlanError ();
        }

      // Call split_plan()
      if (! this->split_plan ())
        {
          ACE_ERROR ((LM_ERROR,
                      "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                      "CIAO::DomainApplicationManager_AMH_Impl::init -"
                      "ERROR while calling split_plan () \n"));
          throw Deployment::PlanError ();
        }

      // Invoke preparePlan on each NodeManager by giving child plan.
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeManager object reference.
          ::Deployment::NodeManager_var my_node_manager =
            this->deployment_config_.get_node_manager
              (this->node_manager_names_[i].c_str ());

          // Get the child deployment plan reference.
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> *entry;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                          "CIAO::DomainApplicationManager_AMH_Impl::init -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));
              throw Deployment::PlanError ();
            }

          Chained_Artifacts & artifacts = entry->int_id_;

          // Call preparePlan() method on the NodeManager with the
          // corresponding child plan as input, which returns a
          // NodeApplicationManager object reference.
          Deployment::ApplicationManager_var tmp_app_manager =
            my_node_manager->preparePlan (artifacts.child_plan_.in ());

          Deployment::NodeApplicationManager_var app_manager =
            Deployment::NodeApplicationManager::_narrow
              (tmp_app_manager.in ());

          if (CORBA::is_nil (app_manager.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                          "CIAO::DomainApplicationManager_AMH_Impl::init -"
                          "ERROR while narroing the NAM "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error ("DomainAppMgr::init () received a nil \
                                 reference for NodeApplicationManager\n");

              ACE_DEBUG ((LM_DEBUG, error.c_str ()));
              throw
                 (Deployment::StartError ("DomainApplicationManager_AMH_Impl:init",
                                          error.c_str ()));
            }

          // Cache the NodeApplicationManager object reference.
          artifacts.node_application_manager_ = app_manager._retn ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("DomainApplicationManager_AMH_Impl::init\t\n");
      throw;
    }
}

bool
CIAO::DomainApplicationManager_AMH_Impl::get_plan_info (void)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_AMH_Impl::get_plan_info");

  // Read the deployment.dat file and get to know the different nodes
  // that are present in the deployment domain.
  // We should be able to replace this way, by parsing the domain
  // descriptor to get to know more on the domain.
  //
  if ( this->deployment_config_.init (this->deployment_file_) == -1 )
    {
      ACE_ERROR ((LM_ERROR,
                  "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                  "CIAO::DomainApplicationManager_AMH_Impl::get_plan_info -"
                  "ERROR while trying to initialize after reading "
                  "deployment DAT file \n"));
      return 0;
    }

  // Error: If there are no nodes in the plan => No nodes to deploy the
  // components
  CORBA::ULong const length = this->plan_.instance.length ();

  if (length == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                  "CIAO::DomainApplicationManager_AMH_Impl::get_plan_info -"
                  "ERROR while trying to get the total instances to "
                  "be deployed \n"));
      return false;
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
  int num_plans = 0;
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
            return false; /* Failure */

          // Add this unique node_name to the list of NodeManager names
          this->node_manager_names_.push_back (this->plan_.instance [index].node.in ());

          // Increment the number of plans
          ++num_plans;
        }
    }

  // Set the length of the Node Managers
  //
  // why cannot we use the provate variable in the above "for" loop?
  //
  this->num_child_plans_ = num_plans;

  // Indicate success
  return true;
}

//@@ We should ask those spec writers to look at the code below, hopefully
//   They will realize some thing.
int
CIAO::DomainApplicationManager_AMH_Impl::split_plan (void)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_AMH_Impl::split_plan");

  // Initialize the total number of child deployment plans specified
  // by the global plan.
  CORBA::ULong i;

  for ( i = 0; i < this->num_child_plans_; ++i)
  {
    ::Deployment::DeploymentPlan_var tmp_plan;
    ACE_NEW_RETURN (tmp_plan,
                    ::Deployment::DeploymentPlan,
                    0);

    // Construct the UUID for the child plan.
    // Currently, this UUID consists of:
    // The global UUID for the parent plan + the name of the node the
    // child plan is to be installed on.
    ACE_CString child_uuid (this->plan_.UUID.in ());
    child_uuid += "@";
    child_uuid += this->node_manager_names_[i];

    tmp_plan->UUID = CORBA::string_dup (child_uuid.c_str ());
    tmp_plan->implementation.length (0);
    tmp_plan->instance.length (0);
    tmp_plan->connection.length (0);
    tmp_plan->externalProperty.length (0);
    tmp_plan->dependsOn.length (0);
    tmp_plan->artifact.length (0);

    // @@ There is an optimization point here, since we really don't have to
    // pass the entire CIAOServerResources into each individual child plan.
    tmp_plan->infoProperty = this->plan_.infoProperty;

    //tmp_plan->infoProperty[0].name = CORBA::string_dup ("CIAOServerResource");
    //tmp_plan->infoProperty[0].value = this->plan_.infoProperty[0].value;

    Chained_Artifacts artifacts;

    // Fill in the child_plan_ field, relinquishing ownership
    artifacts.child_plan_ = tmp_plan._retn ();

    // Fill in the node_manager_ field.
    artifacts.node_manager_ =
      this->deployment_config_.get_node_manager
              (this->node_manager_names_[i].c_str ());

    ACE_Hash_Map_Entry
      <ACE_CString,
      Chained_Artifacts> *entry = 0;

    if (this->artifact_map_.find
        (node_manager_names_[i], entry) == 0)
      artifacts.old_child_plan_ = (entry->int_id_).child_plan_;
    else
      artifacts.old_child_plan_ = 0;

    // In case we are doing redeployment, rebind will help replace the
    // old child plan with the new child plan.
    this->artifact_map_.rebind (node_manager_names_[i], artifacts);
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
      ACE_Hash_Map_Entry
        <ACE_CString,
        Chained_Artifacts> *entry = 0;

      if (this->artifact_map_.find
          (ACE_CString (my_instance.node.in ()),
                        entry) != 0)
        return 0;                          // no valid name found.

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

  return 1;
}

void
CIAO::DomainApplicationManager_AMH_Impl::
add_connections (const Deployment::Connections & incoming_conn)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_AMH_Impl::add_connections");

  CORBA::ULong const old_len = this->all_connections_->length ();

  // Expand the length of the <all_connection_> sequence.
  this->all_connections_->length (old_len + incoming_conn.length ());

  // Store the connections to the <all_conections_> sequence
  const CORBA::ULong conn_lenth = incoming_conn.length ();
  for (CORBA::ULong i = 0; i < conn_lenth; ++i)
  {
    (*this->all_connections_)[old_len + i] = incoming_conn[i];
  }
}

void
CIAO::DomainApplicationManager_AMH_Impl::
startLaunch (Deployment::AMH_DomainApplicationManagerResponseHandler_ptr _tao_rh,
             const ::Deployment::Properties & configProperty,
             ::CORBA::Boolean start)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_AMH_Impl::startLaunch");
  ACE_UNUSED_ARG (start);

  try
    {
      // Clean up all cached connections first
      this->all_connections_->length (0);

      // Invoke startLaunch() operations on each cached NodeApplicationManager
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeApplicationManager object reference.
          ACE_Hash_Map_Entry
            <ACE_CString,
            Chained_Artifacts> *entry = 0;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                          "CIAO::DomainApplicationManager_AMH_Impl::startLaunch -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                 ("Unable to resolve a reference to node manager: ");
              error += this->node_manager_names_[i];

              throw
                 (Deployment::StartError
                    ("DomainApplicationManager_AMH_Impl:startLaunch",
                     error.c_str ()));
            }

          ::Deployment::NodeApplicationManager_ptr my_nam =
            (entry->int_id_).node_application_manager_.in ();

          if (CORBA::is_nil (my_nam))
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                          "CIAO::DomainApplicationManager_AMH_Impl::startLaunch -"
                          "ERROR while narrowing the NAM "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                 ("While starting launch, the DomainApplicationManager\
                  has a nil reference for NodeApplicationManager\n");
              ACE_DEBUG ((LM_DEBUG, error.c_str ()));

              throw
                (Deployment::StartError
                   ("DomainApplicationManager_AMH_Impl::startLaunch",
                     error.c_str ()));
            }


          ::Deployment::Connections_var retn_connections;

          // Obtained the returned NodeApplication object reference
          // and the returned Connections variable.
          ::Deployment::Application_var temp_application;
          if (!is_redeployment_)
            {
              temp_application =
                my_nam->startLaunch (configProperty,
                                     retn_connections.out (),
                                     0);
            }
          else
            {
              //=============================================================
              //                  Add New Components Logic
              //=============================================================
              // Let's add new components only now, the to-be-removed
              // components should be removed AFTER the connections
              // are removed.
              temp_application =
                my_nam->perform_redeployment (configProperty,
                                              retn_connections.out (),
                                              true, // add new components only now
                                              0);
            }

          // Narrow down to NodeApplication object reference
          ::Deployment::NodeApplication_var my_na =
            ::Deployment::NodeApplication::_narrow (temp_application.in ());

          if (CORBA::is_nil (my_na.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                          "CIAO::DomainApplicationManager_AMH_Impl::startLaunch -"
                          "ERROR while narrowing the NAM "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                ("The DomainApplicationManager receives a nil "
                 "reference of NodeApplication after calling "
                 "startLaunch on NodeApplicationManager.\n");
              ACE_ERROR ((LM_ERROR, error.c_str ()));

              throw
                (Deployment::StartError
                   ("DomainApplicationManager_AMH_Impl::startLaunch",
                     error.c_str ()));
            }

          // Cache the returned set of connections into the list.
          this->add_connections (retn_connections.in ());

          // Cache the returned NodeApplication object reference into
          // the hash table.
          (entry->int_id_).node_application_ = my_na._retn ();
        }

      this->synchronize_shared_components_with_node_managers ();
    }
  catch (const Deployment::StartError& ex)
    {
      ACE_DEBUG ((LM_DEBUG, "DAM_Impl:StartError: %s, %s\n",
      ex.name.in (),
      ex.reason.in ()));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "DomainApplicationManager_AMH_Impl::startLaunch\t\n");
      throw;
    }

  _tao_rh->startLaunch ();

}


void
CIAO::DomainApplicationManager_AMH_Impl::install_all_es (void)
{
  try
    {
      for (CORBA::ULong j = 0; j < this->esd_->length (); ++j)
        {
          // Find NA, and then invoke operation on it
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> *entry = 0;

          if (this->artifact_map_.find (this->esd_[j].node.in (),
                                        entry) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                          "CIAO::DomainApplicationManager_AMH_Impl::install_all_es -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                           this->esd_[j].node.in ()));

              ACE_CString error
                ("Unable to resolve a reference to NodeManager: ");
              error += this->esd_[j].node.in ();

              throw
                (Deployment::StartError
                  ("DomainApplicationManager_AMH_Impl::install_all_es",
                    error.c_str ()));
            }

          // Invoke install_es () operation on each cached NodeApplication object.
          ::Deployment::NodeApplication_ptr my_na =
              (entry->int_id_).node_application_.in ();

          ::CIAO::CIAO_Event_Service_var ciao_es =
              my_na->install_es (this->esd_[j]);

          // Add these returned ES objects into the cached map
          this->add_es_to_map (this->esd_[j].name.in (), ciao_es.in ());
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "DomainApplicationManager_AMH_Impl::install_all_es.\n");
      throw;
    }

}

void
CIAO::DomainApplicationManager_AMH_Impl::
add_es_to_map (const char * node_name,
               CIAO::CIAO_Event_Service * ciao_es)
{
  try
    {
      this->es_map_.bind (
        node_name,
        CIAO::CIAO_Event_Service::_duplicate (ciao_es));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "DomainApplicationManager_AMH_Impl::add_es_to_map.\n");
      throw;
    }

}

void
CIAO::DomainApplicationManager_AMH_Impl::
finishLaunch (Deployment::AMH_DomainApplicationManagerResponseHandler_ptr _tao_rh,
              CORBA::Boolean start,
              CORBA::Boolean is_ReDaC)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_AMH_Impl::finishLaunch");
  try
    {
      // Install all the CIAO_Event_Services within the Deployment Plan
      this->install_all_es ();

      // Invoke finishLaunch() operation on each cached NodeApplication object.
      // This will establish bindings for only those internal components, but
      // NOT for those external/shared components, which requires special
      // handling, since these components are outside the control of this
      // DomainApplicationManager.
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeApplication object reference.
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> * entry = 0;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                          "CIAO::DomainApplicationManager_AMH_Impl::finishLaunch -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                 ("Unable to resolve a reference to NodeManager: ");
              error += this->node_manager_names_[i];

              throw
                (Deployment::StartError
                   ("DomainApplicationManager_AMH_Impl::finishLaunch",
                     error.c_str ()));
            }

          //@@ Note: Don't delete the below debugging helpers.
          // Dump the connections for debug purpose.
          if (CIAO::debug_level () > 9)
            {
              ACE_DEBUG ((LM_DEBUG,
                 "==============================================\n"));
              ACE_DEBUG ((LM_DEBUG,
                 "dump incoming connections for child plan:%s\n",
                 (entry->int_id_).child_plan_->UUID.in ()));
              dump_connections (this->all_connections_.in ());
              ACE_DEBUG ((LM_DEBUG,
                  "==============================================\n"));
            }

          // Get the Connections variable, if ReDaC is true, then we get
          // those new connections only. NOTE: get_outgoing_connections
          // by default will get *all* connections.
          Deployment::Connections * my_connections =
            this->get_outgoing_connections (
              (entry->int_id_).child_plan_.in (),
              !is_ReDaC,
              true,  // we search *new* plan
              DomainApplicationManager_AMH_Impl::Internal_Connections);

          if (my_connections == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                          "CIAO::DomainApplicationManager_AMH_Impl::finishLaunch -"
                          "ERROR while getting the outgoing connections "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              throw
                (Deployment::StartError
                  ("DomainApplicationManager_AMH_Impl::finish_launch",
                    "There was some error establishing connections."));
            }

          // Dump the connections for debug purpose.
          if (CIAO::debug_level () > 1)
            {
              ACE_DEBUG ((LM_DEBUG,
                  "==============================================\n"));
              ACE_DEBUG ((LM_DEBUG,
                  "dump outgoing connections for child plan:%s\n",
                 (entry->int_id_).child_plan_->UUID.in ()));
              dump_connections (*my_connections);
              ACE_DEBUG ((LM_DEBUG,
                  "==============================================\n"));
            }

          // Invoke finishLaunch() operation on NodeApplication.
          if (my_connections->length () != 0)
            {
              entry->int_id_.node_application_->finishLaunch
                 (*my_connections,
                  start,
                  true // "true" => establish new connections only
                  );
            }

          //=============================================================
          //                  Remove Old Connections Logic
          //=============================================================

          if (is_ReDaC) // We should also *REMOVE* unnecessary connections
            {
              // If this is a brand new child plan, then continue.
              if ((entry->int_id_).old_child_plan_ == 0)
                continue;

              // Get all the connections in the old deployment plan
              Deployment::Connections * connections_in_old_plan =
                this->get_outgoing_connections (
                        (entry->int_id_).old_child_plan_.in (),
                        true, // yes, get *all* the connections
                        false, // search in the *old* plan
                        DomainApplicationManager_AMH_Impl::Internal_Connections);

              // Pass in the "false" parameter to get *all* the connections in
              // the new deployment plan, regardless those in old plan
              Deployment::Connections * connections_in_new_plan =
                this->get_outgoing_connections (
                        (entry->int_id_).child_plan_.in (),
                        true, // yes, get *all* the connections
                        true,  // search in the *new* plan
                        DomainApplicationManager_AMH_Impl::Internal_Connections);

              Deployment::Connections * unnecessary_connections =
                this->subtract_connections (*connections_in_old_plan,
                                            *connections_in_new_plan);

              delete connections_in_old_plan;
              delete connections_in_new_plan;

              // Dump the connections for debug purpose.
              if (CIAO::debug_level () > 1)
                {
                  ACE_DEBUG ((LM_DEBUG,
                      "==============================================\n"));
                  ACE_DEBUG ((LM_DEBUG,
                      "dump to-be-removed connections for child plan:%s\n",
                      (entry->int_id_).child_plan_->UUID.in ()));
                  dump_connections (*unnecessary_connections);
                  ACE_DEBUG ((LM_DEBUG,
                      "==============================================\n"));
                }

              // Invoke finishLaunch() operation on NodeApplication.
              if (unnecessary_connections->length () != 0)
                {
                  if (CORBA::is_nil (entry->int_id_.node_application_.in ()))
                    throw Deployment::StartError ();

                  entry->int_id_.node_application_->finishLaunch
                    (*unnecessary_connections,
                     start,
                     false  // false ==> remove unnecessary connections
                     );
                }

              //=============================================================
              //                  Remove Old Components
              //=============================================================
              // Finally we need to remove those to-be-removed components
              ::Deployment::Properties_var configProperty;
              ACE_NEW (configProperty,
                       Deployment::Properties);

              ::Deployment::Connections_var retn_connections;

              Deployment::Application_var temp_application =
                entry->int_id_.node_application_manager_->
                   perform_redeployment (configProperty,
                                         retn_connections.out (),
                                         false, // remove old components only
                                         false);// do not "start"
            }
        }

      // Establish bindings on external/shared components of this deployment plan.
      this->post_finishLaunch ();
    }
  catch (const Deployment::StartError& ex)
    {
      ACE_ERROR ((LM_ERROR, "DAM_Impl::finishLaunch - StartError: %s, %s\n",
      ex.name.in (),
      ex.reason.in ()));
   }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "DomainApplicationManager_AMH_Impl::finishLaunch: ""Killing NodeApplications.\n");

      // Invoke destroyManager() operation on each cached
      // NodeManager object.
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeManager and NodeApplicationManager object references.
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> * entry = 0;
          this->artifact_map_.find (this->node_manager_names_[i],entry);

          ::Deployment::NodeApplicationManager_ptr
              my_node_application_manager =
                  (entry->int_id_).node_application_manager_.in ();

          // Invoke destoryApplication() operation on the NodeApplicationManger.
          // Since we have the first arg is not used by NAM anyway.
          my_node_application_manager->destroyApplication
             (0);
        }
      throw;
    }

  _tao_rh->finishLaunch ();
}

void
CIAO::DomainApplicationManager_AMH_Impl::
post_finishLaunch (void)
{
  try
    {
      // For each "external" component...
      CORBA::ULong length = this->shared_->length ();
      for (CORBA::ULong j = 0; j < length; ++j)
        {
          // Construct <Component_Binding_Info> struct for the component
          CIAO::Component_Binding_Info *
              binding = this->populate_binding_info (
                this->shared_[j].name.in (),
                this->shared_[j].plan_uuid.in ());

          // Invoke <finalize_global_binding> on ExecutionManager
          this->execution_manager_->finalize_global_binding (
            *binding, true);

          delete binding;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "DomainApplicationManager_AMH_Impl::post_finishLaunch.\n");
      throw;
    }

}

CIAO::Component_Binding_Info *
CIAO::DomainApplicationManager_AMH_Impl::
populate_binding_info (const ACE_CString& name,
                       const ACE_CString& child_uuid)
{
  CIAO::Component_Binding_Info * retv;
  ACE_NEW_RETURN (retv,
                  CIAO::Component_Binding_Info (),
                  0);

  retv->name_ = name;

  // Parse the child_uuid string and populate the "node" name and
  // "plan_uuid" fields.  Our protocol here is searching for the "@",
  // the substring *before* that is the global plan uuid, and the
  // substring *after* that is the node name.
  ACE_CString::size_type pos = child_uuid.find ('@');

  retv->plan_uuid_ =
    child_uuid.substring (0, pos);

  retv->node_ =
    child_uuid.substring (pos + 1,
                          ACE_CString::npos); // get the rest of the string

  Deployment::Connections_var connections;
  ACE_NEW_RETURN (connections,
                  Deployment::Connections,
                  0);

  this->get_outgoing_connections_i (name.c_str (),
                                    connections.inout (),
                                    false,  // get *all* connections
                                    true);  // search current plan

  retv->providedReference_ = connections._retn ();

  return retv;
}

CIAO::Component_Binding_Info *
CIAO::DomainApplicationManager_AMH_Impl::
populate_binding_info (const ACE_CString& name)
{
  CIAO::Component_Binding_Info * retv;
  ACE_NEW_RETURN (retv,
                  CIAO::Component_Binding_Info (),
                  0);

  // Looking for the child plan uuid through the shared compoonent list
  for (CORBA::ULong i = 0; i < this->shared_->length (); ++i)
    {
      if (ACE_OS::strcmp (this->shared_[i].name.in (),
                          name.c_str ()) == 0)
        {
          ACE_CString child_uuid = this->shared_[i].plan_uuid.in ();
          retv = this->populate_binding_info (name, child_uuid);
          return retv;
        }
    }

  return 0; // If no matching is found (should never happen).
}

void
CIAO::DomainApplicationManager_AMH_Impl::
add_shared_components (const Deployment::ComponentPlans & shared)
{
  for (CORBA::ULong i = 0; i < shared.length (); ++i)
    {
      CORBA::ULong curr_len = this->shared_->length ();
      this->shared_->length (curr_len + 1);
      this->shared_[curr_len] = shared[i];
    }
}

bool
CIAO::DomainApplicationManager_AMH_Impl::
is_shared_component (const char * name)
{
  for (CORBA::ULong i = 0; i < this->shared_->length (); ++i)
    {
      if (ACE_OS::strcmp (this->shared_[i].name.in (),
                          name) == 0)
        return true;
    }

  return false;
}

void
CIAO::DomainApplicationManager_AMH_Impl::
synchronize_shared_components_with_node_managers (void)
{
  for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
    {
      // Get the NodeManager object reference.
      ACE_Hash_Map_Entry
        <ACE_CString,
        Chained_Artifacts> *entry = 0;

      if (this->artifact_map_.find (this->node_manager_names_[i],
                                    entry) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                      "CIAO::DomainApplicationManager_AMH_Impl::startLaunch -"
                      "ERROR while finding the node specific plan "
                      "for the node [%s] \n",
                      this->node_manager_names_[i].c_str ()));

          ACE_CString error
              ("Unable to resolve a reference to node manager: ");
          error += this->node_manager_names_[i];

          throw
              (Deployment::StartError
                ("DomainApplicationManager_AMH_Impl:startLaunch",
                  error.c_str ()));
        }

      // Update the shared components  list
      ::Deployment::NodeManager_ptr
          my_node_manager = (entry->int_id_).node_manager_.in ();

      Deployment::ComponentPlans_var
        shared = my_node_manager->get_shared_components ();

      this->add_shared_components (shared.in ());
    }
}


Deployment::Connections *
CIAO::DomainApplicationManager_AMH_Impl::
get_outgoing_connections (const Deployment::DeploymentPlan &plan,
                          bool is_getting_all_connections,
                          bool is_search_new_plan,
                          Connection_Search_Type t)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_AMH_Impl::get_outgoing_connections");
  Deployment::Connections_var connections;
  ACE_NEW_RETURN (connections,
                  Deployment::Connections,
                  0);

  // For each component instance in the child plan ...
  CORBA::ULong number = plan.instance.length ();
  for (CORBA::ULong i = 0; i < number; ++i)
  {

    if (t == Internal_Connections &&
        this->is_shared_component (plan.instance[i].name.in ()))
      continue;

    // Get the outgoing connections of the component
    if (!get_outgoing_connections_i (plan.instance[i].name.in (),
                                     connections.inout (),
                                     is_getting_all_connections,
                                     is_search_new_plan))
      return 0;
  }
  return connections._retn ();
}


// Get outgoing connections for particular instance
bool
CIAO::DomainApplicationManager_AMH_Impl::
get_outgoing_connections_i (const char * instname,
                            Deployment::Connections & retv,
                            bool is_getting_all_connections,
                            bool is_search_new_plan)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_AMH_Impl::get_outoing_connections_i");

  Deployment::DeploymentPlan tmp_plan;
  if (is_search_new_plan)
    tmp_plan = this->plan_;
  else
    tmp_plan = this->old_plan_;

  // Search for all the bindings in the plan.
  const CORBA::ULong total_length = tmp_plan.connection.length();
  for (CORBA::ULong i = 0; i < total_length; ++i)
    {
      // Current binding that we are looking at.
      const Deployment::PlanConnectionDescription & binding =
        tmp_plan.connection[i];

      // If this is a ReDaC case, then we ignore the connection if it
      // already exists in the old_plan
      if (already_exists (binding) &&
          !is_getting_all_connections)
        continue;

      this->populate_connection_for_binding (instname,
                                             binding,
                                             tmp_plan,
                                             retv);
    }
  return true;
}

// Search the current binding to see whether a connection of this
// component needs to be populated
bool
CIAO::DomainApplicationManager_AMH_Impl::
populate_connection_for_binding (
     const char * instname,
     const Deployment::PlanConnectionDescription & binding,
     const Deployment::DeploymentPlan & plan,
     Deployment::Connections & retv)
{
  const CORBA::ULong binding_len = binding.internalEndpoint.length ();

  // If only 1 internalEndpoint, then we know it's through event service.
  if (binding_len == 1)
    {
      return
        this->handle_es_connection (instname,
                                    binding,
                                    plan,
                                    retv);
    }
  else if (binding_len == 2)
    {
      return this->handle_direct_connection (instname,
                                             binding,
                                             plan,
                                             retv);
    }
  else // invalid binding encounted...
    return false;
}

bool
CIAO::DomainApplicationManager_AMH_Impl::
handle_es_connection (
     const char * instname,
     const Deployment::PlanConnectionDescription & binding,
     const Deployment::DeploymentPlan & plan,
     Deployment::Connections & retv)
{
  // The initial retv might have something inside
  CORBA::ULong len = retv.length ();

  const Deployment::PlanSubcomponentPortEndpoint & endpoint =
    binding.internalEndpoint[0];

  // If the instance name does NOT match one of the names in the binding
  // Then thsi binding has nothing related to myself (an instance)
  if (ACE_OS::strcmp (instname,
                      plan.instance[endpoint.instanceRef].name.in ()) != 0)
    return false;

  if (binding.externalReference.length () != 1)
    {
      ACE_ERROR ((LM_ERROR,
                  "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                  "CIAO::DomainApplicationManager_AMH_Impl::"
                  "handle_publisher_es_connection -"
                  "externalReference must have length of 1.\n"));
      return false;
    }

  retv.length (len+1);
  retv[len].instanceName = instname;
  retv[len].portName = endpoint.portName.in ();
  retv[len].kind = endpoint.kind;

  ACE_CString es_id = binding.externalReference[0].location.in ();

  retv[len].endpointInstanceName = es_id.c_str ();
  retv[len].endpointPortName = CORBA::string_dup ("CIAO_ES");

  // We need to populate the actual filter and store it into
  // the <connection.config> field
  if (binding.deployRequirement.length () != 0)
    {
      retv[len].config =
        * (this->get_connection_QoS_configuration (binding.deployRequirement[0]));
    }

  // If we didnt find the objref of the connection ...
  CIAO::CIAO_Event_Service_var es;

  if (this->es_map_.find (es_id.c_str (), es) != 0)
    {
      ACE_CString error ("Creating connections for ");
      error += instname;
      error += ": unable to find object reference for connection ";
      error += binding.name.in ();
      throw Deployment::StartError(
        "DomainApplicationManager_AMH_Impl::create_connections_i",
        error.c_str ());
    }

  retv[len].event_service = es._retn ();

  // One more thing needs to be done if this is a es_consumer connection
  // We need to populate the <endpoint> field of this connection, which
  // is the object reference of this event consumer port.
  if (endpoint.kind == Deployment::EventConsumer)
    {
      // Now we search in the received connections to get the objRef of event sink
      bool found = false;
      const CORBA::ULong all_conn_len = this->all_connections_->length ();
      for (CORBA::ULong j = 0; j < all_conn_len; ++j)
        {
          const Deployment::Connection & curr_recv_conn =
              this->all_connections_[j];

          // We need to look at the instance name and the port name to confirm.
          if (ACE_OS::strcmp (curr_recv_conn.instanceName.in (),
                              instname) == 0 &&
              ACE_OS::strcmp (curr_recv_conn.portName.in (),
                              endpoint.portName.in ()) == 0)
            {
              retv[len].endpoint =
                  CORBA::Object::_duplicate(curr_recv_conn.endpoint.in ());
              found = true;
            }
        } // End of searching received connections

      if (!found)
        {
          ACE_CString error ("Creating connections for ");
          error += instname;
          error += ": unable to find object reference for connection ";
          error += binding.name.in ();
          throw Deployment::StartError(
            "DomainApplicationManager_AMH_Impl::handle_es_connection",
            error.c_str ());
        }
    }

  return true;
}

bool
CIAO::DomainApplicationManager_AMH_Impl::
handle_direct_connection (
     const char * instname,
     const Deployment::PlanConnectionDescription & binding,
     const Deployment::DeploymentPlan & plan,
     Deployment::Connections & retv)
{
  // The initial retv might have something inside
  CORBA::ULong len = retv.length ();

  const CORBA::ULong binding_len = binding.internalEndpoint.length ();
  for (CORBA::ULong i = 0; i < binding_len; ++i)
    {
      const Deployment::PlanSubcomponentPortEndpoint & endpoint =
        binding.internalEndpoint[i];

      // If the instance name does NOT match one of the names in the binding
      if (ACE_OS::strcmp (instname,
                          plan.instance[endpoint.instanceRef].name.in ()) != 0)
        continue;

      // We are only interested when we are the "client" of the endpoint objref
      if (endpoint.kind == Deployment::EventPublisher ||
          endpoint.kind == Deployment::EventEmitter ||
          endpoint.kind == Deployment::SimplexReceptacle ||
          endpoint.kind == Deployment::MultiplexReceptacle )
        {
          // Obtain the index of the "real" endpoint which has an objref. It
          // is the opposite side of myself.
          CORBA::ULong e_index = (i + 1) % 2;

          ACE_CString source_port =
            binding.internalEndpoint[i].portName.in ();

          ACE_CString endpoint_inst =
            plan.instance[binding.internalEndpoint[e_index]
              .instanceRef].name.in ();

          ACE_CString endpoint_port =
            binding.internalEndpoint[e_index].portName.in ();

          bool found = false;

          // Now we search in the received connections to get the objRef.
          const CORBA::ULong all_conn_len = this->all_connections_->length ();
          for (CORBA::ULong j = 0; j < all_conn_len; ++j)
            {
              const Deployment::Connection & curr_recv_conn =
                  this->all_connections_[j];

              // We need to look at the instance name and the port name to confirm.
              if (ACE_OS::strcmp (curr_recv_conn.instanceName.in (),
                                  endpoint_inst.c_str ()) == 0 &&
                  ACE_OS::strcmp (curr_recv_conn.portName.in (),
                                  endpoint_port.c_str ()) == 0)
                  {
                    retv.length (len+1);
                    retv[len].instanceName = instname;
                    retv[len].portName = source_port.c_str ();
                    retv[len].endpointInstanceName = endpoint_inst.c_str ();
                    retv[len].endpointPortName = endpoint_port.c_str ();

                    retv[len].endpoint =
                      CORBA::Object::_duplicate(curr_recv_conn.endpoint.in ());
                    retv[len].kind = binding.internalEndpoint[i].kind;

                    ++len;
                    found = true;
                    break;
                  }
              } // End of searching received connections

          // We didnt find the objref of the connection ...
          if (!found)
            {
              ACE_CString error ("Creating connections for ");
              error += instname;
              error += ": unable to find object reference for connection ";
              error += binding.name.in ();
              throw Deployment::StartError(
                "DomainApplicationManager_AMH_Impl::create_connections_i",
                error.c_str ());
            }
          break;
        }
    }
  return true;
}

void
CIAO::DomainApplicationManager_AMH_Impl::
start (
   Deployment::AMH_DomainApplicationManagerResponseHandler_ptr _tao_rh)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_AMH_Impl::start");

  try
    {
      CORBA::ULong i;

      for (i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeApplication object reference.
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> *entry = 0;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                          "CIAO::DomainApplicationManager_AMH_Impl::start -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                ("Unable to resolve a reference to node manager: ");
              error += this->node_manager_names_[i];

              throw Deployment::StartError(
                "DomainApplicationManager_AMH_Impl:startLaunch",
                error.c_str ());
            }

          ::Deployment::NodeApplication_ptr my_na =
            (entry->int_id_).node_application_.in ();

          my_na->ciao_preactivate ();
        }

      // Invoke start () operation on each cached NodeApplication object.
      for (i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeApplication object reference.
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> *entry;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                          "CIAO::DomainApplicationManager_AMH_Impl::start -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              throw Deployment::StartError ();
            }

          ::Deployment::NodeApplication_ptr my_na =
           (entry->int_id_).node_application_.in ();

          my_na->start ();
        }

     // Invoke ciao_postctivate () operation on each
     // cached NodeApplication object.

      for (i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeApplication object reference.
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> *entry;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                          "CIAO::DomainApplicationManager_AMH_Impl::start -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                ("Unable to resolve a reference to node manager: ");
              error += this->node_manager_names_[i];

              throw Deployment::StartError(
                "DomainApplicationManager_AMH_Impl:startLaunch",
                error.c_str ());
            }

          ::Deployment::NodeApplication_ptr my_na =
            (entry->int_id_).node_application_.in ();

          my_na->ciao_postactivate ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("DomainApplicationManager_AMH_Impl::start\t\n");
      throw;
    }

  _tao_rh->start ();
}

void
CIAO::DomainApplicationManager_AMH_Impl::
destroyApplication (
  Deployment::AMH_DomainApplicationManagerResponseHandler_ptr _tao_rh)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_AMH_Impl::destroyApplication");
  try
    {
      // Passivate shared components also, which should delegate to EM to handle
      this->passivate_shared_components ();

      // Passivate all components associated with the plan
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> *entry = 0;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_AMH_Impl.cpp -"
                          "CIAO::DomainApplicationManager_AMH_Impl::destroyApplication -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                 ("Unable to resolve a reference to NodeManager: ");
              error += this->node_manager_names_[i];

              throw
                (Deployment::StopError
                   ("DomainApplicationManager_AMH_Impl::destroyApplication",
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
              DomainApplicationManager_AMH_Impl::External_Connections);

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
                 );
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

          my_node_manager->destroyPlan ((entry->int_id_).child_plan_);
        }

      // ??
      // Shall we invoke destroyManager () operation on the NodeManager, since we need
      // to clean up all the NodeApplicationManagers associated with this deployment
      // plan (one NodeApplicationManager per Node per plan).

      // We should again activate those shared components
      this->activate_shared_components ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "DomainApplicationManager_AMH_Impl::destroyApplication\t\n");
      throw;
    }

  _tao_rh->destroyApplication ();
}

void
CIAO::DomainApplicationManager_AMH_Impl::
destroyManager (
  Deployment::AMH_DomainApplicationManagerResponseHandler_ptr _tao_rh)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_AMH_Impl::destroyManager");
  try
    {
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeManager and NodeApplicationManager object references.
          ACE_Hash_Map_Entry
            <ACE_CString,
            Chained_Artifacts> *entry = 0;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            throw Deployment::StopError (); // Should never happen!

          ::Deployment::NodeManager_var my_node_manager =
             (entry->int_id_).node_manager_;

          ::Deployment::NodeApplicationManager_var my_nam =
              (entry->int_id_).node_application_manager_;

          my_node_manager->destroyManager (my_nam.in ());
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "DomainApplicationManager_AMH_Impl::destroyManager\t\n");
      throw;
    }
  _tao_rh->destroyManager ();
}

void
CIAO::DomainApplicationManager_AMH_Impl::
perform_redeployment (
  Deployment::AMH_DomainApplicationManagerResponseHandler_ptr _tao_rh,
  const Deployment::DeploymentPlan & plan)
{
  // Currently we could dynamically update the NodeManagerMap topology,
  // but later maybe we could add another parameter to this operation,
  // which allows the user to specify the new NodeManagerMap data file.
  this->old_plan_ = this->plan_;
  this->is_redeployment_ = true;
  this->plan_ = plan;

  try
    {
      // Call init() on the myself, which will validate/split the plan and
      // call preparePlan on each NodeManager, by this, we shall get
      // all the object references of NM and NAM associated with each
      // component instance populated.
      this->init ();

      // Call startLaunch on each NM for each child plan, this should not only
      // install all the new components specified, but should also remove
      // the components that are no longer in the new deployment plan.
      // Meanwhile, we should set up the container configurations appropriately,
      // whose information is fully captured in the node-level deployment plan.
      ::Deployment::Properties_var properties;
      ACE_NEW (properties,
               Deployment::Properties);

      this->startLaunch (_tao_rh, properties.in (), false);

      // finishLaunch will (1) establish new connections, and (2)
      // get rid of those non-existing connections. As we know, in the
      // node level, the connections are cached within the NodeApplication *and*
      // Container, then we should modify the implementation of the
      // <finishLaunch> on the NodeApplication to accomplish this.
      this->finishLaunch (_tao_rh, true, true);  // true means start activation also.
                                        // ture means "ReDaC" is desired

      this->start (_tao_rh);

      _tao_rh->perform_redeployment ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "DomainApplicationManager_AMH_Impl::perform_redeployment\t\n");
      this->plan_ = this->old_plan_;
      throw;
    }
}


void
CIAO::DomainApplicationManager_AMH_Impl::
getPlan (Deployment::AMH_DomainApplicationManagerResponseHandler_ptr _tao_rh)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_AMH_Impl::getPlan");
  Deployment::DeploymentPlan_var plan = 0;
  // Make a deep copy of the Plan
  ACE_NEW_THROW_EX (plan,
                    Deployment::DeploymentPlan (this->plan_),
                    CORBA::NO_MEMORY ());

  _tao_rh->getPlan (plan.in ());
}

void
CIAO::DomainApplicationManager_AMH_Impl::
dump_connections (const ::Deployment::Connections & connections)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_AMH_Impl::dump_connections");
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

bool
CIAO::DomainApplicationManager_AMH_Impl::
already_exists (const Deployment::PlanConnectionDescription & conn)
{
  const CORBA::ULong conn_len = this->old_plan_.connection.length();
  for(CORBA::ULong i = 0; i < conn_len; ++i)
    {
      if (ACE_OS::strcmp (this->old_plan_.connection[i].name.in (),
                          conn.name.in ()) == 0)
        return true;
    }

  return false;
}

Deployment::Connections *
CIAO::DomainApplicationManager_AMH_Impl::
subtract_connections (const Deployment::Connections & left,
                      const Deployment::Connections & right)
{
  Deployment::Connections_var retv;
  ACE_NEW_RETURN (retv,
                  Deployment::Connections,
                  0);

  CORBA::ULong left_length = left.length ();
  CORBA::ULong right_length = right.length ();
  for (CORBA::ULong i = 0; i < left_length; ++i)
    {
      bool found = false;
      for (CORBA::ULong j = 0; j < right_length; ++j)
        {
          if (ACE_OS::strcmp (left[i].instanceName.in (),
                              right[j].instanceName.in ()) == 0 &&
              ACE_OS::strcmp (left[i].portName.in (),
                              right[j].portName.in ()) == 0 &&
              left[i].kind == right[j].kind &&
              left[i].endpoint->_is_equivalent (right[j].endpoint.in ()))
            {
              found = true;
              break;
            }
        }

        if (!found)
          {
            CORBA::ULong curr_length = retv->length ();
            retv->length (curr_length + 1);
            retv[curr_length] = left[i];
          }
    }
  return retv._retn ();
}

void
CIAO::DomainApplicationManager_AMH_Impl::
passivate_shared_components (void)
{
  try
    {
      // For each "external" component...
      CORBA::ULong length = this->shared_->length ();
      for (CORBA::ULong j = 0; j < length; ++j)
        {
          // Construct <Component_Binding_Info> struct for the component
          CIAO::Component_Binding_Info *
              binding = this->populate_binding_info (
                this->shared_[j].name.in (),
                this->shared_[j].plan_uuid.in ());

          // Invoke <finalize_global_binding> on ExecutionManager
          this->execution_manager_->passivate_shared_components (
            *binding);

          delete binding;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "DomainApplicationManager_AMH_Impl::passivate_shared_components.\n");
      throw;
    }

}

void
CIAO::DomainApplicationManager_AMH_Impl::
activate_shared_components (void)
{
  try
    {
      // For each "external" component...
      CORBA::ULong length = this->shared_->length ();
      for (CORBA::ULong j = 0; j < length; ++j)
        {
          // Construct <Component_Binding_Info> struct for the component
          CIAO::Component_Binding_Info *
              binding = this->populate_binding_info (
                this->shared_[j].name.in (),
                this->shared_[j].plan_uuid.in ());

          // Invoke <finalize_global_binding> on ExecutionManager
          this->execution_manager_->activate_shared_components (
            *binding);

          delete binding;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "DomainApplicationManager_AMH_Impl::activate_shared_components.\n");
      throw;
    }

}

void
CIAO::DomainApplicationManager_AMH_Impl::
purge_connections (Deployment::Connections_var & connections,
                   const char * inst)
{
  CORBA::ULong total_len = connections->length ();

  for (CORBA::ULong i = 0; i < total_len; ++i)
    {
      bool found = false;

      // Remove all the connections whose "source" component
      // name is <inst>
      if (ACE_OS::strcmp (connections[i].instanceName.in (),
                          inst) == 0)
        {
          found = true;

          for (CORBA::ULong j = i; j < total_len - 1; ++j)
            {
              connections[j] = connections[j + 1];
            }
          connections->length (total_len - 1);
        }

      if (found)
        this->purge_connections (connections, inst);
    }
}

const Deployment::Properties *
CIAO::DomainApplicationManager_AMH_Impl::
get_connection_QoS_configuration (const Deployment::Requirement & requirement)
{
  // Get the name/identifier of the filter associated with
  // this connection
  Deployment::Properties * retv;
  ACE_NEW_NORETURN (retv, Deployment::Properties);

  CORBA::ULong len = retv->length ();

  for (CORBA::ULong i = 0;
       i < requirement.property.length ();
       ++i)
    {
      const char *filter_name;
      if (ACE_OS::strcmp ("EventFilter",
                          requirement.property[i].name) == 0)
        {

          if ((requirement.property[i].value >>= filter_name) == false)
            ACE_ERROR ((LM_ERROR,
                        "ERROR: DomainApplicationManager_AMH_Impl::"
                        "get_connection_QoS_configuration unable to "
                        "extract event filter information\n"));
        }

      // Search for the desired filter
      for (CORBA::ULong j = 0; j < this->esd_->length (); ++j)
        {
          // Populate the "filters" info, in case this CIAO_Event_Service has
          // one or more filters specified through descriptors
          for (CORBA::ULong k = 0; k < this->esd_[j].filters.length (); ++k)
            {
              if (ACE_OS::strcmp (this->esd_[j].filters[k].name.in (),
                                  filter_name) == 0)
                {
                  retv->length (len + 1);
                  (*retv)[len].name =  CORBA::string_dup ("EventFilter");
                  (*retv)[len].value <<= this->esd_[j].filters[k];
                  break;
                }
            }
        }
    }
  return retv;
}
