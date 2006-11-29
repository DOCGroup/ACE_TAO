// $Id$

#include "NodeManager_Impl.h"
#include "../NodeApplicationManager/NodeApplicationManager_Impl.h"
#include "ace/Log_Msg.h"

CIAO::NodeManager_Impl_Base::NodeManager_Impl_Base (const char *name,
                                                    CORBA::ORB_ptr orb,
                                                    PortableServer::POA_ptr poa,
                                                    const char * nodeapp_loc,
                                                    const char * nodeapp_options,
                                                    int spawn_delay)
  ACE_THROW_SPEC ((CORBA::SystemException))
    : orb_ (CORBA::ORB::_duplicate (orb)),
      poa_ (PortableServer::POA::_duplicate (poa)),
      name_ (CORBA::string_dup (name)),
      nodeapp_location_ (CORBA::string_dup (nodeapp_loc)),
      nodeapp_options_ (CORBA::string_dup (nodeapp_options)),
      callback_poa_ (PortableServer::POA::_nil ()),
      spawn_delay_ (spawn_delay)
{
}

CIAO::NodeManager_Impl_Base::~NodeManager_Impl_Base ()
{
}

void
CIAO::NodeManager_Impl_Base::init (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      //create the call back poa for NAM.
      PortableServer::POAManager_var mgr
        = this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->callback_poa_ =
        this->poa_->create_POA ("callback_poa",
                                mgr.in (),
                                0
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "NodeManager_Impl::init\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

PortableServer::POA_ptr
CIAO::NodeManager_Impl_Base::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}


char *
CIAO::NodeManager_Impl_Base::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->name_.in ());
}

void
CIAO::NodeManager_Impl_Base::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
CIAO::NodeManager_Impl_Base::joinDomain (const Deployment::Domain & domain,
                                         Deployment::TargetManager_ptr target,
                                         Deployment::Logger_ptr
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //  ACE_THROW (CORBA::NO_IMPLEMENT ());
  // Here start the Monitor
  CIAO_TRACE("CIAO::NodeManager_Impl_Base::joinDomain");

  ::Deployment::Domain this_domain = domain;

  monitor_controller_.reset (
                             new MonitorController (orb_.in (),
                                                    this_domain,
                                                    target,
                                                    this));

  if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG , "Before Activate\n"));
    }

  // Activate the Monitor Controller to
  // start the monitoring
  monitor_controller_->activate ();

  if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG , "Monitor Activated\n"));
    }
}

void
CIAO::NodeManager_Impl_Base::leaveDomain (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Delete the monitor , this will also terminate the thread
  monitor_controller_.reset ();
}

CORBA::Long
CIAO::NodeManager_Impl_Base::set_priority (
                                           const char * plan_id,
                                           const char * cid,
                                           const ::Deployment::Sched_Params & nm_params
                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS
                                           )
  ACE_THROW_SPEC ((::CORBA::SystemException))

{
  ACE_CString key (plan_id);
  key += "@";
  key += this->name_.in ();

  if (CIAO::debug_level () > 10)
    {
      ACE_DEBUG ((LM_DEBUG , "Inside the set_priority\n"));
      ACE_DEBUG ((LM_DEBUG , "pid = [%s] , cid = [%s]\n", key.c_str () , cid));
    }

  try {
    CORBA::Object_var obj =
      this->poa_->id_to_reference (this->map_.get_nam (key));

    Deployment::NodeApplicationManager_var nam =
      Deployment::NodeApplicationManager::_narrow (obj.in ());

    return nam->set_priority (cid, nm_params);
  }
  catch (CORBA::Exception& ex)
    {

      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P|%t) NodeManager_Impl::set_priority ()\t\n");
      ACE_RE_THROW;
    }
}

::Components::FacetDescriptions *
CIAO::NodeManager_Impl_Base::
get_all_facets (ACE_CString & name)
{
  Component_Facets_Map::ENTRY *entry = 0;

  if (this->comp_facets_map_.find (name.c_str (), entry) != 0)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) - NodeManager_Impl_Base::get_all_facets - "
                  "No component with name [%s] was found in the NodeManager\n", name.c_str ()));
    }

  CORBA::ULong const facet_len = entry->int_id_->length ();

  Components::FacetDescriptions_var retv;
  ACE_NEW_RETURN (retv,
                  Components::FacetDescriptions,
                  0);

  retv->length (facet_len);

  for (CORBA::ULong i = 0; i < facet_len; ++i)
    {
      retv[i] = entry->int_id_[i];
    }

  return retv._retn ();
}

::Components::ConsumerDescriptions *
CIAO::NodeManager_Impl_Base::
get_all_consumers (ACE_CString & name)
{
  Component_Consumers_Map::ENTRY *entry = 0;

  if (this->comp_consumers_map_.find (name.c_str (), entry) != 0)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) - NodeManager_Impl_Base::get_all_facets - "
                  "Component [%s] was not found in the NodeManager\n", name.c_str ()));
    }

  CORBA::ULong const consumer_len = entry->int_id_->length ();

  Components::ConsumerDescriptions_var retv;
  ACE_NEW_RETURN (retv,
                  Components::ConsumerDescriptions,
                  0);

  retv->length (consumer_len);

  for (CORBA::ULong i = 0; i < consumer_len; ++i)
    {
      retv[i] = entry->int_id_[i];
    }

  return retv._retn ();
}

void
CIAO::NodeManager_Impl_Base::
set_all_facets (ACE_CString &name,
                const ::Components::FacetDescriptions_var & facets)
{
  this->comp_facets_map_.rebind (name, facets);
}

void
CIAO::NodeManager_Impl_Base::
set_all_consumers (ACE_CString &name,
                   const ::Components::ConsumerDescriptions_var & consumers)
{
  this->comp_consumers_map_.rebind (name, consumers);
}


Deployment::NodeApplicationManager_ptr
CIAO::NodeManager_Impl_Base::
preparePlan (const Deployment::DeploymentPlan &plan
             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError,
                   Deployment::PlanError))
{
  CIAO_TRACE("CIAO::NodeManager_Impl::preparePlan");

  if (! this->validate_plan (plan))
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) NodeManager <%s>:prepare_plan:Plan_Error.\n",
                  plan.instance[0].node.in ()));
      ACE_ERROR ((LM_ERROR, "(%P|%t) All component instances hosted in the "
                  "same component server must have the "
                  "same \"resourceName\" defined.\n"));

      ACE_THROW_RETURN (Deployment::PlanError (),
                        Deployment::NodeApplicationManager::_nil ());
    }

  // Update the reference count map based on the deployment plan input
  for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
    {
      Reference_Count_Map::ENTRY *entry = 0;
      if (this->ref_count_map_.find (plan.instance[i].name.in (), entry) != 0)
        {
          // Create a new entry, set the initial ref count "1", and insert to the map.
          Ref_Count_Info new_entry;
          new_entry.plan_uuid_ = plan.UUID.in ();
          new_entry.count_ = 1;
          this->ref_count_map_.bind (plan.instance[i].name.in (), new_entry);
        }
      else
        {
          // If the instance is  within the same deployment plan, e.g.,
          // when ReDaC service is used, then just do nothing since it is NOT
          // actually a shared component at all.
          if (ACE_OS::strcmp (plan.UUID.in (),
                              entry->int_id_.plan_uuid_.c_str ()) == 0)
            continue;

          // Otherwise, it is really a shared component, so let's increase
          // the ref count by 1
          this->shared_components_.insert (plan.instance[i].name.in ());
          ++ entry->int_id_.count_;
        }
    }

  // Create/find NodeApplicationManager and set/reset plan on it
  ACE_TRY
    {
      if (!this->map_.is_available (plan.UUID.in ()))
        {
          if (CIAO::debug_level () > 10)
            {
              ACE_DEBUG ((LM_DEBUG, "NM:prepare_plan: "
                          "creating a new NAM with UUID: %s\n",
                          plan.UUID.in ()));
            }

          // Implementation undefined.
          CIAO::NodeApplicationManager_Impl_Base *node_app_mgr =
            this->create_node_app_manager (this->orb_.in (), this->poa_.in ()
                                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          PortableServer::ServantBase_var safe (node_app_mgr);

          //@@ Note: after the init call the servant ref count would
          //   become 2. so we can leave the safeservant along and be
          //   dead. Also note that I added
          PortableServer::ObjectId_var oid  =
            node_app_mgr->init (this->nodeapp_location_.in (),
                                this->nodeapp_options_.in (),
                                this->spawn_delay_,
                                plan,
                                this->callback_poa_.in (),
                                this // pass in a copy of ourself (servant object)
                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->map_.insert_nam (plan.UUID.in (), oid.in ());

          CORBA::Object_var obj =
            this->poa_->id_to_reference (this->map_.get_nam (plan.UUID.in ()));
          ACE_TRY_CHECK;

          // We should inform NAM about "shared" components, so they
          // won't be instantiated again
          Deployment::NodeApplicationManager_var nam =
            Deployment::NodeApplicationManager::_narrow (obj.in ());

          // Convert the ACE Set into CORBA sequence, and make the remote invocation
          Deployment::ComponentPlans_var shared =
            this->get_shared_components_i ();
          nam->set_shared_components (shared.in ());

          // narrow should return a nil reference if it fails.
          return Deployment::NodeApplicationManager::_narrow (nam.in ());
        }
      else
        {
          if (CIAO::debug_level () > 10)
            {
              ACE_DEBUG ((LM_DEBUG, "NM:prepare_plan: reusing an old NAM "
                          "with UUID: %s\n",
                          plan.UUID.in ()));
            }

          CORBA::Object_var obj =
            this->poa_->id_to_reference (this->map_.get_nam (plan.UUID.in ()));
          ACE_TRY_CHECK;

          Deployment::NodeApplicationManager_var nam =
            Deployment::NodeApplicationManager::_narrow (obj.in ());
          ACE_TRY_CHECK;

          nam->reset_plan (plan);
          ACE_TRY_CHECK;

          // Similarly, we should inform NAM about "shared" components, so
          // they won't be instantiated again
          Deployment::ComponentPlans_var shared =
            this->get_shared_components_i ();
          nam->set_shared_components (shared.in ());

          // Potentially we could reset many other configuration settings
          // such as command line options, service configuration file, etc.
          return nam._retn ();
        }
    }
  ACE_CATCH (PortableServer::POA::ObjectNotActive, ex)
    {
      ACE_THROW_RETURN (Deployment::StartError (),
                        Deployment::NodeApplicationManager::_nil ());
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P|%t) NodeManager_Impl::preparePlan\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (Deployment::NodeApplicationManager::_nil ());

  return Deployment::NodeApplicationManager::_nil ();
}

void
CIAO::NodeManager_Impl_Base::destroyManager
(Deployment::NodeApplicationManager_ptr manager
 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StopError,
                   Deployment::InvalidReference))
{
  CIAO_TRACE("CIAO::NodeManager_Impl::destroyManager");
  ACE_TRY
    {
      // Deactivate this object
      PortableServer::ObjectId_var id =
        this->poa_->reference_to_id (manager
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!this->map_.remove_nam (id.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "NodeManager_Impl::destroyManager: "
                      "Unable to remove object from map!\n"));
        }

      this->poa_->deactivate_object (id.in ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (PortableServer::POA::WrongAdapter, ex)
    {
      ACE_ERROR ((LM_ERROR,
                  "NodeManager_Impl::destroyManager: EXCEPTION -  "
                  "Invalid reference passed to destroyManager\n"));

      ACE_THROW (::Deployment::InvalidReference ());
    }
  ACE_CATCH (PortableServer::POA::ObjectNotActive, ex)
    {
      ACE_ERROR ((LM_ERROR,
                  "NodeManager_Impl::destroyManager: EXCEPTION - "
                  " asked to destroy an already inactive object.\n"));
      ACE_THROW (::Deployment::InvalidReference ());
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "NodeManager_Impl::destroyManager\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
}

void
CIAO::NodeManager_Impl_Base::
destroyPlan (const Deployment::DeploymentPlan & plan
             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((::CORBA::SystemException,
                   ::Deployment::StopError))
{
  // Update the reference counting map (subtract by 1 for each instance)
  // If the ref count becomes 0, then remove this component, otherwise,
  // we should remove the necesary bindings on this component specified
  // in the deployment plan.

  // Clean up the cached "Facets" and "Consumers" map of the components
  // if their ref count become 0
  CORBA::ULong const length = plan.instance.length ();
  for (CORBA::ULong i = 0; i <  length; ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "NM_DP - forloop instance %s\n",
                  plan.instance[i].name.in ()));

      Reference_Count_Map::ENTRY *entry = 0;
      if (this->ref_count_map_.find (plan.instance[i].name.in (), entry) == 0)
        {
          --entry->int_id_.count_; // decrease ref count by 1

          if (entry->int_id_.count_ == 0)
            {
              // Remove this component from the shared set
              ACE_DEBUG ((LM_DEBUG, "\tremoving shared...\n"));
              this->shared_components_.remove (plan.instance[i].name.in ());
              ACE_DEBUG ((LM_DEBUG, "\tunbinding from the ref count map\n"));
              // Unbind this component from the ref_count_map_
              this->ref_count_map_.unbind (plan.instance[i].name.in ());

              ACE_DEBUG ((LM_DEBUG, "\tunbinding from the facet/consumer maps\n"));
              // Unbind this component from the facet/consumer maps
              if (this->comp_facets_map_.unbind (
                                                 plan.instance[i].name.in ()) != 0 ||
                  this->comp_consumers_map_.unbind (
                                                    plan.instance[i].name.in ()) != 0)
                {
                  ACE_TRY_THROW
                    (Deployment::StopError ("NodeManager_Impl_Base::destroyPlan ",
                                            "Unable to find component instance"));
                }
            }
        }
    }

  // Find the NAM from the map and invoke the destroyPlan() operation on
  // it, which will actuall remove components and connections in this plan.
  // If
  CORBA::Object_var obj =
    this->poa_->id_to_reference (this->map_.get_nam (plan.UUID.in ()));

  Deployment::NodeApplicationManager_var nam =
    Deployment::NodeApplicationManager::_narrow (obj.in ());


  // Reset each NAM about the shared components information
  Deployment::ComponentPlans_var shared = this->get_shared_components_i ();
  nam->set_shared_components (shared.in ());

  nam->destroyApplication (0);

  // @@ The problem is that we should NOT actually kill the NA process if
  // there are some components that are shared by other plans.
}

Deployment::ComponentPlans *
CIAO::NodeManager_Impl_Base::
get_shared_components (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((::CORBA::SystemException))
{
  return this->get_shared_components_i ();
}

Deployment::ComponentPlans *
CIAO::NodeManager_Impl_Base::get_shared_components_i (void)
{
  Deployment::ComponentPlans_var retv;
  ACE_NEW_RETURN (retv,
                  Deployment::ComponentPlans,
                  0);
  retv->length (0);

  ACE_Unbounded_Set<ACE_CString>::iterator
    end = this->shared_components_.end ();

  for (ACE_Unbounded_Set<ACE_CString>::iterator
         iter = this->shared_components_.begin ();
       iter != end;
       ++iter)
    {
      CORBA::ULong const curr_len = retv->length ();
      retv->length (curr_len + 1);
      (*retv)[curr_len].name = (*iter).c_str ();

      // Fill in the plan_uuid information about this component, by
      // searching in the ref_count_map_
      Reference_Count_Map::ENTRY *entry = 0;
      if (this->ref_count_map_.find ((*iter).c_str (), entry) == 0)
        {
          // Get the plan_uuid_ info and populate the field
          (*retv)[curr_len].plan_uuid = entry->int_id_.plan_uuid_.c_str ();
        }
      else
        {
          // should never happen
          ACE_ERROR ((LM_ERROR, "Component [%s] in the list of shared component, "
                      "was not found in the NodeManager ref count map.\n",
                      (*iter).c_str ()));
        }
    }

  return retv._retn ();
}


CORBA::StringSeq *
CIAO::NodeManager_Impl_Base::shared_components_seq (void)
{
  CORBA::StringSeq * retv = 0;
  ACE_NEW_RETURN (retv, CORBA::StringSeq, 0);
  retv->length (0);

  // @todo rewrite to just set the length once and then copy the strings
  // to reduce allocations

  ACE_Unbounded_Set<ACE_CString>::iterator end = this->shared_components_.end ();
  for (ACE_Unbounded_Set<ACE_CString>::iterator
         iter = this->shared_components_.begin ();
       iter != end;
       ++iter)
    {
      CORBA::ULong const curr_len = retv->length ();
      retv->length (curr_len + 1);
      (*retv)[curr_len] = (*iter).c_str ();
    }

  return retv;
}

bool
CIAO::NodeManager_Impl_Base::
validate_plan (const Deployment::DeploymentPlan &plan)
{
  const char * resource_id = 0;
  CORBA::ULong i = 0;

  // Update the name of ourself
  this->name_ = plan.instance[0].node.in ();

  for (i = 0; i < plan.instance.length (); ++i)
    {
      if (plan.instance[i].deployedResource.length () != 0)
        {
          // Since the "name" field represents the server_resource_id, and
          // the "resourceValue" field represents the policy_set_id, so we
          // are checking to make sure that all component instances have
          // the same server_resource_id.

          resource_id =
            plan.instance[i].deployedResource[0].resourceName.in ();
          break;
        }
    }
  if (i == plan.instance.length ()) // No server resource id has been set for any instance
    return true;

  for (i = 0; i < plan.instance.length (); ++i)
    {
      const char * my_resource_id = 0;
      if (plan.instance[i].deployedResource.length () == 0)
        {
          continue;
        }
      else
        {
          my_resource_id =
            plan.instance[i].deployedResource[0].resourceName.in ();;
          if (ACE_OS::strcmp (resource_id, my_resource_id) != 0)
            {
              // Error, inconsistent server resource id found.
              return false;
            }
        }
    }
  return true;
}


void CIAO::NodeManager_Impl_Base::
push_component_id_info (Component_Ids comps)
{
  components_ = comps;
}

CIAO::NodeManager_Impl_Base::Component_Ids
CIAO::NodeManager_Impl_Base::
get_component_detail ()
{
  return components_;
}

CIAO::NodeManager_Impl::~NodeManager_Impl ()
{
}

CIAO::NodeManager_Impl::
NodeManager_Impl (const char *name,
                  CORBA::ORB_ptr orb,
                  PortableServer::POA_ptr poa,
                  const char * nodeapp_loc,
                  const char * nodeapp_options,
                  int spawn_delay)
  : NodeManager_Impl_Base (name, orb, poa, nodeapp_loc, nodeapp_options, spawn_delay)
{}


::CIAO::NodeApplicationManager_Impl_Base *
CIAO::NodeManager_Impl::
create_node_app_manager (CORBA::ORB_ptr orb,
                         PortableServer::POA_ptr poa
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CIAO::NodeApplicationManager_Impl_Base *app_mgr = 0;
  ACE_NEW_THROW_EX (app_mgr,
                    CIAO::NodeApplicationManager_Impl (orb,
                                                       poa),
                    CORBA::NO_MEMORY ());
  return app_mgr;
}

CIAO::Static_NodeManager_Impl::~Static_NodeManager_Impl ()
{
}

CIAO::Static_NodeManager_Impl::
Static_NodeManager_Impl (const char *name,
                         CORBA::ORB_ptr orb,
                         PortableServer::POA_ptr poa,
                         const char * nodeapp_loc,
                         const char * nodeapp_options,
                         int spawn_delay,
                         Static_Config_EntryPoints_Maps* static_config_entrypoints_maps)
  : NodeManager_Impl_Base (name, orb, poa, nodeapp_loc, nodeapp_options, spawn_delay),
    static_config_entrypoints_maps_ (static_config_entrypoints_maps)
{}

::CIAO::NodeApplicationManager_Impl_Base *
CIAO::Static_NodeManager_Impl::
create_node_app_manager (CORBA::ORB_ptr orb,
                         PortableServer::POA_ptr poa
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CIAO::debug_level () > 10)
    {
      ACE_DEBUG ((LM_DEBUG, "creating static_node_app_manager\n"));
    }

  CIAO::NodeApplicationManager_Impl_Base *app_mgr = 0;
  ACE_NEW_THROW_EX (app_mgr,
                    CIAO::Static_NodeApplicationManager_Impl (orb,
                                                              poa,
                                                              this->static_config_entrypoints_maps_),
                    CORBA::NO_MEMORY ());
  return app_mgr;
}

void
CIAO::Static_NodeManager_Impl::destroyManager
(Deployment::NodeApplicationManager_ptr manager
 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StopError,
                   Deployment::InvalidReference))
{
  CIAO_TRACE("CIAO::Static_NodeManager_Impl::destroyManager");
  ACE_TRY
    {
      CIAO::NodeManager_Impl_Base::destroyManager (manager ACE_ENV_ARG_PARAMETER);

      this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Static_NodeManager_Impl::destroyManager\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  ACE_CHECK;
}
