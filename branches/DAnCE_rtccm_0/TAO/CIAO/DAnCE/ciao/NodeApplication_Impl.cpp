//$Id$

#include "NodeApplication_Impl.h"

#if !defined (__ACE_INLINE__)
# include "NodeApplication_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::NodeApplication_Impl::~NodeApplication_Impl (void)
{
  //@@ Unbind the container map
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
              comp->connect(providedReference[i].portName.in (),
                            providedReference[i].endpoint.in ()
                            ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              break;

            case Deployment::EventEmitter:
              consumer = Components::EventConsumerBase::
                _narrow (providedReference[i].endpoint.in ()
                         ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              if (CORBA::is_nil (consumer.in ()))
                {
                  ACE_THROW (Deployment::InvalidConnection ());
                }

              comp->connect_consumer(providedReference[i].portName.in (),
                                     consumer.in ()
                                     ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              break;

            case Deployment::EventPublisher:
              consumer = Components::EventConsumerBase::
                _narrow (providedReference[i].endpoint.in ()
                         ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              if (CORBA::is_nil (consumer.in ()))
                ACE_THROW (Deployment::InvalidConnection ());

              comp->subscribe (providedReference[i].portName.in (),
                               consumer.in ()
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

CORBA::Long
CIAO::NodeApplication_Impl::init (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  //@@ What to do here?

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
