//$Id$

#include "NodeApplication_Impl.h"
#include "ace/SString.h"
#include "Container_Impl.h"

#if !defined (__ACE_INLINE__)
# include "NodeApplication_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::NodeApplication_Impl::~NodeApplication_Impl (void)
{
}

CORBA::Long
CIAO::NodeApplication_Impl::init (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //@@TODO initialize this NodeApplication properties
  return 0;
}

CORBA::Long
CIAO::NodeApplication_Impl::create_all_containers (
    const ::Deployment::ContainerImplementationInfos & container_infos
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Create all the containers here based on the input node_impl_info.
  const CORBA::ULong len = container_infos.length ();
  
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      // The factory method <create_container> will intialize the container
      // servant with properties, so we don't need to call <init> on the
      // container object reference.
      // Also, the factory method will add the container object reference
      // to the set for us.
      ::Deployment::Container_var cref =
        this->create_container (container_infos[i].container_config);
    }
  
  return 0;
}

void
CIAO::NodeApplication_Impl::finishLaunch (
    const Deployment::Connections & providedReference,
    CORBA::Boolean start
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError,
                   Deployment::InvalidConnection))
{
  ACE_UNUSED_ARG (start);

  ACE_TRY
    {
      const CORBA::ULong length = providedReference.length ();

      // For every connection struct we finish the connection.
      for (CORBA::ULong i = 0; i < length; ++i)
        {
          ACE_CString name = providedReference[i].instanceName.in ();
          Components::CCMObject_ptr comp;

          if (this->component_map_.find (name, comp) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "CIAO (%P|%t) - NodeApplication_Impl.cpp, "
                          "CIAO::NodeApplication_Impl::finishLaunch, "
                          "invalid port name [%s] in instance [%s] \n",
                          providedReference[i].portName.in (),
                          name.c_str ()));
              ACE_TRY_THROW (Deployment::InvalidConnection ());
            }

          Components::EventConsumerBase_var consumer;

          // Since we know CCMObject inherits from 
          // navigation/event/receptacle, no need
          // to narrow here.

          switch (providedReference[i].kind)
            {
            case Deployment::SimplexReceptacle:
            case Deployment::MultiplexReceptacle:

              if (CIAO::debug_level () > 9)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "CIAO (%P|%t) - NodeApplication_Impl.cpp, "
                              "CIAO::NodeApplication_Impl::finishLaunch, "
                              "connecting port name [%s] in instance [%s] \n",
                              providedReference[i].portName.in (),
                              name.c_str ()));
                }

              comp->connect (providedReference[i].portName.in (),
                             providedReference[i].endpoint.in ()
                             ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              if (CIAO::debug_level () > 9)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "CIAO (%P|%t) - NodeApplication_Impl.cpp, "
                              "CIAO::NodeApplication_Impl::finishLaunch, "
                              "success connecting port name [%s] in "
                              "instance [%s] \n",
                              providedReference[i].portName.in (),
                              name.c_str ()));
                }
              break;

	        // @@ (GD) A place holder where the Event Channel connections
	        //         should be set up.

            case Deployment::EventEmitter:

              consumer = Components::EventConsumerBase::
                _narrow (providedReference[i].endpoint.in ()
                         ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              if (CORBA::is_nil (consumer.in ()))
                {
                  ACE_ERROR ((LM_ERROR,
                              "CIAO (%P|%t) - NodeApplication_Impl.cpp, "
                              "CIAO::NodeApplication_Impl::finishLaunch, "
                              "for port name [%s] in instance [%s] ,"
                              "there is an invalid endPoint. \n",
                              providedReference[i].portName.in (),
                              name.c_str ()));
                  ACE_TRY_THROW (Deployment::InvalidConnection ());
                }

              if (CIAO::debug_level () > 9)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "CIAO (%P|%t) - NodeApplication_Impl.cpp, "
                              "CIAO::NodeApplication_Impl::finishLaunch, "
                              "connecting port name [%s] in instance [%s] \n",
                              providedReference[i].portName.in (),
                              name.c_str ()));
                }

              comp->connect_consumer (providedReference[i].portName.in (),
                                      consumer.in ()
                                      ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              if (CIAO::debug_level () > 9)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "CIAO (%P|%t) - NodeApplication_Impl.cpp, "
                              "CIAO::NodeApplication_Impl::finishLaunch, "
                              "success connecting port name [%s] in "
                              "instance [%s] \n",
                              providedReference[i].portName.in (),
                              name.c_str ()));
                }
              break;

            case Deployment::EventPublisher:

              consumer = Components::EventConsumerBase::
                _narrow (providedReference[i].endpoint.in ()
                         ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              if (CORBA::is_nil (consumer.in ()))
                {
                  ACE_ERROR ((LM_ERROR,
                              "CIAO (%P|%t) - NodeApplication_Impl.cpp, "
                              "CIAO::NodeApplication_Impl::finishLaunch, "
                              "for port name [%s] in instance [%s] ,"
                              "there is an invalid endPoint. \n",
                              providedReference[i].portName.in (),
                              name.c_str ()));
                  ACE_TRY_THROW (Deployment::InvalidConnection ());
                }

              if (CIAO::debug_level () > 9)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "CIAO (%P|%t) - NodeApplication_Impl.cpp, "
                              "CIAO::NodeApplication_Impl::finishLaunch, "
                              "connecting port name [%s] in instance [%s] \n",
                              providedReference[i].portName.in (),
                              name.c_str ()));
                }

              comp->subscribe (providedReference[i].portName.in (),
                               consumer.in ()
                               ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              if (CIAO::debug_level () > 9)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "CIAO (%P|%t) - NodeApplication_Impl.cpp, "
                              "CIAO::NodeApplication_Impl::finishLaunch, "
                              "success connecting port name [%s] in "
                              "instance [%s] \n",
                              providedReference[i].portName.in (),
                              name.c_str ()));
                }
              break;

            default:
              ACE_TRY_THROW (Deployment::InvalidConnection ());
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "NodeApplication_Impl::finishLaunch\t\n");
      ACE_RE_THROW;
    }

  ACE_ENDTRY;
}

void
CIAO::NodeApplication_Impl::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError))
{
  Component_Iterator end = this->component_map_.end ();
  for (Component_Iterator iter (this->component_map_.begin ());
       iter != end;
       ++iter)
  {
    ((*iter).int_id_)->ciao_preactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
}

void
CIAO::NodeApplication_Impl::start (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError))
{
  Component_Iterator end = this->component_map_.end ();
  for (Component_Iterator iter (this->component_map_.begin ());
       iter != end;
       ++iter)
  {
    ((*iter).int_id_)->ciao_activate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
}

void
CIAO::NodeApplication_Impl::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError))
{
  Component_Iterator end = this->component_map_.end ();
  for (Component_Iterator iter (this->component_map_.begin ());
       iter != end;
       ++iter)
  {
    ((*iter).int_id_)->ciao_postactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
}

void
CIAO::NodeApplication_Impl::ciao_passivate (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StopError))
{
  Component_Iterator end = this->component_map_.end ();
  for (Component_Iterator iter (this->component_map_.begin ());
       iter != end;
       ++iter)
  {
    ((*iter).int_id_)->ciao_passivate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
}

Deployment::ComponentInfos *
CIAO::NodeApplication_Impl::install (
    const ::Deployment::NodeImplementationInfo & node_impl_info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::UnknownImplId,
                   Deployment::ImplEntryPointNotFound,
                   Deployment::InstallationFailure,
                   Components::InvalidConfiguration))
{
  Deployment::ComponentInfos_var retv;
  ACE_TRY
    {
      // Extract ORB resource def here.
      // this->configurator_.init_resource_manager (xxx);

      const ::Deployment::ContainerImplementationInfos container_infos =
        node_impl_info.impl_infos;

      ACE_NEW_THROW_EX (retv,
                        Deployment::ComponentInfos,
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      retv->length (0UL);

      // Call create_all_containers to create all the necessary containers..
      if (CIAO::debug_level () > 9)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "CIAO (%P|%t) NodeApplication_Impl.cpp -"
                      "CIAO::NodeApplication_Impl::install -"
                      "creating all the containers. \n"));
        }
      (void) this->create_all_containers (container_infos);
      if (CIAO::debug_level () > 9)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "CIAO (%P|%t) NodeApplication_Impl.cpp -"
                      "CIAO::NodeApplication_Impl::install -"
                      "created all the containers. \n"));
        }

      // For each container, invoke <install> operation, this will return
      // the ComponentInfo for components installed in each container.
      // Merge all the returned ComponentInfo, which will be used
      // as the return value of this method.
      const CORBA::ULong num_containers = container_infos.length ();
      for (CORBA::ULong i = 0; i < num_containers; ++i)
        {
          Deployment::ComponentInfos_var comp_infos =
            this->container_set_.at(i)->install (container_infos[i]
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Append the return sequence to the *big* return sequence
          CORBA::ULong curr_len = retv->length ();
          retv->length (curr_len + comp_infos->length ());

          for (CORBA::ULong j = curr_len; j < retv->length (); j++)
            retv[j] = comp_infos[j-curr_len];
        }

      // @@ Maybe we can optimize this. We can come up with a decision later.
      // Cache a copy of the component object references for all the components
      // installed on this NodeApplication. I know we can delegates these to the
      // undelying containers, but in that case, we should loop 
      // all the containers
      // to find the component object reference. - Gan
      const CORBA::ULong comp_len = retv->length ();
      for (CORBA::ULong len = 0;
          len < comp_len;
          ++len)
      {
        //Since we know the type ahead of time...narrow is omitted here.
        if (this->component_map_.bind (retv[len].component_instance_name.in(),
              Components::CCMObject::_duplicate (retv[len].
                component_ref.in ())))
          {
            ACE_DEBUG ((LM_DEBUG,
                        "CIAO (%P|%t) NodeApplication_Impl.cpp -"
                        "CIAO::NodeApplication_Impl::install -"
                        "error binding component instance [%s] "
                        "into the map. \n",
                        retv[len].component_instance_name.in ()));
            ACE_TRY_THROW (
               Deployment::InstallationFailure ("NodeApplication_Imp::install",
                                       "Duplicate component instance name"));
          }
      }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "CIAO_NodeApplication::install error\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  return retv._retn ();
}

void
CIAO::NodeApplication_Impl::remove (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // For each container, invoke <remove> operation to remove home and components.
  const CORBA::ULong set_size = this->container_set_.size ();
  for (CORBA::ULong i = 0; i < set_size; ++i)
    {
      this->container_set_.at(i)->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  // Remove all containers
  // Maybe we should also deactivate container object reference.
  this->container_set_.remove_all ();

  if (CIAO::debug_level () > 1)
    ACE_DEBUG ((LM_DEBUG, "Removed all containers from this NodeApplication!\n"));

  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}


// Create a container interface, which will be hosted in this NodeApplication.
::Deployment::Container_ptr
CIAO::NodeApplication_Impl::create_container (const ::Deployment::Properties &properties
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                  ::Components::CreateFailure,
                  ::Components::InvalidConfiguration))
{
  if (CIAO::debug_level () > 1)
    ACE_DEBUG ((LM_DEBUG, "ENTERING: NodeApplication_Impl::create_container()\n"));

  CIAO::Container_Impl *container_servant = 0;

  ACE_NEW_THROW_EX (container_servant,
                    CIAO::Container_Impl (this->orb_.in (),
                                          this->poa_.in (),
                                          this->get_objref ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  PortableServer::ServantBase_var safe_servant (container_servant);

  // @TODO: Need to decide a "component_installation" equivalent data
  // structure  to pass to the container, which will be used to
  // suggest how to install the components.  Each such data stucture
  // should be correspond to one <process_collocation> tag  in the XML
  // file to describe the deployment plan.
  container_servant->init (properties
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  PortableServer::ObjectId_var oid
    = this->poa_->activate_object (container_servant
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Object_var obj
    = this->poa_->id_to_reference (oid.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ::Deployment::Container_var ci
    = ::Deployment::Container::_narrow (obj.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Cached the objref in its servant.
  container_servant->set_objref (ci.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

    this->container_set_.add (ci.in ());
  }

  if (CIAO::debug_level () > 1)
    ACE_DEBUG ((LM_DEBUG,
                "LEAVING: NodeApplication_Impl::create_container()\n"));
  return ci._retn ();
}

// Remove a container interface.
void
CIAO::NodeApplication_Impl::remove_container (::Deployment::Container_ptr cref
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                  ::Components::RemoveFailure))
{
  ACE_DEBUG ((LM_DEBUG, "ENTERING: NodeApplication_Impl::remove_container()\n"));
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->container_set_.object_in_set (cref) == 0)
    {
      ACE_THROW (Components::RemoveFailure());
    }

  cref->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // @@ Deactivate object.
  PortableServer::ObjectId_var oid
    = this->poa_->reference_to_id (cref
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_->deactivate_object (oid.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Should we remove the server still, even if the previous call failed.

  if (this->container_set_.remove (cref) == -1)
    {
      ACE_THROW (::Components::RemoveFailure ());
    }
    
  ACE_DEBUG ((LM_DEBUG, "LEAVING: NodeApplication_Impl::remove_container()\n"));
}

// Get containers
::Deployment::Containers *
CIAO::NodeApplication_Impl::get_containers (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}
