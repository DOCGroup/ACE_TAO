// $Id$

#include "DomainApplicationManager_Impl.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_string.h"

#include "CIAO/DnC/Config_Handlers/DnC_Dump.h"

#if !defined (__ACE_INLINE__)
# include "DomainApplicationManager_Impl.inl"
#endif /* __ACE_INLINE__ */

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
  // Initialize the <all_connections_> sequence.
  //@@ Gan, this is not necessary.
  this->all_connections_->length (0);
}

CIAO::DomainApplicationManager_Impl::~DomainApplicationManager_Impl ()
{
  //@@ Gan, this is not necessary either. The destructor will do this for you.
  this->artifact_map_.unbind_all ();
}

void
CIAO::DomainApplicationManager_Impl::
init (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((Deployment::ResourceNotAvailable,
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
      for (size_t i = 0; i < this->num_child_plans_; i++)
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

          Chained_Artifacts artifacts = entry->int_id_;

          // Call preparePlan() method on the NodeManager with the
          // corresponding child plan as input, which returns a
          // NodeApplicationManager object reference.  @@TODO: Does
          // preparePlan take a _var type variable?

	  //@@ Gan, shouldn't you do narrow here in case something goes wrong?
          ::Deployment::NodeApplicationManager_var my_nam =
            my_node_manager->preparePlan (artifacts.child_plan_
                                          ACE_ENV_ARG_PARAMETER);

          if (CORBA::is_nil (my_nam.in ()))
            {
              ACE_DEBUG ((LM_DEBUG, "DomainAppMgr::init () received a nil\
                                     reference for NodeApplicationManager\n"));
              ACE_THROW (Deployment::StartError ());
            }
          ACE_TRY_CHECK;

          // Cache the NodeApplicationManager object reference.
          artifacts.node_application_manager_ = my_nam._retn ();
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "DomainApplicationManager_Impl::init\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

int
CIAO::DomainApplicationManager_Impl::
get_plan_info (void)
{
  size_t length = this->plan_.instance.length ();

  // Error: If there are no nodes in the plan => No nodes to deploy the
  // components
  if (length == 0)
    return 0;

  // Copy the name of the node in the plan on to the node manager
  // array, Making sure that duplicates are not added twice
  int num_plans = 0;
  for (CORBA::ULong index = 0; index < length; index ++)
    {
      int matched = 0;
      for (size_t i = 0; i < this->node_manager_names_.size (); i++)
        // If a match is found do not add it to the list of unique
        // node names
        if (ACE_OS::strcmp (this->plan_.instance [index].node.in (),
                            (this->node_manager_names_ [i]).c_str ()) == 0)
          {
            // Break out -- Duplicates found
            matched = 1;
            break;
          }

      if (! matched)
        {
          // Check if there is a corresponding NodeManager instance existing
          // If not present return false
          if (this->deployment_config_.get_node_manager
              (this->plan_.instance [index].node.in ()) == 0)
            return 0; /* Failure */

          // Add this unique node_name to the list of NodeManager names
          this->node_manager_names_.push_back
            (CORBA::string_dup
             (this->plan_.instance [index].node.in ()));

          // Increment the number of plans
          ++ num_plans;
        }
    }

  // Set the length of the Node Managers
  this->num_child_plans_ = num_plans;

  // Indicate success
  return 1;
}

//@@ We should ask those spec writers to look at the code below, hopefully
//   They will realize some thing.
int
CIAO::DomainApplicationManager_Impl::
split_plan (void)
{
  // Initialize the total number of child deployment plans specified
  // by the global plan.

 for (size_t i = 0; i < this->num_child_plans_; i++)
  {
    ::Deployment::DeploymentPlan_var tmp_plan;
    ACE_NEW_RETURN (tmp_plan,
                    ::Deployment::DeploymentPlan,
                    0);

    tmp_plan->UUID = CORBA::string_dup (this->plan_.UUID.in ());

    //@@Gan, all the following initialization is not necessary.
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

   ACE_DEBUG ((LM_DEBUG, "after: initialize empty child plans...\n"));

  // (1) Iterate over the <instance> field of the global DeploymentPlan
  //     variabl.
  // (2) Retrieve the necessary information to contruct the node-level
  //     plans one by one.

  for (CORBA::ULong i = 0; i < (this->plan_.instance).length (); i++)
    {
      // Fill in the child deployment plan in the map.

      // Get the instance deployment description

      //@@  Gan, after the first cut we should use either pointer or reference
      //    to reduce a bit of overhead of copying.
      ::Deployment::InstanceDeploymentDescription my_instance =
        (this->plan_.instance)[i];

      // Find the corresponding child deployment plan entry in
      // the hash map for this instance.
      ACE_Hash_Map_Entry
        <ACE_CString,
        Chained_Artifacts> *entry;

      if (this->artifact_map_.find
          (ACE_CString (my_instance.node.in ()), //is this parameter correct?
	                                         //@@ Gan, now it should be correct.
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
      ::Deployment::MonolithicDeploymentDescription my_implementation =
        (this->plan_.implementation)[my_instance.implementationRef];

      CORBA::ULong index_imp = child_plan->implementation.length ();
      child_plan->implementation.length (++index_imp);
      child_plan->implementation[index_imp-1] = my_implementation;

      // @@TODO: Create a ULong sequence of artifactRef, which will be
      // as the new artifactRef field for the implementation struct.

      // Initialize with the correct sequence length.
      CORBA::ULongSeq ulong_seq (my_implementation.artifactRef.length ());

      // Append the "ArtifactDeploymentDescriptions artifact" field
      // with some new "artifacts", which is specified by the
      // <artifactRef> sequence of <my_implementation> entry.
      for (CORBA::ULong iter = 0;
           iter < my_implementation.artifactRef.length ();
           iter ++)
        {
          CORBA::ULong artifact_ref = my_implementation.artifactRef[iter];

          CORBA::ULong index_art = child_plan->artifact.length ();
          child_plan->artifact.length (++index_art);
          child_plan->artifact[index_art-1] =
            (this->plan_.artifact)[artifact_ref];

          // @@ The artifactRef starts from 0.
          ulong_seq[iter] = index_art;
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
      child_plan->instance[index_ins-1].implementationRef = index_ins-1;
    }

  return 1;
}

void
CIAO::DomainApplicationManager_Impl::
add_connections (::Deployment::Connections & incoming_conn)
{
  CORBA::ULong old_len = this->all_connections_->length ();

  // Expand the length of the <all_connection_> sequence.
  this->all_connections_->length (old_len + incoming_conn.length ());

  // Store the connections to the <all_conections_> sequence
  for (CORBA::ULong i = 0; i < incoming_conn.length (); i++)
  {
    (*this->all_connections_)[old_len + i] = incoming_conn[i];
  }
}

void
CIAO::DomainApplicationManager_Impl::
get_outgoing_connections (::Deployment::Connections_out provided,
                          ::Deployment::DeploymentPlan &plan)
{
  ::Deployment::Connections_var retn_connections;
  ACE_NEW (retn_connections,
           ::Deployment::Connections);

  // For each component instance in the child plan ...
  for (CORBA::ULong i = 0; i < plan.instance.length (); i++)
    {
      // Get the component instance name
      CORBA::String_var instance_name = (plan.instance[i].name).in();

      // Find out all the PlanConnectionDescriptions from the global plan
      // where this component instance plays the role as either "receptacle"
      // or "event sink".
      for (CORBA::ULong j = 0; j < this->plan_.connection.length (); j++)
        {
	  //@@Gan, seems that you are assuming that there is a special sequence
	  //  between the two subcomponentportendpoints in a connection. Has Jai
	  //  confirmed this for you about if he will follow the rule?

	  //@@  Gan, after the first cut we should use either pointer or reference
	  //    to reduce a bit of overhead of copying.
          ::Deployment::PlanConnectionDescription tmp_conn
            = this->plan_.connection[j];

	  //@@  Gan, after the first cut we should use either pointer or reference
	  //    to reduce a bit of overhead of copying.
          ::Deployment::PlanSubcomponentPortEndpoint dest_endpoint
            = tmp_conn.internalEndpoint[1]; /* The "destination" end point */

          // instanceRef of this particular receiver side component instance.
          CORBA::ULong dest_instanceRef = dest_endpoint.instanceRef;

          // Check whether the name is the same as <instance_name>.
          if (! ACE_OS::strcmp (this->plan_.instance[dest_instanceRef].name.in (),
                                instance_name.in ()))
            continue; // This connection is not got involved.

          // Otherwise, this connection is what we are interested in ...

	  //@@  Gan, after the first cut we should use either pointer or reference
	  //    to reduce a bit of overhead of copying.
          ::Deployment::PlanSubcomponentPortEndpoint src_endpoint
            = tmp_conn.internalEndpoint[0]; /* The "source" end point */

          // instanceRef of the provider side component instance.
          CORBA::ULong src_instanceRef = src_endpoint.instanceRef;

          // Get the prvoider side component instance name and port name.
	  //@@ Gan, using string_var here is not really necessary,
	  //   a plain const char * is better. Too many copies are made unnecessarily.
          CORBA::String_var provider_name =
            (this->plan_.instance[src_instanceRef].name).in ();

          CORBA::String_var port_name = (src_endpoint.portName).in ();

          // Fetch the connections out from the <all_connections_> by
          // comparing the "component instance name" and "port name".
          for (CORBA::ULong k = 0; k < this->all_connections_->length (); k++)
            {
              if (! ACE_OS::strcmp ((*this->all_connections_)[k].instanceName.in (),
                                    provider_name.in ()))
                continue;

              if (! ACE_OS::strcmp ((*this->all_connections_)[k].portName.in (),
                                    port_name.in ()))
                continue;

              CORBA::ULong length = retn_connections->length ();
              retn_connections->length (length + 1);

	      //@@ Gan, here you have to switch the name for me. You should give back
	      // the event_publisher/emitter or the recetacle's name to me. Because
	      // I don't know anything about the connection as a pair of two endpoints.
	      // also the port kind is needed to be switched.
              (*retn_connections)[length] = this->all_connections_[k];
              (*retn_connections)[length].kind = src_endpoint.kind;
            }
        }
    }

    provided = retn_connections._retn ();
    return;
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
      for (size_t i = 0; i < this->num_child_plans_; i++)
        {
          // Get the NodeApplicationManager object reference.
          ACE_Hash_Map_Entry
            <ACE_CString,
            Chained_Artifacts> *entry;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            ACE_THROW (Deployment::StartError ()); // Should never happen!

          ::Deployment::NodeApplicationManager_var my_nam =
            (entry->int_id_).node_application_manager_;

          ::Deployment::Connections_var retn_connections;

          // Obtained the returned NodeApplication object reference
          // and the returned Connections variable.
          ::Deployment::Application_var temp_application =
            my_nam->startLaunch (configProperty,
                                 retn_connections.out (),
                                 start);

          // Narrow down to NodeApplication object reference
          ::Deployment::NodeApplication_var my_na =
            ::Deployment::NodeApplication::_narrow (temp_application.in ()
                                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Cache the returned set of connections into the list.
          this->add_connections (retn_connections);

          // Cache the returned NodeApplication object reference into
          // the hash table.
          (entry->int_id_).node_application_ = my_na;
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
  ACE_TRY
    {
      // Invoke finishLaunch() operation on each cached NodeApplication object.
      for (size_t i = 0; i < this->num_child_plans_; i++)
        {
          // Get the NodeApplication object reference.
          ACE_Hash_Map_Entry
            <ACE_CString,
            Chained_Artifacts> *entry;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            ACE_THROW (Deployment::StartError ()); // Should never happen!

          ::Deployment::NodeApplication_var my_na =
            (entry->int_id_).node_application_;

          // Get the Connections variable.
          ::Deployment::Connections_var my_connections;
          this->get_outgoing_connections (my_connections.out (),
                                          (entry->int_id_).child_plan_);

          // Invoke finishLaunch() operation on NodeApplication.
          my_na->finishLaunch (my_connections,
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
      return;
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (0);
}


void
CIAO::DomainApplicationManager_Impl::
start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Deployment::StartError))
{
  ACE_TRY
    {
      // Invoke start() operation on each cached NodeApplication object.
      for (size_t i = 0; i < this->num_child_plans_; i++)
        {
          // Get the NodeApplication object reference.
          ACE_Hash_Map_Entry
            <ACE_CString,
            Chained_Artifacts> *entry;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            ACE_THROW (Deployment::StartError ()); // Should never happen!

          ::Deployment::NodeApplication_var my_na =
            (entry->int_id_).node_application_;

          my_na->start (ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "DomainApplicationManager_Impl::start\t\n");
      ACE_RE_THROW;
      return;
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (0);
}

void
CIAO::DomainApplicationManager_Impl::
destroyApplication ()
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Deployment::StopError))
{
  ACE_TRY
    {
      // Invoke destroyManager() operation on each cached
      // NodeManager object.
      // @@NOTE: This is different from the DnC spec since we think the
      for (size_t i = 0; i < this->num_child_plans_; i++)
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

          ::Deployment::NodeApplicationManager_var my_node_application_manager =
            (entry->int_id_).node_application_manager_;

          // Invoke destoryManager() operation on the NodeManger.
          my_node_manager->destroyManager (my_node_application_manager.in ()
                                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "DomainApplicationManager_Impl::destroyApplication\t\n");
      ACE_RE_THROW;
      return;
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (0);
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
