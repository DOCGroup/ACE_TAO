// $Id$

#include "DomainApplicationManager_Impl.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_string.h"
#include "NodeManager/NodeDaemonC.h"

#if !defined (__ACE_INLINE__)
# include "DomainApplicationManager_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::DomainApplicationManager_Impl::
DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               Deployment::TargetManager_ptr manager,
                               const Deployment::DeploymentPlan & plan,
                               const char * deployment_file)
  ACE_THROW_SPEC ((CORBA::SystemException))
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    target_manager_ (Deployment::TargetManager::_duplicate (manager)),
    plan_ (plan),
    num_child_plans_ (0),
  // @@ (OO) The default size for an ACE_Hash_Map_Mapanger is quiet
  //         large.  The maximum size of an ACE_Hash_Map_Manager is
  //         also fixed, i.e. it does not grow dynamically on demand.
  //         Make sure the default size of artifact_map_ is
  //         appropriate for your needs.  You may also want to make
  //         the size configurable at compile-time, at least.
    deployment_file_ (CORBA::string_dup (deployment_file)),
    deployment_config_ (orb)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_NEW_THROW_EX (this->all_connections_,
                    Deployment::Connections (),
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
      // (1) Call get_plan_info() method to get the total number
      //     of child plans and list of NodeManager names, and
      // (2) Check the validity of the global deployment plan.
      if (! this->get_plan_info ())
        ACE_THROW (Deployment::PlanError ());

      // Call split_plan()
      if (! this->split_plan ())
        ACE_THROW (Deployment::PlanError ());

      // Invoke preparePlan for each child deployment plan.
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeManager object reference.
          ::Deployment::NodeManager_var my_node_manager =
            this->deployment_config_.get_node_manager
              (this->node_manager_names_[i].c_str ());

          // Get the child deployment plan reference.
          ACE_Hash_Map_Entry
            <ACE_CString,
            Chained_Artifacts> *entry;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            ACE_THROW (Deployment::PlanError ());

          Chained_Artifacts & artifacts = entry->int_id_;

          // Dump plans
          if (CIAO::debug_level () > 1)
            {
              // The dump() function is broken.
              //Deployment::DnC_Dump::dump (artifacts.child_plan_);
            }

          // Call preparePlan() method on the NodeManager with the
          // corresponding child plan as input, which returns a
          // NodeApplicationManager object reference.
          Deployment::ApplicationManager_var tmp_app_manager =
            my_node_manager->preparePlan (artifacts.child_plan_.in ()
                                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          Deployment::NodeApplicationManager_var app_manager
                  = Deployment::NodeApplicationManager::_narrow (tmp_app_manager.in ()
                                                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (app_manager.in ()))
            {
              ACE_DEBUG ((LM_DEBUG, "DomainAppMgr::init () received a nil\
                                     reference for NodeApplicationManager\n"));
              ACE_THROW (Deployment::StartError ());
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
  if ( this->deployment_config_.init (this->deployment_file_) == -1 )
    return 0;

  const CORBA::ULong length = this->plan_.instance.length ();

  // Error: If there are no nodes in the plan => No nodes to deploy the
  // components
  if (length == 0)
    return false;

  // Copy the name of the node in the plan on to the node manager
  // array, Making sure that duplicates are not added twice
  int num_plans = 0;
  for (CORBA::ULong index = 0; index < length; index ++)
    {
      bool matched = false;

      const CORBA::ULong number_node_managers = this->node_manager_names_.size ();
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
            return 0; /* Failure */

          // Add this unique node_name to the list of NodeManager names
          this->node_manager_names_.push_back (this->plan_.instance [index].node.in ());

          // Increment the number of plans
          ++ num_plans;
        }
    }

  // Set the length of the Node Managers
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
  // Initialize the total number of child deployment plans specified
  // by the global plan.
  CORBA::ULong i;
  for ( i = 0; i < this->num_child_plans_; ++i)
  {
    ::Deployment::DeploymentPlan_var tmp_plan;
    ACE_NEW_RETURN (tmp_plan,
                    ::Deployment::DeploymentPlan,
                    0);

    tmp_plan->UUID = CORBA::string_dup (this->plan_.UUID.in ());

    tmp_plan->implementation.length (0);
    tmp_plan->instance.length (0);
    tmp_plan->connection.length (0);
    tmp_plan->externalProperty.length (0);
    tmp_plan->dependsOn.length (0);
    tmp_plan->artifact.length (0);
    tmp_plan->infoProperty.length (0);

    Chained_Artifacts artifacts;

    // Fill in the child_plan_ field, relinquishing ownership
    artifacts.child_plan_ = tmp_plan._retn ();

    // Fill in the node_manager_ field.
    artifacts.node_manager_ =
      this->deployment_config_.get_node_manager
              (this->node_manager_names_[i].c_str ());

    this->artifact_map_.bind (node_manager_names_[i], artifacts);
  }

  // (1) Iterate over the <instance> field of the global DeploymentPlan
  //     variabl.
  // (2) Retrieve the necessary information to contruct the node-level
  //     plans one by one.
  for ( i = 0; i < (this->plan_.instance).length (); ++i)
    {
      // Fill in the child deployment plan in the map.

      // Get the instance deployment description
      const ::Deployment::InstanceDeploymentDescription & my_instance =
        (this->plan_.instance)[i];

      // Find the corresponding child deployment plan entry in
      // the hash map for this instance.
      ACE_Hash_Map_Entry
        <ACE_CString,
        Chained_Artifacts> *entry;

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

      //Deployment::DnC_Dump::dump (this->plan_);
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

  CORBA::ULong old_len = this->all_connections_->length ();

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
  ACE_UNUSED_ARG (start);
  ACE_TRY
    {
      // Invoke startLaunch() operations on each cached NodeApplicationManager
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeApplicationManager object reference.
          ACE_Hash_Map_Entry
            <ACE_CString,
            Chained_Artifacts> *entry;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            ACE_THROW (Deployment::StartError ()); // Should never happen!

          ::Deployment::NodeApplicationManager_ptr my_nam =
            (entry->int_id_).node_application_manager_.in ();

          if (CORBA::is_nil (my_nam))
            {
              ACE_DEBUG ((LM_DEBUG, "While starting launch, the DomainApplicationManager\
                                     has a nil reference for NodeApplicationManager\n"));
              ACE_THROW (Deployment::StartError ());
            }
          ACE_TRY_CHECK;

          ::Deployment::Connections_var retn_connections;

          // Obtained the returned NodeApplication object reference
          // and the returned Connections variable.
          ::Deployment::Application_var temp_application =
            my_nam->startLaunch (configProperty,
                                 retn_connections.out (),
                                 0);  // This is a mistake. This should never be here.

          // Narrow down to NodeApplication object reference
          ::Deployment::NodeApplication_var my_na =
            ::Deployment::NodeApplication::_narrow (temp_application.in ()
                                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (my_na.in ()))
            {
              ACE_DEBUG ((LM_DEBUG, "The DomainApplicationManager receives a nil\
                                     reference of NodeApplication after calling\
                                     startLaunch on NodeApplicationManager.\n"));
              ACE_THROW (Deployment::StartError ());
            }
          ACE_TRY_CHECK;

          // Cache the returned set of connections into the list.
          this->add_connections (retn_connections.in ());

          // Cache the returned NodeApplication object reference into
          // the hash table.
          (entry->int_id_).node_application_ = my_na._retn ();
        }
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
finishLaunch (::CORBA::Boolean start
              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError))
{
  ACE_TRY
    {
      // Invoke finishLaunch() operation on each cached NodeApplication object.
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeApplication object reference.
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> * entry;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            ACE_THROW (Deployment::StartError ()); // Should never happen!

          //@@ Note: Don't delete the below debugging helpers.
          // Dump the connections for debug purpose.
          if (CIAO::debug_level () > 1)
            {
              ACE_DEBUG ((LM_DEBUG, "==============================================\n"));
              ACE_DEBUG ((LM_DEBUG, "dump incomming connection for child plan:%d\n", i));
              dump_connections (this->all_connections_.in ());
              ACE_DEBUG ((LM_DEBUG, "==============================================\n"));
            }

          // Get the Connections variable.
          Deployment::Connections * my_connections =
            this->get_outgoing_connections ((entry->int_id_).child_plan_.in ());

          if (my_connections == 0)
            ACE_THROW (Deployment::StartError ());

          Deployment::Connections_var safe (my_connections);

          // Dump the connections for debug purpose.
          if (CIAO::debug_level () > 1)
            {
              ACE_DEBUG ((LM_DEBUG, "==============================================\n"));
              ACE_DEBUG ((LM_DEBUG, "dump outgoingcomming connection for child plan:%d\n", i));
              dump_connections (safe.in ());
              ACE_DEBUG ((LM_DEBUG, "==============================================\n"));
            }

          // Invoke finishLaunch() operation on NodeApplication.
          entry->int_id_.node_application_->finishLaunch (safe.in (),
                                                          start
                                                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "DomainApplicationManager_Impl::finishLaunch\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  ACE_CHECK;
}

void
CIAO::DomainApplicationManager_Impl::
start (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Deployment::StartError))
{
  ACE_TRY
    {
      // Invoke start() operation on each cached NodeApplication object.
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeApplication object reference.
          ACE_Hash_Map_Entry
            <ACE_CString,
            Chained_Artifacts> *entry;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            ACE_THROW (Deployment::StartError ()); // Should never happen!

          // @@TODO: This might cause problem!
          ::Deployment::NodeApplication_var my_na =
            (entry->int_id_).node_application_.in ();

          my_na->start ();
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
  ACE_TRY
    {
      // Invoke destroyManager() operation on each cached
      // NodeManager object.
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeManager and NodeApplicationManager object references.
          ACE_Hash_Map_Entry
            <ACE_CString,
            Chained_Artifacts> *entry;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            ACE_THROW (Deployment::StopError ()); // Should never happen!

          ::Deployment::NodeApplicationManager_ptr my_node_application_manager =
                  (entry->int_id_).node_application_manager_.in ();

          // Invoke destoryApplication() operation on the NodeApplicationManger.
          // Since we have the first arg is not used by NAM anyway.
          my_node_application_manager->destroyApplication (0
                                                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
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
  ACE_TRY
    {
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeManager and NodeApplicationManager object references.
          ACE_Hash_Map_Entry
            <ACE_CString,
            Chained_Artifacts> *entry;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            ACE_THROW (Deployment::StopError ()); // Should never happen!

          ::Deployment::NodeManager_var my_node_manager =
             (entry->int_id_).node_manager_;
          
          // Since we have the first arg is not used by NM anyway.
          my_node_manager->destroyManager (0 ACE_ENV_ARG_PARAMETER);
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

// Returns the DeploymentPlan associated with this ApplicationManager.
::Deployment::DeploymentPlan *
CIAO::DomainApplicationManager_Impl::
getPlan (ACE_ENV_SINGLE_ARG_DECL)
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

Deployment::Connections *
CIAO::DomainApplicationManager_Impl::
get_outgoing_connections (const Deployment::DeploymentPlan &plan)
{
  Deployment::Connections_var connections;
  ACE_NEW_RETURN (connections,
                  Deployment::Connections,
                  0);

  // For each component instance in the child plan ...
  for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
  {
    // Get the component instance name
    if (!get_outgoing_connections_i (plan.instance[i].name.in (),
                                     connections.inout ()))
    return 0;
  }
  return connections._retn ();
}

bool
CIAO::DomainApplicationManager_Impl::
get_outgoing_connections_i (const char * instname,
                            Deployment::Connections & retv)
{
  // Search in all the connections in the plan.
  for (CORBA::ULong i = 0; i < this->plan_.connection.length(); ++i)
  {
    CORBA::ULong len = retv.length ();

    // Current connection that we are looking at.
    const Deployment::PlanConnectionDescription & curr_conn =
      this->plan_.connection[i];

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
      if (ACE_OS::strcmp (this->plan_.instance[endpoint.instanceRef].name.in (),
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
              this->plan_.instance[curr_conn.internalEndpoint[index].instanceRef].name.in ();

            //Cache the name of the port from the other component for searching later.
            ACE_CString port_name =
              curr_conn.internalEndpoint[index].portName.in ();

            bool found = false;

            // Now we have to search in the received connections to get the objRef.
            const CORBA::ULong all_conn_len = this->all_connections_->length ();
            for (CORBA::ULong conn_index = 0;
                conn_index < all_conn_len;
                ++conn_index)
              {
                //ACE_DEBUG ((LM_DEBUG, "step4\n"));

                const Deployment::Connection curr_rev_conn = this->all_connections_[conn_index];

                // We need to look at the instance name and the port name to confirm.
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
                    retv[len].endpoint = CORBA::Object::_duplicate(curr_rev_conn.endpoint.in ());
                    ++len;
                    found = true;
                    break;            // Since we know there is only 2 endpoints in a connection.
                                      // so we dont have to worry about multiplex Receptacle etc.
                  }
              }

              // We didnt find the counter part connection even we are sure there must be 1.
              if (!found) return false;
              break; // We know we have found the connection so even we are still on
                     // internalpoint 0 we can skip internalpoint 1.
          }
      }
    }  /* close for loop on internal endpoints */
  }  /* close for loop on all connections in the plan */
  return 1;
}

void
CIAO::DomainApplicationManager_Impl::
dump_connections (const ::Deployment::Connections & connections)
{
  const CORBA::ULong conn_len = connections.length ();
  for (CORBA::ULong i = 0; i < conn_len; ++i)
  {
    ACE_DEBUG ((LM_DEBUG, "instanceName: %s\n", connections[i].instanceName.in ()));
    ACE_DEBUG ((LM_DEBUG, "portName: %s\n", connections[i].portName.in ()));
    ACE_DEBUG ((LM_DEBUG, "portkind: "));
    switch (connections[i].kind) {
    case Deployment::Facet: ACE_DEBUG ((LM_DEBUG, "Facet\n")); break;
    case Deployment::SimplexReceptacle: ACE_DEBUG ((LM_DEBUG, "SimplexReceptacle\n")); break;
    case Deployment::MultiplexReceptacle: ACE_DEBUG ((LM_DEBUG, "MultiplexReceptacle\n")); break;
    case Deployment::EventEmitter: ACE_DEBUG ((LM_DEBUG, "EventEmitter\n")); break;
    case Deployment::EventPublisher: ACE_DEBUG ((LM_DEBUG, "EventPublisher\n")); break;
    case Deployment::EventConsumer: ACE_DEBUG ((LM_DEBUG, "EventConsumer\n")); break;
    }

    // object reference.
    ACE_DEBUG ((LM_DEBUG, "endpoint: \n"));
  }
}
