// $Id$

#include "Static_Configurator.h"
#include "ace/OS_NS_stdio.h"

int CIAO::Static_Configurator::configure(
                   CORBA::ORB_ptr orb,
                   Components::Deployment::ComponentServer_ptr component_server,
                   CIAO::Static_Config::ContainerAttributes* containers,
                   int containers_count,
                   CIAO::Static_Config::HomeAttributes* homes,
                   int homes_count,
                   CIAO::Static_Config::ComponentAttributes* components,
                   int components_count,
                   CIAO::Assembly_Placement::componentinstantiation::Register_Info*
                   component_instantiations,
                   int component_instantiations_count,
                   CIAO::Static_Config::ConnectionAttributes* connections,
                   int connections_count,
                   CIAO::Static_Config::ResolveInfoAttributes* resolvers,
                   int resolvers_count)
{
  orb_ = orb;
  component_server_ = component_server;
  containers_ = containers;
  containers_count_ = containers_count;
  homes_ = homes;
  homes_count_ = homes_count;
  components_ = components;
  components_count_ = components_count;
  component_instantiations_ = component_instantiations;
  component_instantiations_count_ = component_instantiations_count;
  connections_ = connections;
  connections_count_ = connections_count;
  resolvers_ = resolvers;
  resolvers_count_ = resolvers_count;
  this->create_homes ();
  this->create_components ();
  this->create_connections ();
  return 0;
}

int CIAO::Static_Configurator::create_homes (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "Creating Homes...\n"));
  for (int i=0; i<homes_count_; ++i)
    {
      // install home
      Components::ConfigValues home_config;
      // Setting home config value here:
      home_config.length (2);

      Components::ConfigValue *item = new OBV_Components::ConfigValue ();
      item->name ((const char *) "CIAO-servant-UUID");
      item->value () <<= homes_[i].servant_UUID_.c_str ();
      home_config[0] = item;

      item = new OBV_Components::ConfigValue ();
      item->name ((const char *) "CIAO-servant-entrypt");
      item->value () <<= homes_[i].servant_entrypt_.c_str ();
      home_config[1] = item;

      // @@ How do I get a customized container here, if we named a
      // RTpolicy_Set for this home placement?

      // @@ We actually have to take care of both home_config and
      // container_config here.  Should we use a Container Manager
      // here to make sure that we always use the same container for
      // different homes with similar policies?  Naw, we'll just
      // require putting similar policied home together for now.  They
      // have abandoned this implementation already anyway.

      Components::Deployment::Container_var container
        = this->get_container (containers_[homes_[i].containers_table_index_].rtpolicyset_ref_
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (CORBA::is_nil (container.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Unable to acquire a reference to ServerActivator\n"),
                          -1);

      Components::CCMHome_var home =
        container->install_home (homes_[i].executor_UUID_.c_str (),
                                 homes_[i].executor_entrypt_.c_str (),
                                 home_config
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      Components::KeylessCCMHome_var klhome =
        Components::KeylessCCMHome::_narrow (home.in ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (CORBA::is_nil (klhome.in ()))
        return -1;

      this->installed_homes_.bind (homes_[i].id_,
                                                     home);

      homes_[i].home_ = klhome;
    }
  ACE_DEBUG ((LM_DEBUG, "Homes installed...\n"));
  return 0;
}

int CIAO::Static_Configurator::create_connections (ACE_ENV_SINGLE_ARG_DECL)
{
  for (int i=0; i<connections_count_; ++i)
    {
      make_connection (i ACE_ENV_ARG_PARAMETER);
    }
  return 0;
}


void
CIAO::Static_Configurator::make_connection (int connections_table_index
                                      ACE_ENV_ARG_DECL)
{
  CIAO::Static_Config::ConnectionAttributes& info =
    connections_[connections_table_index];

  switch (info.type_)
    {
    case CIAO::Assembly_Connection::INTERFACE:
      {
        CORBA::Object_var source
          = this->resolve_interface (info.interface_index_
                                     ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        Components::CCMObject_var comp
          = this->resolve_component (info.component_index_
                                     ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        comp->connect (info.name_.c_str (),
                       source.in ()
                       ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        // @@ Register the connection?  How?
      }
      break;

    case CIAO::Assembly_Connection::EMITTER_CONSUMER:
      {
        Components::EventConsumerBase_var source
          = this->resolve_consumer (info.interface_index_
                                    ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        Components::CCMObject_var comp
          = this->resolve_component (info.component_index_
                                     ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        comp->connect_consumer (info.name_.c_str (),
                                source.in ()
                                ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }
      break;

    case CIAO::Assembly_Connection::PUBLISHER_CONSUMER:
      {
        Components::EventConsumerBase_var source
          = this->resolve_consumer (info.interface_index_
                                    ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        Components::CCMObject_var comp
          = this->resolve_component (info.component_index_
                                     ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        comp->subscribe (info.name_.c_str (),
                         source.in ()
                         ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }
      break;

    case CIAO::Assembly_Connection::HOME:
      ACE_THROW (CORBA::NO_IMPLEMENT ());

    default:
      ACE_THROW (CORBA::INTERNAL ());
    }
}

CORBA::Object_ptr
CIAO::Static_Configurator::resolve_interface (int resolvers_table_index
                                        ACE_ENV_ARG_DECL)
{
  if (resolvers_table_index == -1)
    return CORBA::Object::_nil ();

  CIAO::Static_Config::ResolveInfoAttributes& info =
    resolvers_[resolvers_table_index];

  switch (info.resolver_type_)
    {
    case CIAO::Assembly_Connection::PROVIDER:
      {
        Components::CCMObject_var comp =
          this->resolve_component (info.nested_resolver_index_
                                   ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        return comp->provide_facet (info.resolver_info_.c_str ()
                                    ACE_ENV_ARG_PARAMETER);
      }

    case CIAO::Assembly_Connection::CONSUMER:
      return this->resolve_consumer (resolvers_table_index
                                     ACE_ENV_ARG_PARAMETER);

    case CIAO::Assembly_Connection::COMP_IDREF:
      return this->resolve_component (resolvers_table_index
                                      ACE_ENV_ARG_PARAMETER);

    case CIAO::Assembly_Connection::HOME_IDREF:
      return this->resolve_home (resolvers_table_index
                                 ACE_ENV_ARG_PARAMETER);

    case CIAO::Assembly_Connection::NAMINGSERVICE:
      ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Object::_nil ());

    case CIAO::Assembly_Connection::STRINGIFIEDOBJECTREF:
      return this->orb_->string_to_object (info.resolver_info_.c_str ()
                                           ACE_ENV_ARG_PARAMETER);

    case CIAO::Assembly_Connection::HOMEFINDER:
      return this->resolve_home (resolvers_table_index
                                 ACE_ENV_ARG_PARAMETER);

    case CIAO::Assembly_Connection::TRADERQUERY:
      ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Object::_nil ());

    default:
      ACE_THROW_RETURN (CORBA::INTERNAL (), CORBA::Object::_nil ());
    }
  return CORBA::Object::_nil ();
}

Components::CCMObject_ptr
CIAO::Static_Configurator::resolve_component (int resolvers_table_index
                                        ACE_ENV_ARG_DECL)
{
  if (resolvers_table_index == -1)
    return 0;

  CIAO::Static_Config::ResolveInfoAttributes& info =
    resolvers_[resolvers_table_index];

  if (info.resolver_type_ == CIAO::Assembly_Connection::COMP_IDREF)
    {
      COMP_MAP::ENTRY *entry;

      if (this->instantiated_components_.find (info.resolver_info_,
                                               entry) != 0)
        // This isn't really a good exception to throw unless we can
        // specify FailureReason somehow.
        ACE_THROW_RETURN (Components::CreateFailure (), 0);

      return Components::CCMObject::_duplicate (entry->int_id_.in ());
    }

  CORBA::Object_var obj = this->resolve_interface (resolvers_table_index
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  Components::CCMObject_var comp
    = Components::CCMObject::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return comp._retn ();
}

Components::CCMHome_ptr
CIAO::Static_Configurator::resolve_home (int resolvers_table_index
                                   ACE_ENV_ARG_DECL)
{
  if (resolvers_table_index == -1)
    return 0;

  CIAO::Static_Config::ResolveInfoAttributes& info =
    resolvers_[resolvers_table_index];

  switch (info.resolver_type_)
    {
    case CIAO::Assembly_Connection::HOME_IDREF:
      {
        HOME_MAP::ENTRY *entry;

        if (this->installed_homes_.find (info.resolver_info_,
                                         entry) != 0)
          // This isn't really a good exception to throw unless we can
          // specify FailureReason somehow.
          ACE_THROW_RETURN (Components::CreateFailure (), 0);

        return Components::CCMHome::_duplicate (entry->int_id_.in ());
      }

    case CIAO::Assembly_Connection::HOMEFINDER:
      ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);

    default:
      {
        CORBA::Object_var obj = this->resolve_interface (resolvers_table_index
                                                   ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        Components::CCMHome_var home
          = Components::CCMHome::_narrow (obj.in ()
                                          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        return home._retn ();
      }
    }
  ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
}

Components::EventConsumerBase_ptr
CIAO::Static_Configurator::resolve_consumer (int resolvers_table_index
                                       ACE_ENV_ARG_DECL)
{
  if (resolvers_table_index == -1)
    return 0;

  CIAO::Static_Config::ResolveInfoAttributes& info =
    resolvers_[resolvers_table_index];

  if (info.resolver_type_ == CIAO::Assembly_Connection::CONSUMER)
    {
      Components::CCMObject_var comp =
        this->resolve_component (info.nested_resolver_index_
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return comp->get_consumer (info.resolver_info_.c_str ()
                                 ACE_ENV_ARG_PARAMETER);
    }

  CORBA::Object_var obj = this->resolve_interface (resolvers_table_index
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  Components::EventConsumerBase_var consumer
    = Components::EventConsumerBase::_narrow (obj.in ()
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return consumer._retn ();
}

Components::Deployment::Container_ptr
CIAO::Static_Configurator::get_container (const ACE_CString& rtpolicy
                                          ACE_ENV_ARG_DECL)
{
  // If we are not using the same rtpolicy set, or the there's no
  // cached container, then create a new one.

  int i;
  for (i=0; i<containers_count_; ++i)
    {
      if (containers_[i].rtpolicyset_ref_ == rtpolicy)
        {
          if (!CORBA::is_nil (containers_[i].container_.in ()))
            return Components::Deployment::Container::_duplicate
              (containers_[i].container_.in ());
          else break;
        }
    }

  int container_index=0;

  if (i<containers_count_) container_index = i;

  Components::ConfigValues container_config;

  if (rtpolicy.length () > 0)
    {
      CORBA::ULong len = container_config.length ();
      container_config.length (len + 1);

      Components::ConfigValue *newconfig
        = new OBV_Components::ConfigValue;

      newconfig->name ((const char *) "CIAO-RTPolicySet");
      newconfig->value () <<= rtpolicy.c_str ();
      container_config[len] = newconfig;
    }

  containers_[container_index].container_ =
    this->component_server_->create_container (container_config
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (rtpolicy.length () > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Creating container with RTPolicySet %s\n",
                rtpolicy.c_str ()));
  else
    ACE_DEBUG ((LM_DEBUG,
                "Creating container with empty policy set\n"));

  return Components::Deployment::Container::_duplicate
    (this->containers_[container_index].container_.in ());
}

int CIAO::Static_Configurator::create_components (ACE_ENV_SINGLE_ARG_DECL)
{
  for (int i=0; i<components_count_; ++i)
    {
      // @@ instantiation and register component.
      ACE_DEBUG ((LM_DEBUG, "ComponentInstantiation %s\n",
                  components_[i].id_.c_str ()));

      Components::CCMObject_var comp
        = homes_[components_[i].homes_table_index_].home_->create_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      this->instantiated_components_.bind (components_[i].id_,
                                                             comp);

      int begin_index = components_[i].component_registration_begin_index_;
      int end_index = components_[i].component_registration_end_index_;

      // Registering component.
      if ( begin_index>= 0 && end_index >= 0)
        {
          for (int j=begin_index; j<=end_index; ++j)
            {
              CIAO::Assembly_Placement::componentinstantiation::Register_Info
                info = component_instantiations_[j];
              this->register_component (info,
                                        comp.in ()
                                        ACE_ENV_ARG_PARAMETER);

            }
        }
    }
  return 0;
}

void
CIAO::Static_Configurator::register_component (
                 CIAO::Assembly_Placement::componentinstantiation::Register_Info& i,
                 Components::CCMObject_ptr c ACE_ENV_ARG_DECL)
{
  CORBA::Object_ptr reg_obj;

  // Extract the right interface to register:
  switch (i.type_)
    {
    case CIAO::Assembly_Placement::componentinstantiation::COMPONENT:
      reg_obj = c;
      break;

    case CIAO::Assembly_Placement::componentinstantiation::PROVIDESID:
      reg_obj = c->provide_facet (i.port_id_.c_str ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      break;

    case CIAO::Assembly_Placement::componentinstantiation::CONSUMESID:
      reg_obj = c->get_consumer (i.port_id_.c_str ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      break;

    default:
      ACE_THROW (CORBA::INTERNAL ());
    }

  // Now actually register the interface
  switch (i.method_)
    {
    case CIAO::Assembly_Placement::componentinstantiation::NAMINGSERVICE:
      ACE_THROW (CORBA::NO_IMPLEMENT ());

    case CIAO::Assembly_Placement::componentinstantiation::IORFILE:
      {
        CORBA::String_var ior
          = this->orb_->object_to_string (reg_obj
                                          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        FILE* ior_output_file =
          ACE_OS::fopen (i.name_.c_str (), "w");

        if (ior_output_file)
          {
            ACE_OS::fprintf (ior_output_file,
                             "%s",
                             ior.in ());
            ACE_DEBUG ((LM_DEBUG, "Done writing ior to file %s\n", i.name_.c_str ()));
            ACE_OS::fclose (ior_output_file);
          }
        else
        {
            ACE_DEBUG ((LM_DEBUG, "Error writing ior to file %s\n", i.name_.c_str ()));
		}
      }
    break;

    default:
      ACE_THROW (CORBA::INTERNAL ());
    }
}
