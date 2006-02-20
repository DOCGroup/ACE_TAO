// $Id$

#include "DomainApplicationManager_Impl.h"
//#include "NodeManager/NodeManagerC.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_string.h"
#include "ace/SString.h"

#if !defined (__ACE_INLINE__)
# include "DomainApplicationManager_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::DomainApplicationManager_Impl::
DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               Deployment::TargetManager_ptr manager,
                               Execution_Manager::Execution_Manager_Impl * em,
                               const Deployment::DeploymentPlan & plan,
                               const char * deployment_file)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
    is_redeployment_ (false)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_NEW_THROW_EX (this->all_connections_,
                    Deployment::Connections (),
                    CORBA::NO_MEMORY ());

  ACE_NEW_THROW_EX (this->shared_,
                    Deployment::ComponentPlans (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;
}

CIAO::DomainApplicationManager_Impl::~DomainApplicationManager_Impl ()
{
  if (CIAO::debug_level () > 1)
    {
      ACE_DEBUG ((LM_DEBUG, "DomainApplicationManager destroyed\n"));
    }
}

Deployment::NodeApplication_ptr
CIAO::DomainApplicationManager_Impl::
get_node_app (const char * node_name)
  ACE_THROW_SPEC ((::CORBA::SystemException,
                   ::Deployment::NoSuchName))
{
  // Get the NodeApplication object reference.
  ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> *entry = 0;

  if (this->artifact_map_.find (node_name,
                                entry) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                  "CIAO::DomainApplicationManager_Impl::get_node_app -"
                  "ERROR while finding the node application "
                  "for the node [%s] \n",
                  node_name));
      ACE_THROW (Deployment::NoSuchName ());
    }

  return
    Deployment::NodeApplication::_duplicate (
      entry->int_id_.node_application_.in ());
}

void
CIAO::DomainApplicationManager_Impl::
init (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::ResourceNotAvailable,
                   Deployment::StartError,
                   Deployment::PlanError))
{
  ACE_TRY
    {

      //Deployment::DnC_Dump::dump (this->plan_);
      /*
      //======================================
      // Dump the contents of infoProperty to a XML file
      if (CIAO::debug_level () > 1)
      {
        CIAO::DAnCE::ServerResource *sr = 0;
        this->plan_.infoProperty[0].value >>= sr;

        Config_Handlers::SRD_Handler reverse_handler (sr);

        Config_Handlers::XML_Helper the_helper;
        xercesc::DOMDocument *the_xsc
              (the_helper.create_dom ("CIAO:ServerResources",
               "http://www.dre.vanderbilt.edu/ServerResources"));

        ServerResources (*reverse_handler.srd_xsc (), the_xsc);

        // write out the result
        the_helper.write_DOM (the_xsc, "output.srd");
      }
      */
      //======================================

      // (1) Call get_plan_info() method to get the total number
      //     of child plans and list of NodeManager names, and
      // (2) Check the validity of the global deployment plan.
      if (! this->get_plan_info ())
        {
          ACE_ERROR ((LM_ERROR,
                      "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                      "CIAO::DomainApplicationManager_Impl::init -"
                      "ERROR while calling get_plan_info () \n"));
          ACE_TRY_THROW (Deployment::PlanError ());
        }

      // Call split_plan()
      if (! this->split_plan ())
        {
          ACE_ERROR ((LM_ERROR,
                      "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                      "CIAO::DomainApplicationManager_Impl::init -"
                      "ERROR while calling split_plan () \n"));
          ACE_TRY_THROW (Deployment::PlanError ());
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
                          "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                          "CIAO::DomainApplicationManager_Impl::init -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));
              ACE_TRY_THROW (Deployment::PlanError ());
            }

          Chained_Artifacts & artifacts = entry->int_id_;

          // Call preparePlan() method on the NodeManager with the
          // corresponding child plan as input, which returns a
          // NodeApplicationManager object reference.
          Deployment::ApplicationManager_var tmp_app_manager =
            my_node_manager->preparePlan (artifacts.child_plan_.in ()
                                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          Deployment::NodeApplicationManager_var app_manager =
            Deployment::NodeApplicationManager::_narrow
              (tmp_app_manager.in ()
               ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (app_manager.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                          "CIAO::DomainApplicationManager_Impl::init -"
                          "ERROR while narroing the NAM "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error ("DomainAppMgr::init () received a nil \
                                 reference for NodeApplicationManager\n");

              ACE_DEBUG ((LM_DEBUG, error.c_str ()));
              ACE_TRY_THROW
                 (Deployment::StartError ("DomainApplicationManager_Impl:init",
                                          error.c_str ()));
            }
          ACE_TRY_CHECK;

          // Cache the NodeApplicationManager object reference.
          artifacts.node_application_manager_ = app_manager._retn ();
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "DomainApplicationManager_Impl::init\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

bool
CIAO::DomainApplicationManager_Impl::
get_plan_info (void)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::get_plan_info");

  // Read the deployment.dat file and get to know the different nodes
  // that are present in the deployment domain.
  // We should be able to replace this way, by parsing the domain
  // descriptor to get to know more on the domain.
  //
  if ( this->deployment_config_.init (this->deployment_file_) == -1 )
    {
      ACE_ERROR ((LM_ERROR,
                  "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                  "CIAO::DomainApplicationManager_Impl::get_plan_info -"
                  "ERROR while trying to initialize after reading "
                  "deployment DAT file \n"));
      return 0;
    }

  // Error: If there are no nodes in the plan => No nodes to deploy the
  // components
  const CORBA::ULong length = this->plan_.instance.length ();

  if (length == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                  "CIAO::DomainApplicationManager_Impl::get_plan_info -"
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
CIAO::DomainApplicationManager_Impl::
split_plan (void)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::split_plan");

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
CIAO::DomainApplicationManager_Impl::
add_connections (const Deployment::Connections & incoming_conn)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::add_connections");

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
CIAO::DomainApplicationManager_Impl::
startLaunch (const ::Deployment::Properties & configProperty,
             ::CORBA::Boolean start
             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Deployment::ResourceNotAvailable,
                   ::Deployment::StartError,
                   ::Deployment::InvalidProperty))
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::startLaunch");
  ACE_UNUSED_ARG (start);

  ACE_TRY
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
                          "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                          "CIAO::DomainApplicationManager_Impl::startLaunch -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                 ("Unable to resolve a reference to node manager: ");
              error += this->node_manager_names_[i];

              ACE_TRY_THROW
                 (Deployment::StartError
                    ("DomainApplicationManager_Impl:startLaunch",
                     error.c_str ()));
            }

          ::Deployment::NodeApplicationManager_ptr my_nam =
            (entry->int_id_).node_application_manager_.in ();

          if (CORBA::is_nil (my_nam))
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                          "CIAO::DomainApplicationManager_Impl::startLaunch -"
                          "ERROR while narrowing the NAM "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                 ("While starting launch, the DomainApplicationManager\
                  has a nil reference for NodeApplicationManager\n");
              ACE_DEBUG ((LM_DEBUG, error.c_str ()));

              ACE_TRY_THROW
                (Deployment::StartError
                   ("DomainApplicationManager_Impl::startLaunch",
                     error.c_str ()));
            }

          ACE_TRY_CHECK;

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
              temp_application =
                my_nam->perform_redeployment (configProperty,
                                              retn_connections.out (),
                                              0);
            }

          // Narrow down to NodeApplication object reference
          ::Deployment::NodeApplication_var my_na =
            ::Deployment::NodeApplication::_narrow (temp_application.in ()
                                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (my_na.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                          "CIAO::DomainApplicationManager_Impl::startLaunch -"
                          "ERROR while narrowing the NAM "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                ("The DomainApplicationManager receives a nil "
                 "reference of NodeApplication after calling "
                 "startLaunch on NodeApplicationManager.\n");
              ACE_ERROR ((LM_ERROR, error.c_str ()));

              ACE_TRY_THROW
                (Deployment::StartError
                   ("DomainApplicationManager_Impl::startLaunch",
                     error.c_str ()));
            }
          ACE_TRY_CHECK;

          // Cache the returned set of connections into the list.
          this->add_connections (retn_connections.in ());

          // Cache the returned NodeApplication object reference into
          // the hash table.
          (entry->int_id_).node_application_ = my_na._retn ();
        }

      this->synchronize_shared_components_with_node_managers ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "DomainApplicationManager_Impl::startLaunch\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  ACE_CHECK;
}

void
CIAO::DomainApplicationManager_Impl::
finishLaunch (CORBA::Boolean start,
              CORBA::Boolean is_ReDAC
              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError))
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::finishLaunch");
  ACE_TRY
    {
      // Invoke finishLaunch() operation on each cached NodeApplication object.
      // This will establish bindings for only those internal components, but
      // NOT for those external/shared components, which requires special
      // handle, since these components are outside the control of this
      // DomainApplicationManager.
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeApplication object reference.
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> * entry;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                          "CIAO::DomainApplicationManager_Impl::finishLaunch -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                 ("Unable to resolve a reference to NodeManager: ");
              error += this->node_manager_names_[i];

              ACE_TRY_THROW
                (Deployment::StartError
                   ("DomainApplicationManager_Impl::finishLaunch",
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

          // Get the Connections variable, if ReDAC is true, then we get
          // those new connections only. NOTE: get_outgoing_connections
          // by default will get *all* connections.
          Deployment::Connections * my_connections =
            this->get_outgoing_connections (
              (entry->int_id_).child_plan_.in (),
              !is_ReDAC,
              true,  // we search *new* plan
              DomainApplicationManager_Impl::Internal_Connections
                                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (my_connections == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                          "CIAO::DomainApplicationManager_Impl::finishLaunch -"
                          "ERROR while getting the outgoing connections "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_TRY_THROW
                (Deployment::StartError
                  ("DomainApplicationManager_Impl::finish_launch",
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
                  ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          if (is_ReDAC) // We should also remove unnecessary connections
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
                        DomainApplicationManager_Impl::Internal_Connections
                                                      ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // Pass in the "false" parameter to get *all* the connections in
              // the new deployment plan, regardless those in old plan
              Deployment::Connections * connections_in_new_plan =
                this->get_outgoing_connections (
                        (entry->int_id_).child_plan_.in (),
                        true, // yes, get *all* the connections
                        true,  // search in the *new* plan
                        DomainApplicationManager_Impl::Internal_Connections
                                                      ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

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
                  entry->int_id_.node_application_->finishLaunch
                    (*unnecessary_connections,
                      start,
                      false  // false ==> remove unnecessary connections
                      ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;
                }
            }
        }

      // Establish bindings on external/shared components of this deployment plan.
      this->post_finishLaunch ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION  (ACE_ANY_EXCEPTION,
                            "DomainApplicationManager_Impl::finishLaunch: "
                            "Killing NodeApplications.\n");

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
             (0
              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  ACE_CHECK;
}

void
CIAO::DomainApplicationManager_Impl::
post_finishLaunch (void)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError))
{
  ACE_TRY
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
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION  (ACE_ANY_EXCEPTION,
                            "DomainApplicationManager_Impl::post_finishLaunch.\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  ACE_CHECK;
}

CIAO::Component_Binding_Info *
CIAO::DomainApplicationManager_Impl::
populate_binding_info (const ACE_CString& name,
                       const ACE_CString& child_uuid)
{
  CIAO::Component_Binding_Info * retv;
  ACE_NEW_RETURN (retv,
                  CIAO::Component_Binding_Info (),
                  0);

  retv->name_ = name;

  // Parse the child_uuid string and populate the "node" name and "plan_uuid" fields.
  // Our protocol here is searching for the "@", the substring *before* that is the
  // global plan uuid, and the substring *after* that is the node name.
  size_t pos = child_uuid.find ('@');

  retv->plan_uuid_ =
    child_uuid.substring (0, pos);

  retv->node_ =
    child_uuid.substring (pos+1, -1); // get the rest of the string

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
CIAO::DomainApplicationManager_Impl::
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
CIAO::DomainApplicationManager_Impl::
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
CIAO::DomainApplicationManager_Impl::
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
CIAO::DomainApplicationManager_Impl::
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
                      "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                      "CIAO::DomainApplicationManager_Impl::startLaunch -"
                      "ERROR while finding the node specific plan "
                      "for the node [%s] \n",
                      this->node_manager_names_[i].c_str ()));

          ACE_CString error
              ("Unable to resolve a reference to node manager: ");
          error += this->node_manager_names_[i];

          ACE_TRY_THROW
              (Deployment::StartError
                ("DomainApplicationManager_Impl:startLaunch",
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
CIAO::DomainApplicationManager_Impl::
get_outgoing_connections (const Deployment::DeploymentPlan &plan,
                          bool is_getting_all_connections,
                          bool is_search_new_plan,
                          Connection_Search_Type t
                          ACE_ENV_ARG_DECL)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::get_outgoing_connections");
  Deployment::Connections_var connections;
  ACE_NEW_RETURN (connections,
                  Deployment::Connections,
                  0);

  // For each component instance in the child plan ...
  for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
  {

    if (t == Internal_Connections &&
        this->is_shared_component (plan.instance[i].name.in ()))
      continue;

    // Get the outgoing connections of the component
    if (!get_outgoing_connections_i (plan.instance[i].name.in (),
                                     connections.inout (),
                                     is_getting_all_connections,
                                     is_search_new_plan
                                     ACE_ENV_ARG_PARAMETER))
      return 0;
  }
  return connections._retn ();
}

bool
CIAO::DomainApplicationManager_Impl::
get_outgoing_connections_i (const char * instname,
                            Deployment::Connections & retv,
                            bool is_getting_all_connections,
                            bool is_search_new_plan
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((Deployment::StartError))
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::get_outoing_connections_i");

  Deployment::DeploymentPlan tmp_plan;
  if (is_search_new_plan)
    tmp_plan = this->plan_;
  else
    tmp_plan = this->old_plan_;

  // Search for all the connections in the plan.
  const CORBA::ULong total_length = tmp_plan.connection.length();
  for (CORBA::ULong i = 0; i < total_length; ++i)
  {
    CORBA::ULong len = retv.length ();

    // Current connection that we are looking at.
    const Deployment::PlanConnectionDescription & curr_conn =
      tmp_plan.connection[i];

    if (already_exists (curr_conn) &&
        !is_getting_all_connections) // ignore existing connections
      continue;

    //The modeling tool should make sure there are always 2 endpoints
    //in a connection.
    const CORBA::ULong curr_conn_len = curr_conn.internalEndpoint.length ();
    for (CORBA::ULong p_index = 0;
         p_index < curr_conn_len;
         ++p_index)
    {
      const Deployment::PlanSubcomponentPortEndpoint & endpoint =
        curr_conn.internalEndpoint[p_index];

      // If the component name matches the name of one of the
      // endpoints in the connection.
      if (ACE_OS::strcmp (tmp_plan.instance[endpoint.instanceRef].name.in (),
                          instname) == 0 )
      {
        //Look at the port kind to make sure it's what we are interested in.
        if (endpoint.kind != Deployment::Facet &&
            endpoint.kind != Deployment::EventConsumer)
          {
            // The other endpoints in this connection is what we want.
            CORBA::ULong index = (p_index +1)%2;

            //Cache the name of the other component for later usage (search).
            ACE_CString name =
              tmp_plan.instance[curr_conn.internalEndpoint[index].
                 instanceRef].name.in ();

            // Cache the name of the port from the
            // other component for searching later.
            ACE_CString port_name =
              curr_conn.internalEndpoint[index].portName.in ();

            ACE_DEBUG ((LM_ERROR, "Looking: %s,%s \n",
                        name.c_str (),
                        port_name.c_str ()));

            bool found = false;
            // Now we have to search in the received
            // connections to get the objRef.
            const CORBA::ULong all_conn_len = this->all_connections_->length ();
            for (CORBA::ULong conn_index = 0;
                 conn_index < all_conn_len;
                 ++conn_index)
              {
                const Deployment::Connection curr_rev_conn =
                    this->all_connections_[conn_index];

                // We need to look at the instance name and the
                // port name to confirm.
                if (ACE_OS::strcmp (curr_rev_conn.instanceName.in (),
                                    name.c_str ()) == 0 &&
                    ACE_OS::strcmp (curr_rev_conn.portName.in (),
                                    port_name.c_str ()) == 0)
                  {
                    //ACE_DEBUG ((LM_DEBUG, "step5\n"));
                    retv.length (len+1);
                    retv[len].instanceName = instname;
                    retv[len].portName = endpoint.portName.in ();
                    retv[len].kind = endpoint.kind;
                    retv[len].endpoint =
                       CORBA::Object::_duplicate(curr_rev_conn.endpoint.in ());

                    retv[len].endpointInstanceName = name.c_str ();
                    retv[len].endpointPortName = port_name.c_str ();

                    ++len;
                    found = true;
                    break;
                  }
              }

              // We didnt find the counter part connection even
              // we are sure there must be 1.
              if (!found)
                {
                  ACE_CString error ("Creating connections for ");
                  error += instname;
                  error += ": unable to find object reference for connection ";
                  error += curr_conn.name.in ();
                  ACE_THROW_RETURN (Deployment::StartError
                    ("DomainApplicationManager_Impl::create_connections_i",
                                  error.c_str ()),
                                  false);
                }

              break;
          }
      }
    }
  }
  return true;
}

void
CIAO::DomainApplicationManager_Impl::
start (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Deployment::StartError))
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::start");

  ACE_TRY
    {
      CORBA::ULong i;

      for (i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeApplication object reference.
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> *entry;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                          "CIAO::DomainApplicationManager_Impl::start -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                ("Unable to resolve a reference to node manager: ");
              error += this->node_manager_names_[i];

              ACE_TRY_THROW (Deployment::StartError
                ("DomainApplicationManager_Impl:startLaunch",
                  error.c_str ()));
            }

          ::Deployment::NodeApplication_ptr my_na =
            (entry->int_id_).node_application_.in ();

          my_na->ciao_preactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
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
                          "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                          "CIAO::DomainApplicationManager_Impl::start -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_TRY_THROW (Deployment::StartError ());
            }

          ::Deployment::NodeApplication_ptr my_na =
           (entry->int_id_).node_application_.in ();

          my_na->start (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
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
                          "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                          "CIAO::DomainApplicationManager_Impl::start -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                ("Unable to resolve a reference to node manager: ");
              error += this->node_manager_names_[i];

              ACE_TRY_THROW (Deployment::StartError
                  ("DomainApplicationManager_Impl:startLaunch",
                    error.c_str ()));
            }

          ::Deployment::NodeApplication_ptr my_na =
            (entry->int_id_).node_application_.in ();

          my_na->ciao_postactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "DomainApplicationManager_Impl::start\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  ACE_CHECK;
}

void
CIAO::DomainApplicationManager_Impl::
destroyApplication (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Deployment::StopError))
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::destroyApplication");
  ACE_TRY
    {
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

          // Invoke ciao_passivate () operation on each cached NodeApplication object.
          ::Deployment::NodeApplication_ptr my_na =
              (entry->int_id_).node_application_.in ();

          my_na->ciao_passivate ();

          Deployment::Connections_var connections =
            this->get_outgoing_connections (
              (entry->int_id_).child_plan_.in (),
              true, // yes, get *all* the connections
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
                  CIAO::Component_Binding_Info *
                      binding = this->populate_binding_info (
                        connections[j].instanceName.in ());

                  this->execution_manager_->finalize_global_binding (
                    *binding, false);

                  // If this element is the last one in the sequence
                  if (j == connections->length () - 1)
                    {
                      connections->length (connections->length () - 1);
                      break;
                    }

                  // otherwise, remove this element from the list
                  for (CORBA::ULong k = j; k < connections->length (); ++k)
                    {
                      connections[k] = connections [k + 1];
                      connections->length (connections->length () - 1);
                    }
                }
            }

          entry->int_id_.node_application_->finishLaunch
              (connections.in (),
               true, // "true" ==> start the components
               false // "false" => remove connections
               ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // To invoke <destroy> operations on NodeManagers is the way to go.

          // Invoke destroyPlan() operation on the NodeManager
          Deployment::NodeManager_var
              my_node_manager = (entry->int_id_).node_manager_;

          my_node_manager->destroyPlan ((entry->int_id_).child_plan_
                                        ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Invoke destroyManager () operation on the NodeManager, since we need
      // to clean up all the NodeApplicationManagers associated with this deployment
      // plan (one NodeApplicationManager per Node per plan).

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "DomainApplicationManager_Impl::destroyApplication\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  ACE_CHECK;
}

void
CIAO::DomainApplicationManager_Impl::
destroyManager (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StopError))
{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::destroyManager");
  ACE_TRY
    {
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeManager and NodeApplicationManager object references.
          ACE_Hash_Map_Entry
            <ACE_CString,
            Chained_Artifacts> *entry = 0;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            ACE_TRY_THROW (Deployment::StopError ()); // Should never happen!

          ::Deployment::NodeManager_var my_node_manager =
             (entry->int_id_).node_manager_;

          ::Deployment::NodeApplicationManager_var my_nam =
              (entry->int_id_).node_application_manager_;

          my_node_manager->destroyManager (my_nam.in ()
                                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "DomainApplicationManager_Impl::destroyManager\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  ACE_CHECK;
}

void
CIAO::DomainApplicationManager_Impl::
perform_redeployment (
  const Deployment::DeploymentPlan & plan
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                    Deployment::PlanError,
                    Deployment::InstallationFailure,
                    Deployment::UnknownImplId,
                    Deployment::ImplEntryPointNotFound,
                    Deployment::InvalidConnection,
                    ::Components::RemoveFailure))
{
  // Currently we could dynamically update the NodeManagerMap topology,
  // but later maybe we could add another parameter to this operation,
  // which allows the user to specify the new NodeManagerMap data file.
  this->old_plan_ = this->plan_;
  this->is_redeployment_ = true;
  this->plan_ = plan;

  ACE_TRY
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

      this->startLaunch (properties.in (), false);

      // finishLaunch() will not only establish new connections, but also
      // should get rid of those non-existing connections. As we know, in the
      // node level, the connections are cached within the NodeApplication *and*
      // Container, then we should modify the implementation of the
      // <finishLaunch> on the NodeApplication to accomplish this.
      this->finishLaunch (true, true);  // true means start activation also.
                                        // ture means "ReDAC" is desired
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                            "DomainApplicationManager_Impl::perform_redeployment\t\n");
      this->plan_ = this->old_plan_;
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}


// Returns the DeploymentPlan associated with this ApplicationManager.
::Deployment::DeploymentPlan *
CIAO::DomainApplicationManager_Impl::
getPlan (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
    }
}

bool
CIAO::DomainApplicationManager_Impl::
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
CIAO::DomainApplicationManager_Impl::
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
