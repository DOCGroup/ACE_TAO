//$Id$

#include "NodeApplication_Impl.h"
#include "ace/UUID.h"
#include "string.h" // for debug

#include "orbsvcs/CosNamingC.h"

#if !defined (__ACE_INLINE__)
# include "NodeApplication_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::NodeApplication_Impl::~NodeApplication_Impl (void)
{
  delete this->container_;
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
              ACE_TRY_THROW (Deployment::InvalidConnection ());
            }

          Components::EventConsumerBase_var consumer;
          //Since we know CCMObject inherits from navigation/event/receptacle, no need
          //to narrow here.
          switch (providedReference[i].kind)
            {
              case Deployment::SimplexReceptacle:
                comp->connect (providedReference[i].portName.in (),
                              providedReference[i].endpoint.in ()
                              ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK;
                break;

              case Deployment::MultiplexReceptacle:
                comp->connect (providedReference[i].portName.in (),
                              providedReference[i].endpoint.in ()
                              ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK;
                break;

	          // @@ (GD) A place holder where the Event Channel connections
	          //         should be set up.
              case Deployment::EventEmitter:
              case Deployment::EventPublisher:
                ACE_DEBUG ((LM_DEBUG, "Building direct connection.\n"));
                this->build_event_connection (providedReference[i], 
                                              CIAO::DIRECT
                                              ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK;
			          break;

			        case Deployment::rtecEventEmitter:
              case Deployment::rtecEventPublisher:
                ACE_DEBUG ((LM_DEBUG, "Building real-time event channel connection.\n"));
                this->build_event_connection (providedReference[i], 
                                              CIAO::RTEC
                                              ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK;
			          break;

			        case Deployment::ecEventEmitter:
              case Deployment::ecEventPublisher:
                ACE_DEBUG ((LM_DEBUG, "Building CoS event channel connection.\n"));
                this->build_event_connection (providedReference[i], 
                                              CIAO::EC
                                              ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK;
			          break;

              case Deployment::nsEventEmitter:
              case Deployment::nsEventPublisher:
                ACE_DEBUG ((LM_DEBUG, "Building notification channel connection.\n"));
                this->build_event_connection (providedReference[i], 
                                              CIAO::NOTIFY
                                              ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK;
			          break;

              default:
                ACE_TRY_THROW (Deployment::InvalidConnection ());
            }
        }
      if (start)
        {
          this->start (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
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
CIAO::NodeApplication_Impl::start (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError))
{
  //@@ Note: set_session_context will be called when the servant is created.
  Funct_Ptr functor = & Components::CCMObject::ciao_preactivate;
  start_i (functor ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  functor = & Components::CCMObject::ciao_activate;
  start_i (functor ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  functor = & Components::CCMObject::ciao_postactivate;
  start_i (functor ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
CIAO::NodeApplication_Impl::start_i (Funct_Ptr functor
         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError))
{
  const Component_Iterator end (this->component_map_.end ());
  for (Component_Iterator iter (this->component_map_.begin ());
       iter != end;
       ++iter)
  {
    //@@ I don't know what if Components::InvalidConfiguration
    //   is thrown from here, so it's ignored for now.  --Tao
    (((*iter).int_id_)->*functor) (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
}

Deployment::Properties *
CIAO::NodeApplication_Impl::properties (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Deployment::Properties * tmp;

  ACE_NEW_THROW_EX (tmp,
                    Deployment::Properties (this->properties_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return tmp;
}

Deployment::ComponentInfos *
CIAO::NodeApplication_Impl::install (
    const ::Deployment::ImplementationInfos & impl_infos
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
     Deployment::ComponentInfos * tmp;
     ACE_NEW_THROW_EX (tmp,
                       Deployment::ComponentInfos,
                       CORBA::NO_MEMORY ());
     ACE_TRY_CHECK;

     retv = tmp;

     const CORBA::ULong len = impl_infos.length ();

     retv->length (len);

     // @@ (OO) There is no need to declare these variables outside of
     //         the loop.  Some folks doing so is an optimization but
     //         doing so generally defeats some compiler optimizations.
     //         Please move these declaration within the loop.
     Components::CCMHome_var home;
     Components::CCMObject_var comp;

     for (CORBA::ULong i = 0; i < len; ++i)
       {
         home = this->install_home (impl_infos[i]
                                    ACE_ENV_ARG_PARAMETER);
         ACE_TRY_CHECK;

         Components::KeylessCCMHome_var kh =
           Components::KeylessCCMHome::_narrow (home.in ()
                                                ACE_ENV_ARG_PARAMETER);
         ACE_TRY_CHECK;

         if (CORBA::is_nil (kh.in ()))
           ACE_THROW_RETURN (Deployment::InstallationFailure (), 0);

         // @@ Note, here we are missing the CreateFailure.
         // Sometime I will come back to add exception rethrow.
         comp = kh->create_component (ACE_ENV_SINGLE_ARG_PARAMETER);
         ACE_TRY_CHECK;

         
         // @@ Set up a component_UUID for this component
         /*
         ACE_Utils::UUID_Generator uuid_gen;
         ACE_Utils::UUID * p_uuid = uuid_gen.generateUUID ();

         char* str_tmp = p_uuid->to_string ()->c_str ();
         */

         //char* str_tmp;
         //strcpy (str_tmp, impl_infos[i].component_instance_name.in ());
         ACE_DEBUG ((LM_DEBUG, "UUID is %s\n", impl_infos[i].component_instance_name.in ()));
         ACE_DEBUG ((LM_DEBUG, "################ Making a remote CORBA call on component named: %s\n", 
                    impl_infos[i].component_instance_name.in ()));

         comp->component_UUID (impl_infos[i].component_instance_name.in ());
         


         if (this->component_map_.bind (impl_infos[i].component_instance_name.in (),
                                        Components::CCMObject::_duplicate (comp.in ())))
           ACE_TRY_THROW (Deployment::InstallationFailure ());

         // Set the return value.
         (*retv)[i].component_instance_name
           = impl_infos[i].component_instance_name.in ();

         (*retv)[i].component_ref = Components::CCMObject::_duplicate (comp.in ());

         // Deal with Component instance related Properties.
         // Now I am only concerning about the COMPOENTIOR and here is only
         // the hardcoded version of the configuration. Hopefully we will
         // reach an agreement after the RTWS about how the configuration
         // should be done.

         const CORBA::ULong clen = impl_infos[i].component_config.length ();
         for (CORBA::ULong prop_len = 0; prop_len < clen; ++prop_len)
           {
             if (ACE_OS::strcmp (impl_infos[i].component_config[prop_len].name.in (),
                                 "ComponentIOR") == 0)
               {
                 if (CIAO::debug_level () > 1)
                  {
                    ACE_DEBUG ((LM_DEBUG, "Found property to write the IOR.\n"));
                  }
                 const char * path;
                 impl_infos[i].component_config[prop_len].value >>= path;

                 CORBA::String_var ior =
                   this->orb_->object_to_string (comp.in ()
                                                 ACE_ENV_ARG_PARAMETER);
                 ACE_TRY_CHECK;

                 if (write_IOR (path, ior.in ()) != 0)
                   {
                     if (CIAO::debug_level () > 1)
                       ACE_DEBUG ((LM_DEBUG, "Failed to write the IOR.\n"));

                     ACE_TRY_THROW (CORBA::INTERNAL ());
                   }

                 // Also register the component with the naming service
                 ACE_DEBUG ((LM_DEBUG, "Register component with the naming service.\n"));
                 if (! register_with_ns (impl_infos[i].component_instance_name.in (),
                                         this->orb_.in (),
                                         comp.in ()
                                         ACE_ENV_ARG_PARAMETER))
                   {
                     ACE_DEBUG ((LM_DEBUG, "Failed to register with the naming service.\n"));
                   }
                 ACE_TRY_CHECK;
               }
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

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
Components::CCMHome_ptr
CIAO::NodeApplication_Impl::install_home (
    const ::Deployment::ImplementationInfo & impl_info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::UnknownImplId,
                   Deployment::ImplEntryPointNotFound,
                   Deployment::InstallationFailure,
                   Components::InvalidConfiguration))
{
  Components::CCMHome_var newhome =
    this->container_->ciao_install_home
    (impl_info.executor_dll.in (),
     impl_info.executor_entrypt.in (),
     impl_info.servant_dll.in (),
     impl_info.servant_entrypt.in ()
     ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (Components::CCMHome::_nil ());
  // We don't have to do _narrow since the generated code makes sure of
  // the object type for us

  // Bind the home in the map.
  if (this->home_map_.bind (impl_info.component_instance_name.in (),
                            Components::CCMHome::_duplicate (newhome.in ())))
    ACE_THROW_RETURN (Deployment::InstallationFailure (),
                      Components::CCMHome::_nil ());

  //Note: If the return value will be discarded, it must be kept in a var or
  //      release () will have to be called explicitly.
  return newhome._retn ();
}

void
CIAO::NodeApplication_Impl::remove (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  // Remove all components first.
  remove_components ();

  ACE_CHECK;

  // Even if above op failed we should still remove homes.
  const Home_Iterator end (this->home_map_.end ());
  for (Home_Iterator iter (this->home_map_.begin ());
       iter != end;
       ++iter)
    {
      this->container_->ciao_uninstall_home ( (*iter).int_id_
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::release ( (*iter).int_id_);
    }

  this->home_map_.unbind_all ();

  if (CIAO::debug_level () > 1)
    ACE_DEBUG ((LM_DEBUG, "Shutting down this NodeApplication!\n"));

  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}

void
CIAO::NodeApplication_Impl::remove_home (const char * comp_ins_name
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  Components::CCMHome_ptr home;
  ACE_CString str (comp_ins_name);

  if (this->home_map_.find (str, home) != 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  // We should remove all components created by this home as well.
  // This is not implemented yet.

  this->container_->ciao_uninstall_home (home
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // If the previous calls failed, what should we do here??
  CORBA::release (home);

  // @@ Still need to remove the home if the previous operation fails?
  if (this->home_map_.unbind (str) == -1)
    ACE_THROW (::Components::RemoveFailure ());
}

Components::CCMHomes *
CIAO::NodeApplication_Impl::get_homes (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Components::CCMHomes * tmp;
  ACE_NEW_THROW_EX (tmp,
                    Components::CCMHomes (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  Components::CCMHomes_var retval (tmp);

  // @@ (OO) Please declare len as "const".
  CORBA::ULong len = this->home_map_.current_size ();
  retval->length (len);


  CORBA::ULong i = 0;
  const Home_Iterator end = this->home_map_.end ();
  for (Home_Iterator iter (this->home_map_.begin ());
       iter != end;
       ++iter, ++i)
  {
    retval[i] = Components::CCMHome::_duplicate ( (*iter).int_id_);
  }

  return retval._retn ();
}

CORBA::Long
CIAO::NodeApplication_Impl::init (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_NEW_THROW_EX (this->container_,
                    CIAO::Session_Container (this->orb_.in (),
                                             0,
                                             0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (-1);

  return this->container_->init (0,
                                 0
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

CORBA::Object_ptr
CIAO::NodeApplication_Impl::get_node_application_manager (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ::CORBA::Object::_duplicate (this->node_app_manager_.in ());
}


PortableServer::POA_ptr
CIAO::NodeApplication_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
CIAO::NodeApplication_Impl::remove_components (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  //Remove all the components in the NodeApplication/Container
  // Release all component servant object.
  const Component_Iterator end (this->component_map_.end ());
  for (Component_Iterator iter (this->component_map_.begin ());
       iter != end;
       ++iter)
  {
    Components::CCMHome_ptr home;
    if (this->home_map_.find ( (*iter).ext_id_, home) != 0)
      ACE_THROW (CORBA::BAD_PARAM ());

    // Unbind the component name from the name server.
    if (! unregister_with_ns ((*iter).ext_id_.c_str (),
                              this->orb_.in ()
                              ACE_ENV_ARG_PARAMETER))
      {
        ACE_DEBUG ((LM_DEBUG, "Failed to unregister with the naming service.\n"));
      }
    ACE_CHECK;

    // This will call ccm_passivate on the component executor.
    home->remove_component ((*iter).int_id_);
    ACE_CHECK;

    CORBA::release ((*iter).int_id_);
  }

  this->component_map_.unbind_all ();
  // To this point the servant should have been destroyed. However,
  // if someone is still making calls on the servant, terrible thing
  // will happen.
}

void
CIAO::NodeApplication_Impl::remove_component (const char * comp_ins_name
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  ACE_DEBUG ((LM_DEBUG, "remove_component\n"));

  Components::CCMObject_ptr comp;
  Components::CCMHome_ptr home;

  ACE_CString str (comp_ins_name);

  /* Before we do remove component we have to inform the homeservant so
   * Component::ccm_passivate ()
   * constainer::ninstall_component () ->deactivate_object () will be called.
   *
   * ccm_remove will be called when the poa destroys the servant.
   */

  if (this->component_map_.find (str, comp) != 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  if (this->home_map_.find (str, home) != 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  // This will call ccm_passivate on the component executor.
  home->remove_component (comp);
  ACE_CHECK;

  // If the previous calls failed, what should we do here??
  CORBA::release (comp);

  // @@ Still need to remove the home if the previous operation fails?
  if (this->component_map_.unbind (str) == -1)
    ACE_THROW (::Components::RemoveFailure ());
}

// The code below is obsolete now. However I want to keep it arround as a
// start point for configurations.
/*
void
CIAO::NodeApplication_Impl::
parse_config_values (const ::Deployment::Properties & properties,
  struct home_installation_info &component_install_info
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
    Deployment::UnknownImplId,
    Deployment::ImplEntryPointNotFound,
    Components::InvalidConfiguration))
{

  for (CORBA::ULong i = 0; i < properties.length(); ++i)
  {
    // Place holder for string values
    const char * str = 0;
    const char * name = properties[i].name.in();

    // I assume the property will be given in the following format! --Tao
    if (ACE_OS::strcmp (name, "CIAO-servant-location"))
    {
      properties[i].value >>= str;
      component_install_info.servant_dll_= str;  //deep copy happens here.
    }
    else if (ACE_OS::strcmp (name, "CIAO-servant-entryPoint"))
    {
      properties[i].value >>= str;
      component_install_info.servant_entrypt_= str;  //deep copy happens here.
    }
    else if (ACE_OS::strcmp (name, "CIAO-executor-location"))
    {
      properties[i].value >>= str;
      component_install_info.executor_dll_= str;  //deep copy happens here.
    }
    else if (ACE_OS::strcmp (name, "CIAO-executor-entryPoint"))
    {
      properties[i].value >>= str;
      component_install_info.executor_entrypt_= str;  //deep copy happens here.
    }
    else
    {
      ACE_DEBUG ((LM_DEBUG, "Found unrecognized property: %s\n",name));
      //I should put the name of the configuration inside of the exception. --Tao
      ACE_THROW (Components::InvalidConfiguration ());
    }
  }

  // We don't need to worry about the case when some info is missing since
  // the modeling tool will ensure the complete info to presented in the properties.
}
*/

void 
CIAO::NodeApplication_Impl::build_event_connection (const Deployment::Connection & connection,
                                                    CIAO::EventServiceType type
                                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((Deployment::InvalidConnection,
                   CORBA::SystemException))
{
	  ACE_DEBUG ((LM_DEBUG, "CIAO::NodeApplication_Impl::build_connection ()!!!\n"));

    ACE_DEBUG ((LM_DEBUG, "instanceName: %s\n", connection.instanceName.in ()));
    ACE_DEBUG ((LM_DEBUG, "portName: %s\n", connection.portName.in ()));

    ACE_DEBUG ((LM_DEBUG, "consumer Component Name: %s\n", connection.consumerCompName.in ()));
    ACE_DEBUG ((LM_DEBUG, "consumer Port Name: %s\n", connection.consumerPortName.in ()));

    ACE_DEBUG ((LM_DEBUG, "portkind: "));
    switch (connection.kind) {
      case Deployment::Facet: ACE_DEBUG ((LM_DEBUG, "Facet\n")); break;
      case Deployment::SimplexReceptacle: ACE_DEBUG ((LM_DEBUG, "SimplexReceptacle\n")); break;
      case Deployment::MultiplexReceptacle: ACE_DEBUG ((LM_DEBUG, "MultiplexReceptacle\n")); break;
      case Deployment::EventEmitter: ACE_DEBUG ((LM_DEBUG, "EventEmitter\n")); break;
      case Deployment::EventPublisher: ACE_DEBUG ((LM_DEBUG, "EventPublisher\n")); break;
      case Deployment::EventConsumer: ACE_DEBUG ((LM_DEBUG, "EventConsumer\n")); break;
    }

    // Get the consumer port object reference and put into "consumer"
    Components::EventConsumerBase_var consumer = 
      Components::EventConsumerBase::_narrow (connection.endpoint.in ()
                                              ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (CORBA::is_nil (consumer.in ()))
      {
        ACE_DEBUG ((LM_DEBUG, "Nil consumer port object reference\n"));
        ACE_THROW (Deployment::InvalidConnection ());
      }

    // Get the supplier component object reference.
    ACE_CString supplier_comp_name = connection.instanceName.in ();
    Components::CCMObject_ptr source_objref;

    ACE_DEBUG ((LM_DEBUG, "source component name is: %s\n", supplier_comp_name.c_str ()));
    if (this->component_map_.find (supplier_comp_name, source_objref) != 0)
      {
        ACE_DEBUG ((LM_DEBUG, "Nil source component object reference\n"));
        ACE_THROW (Deployment::InvalidConnection ());
      }

    // Get the consumer component object reference.
    ACE_CString consumer_comp_name = connection.consumerCompName.in ();
    Components::CCMObject_ptr sink_objref;

    ACE_DEBUG ((LM_DEBUG, "sink component name is: %s\n", consumer_comp_name.c_str ()));
    if (this->component_map_.find (consumer_comp_name, sink_objref) != 0)
      {
        ACE_DEBUG ((LM_DEBUG, "Nil sink component object reference\n"));
        ACE_THROW (Deployment::InvalidConnection ());
      }

    // Get the container event service
    CIAO::ContainerEventService_var event_service =
      this->get_event_service (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (event_service.in ()))
      {
        ACE_DEBUG ((LM_DEBUG, "Nil event_service\n"));
        ACE_THROW (Deployment::InvalidConnection ());
      }
    
    // supplier ID
    //ACE_DEBUG ((LM_DEBUG, "################ Making a remote CORBA call on event SOURCE component named: %s\n", supplier_comp_name));
    //ACE_DEBUG ((LM_DEBUG, "Nil source component object reference\n"));
    //ACE_CString sid = source_objref->component_UUID (ACE_ENV_SINGLE_ARG_DECL);

    ACE_CString test_sid = source_objref->component_UUID (ACE_ENV_SINGLE_ARG_DECL);
    ACE_DEBUG ((LM_DEBUG, "The COMPONENT UUID I GOT is: %s\n", test_sid.c_str ()));
    ACE_CString sid = supplier_comp_name;
    ACE_DEBUG ((LM_DEBUG, "The SUPPLIER COMPONENT NAME is: %s\n", supplier_comp_name.c_str ()));
    ACE_CHECK;

    sid += "_";
    sid += connection.portName.in ();
    sid += "_publisher";

    // consumer ID
    //ACE_DEBUG ((LM_DEBUG, "################ Making a remote CORBA call on event SINK component named: %s\n", consumer_comp_name));
    //ACE_CString cid = sink_objref->component_UUID (ACE_ENV_SINGLE_ARG_DECL);
    ACE_CString cid = consumer_comp_name;
    ACE_CHECK;

    cid += "_";
    cid += connection.consumerPortName.in ();
    cid += "_consumer";

    ACE_DEBUG ((LM_DEBUG, "Publisher: %s\n", sid.c_str ()));
    ACE_DEBUG ((LM_DEBUG, "Subscriber: %s\n", cid.c_str ()));

    if (this->connected_publishers_.find (sid) == -1)
      {
        CIAO::Supplier_Config_var supplier_config =
          event_service->create_supplier_config (type ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
    
        supplier_config->supplier_id (sid.c_str () ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        event_service->connect_event_supplier (supplier_config.in () ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        this->connected_publishers_.insert (sid);

        supplier_config->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;
      }

    CIAO::Consumer_Config_var consumer_config =
      event_service->create_consumer_config (type ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    consumer_config->supplier_id (sid.c_str () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    consumer_config->consumer_id (cid.c_str () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    consumer_config->consumer (consumer.in ()
                                ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    event_service->connect_event_consumer (consumer_config.in ()
                                           ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    consumer_config->destroy (ACE_ENV_SINGLE_ARG_DECL);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "CIAO::NodeApplication_Impl::build_connection () completed!!!!\n"));
}

bool
CIAO::NodeApplication_Impl::register_with_ns (const char * obj_name,
                                              CORBA::ORB_ptr orb,
                                              Components::CCMObject_ptr obj
                                              ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
	  // Obtain the naming service
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          false);

      CosNaming::NamingContextExt_var naming_context =
        CosNaming::NamingContextExt::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
  
      // Create a Naming Sequence
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (obj_name);
      name[0].kind = CORBA::string_dup ("");

      // Register with the Name Server
      naming_context->bind (name, obj ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      return true;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "NodeApplication: failed to register with naming service.");
      return false;
    }
  ACE_ENDTRY;
  return true;
}

bool
CIAO::NodeApplication_Impl::unregister_with_ns (const char * obj_name,
                                                CORBA::ORB_ptr orb
                                                ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
	  // Obtain the naming service
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          false);

      CosNaming::NamingContextExt_var naming_context =
        CosNaming::NamingContextExt::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
  
      // Create a Naming Sequence
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (obj_name);
      name[0].kind = CORBA::string_dup ("");

      // Register with the Name Server
      ACE_DEBUG ((LM_DEBUG, "Unregister component with the name server : %s!\n", obj_name));
      naming_context->unbind (name ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      return true;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "NodeApplication: failed to unregister with naming service.");
      return false;
    }
  ACE_ENDTRY;
  return true;
}
