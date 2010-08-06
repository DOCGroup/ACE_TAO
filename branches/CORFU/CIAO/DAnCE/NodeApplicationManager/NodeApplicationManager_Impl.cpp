// $Id$

#include "NodeApplicationManager_Impl.h"

#include "ace/Process.h"
#include "ace/Process_Manager.h"
#include "ace/Reactor.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Sched_Params.h"
#include "ace/Vector_T.h"
#include "ciao/Container_Base.h"
#include "DAnCE/Deployment/CIAO_ServerResourcesC.h"
#include "NodeApplication/NodeApplication_Impl.h"
#include "NodeApplication/NodeApp_Configurator.h"
#include "ace/Reactor.h"

#if !defined (__ACE_INLINE__)
# include "NodeApplicationManager_Impl.inl"
#endif /* __ACE_INLINE__ */


bool
CIAO::NodeApplicationManager_Impl_Base::
is_shared_component (ACE_CString & name)
{
  for (CORBA::ULong i = 0; i < this->shared_components_.length (); ++i)
    {
      if (ACE_OS::strcmp (this->shared_components_[i].name.in (),
                          name.c_str ()) == 0)
        return true;
    }

  return false;
}

bool
CIAO::NodeApplicationManager_Impl_Base::
is_external_component (ACE_CString & name)
{
  for (CORBA::ULong i = 0; i < this->external_components_.length (); ++i)
    {
      if (ACE_OS::strcmp (this->external_components_[i].name.in (),
                          name.c_str ()) == 0 &&
          ACE_OS::strcmp (this->external_components_[i].plan_uuid.in (),
                          this->plan_.UUID.in ()))
        return true;
    }

  return false;
}

Deployment::Connections *
CIAO::NodeApplicationManager_Impl_Base::
create_connections ()
{
  CIAO_TRACE("CIAO::NodeApplicationManager_Impl_Base::create_connections");
  Deployment::Connections_var retv;

  ACE_NEW_THROW_EX (retv,
                    Deployment::Connections (),
                    CORBA::NO_MEMORY ());

  CORBA::ULong len = retv->length ();

  const Component_Iterator end (this->component_map_.end ());
  for (Component_Iterator iter (this->component_map_.begin ());
       iter != end;
       ++iter)
    {
      // If this component is in the "shared components list", then we
      // should just simply fetch the port object references from the
      // NodeManager.
      ACE_CString comp_name ((*iter).ext_id_.c_str ());

      // Get all the facets first
      Components::FacetDescriptions_var facets;

      if (this->is_shared_component (comp_name))
        {
          ACE_DEBUG ((LM_DEBUG, "NAMImpl::create_connections: Component %s is shared\n",
                      comp_name.c_str ()));
          facets = this->node_manager_->get_all_facets (comp_name);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "NAMImpl::create_connections: Component %s is not shared, getting and setting "
                      "all facets\n",
                      comp_name.c_str ()));
          facets = ((*iter).int_id_)->get_all_facets ();
          this->node_manager_->set_all_facets (comp_name, facets);
        }

      if (CIAO::debug_level () > 9)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "DAnCE (%P|%t) NodeApplicationManager_Impl.cpp -"
                      "CIAO::NodeApplicationManager_Impl::create_connections -"
                      "success getting facets for the component "
                      "instance [%s] \n",
                      comp_name.c_str ()));
        }

      // Get all the event consumers
      Components::ConsumerDescriptions_var consumers;

      if (this->is_shared_component (comp_name))
        {
          ACE_DEBUG ((LM_DEBUG, "NAMImpl::create_connections: Component %s is shared\n",
                      comp_name.c_str ()));
          consumers = this->node_manager_->get_all_consumers (comp_name);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "NAMImpl::create_connections: Component %s is not shared, getting and setting "
                      "all facets\n",
                      comp_name.c_str ()));
          consumers =
            ((*iter).int_id_)->get_all_consumers ();
          this->node_manager_->set_all_consumers (comp_name, consumers);
        }

      if (CIAO::debug_level () > 9)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "DAnCE (%P|%t) NodeApplicationManager_Impl.cpp -"
                      "CIAO::NodeApplicationManager_Impl::create_connections -"
                      "success getting consumers for the component "
                      "instance [%s] \n",
                      comp_name.c_str ()));
        }

      CORBA::ULong const facet_len = facets->length ();
      CORBA::ULong const consumer_len = consumers->length ();

      CORBA::ULong const curr_len = retv->length ();
      retv->length (curr_len + facet_len + consumer_len);

      CORBA::ULong i = 0;
      for (i = 0; i < facet_len; ++i)
        {
          Deployment::Connection & conn = retv[len];
          conn.instanceName = (*iter).ext_id_.c_str ();
          conn.portName = facets[i]->name ();
          if (CIAO::debug_level () > 9)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "DAnCE (%P|%t) NodeApplicationManager_Impl.cpp -"
                          "CIAO::NodeApplicationManager_Impl::create_connections -"
                          "adding connection for facet [%s] in instance [%s] \n",
                          conn.portName.in (), conn.instanceName.in ()));
            }
          conn.kind = Deployment::Facet;
          conn.endpoint = CORBA::Object::_duplicate (facets[i]->facet_ref ());
          ++len;
        }

      for (i = 0; i < consumer_len; ++i)
        {
          Deployment::Connection & conn = retv[len];
          conn.instanceName = (*iter).ext_id_.c_str ();
          conn.portName = consumers[i]->name ();
          if (CIAO::debug_level () > 9)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "DAnCE (%P|%t) NodeApplicationManager_Impl.cpp -"
                          "CIAO::NodeApplicationManager_Impl::create_connections -"
                          "adding connection for consumer [%s] in instance [%s] \n",
                          conn.portName.in (), conn.instanceName.in ()));
            }
          conn.kind = Deployment::EventConsumer;
          conn.endpoint = CORBA::Object::_duplicate (consumers[i]->consumer ());
          ++len;
        }
    }
  return retv._retn ();
}

Deployment::Application_ptr
CIAO::NodeApplicationManager_Impl_Base::
startLaunch (const Deployment::Properties & configProperty,
             Deployment::Connections_out providedReference,
             CORBA::Boolean start)
{
  try
    {
      CIAO_TRACE("CIAO::NodeApplicationManager_Impl::startLaunch");
      ACE_UNUSED_ARG (configProperty);
      ACE_UNUSED_ARG (start);

      // In this step, we know all the "shared components" are
      // the external components to ourself.
      this->external_components_ = this->shared_components_;

      // If no additional components need to be installed, then we simply
      // create a NA, but doesn't install any components on it.
      if (this->plan_.instance.length () == this->shared_components_.length ())
        {
          ACE_DEBUG ((LM_DEBUG, "Prespawn a NodeApplication process without "
                      "installing any components.\n"));
        }

      /**
       *  1. First Map properties to TAO/CIAO specific property/configurations
       *  2. Necessary property checking (needed?)
       *  3. Call create_nade_application to spawn new process.
       *  4. Initialize the NodeApplication.
       *  5. get the provided connection endpoints back and return them.
       */

      NodeImplementationInfoHandler handler (this->plan_, this->shared_components_);

      Deployment::NodeImplementationInfo * node_info =
        handler.node_impl_info ();

      if (!node_info)
        {
          ACE_ERROR ((LM_ERROR,
                      "DAnCE (%P|%t) NodeApplicationManager.cpp -"
                      "CIAO::NodeApplicationManager_Impl::startLaunch -"
                      "Failed to create Node Implementation Infos!\n"));

          throw
            (Deployment::StartError ("NodeApplicationManager_Imp::startLaunch",
                                     "Unable to get node level infos"));
        }

      CIAO::DAnCE::ServerResource *server_resource = 0;
      for (CORBA::ULong k = 0; k < node_info->nodeapp_config.length (); ++k)
        {
          if (ACE_OS::strcmp (node_info->nodeapp_config[k].name.in (),
                              "CIAOServerResources") == 0)
            {
              node_info->nodeapp_config[0].value >>= server_resource;
              break; // Ignore the rest of the NodeApp_Config values
            }
        }

      // Now spawn the NodeApplication process.
      // @@TODO: we need to pass arguments to the nodeapplication, ie
      // naming service endpoints, if necessary
      // (will)
      ACE_CString cmd_option (this->nodeapp_command_op_.in ());

      if (server_resource)
        {
          // If command line options are specified through RTCCM descriptors,
          // then we should honor these command line options as well.
          for (CORBA::ULong arg_i = 0;
               arg_i < (*server_resource).args.length ();
               ++arg_i)
            {
              cmd_option += " "; // space between command line args
              cmd_option += (*server_resource).args[arg_i];
            }

          // If service configuration file is specified through RTCCM
          // descriptors, then we should honor it as well.
          if (ACE_OS::strcmp ((*server_resource).svcconf.in (),
                              "") != 0)
            {
              cmd_option += " -ORBSvcConf ";
              cmd_option += (*server_resource).svcconf;
            }
        }

      Deployment::NodeApplication_var tmp =
        create_node_application (cmd_option.c_str ());

      if (CIAO::debug_level () > 9)
        {
          CORBA::ULong curr_len = node_info->impl_infos.length ();
          ACE_UNUSED_ARG (curr_len);

          Deployment::ComponentImplementationInfos infos =
            ((node_info->impl_infos)[0]).impl_infos;

          const CORBA::ULong info_len = infos.length ();
          for (CORBA::ULong i = 0; i < info_len; ++i)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "DAnCE (%P|%t) NodeApplicationManager.cpp -"
                          "CIAO::NodeApplicationManager_Impl::startLaunch -"
                          "The info for installation: "
                          "\n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n",
                          infos[i].component_instance_name.in (),
                          infos[i].executor_dll.in (),
                          infos[i].executor_entrypt.in (),
                          infos[i].servant_dll.in (),
                          infos[i].servant_entrypt.in () ));
            }
        }

      // This is what we will get back, a sequence of component object refs.
      Deployment::ComponentInfos_var comp_info;

      // This will install all homes and components.
      comp_info = this->nodeapp_->install (*node_info);


      // Now fill in the map we have for the "newly installed" components.
      const CORBA::ULong comp_len = comp_info->length ();
      for (CORBA::ULong len = 0;
           len < comp_len;
           ++len)
        {
          //FUZZ: disable check_for_lack_ACE_OS
          //Since we know the type ahead of time...narrow is omitted here.
          if (this->component_map_.
              bind (comp_info[len].component_instance_name.in(),
                    Components::CCMObject::_duplicate
                    (comp_info[len].component_ref.in())))
          //FUZZ: enable check_for_lack_ACE_OS
            {
              ACE_CString error ("Duplicate component instance name ");
              error += comp_info[len].component_instance_name.in();

              throw
                (Deployment::StartError
                 ("NodeApplicationManager_Impl::startLaunch",
                  error.c_str ()));
            }
        }

      // Also, we need to fill in the map about those "shared components"
      // For now, we could use "NIL" component object reference for these
      // shared components since they are not used anyway.
      CORBA::ULong shared_comp_length = this->shared_components_.length ();
      for (CORBA::ULong j = 0; j < shared_comp_length; ++j)
        {
          //FUZZ: disable check_for_lack_ACE_OS
          if (this->component_map_.
              bind (this->shared_components_[j].name.in (),
                    Components::CCMObject::_nil ()))
          //FUZZ: enable check_for_lack_ACE_OS
            {
              ACE_CString error ("Duplicate component instance name ");
              error += this->shared_components_[j].name.in();

              throw
                (Deployment::StartError
                 ("NodeApplicationManager_Impl::startLaunch",
                  error.c_str ()));
            }
        }


      providedReference =
        this->create_connections ();

      if (providedReference == 0)
        {
          throw
            (Deployment::StartError
             ("NodeApplicationManager_Impl::startLaunch",
              "Error creating connections for components during startLaunch."));
        }
    }
  catch (const Deployment::UnknownImplId& e)
    {
      throw Deployment::StartError (e.name.in (), e.reason.in ());
    }
  catch (const Deployment::ImplEntryPointNotFound& e)
    {
      throw Deployment::StartError (e.name.in (), e.reason.in ());
    }
  catch (const Deployment::InstallationFailure& e)
    {
      throw Deployment::StartError (e.name.in (), e.reason.in ());
    }

  return Deployment::NodeApplication::_duplicate (this->nodeapp_.in ());
}


Deployment::Application_ptr
CIAO::NodeApplicationManager_Impl_Base::
perform_redeployment (const Deployment::Properties & configProperty,
                      Deployment::Connections_out providedReference,
                      CORBA::Boolean add_or_remove, // true means "add" only
                      CORBA::Boolean start)
{
  // Prerequisite:
  //    (1) If this is an existiing old NAM, then <nodeapp_> is ready to use.
  //        We also got a copy of <plan_> as well as all the installed components
  //        in the <component_map_>.
  //    (2) Then we should call <install> operation on the NA, but in order to do this,
  //        we must pack all the to-be-added components into some appropriate
  //        data structure called "NodeImplementationInfo".
  //    (3) We should also call <remove> operation on the NA to remove those
  //        to-be-removed components, and the "comp_inst_name" could be as input.
  //    (4) We should also consider removing all the unneeded "connections", but
  //        this should be driven by the DAM, so it looks like that we need to
  //        add another operation on the NA interface which is a counterpart of
  //        <finishLaunch>, something like <finishLaunch_remove_only>.
  //
  //
  //
  //    (1) If this is an brand new NAM, then only new installation is needed.
  //    (2) Then we could pretty much replicate the "startLaunch" implementation.
  //        This capability is useful to install a set of new components into
  //        some totally new nodes.


  CIAO_TRACE ("CIAO::NodeApplicationManager_Impl_Base::perform_redeployment");

  ACE_DEBUG ((LM_DEBUG,
              "CIAO (%P|%t) NodeApplicationManager_Impl_Base: "
              "invoked CIAO::NodeApplicationManager_Impl_Base::perform_redeployment \n"));
  try
    {
      if (! CORBA::is_nil (this->nodeapp_.in ()))
        {
          if (add_or_remove == true)
            {
              this->add_new_components ();
            }
          else
            {
              this->remove_existing_components ();
            }

          // NOTE: We are propogating back "all" the facets/consumers object
          // references to the DAM, including the previous existing ones.
          providedReference =
            this->create_connections ();

          if (providedReference == 0)
            {
              throw Deployment::InstallationFailure ("NodeApplicationManager_Impl::startLaunch",
                                                     "Error creating connections during startLaunch.");
            }
        }
      else // This is a new NodeApplication process, then we need to install
           // all the components. We should try to reuse much of the above code.
        {
          this->startLaunch (configProperty,
                             providedReference,
                             start);
        }
    }
  catch (const Deployment::UnknownImplId& e)
    {
      throw Deployment::UnknownImplId (e.name.in (), e.reason.in ());
    }
  catch (const Deployment::ImplEntryPointNotFound& e)
    {
      throw Deployment::ImplEntryPointNotFound (e.name.in (), e.reason.in ());
    }
  catch (const Deployment::InstallationFailure& e)
    {
      throw Deployment::InstallationFailure (e.name.in (), e.reason.in ());
    }

  return Deployment::NodeApplication::_duplicate (this->nodeapp_.in ());
}


void
CIAO::NodeApplicationManager_Impl_Base::
add_new_components ()
{
  try
    {
      // We ignored those components that are already in the <component_map_>, for
      // the rest ones, we pack them into NodeImplementationInfo.
      Deployment::DeploymentPlan tmp_plan = this->plan_;
      tmp_plan.instance.length (0);

      const CORBA::ULong length = this->plan_.instance.length ();
      for (CORBA::ULong i = 0; i < length; ++i)
        {
          // add the new components into the tmp_plan
          if (this->component_map_.find (this->plan_.instance[i].name.in ()) != 0)
            {
              CORBA::ULong cur_len = tmp_plan.instance.length ();
              tmp_plan.instance.length (cur_len + 1);
              tmp_plan.instance[cur_len] = this->plan_.instance[i];
            }
        }

      // If there are no new components to be installed ...
      if (tmp_plan.instance.length () == 0)
        return;

      // package the components
      NodeImplementationInfoHandler handler (tmp_plan,
                                             this->shared_components_);
      Deployment::NodeImplementationInfo * node_info =
        handler.node_impl_info ();

      if (!node_info)
        {
          ACE_ERROR ((LM_ERROR,
                      "DAnCE (%P|%t) NodeApplicationManager.cpp -"
                      "CIAO::NodeApplicationManager_Impl::perform_redeployment -"
                      "Failed to create Node Implementation Infos!\n"));

          throw
            (Deployment::PlanError ("NodeApplicationManager_Imp::perform_redeployment",
                                    "Unable to get node level infos"));
        }

      // Install the components
      // This is what we will get back, a sequence of component object refs.
      Deployment::ComponentInfos_var comp_info;
      comp_info = this->nodeapp_->install (*node_info);

      // Now fill in the map we have for the components.
      const CORBA::ULong comp_len = comp_info->length ();
      for (CORBA::ULong len = 0;
          len < comp_len;
          ++len)
        {
          //FUZZ: disable check_for_lack_ACE_OS
          //Since we know the type ahead of time...narrow is omitted here.
          if (this->component_map_.
              bind (comp_info[len].component_instance_name.in(),
                    Components::CCMObject::_duplicate
                      (comp_info[len].component_ref.in())))
            {
          //FUZZ: enable check_for_lack_ACE_OS
              ACE_CString error ("Duplicate component instance name ");
              error += comp_info[len].component_instance_name.in();

              throw
                (Deployment::PlanError
                  ("NodeApplicationManager_Impl::startLaunch",
                    error.c_str ()));
            }
        }
    }
  catch (const CORBA::Exception&)
    {
      ACE_ERROR ((LM_ERROR, "NodeApplicationManager_Impl_Base::"
                  "add_new_components () exception caught.\n"));
      throw;
    }
}


void
CIAO::NodeApplicationManager_Impl_Base::
remove_existing_components ()
{
  try
    {
      ACE_Vector<ACE_CString> gone_component_list;

      for (Component_Iterator iter (this->component_map_.begin ());
           iter != this->component_map_.end ();
           ++iter)
        {
          ACE_CString comp_name ((*iter).ext_id_.c_str ());

          // If this component is not in the new deployment plan, then we
          // should destroy this component and unbind from the map.
          if (this->is_to_be_removed (comp_name.c_str ()))
            {
              ((*iter).int_id_)->ciao_passivate ();
              this->nodeapp_->remove_component (comp_name.c_str ());
              gone_component_list.push_back (comp_name);
            }
        }

        for (size_t i = 0; i < gone_component_list.size (); ++i)
          this->component_map_.unbind (gone_component_list[i]);
    }
  catch (const CORBA::Exception&)
    {
      ACE_ERROR ((LM_ERROR, "NodeApplicationManager_Impl_Base::"
                  "remove_existing_components () exception caught.\n"));
      throw;
    }
}

bool
CIAO::NodeApplicationManager_Impl_Base::
is_to_be_removed (const char * name)
{
  const CORBA::ULong length = this->plan_.instance.length ();
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      if (ACE_OS::strcmp (name,
             this->plan_.instance[i].name.in ()) == 0)
        {
          // If we have found it in the new plan, then this component
          // needs to be kept, and should not be removed.
          return false;
        }
    }
  return true;
}

void
CIAO::NodeApplicationManager_Impl_Base::
set_shared_components (const Deployment::ComponentPlans & shared)
{
  this->shared_components_ = shared;
}

void
CIAO::NodeApplicationManager_Impl_Base::
destroyApplication (Deployment::Application_ptr app)
{
  CIAO_TRACE("CIAO::NodeApplicationManager_Impl::destroyApplication");
  ACE_UNUSED_ARG (app);

  ACE_DEBUG ((LM_DEBUG, "NAM: entering DA\n"));
  //ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  //@@ Since we know there is only 1 nodeapp so the passed in
  //   parameter is ignored for now.
  if (CORBA::is_nil (this->nodeapp_.in () ))
    throw Deployment::StopError ();

  // Iterate over all the components within this NAM, and if it's
  // not a shared component, then remove it. If all the components
  // are removed, then we shall kill the NA totally.
  for (CORBA::ULong i = 0; i < this->plan_.instance.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "NAM: first for loop: %s\n",
                  this->plan_.instance[i].name.in ()));
      ACE_CString name = plan_.instance[i].name.in ();
      if (this->is_shared_component (name))
        {
          this->component_map_.unbind (name);
          continue;
        }

      // If this is not a shared component and is installed within
      // this NAM, then remove it. Otherwise, we do nothing.
      // Ideally, we should ask NM to remove this component for
      // us even if this is not within this NAM.
      if (! this->is_external_component (name))
        {
          this->nodeapp_->remove_component (name.c_str ());
          this->component_map_.unbind (name);
        }
    }

  // Call remove on NodeApplication, if all the components are removed,
  // then the NodeApplication will kill itself.
  ACE_DEBUG ((LM_DEBUG, "NAM: calling remove\n"));
  this->nodeapp_->remove ();
  ACE_DEBUG ((LM_DEBUG, "NAM: remove returned\n"));

  return;
}

// The set priority method
::CORBA::Long
CIAO::NodeApplicationManager_Impl_Base::set_priority (
                                                      const char * cid,
                                                      const ::Deployment::Sched_Params & params)
{
  if (CIAO::debug_level () > 20)
    {
      ACE_DEBUG ((LM_DEBUG , "NAM::The component Id received [%s]", cid));

      ACE_DEBUG ((LM_DEBUG ,
                  "NAM::The params are policy [%d], priority [%d], "
                  "scope [%d], time [%d]\n",
                  params.policy_ ,
                  params.priority_,
                  params.scope_, params.msec_));
    }

  // First validate the values coming in ....
  ACE_Sched_Params::Policy policy = params.policy_;

  if (policy != ACE_SCHED_FIFO &&
      policy != ACE_SCHED_RR &&
      policy != ACE_SCHED_OTHER)
    return -1;

  ACE_Sched_Priority priority = params.priority_;

  // check the scope ..
  if (params.scope_ != ACE_SCOPE_PROCESS &&
      params.scope_ != ACE_SCOPE_THREAD &&
      params.scope_ != ACE_SCOPE_LWP)
    {
      return -1;
    }

  // FUZZ: disable check_for_lack_ACE_OS
  // Here form the ACE_Sched_Params structure and pass it on to the Process
  // manager with the current process id.
  // @@ TODO: Right now we are ignoring params.msec_ value since
  // ACE_OS::sched_params fails setting errno = EINVAL if
  // scope = ACE_PROCESS_SCOPE and quantun != ACE_Time_Value:zero.
  ACE_Sched_Params sched_params (policy,
                                 priority,
                                 params.scope_,
                                 ACE_Time_Value::zero);
  // FUZZ: enable check_for_lack_ACE_OS

  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (node_app_process_manager_.set_scheduler (sched_params, process_id_) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "User is not superuser, therefore cannot modify the "
                      "priority of the component\n"));
        }
      else if (ACE_OS::last_error () == ESRCH)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "No process with PID: %d was found!\n",
                      process_id_));
        }
      else if (ACE_OS::last_error () == EINVAL)
        {
          ACE_DEBUG ((LM_DEBUG, "\nGiven sched_params does not make sence "
                      "for the current scheduling policy\n"));
        }
      ACE_ERROR ((LM_ERROR, "NodeApplicationManager  (%P|%t): sched_params "
                  "failed\n"));
      return -1;
    }
  return 1;
}

///////////////////////////////////////////////////////////////////////

CIAO::NodeApplicationManager_Impl::~NodeApplicationManager_Impl (void)
{
}

CIAO::NodeApplicationManager_Impl::
NodeApplicationManager_Impl (CORBA::ORB_ptr o,
                             PortableServer::POA_ptr p,
                             bool is_multi_threaded)
  : NodeApplicationManager_Impl_Base (o, p)
    , is_multi_threaded_ (is_multi_threaded)
{
}

PortableServer::ObjectId
CIAO::NodeApplicationManager_Impl::init (
                                         const char *nodeapp_location,
                                         const char *nodeapp_op,
                                         const CORBA::ULong delay,
                                         const Deployment::DeploymentPlan & plan,
                                         const PortableServer::POA_ptr callback_poa,
                                         NodeManager_Impl_Base * nm)
{
  PortableServer::ObjectId_var oid;

  try
    {
      if (nodeapp_location == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "DAnCE (%P|%t) NodeApplicationManager_Impl.cpp -"
                      "CIAO::NodeApplicationManager_Impl::init -"
                      "NULL NodeApplication location. \n"));
          throw CORBA::BAD_PARAM ();
        }

      if (delay == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "DAnCE (%P|%t) NodeApplicationManager_Impl.cpp -"
                      "CIAO::NodeApplicationManager_Impl::init -"
                      "NodeManager must be started with a -d "
                      "of greter than zero.\n"));
          throw CORBA::BAD_PARAM ();
        }

      this->nodeapp_path_.set (nodeapp_location);
      this->spawn_delay_ = delay;
      this->nodeapp_command_op_ = CORBA::string_dup (nodeapp_op);
      this->node_manager_ = nm;

      // Make a copy of the plan for later usage.
      this->plan_ =  plan;

      // Cache the call back POA for callback object.
      this->callback_poa_ = PortableServer::POA::_duplicate (callback_poa);

      // Activate the ourself.
      oid = this->poa_->activate_object (this);

      CORBA::Object_var obj =
        this->poa_->id_to_reference (oid.in ());

      // And cache the object reference.
      this->objref_ =
        Deployment::NodeApplicationManager::_narrow (obj.in ());

      // add the signal handler to the ACE_REACTOR

      /*
        if (orb_->orb_core ()->reactor ()->
        register_handler (SIGCHLD,
        &child_handler_) == -1)

        if (ACE_Reactor::instance ()->register_handler (SIGCHLD,
        &child_handler_) == -1)
        {
        ACE_DEBUG ((LM_DEBUG, "Error in registering Handler\n\n"));
        }
      */
    }

  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("NodeApplicationManager_Impl_Base::init\t\n");
      throw;
    }

  //return this object reference
  return oid.in ();
}

Deployment::NodeApplication_ptr
CIAO::NodeApplicationManager_Impl::
create_node_application (const ACE_CString & options)
{
  CIAO_TRACE("CIAO::NodeApplicationManager_Impl::create_node_application");
  Deployment::NodeApplication_var retval;
  Deployment::Properties_var prop;

  ACE_NEW_THROW_EX (prop,
                    Deployment::Properties,
                    CORBA::NO_MEMORY ());

  // @@ Create a new callback servant.
  CIAO::NodeApplication_Callback_Impl * callback_servant = 0;
  ACE_NEW_THROW_EX (callback_servant,
                    CIAO::NodeApplication_Callback_Impl (this->orb_.in (),
                                                         this->callback_poa_.in (),
                                                         this->objref_.in (),
                                                         prop.in (),
                                                         this->waitCond_,
                                                         this->mutex_),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var servant_var (callback_servant);
  PortableServer::ObjectId_var cb_id
    = this->callback_poa_->activate_object (callback_servant);

  ACE_Process_Options p_options;
  ACE_Process_Manager process_manager;

  process_manager.open (10, ACE_Reactor::instance ());

  try
    {
      CORBA::Object_var cb_obj =
        this->callback_poa_->id_to_reference (cb_id.in ());

      CIAO::NodeApplication_Callback_var cb =
        CIAO::NodeApplication_Callback::_narrow (cb_obj.in ());

      CORBA::String_var cb_ior =
        this->orb_->object_to_string (cb.in ());

      // spawn the new NodeApplication.
      p_options.command_line ("%s -k %s "
                              "%s",
                              this->nodeapp_path_.c_str (),
                              cb_ior.in (),
                              options.c_str ());

      p_options.avoid_zombies (0);

      process_id_ = node_app_process_manager_.spawn (p_options,
                                                     &child_handler_);

      if (process_id_ ==  ACE_INVALID_PID)
        {
          if (CIAO::debug_level () > 1)
            {
              ACE_ERROR ((LM_ERROR,
                          "Fail to spawn a NodeApplication process\n"));
            }

          throw
            (Deployment::ResourceNotAvailable
             ("Failed to spawn process",
              "NodeApplication",
              "",
              "",
              ""));
        }

      // wait for nodeApp to pass back its object reference. with a
      // timeout value. using perform_work and stuff.
      ACE_Time_Value timeout (this->spawn_delay_, 0);

      if (this->is_multi_threaded_)
        retval = multi_threaded_wait_for_callback (callback_servant, timeout);
      else
        retval = single_threaded_wait_for_callback (callback_servant, timeout);

      if (CORBA::is_nil (retval.in ()))
        {
          if (CIAO::debug_level () > 1)
            {
              ACE_ERROR ((LM_ERROR,
                          "Fail to acquire the NodeApplication object\n"));
            }

          throw Deployment::ResourceNotAvailable ();
        }

      {
        //ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);
        this->nodeapp_ =
          Deployment::NodeApplication::_duplicate (retval.in ());
      }
    }
  catch (const CORBA::Exception&)
    {
      this->callback_poa_->deactivate_object (cb_id.in ());

      throw;
    }

  this->callback_poa_->deactivate_object (cb_id.in ());

  if (CIAO::debug_level () > 1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "CIAO::NodeApplicationManager_Impl::NodeApplication spawned!\n"));
    }

  // push the component info and the process id to the
  // NodeManager
  push_component_info (process_id_);


  return retval._retn ();
}


Deployment::NodeApplication_ptr
CIAO::NodeApplicationManager_Impl::
single_threaded_wait_for_callback (
  CIAO::NodeApplication_Callback_Impl * cb_servant,
  ACE_Time_Value &timeout)
{
  // Below code is broken for thread-per-connection concurrency model,
  // since the main thread is running ORB event loop and will spawn
  // a different thread to handle the call <register_node_application>,
  // the <perform_work> operation will not be invoked and finally
  // a timeout will occur. For a similar reason, it won't work
  // for thread-pool concurrency model.
  Deployment::NodeApplication_var retv;
  while (true)
    {
      this->orb_->perform_work (timeout);

      retv = cb_servant->get_nodeapp_ref ();

      if ((timeout == ACE_Time_Value::zero) || !CORBA::is_nil (retv.in ()))
        break;
    }

  return retv._retn ();
}

Deployment::NodeApplication_ptr
CIAO::NodeApplicationManager_Impl::
multi_threaded_wait_for_callback (
   CIAO::NodeApplication_Callback_Impl* cb_servant,
   ACE_Time_Value &timeout)
{
  Deployment::NodeApplication_var retv;

  // Wait for a conditional variable
  this->mutex_.acquire ();
  while (! cb_servant->is_callback_completed ())
    this->waitCond_.wait (&timeout);
  retv = cb_servant->get_nodeapp_ref ();
  this->mutex_.release ();

  return retv._retn ();
}


void
CIAO::NodeApplicationManager_Impl::
push_component_info (pid_t process_id)
{
  CIAO::NodeManager_Impl_Base::Component_Ids comp;

  for (unsigned int i=0;i < plan_.instance.length ();i++)
    {
      if (CIAO::debug_level () > 10)
        {
          ACE_DEBUG ((LM_DEBUG, "The component id is [%s]",
                      plan_.instance[i].name.in ()));
        }
      comp.cid_seq_.insert (plan_.instance[i].name.in ());
    }

  comp.process_id_ = process_id;

  node_manager_->push_component_id_info (comp);
}

/////////////////////////////////////////////////////////////////////////

CIAO::Static_NodeApplicationManager_Impl::~Static_NodeApplicationManager_Impl (void)
{
}

CIAO::Static_NodeApplicationManager_Impl::
Static_NodeApplicationManager_Impl (CORBA::ORB_ptr o,
                                    PortableServer::POA_ptr p,
                                    Static_Config_EntryPoints_Maps* static_config_entrypoints_maps)
  : NodeApplicationManager_Impl_Base (o, p),
    static_config_entrypoints_maps_ (static_config_entrypoints_maps)
{
}

PortableServer::ObjectId
CIAO::Static_NodeApplicationManager_Impl::init (
                                                const char *nodeapp_location,
                                                const char *nodeapp_op,
                                                const CORBA::ULong delay,
                                                const Deployment::DeploymentPlan & plan,
                                                const PortableServer::POA_ptr callback_poa,
                                                NodeManager_Impl_Base * nm)
{
  PortableServer::ObjectId_var oid;

  ACE_UNUSED_ARG (nodeapp_location);
  ACE_UNUSED_ARG (nodeapp_op);
  ACE_UNUSED_ARG (delay);
  ACE_UNUSED_ARG (callback_poa);

  try
    {
      this->node_manager_ = nm;

      // Make a copy of the plan for later usage.
      this->plan_ =  plan;

      // Activate the ourself.
      oid = this->poa_->activate_object (this);

      CORBA::Object_var obj =
        this->poa_->id_to_reference (oid.in ());

      // And cache the object reference.
      this->objref_ =
        Deployment::NodeApplicationManager::_narrow (obj.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("NodeApplicationManager_Impl_Base::init\t\n");
      throw;
    }

  //return this object reference
  return oid.in ();
}

Deployment::NodeApplication_ptr
CIAO::Static_NodeApplicationManager_Impl::
create_node_application (const ACE_CString & options)
{
  ACE_UNUSED_ARG(options);

  CIAO::NodeApplication_Impl *nodeapp_servant;

  ACE_DEBUG ((LM_DEBUG, "create_static_node_application\n"));

  ACE_NEW_RETURN (nodeapp_servant,
                  CIAO::NodeApplication_Impl (orb_.in (),
                                              poa_.in (),
                                              configurator_,
                                              this->static_config_entrypoints_maps_),
                  Deployment::NodeApplication::_nil ()
                  );
  if (nodeapp_servant->init ())
    {
      ACE_DEBUG ((LM_DEBUG, "NodeApplication Failed on creating and\
                               initializing the session container!"));
      return Deployment::NodeApplication::_nil ();
    }

  // CONFIGURING NodeApplication
  PortableServer::ObjectId_var nodeapp_oid
    = poa_->activate_object (nodeapp_servant);

  CORBA::Object_var
    obj = poa_->id_to_reference (nodeapp_oid.in ());

  Deployment::NodeApplication_var nodeapp_obj =
    Deployment::NodeApplication::_narrow (obj.in ());

  if (CORBA::is_nil (nodeapp_obj.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Unable to activate NodeApplication object\n"));
      return Deployment::NodeApplication::_nil ();
    }

  this->nodeapp_ = Deployment::NodeApplication::_duplicate (nodeapp_obj.in ());

  return nodeapp_obj._retn ();
}

CIAO::NodeApplicationManager_Impl_Base::~NodeApplicationManager_Impl_Base (void)
{
}
