// $Id$

#include "Assembly_Impl.h"
//#include "ACtive_Objref_Map.h"
#include "Cookies.h"
#include "../XML_Helpers/Assembly_Spec.h"
#include "../XML_Helpers/XML_Utils.h"

#if !defined (__ACE_INLINE__)
# include "Assembly_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::AssemblyFactory_Impl::~AssemblyFactory_Impl ()
{
  // @@ remove all Containers?
}

PortableServer::POA_ptr
CIAO::AssemblyFactory_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

int
CIAO::AssemblyFactory_Impl::init (const char *init_file
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->deployment_config_.init (init_file);
}

::Components::Cookie *
CIAO::AssemblyFactory_Impl::create_assembly (const char * assembly_loc
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::InvalidLocation,
                   Components::CreateFailure))
{
  CIAO::Assembly_Spec *assembly_spec;
  ACE_NEW_RETURN (assembly_spec,
                  CIAO::Assembly_Spec,
                  0);

  if (CIAO::XML_Utils::parse_componentassembly (assembly_loc,
                                                assembly_spec) != 0)
    ACE_THROW_RETURN (Components::Deployment::InvalidLocation (), 0);

  CIAO::Assembly_Impl *servant = 0;
  ACE_NEW_RETURN (servant,
                  CIAO::Assembly_Impl (this->orb_.in (),
                                       this->poa_.in (),
                                       assembly_spec,
                                       this->deployment_config_),
                  0);

  PortableServer::ServantBase_var save_servant (servant);

  // @@ TO-DO: acquire and parse a Component Assembly descriptor from
  // the assembly_loc and then the Assembly servant will use it to
  // initialize itself here.

  // Register with our POA.
  PortableServer::ObjectId_var oid
    = this->poa_->activate_object (servant
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Object_var objref
    = this->poa_->id_to_reference (oid
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ::Components::Deployment::Assembly_var assembly =
      ::Components::Deployment::Assembly::_narrow (objref.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_Active_Map_Manager_Key key;
  this->assembly_map_.bind (assembly,
                            key);

  ::Components::Cookie_var retv = new CIAO::Map_Key_Cookie (key);

  return retv._retn ();
}

::Components::Deployment::Assembly_ptr
CIAO::AssemblyFactory_Impl::lookup (Components::Cookie * c
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::InvalidAssembly))
{
  ::Components::Deployment::Assembly_var retv;
  if (c == 0)
    ACE_THROW_RETURN (::Components::Deployment::InvalidAssembly (), 0);

  ACE_Active_Map_Manager_Key key;
  if (CIAO::Map_Key_Cookie::extract (c, key) == -1)
    ACE_THROW_RETURN (::Components::Deployment::InvalidAssembly (), 0);

  if (this->assembly_map_.find (key,
                                retv) != 0)
    ACE_THROW_RETURN (::Components::Deployment::InvalidAssembly (), 0);

  return retv._retn ();
}

void
CIAO::AssemblyFactory_Impl::destroy (Components::Cookie * c
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::InvalidAssembly,
                   Components::RemoveFailure))
{
  ::Components::Deployment::Assembly_var assembly;
  if (c == 0)
    ACE_THROW (::Components::Deployment::InvalidAssembly ());

  ACE_Active_Map_Manager_Key key;
  if (CIAO::Map_Key_Cookie::extract (c, key) == -1)
    ACE_THROW (::Components::Deployment::InvalidAssembly ());

  if (this->assembly_map_.unbind (key,
                                  assembly) != 0)
    ACE_THROW (::Components::Deployment::InvalidAssembly ());

  ::Components::Deployment::AssemblyState
      state = assembly->get_state (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (state == ::Components::Deployment::INSERVICE)
    {
      assembly->tear_down (ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  PortableServer::ObjectId_var oid
    = this->poa_->reference_to_id (assembly.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_->deactivate_object (oid
                                 ACE_ENV_ARG_PARAMETER);
}


CORBA::ULong CIAO::Assembly_Impl::assembly_count_ = 0;

CIAO::Assembly_Impl::~Assembly_Impl ()
{
  // @@ tearing down everything?

  ACE_DEBUG ((LM_DEBUG,
              "CIAO::Assembly_Impl::~Assembly_Impl %d\n",
              this->serial_number_));

  delete this->assembly_spec_;
}

PortableServer::POA_ptr
CIAO::Assembly_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

int
CIAO::Assembly_Impl::init (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Initialize Assembly and create the internal container
  // implementation that actually interacts with installed
  // homes/components.

  // We will probably need two ORBs in this process.  One for the
  // deployment framework, and one for the actual components.
  return 0;
}

void
CIAO::Assembly_Impl::build (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure))
{
  ACE_DEBUG ((LM_DEBUG,
              "CIAO::Assembly_Impl::build %d\n",
              this->serial_number_));
  if (this->state_ == Components::Deployment::INSERVICE)
    return;                     // We are running already.

  ACE_DEBUG ((LM_DEBUG, "------------------------------------\n"));

  // Installing homes and instantiating components

  CIAO::Assembly_Builder_Visitor builder (this->orb_.in (),
                                          this->assembly_context_,
                                          this->assembly_spec_->componentfiles_,
                                          this->deployment_config_);
  int build_result = this->assembly_spec_->partitioning_.accept (builder
                                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "------------------------------------\n"));

  if (build_result != 0)
    {
      ACE_ERROR ((LM_ERROR, "Failed to install all homes or instantiate all components\n"));
      ACE_THROW (Components::CreateFailure ());
    }
  // Setting connections

  CIAO::Assembly_Spec::CONNECTION_QUEUE::ITERATOR
    conn_iter (this->assembly_spec_->connections_);

  while (!conn_iter.done ())
    {
      CIAO::Assembly_Connection::Connect_Info *connection;
      conn_iter.next (connection);

      this->make_connection (connection
                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      conn_iter.advance ();
    }

  this->state_ = ::Components::Deployment::INSERVICE;
}

void
CIAO::Assembly_Impl::tear_down (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  if (this->state_ != Components::Deployment::INSERVICE)
    return;                     // Nothing to do here.

  // @@ At least we should remove home and kill the component server.
  // Remove all components:
  {
    CIAO::Assembly_Context::COMP_MAP::ITERATOR
      iter (this->assembly_context_.instantiated_components_);

    while (!iter.done ())
      {
        CIAO::Assembly_Context::COMP_MAP::ENTRY *entry;
        iter.next (entry);

        entry->int_id_->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;

        iter.advance ();
      }
  }

  // Remove all server
  {
    CIAO::Assembly_Context::SERVER_QUEUE::ITERATOR
      iter (this->assembly_context_.component_servers_);

    while (!iter.done ())
      {
        Components::Deployment::ComponentServer_var *entry;
        iter.next (entry);

        (*entry)->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;

        iter.advance ();
      }
  }

  ACE_DEBUG ((LM_DEBUG,
              "CIAO::Assembly_Impl::tear_down %d\n",
              this->serial_number_));
  this->state_ = ::Components::Deployment::INACTIVE;
}

::Components::Deployment::AssemblyState
CIAO::Assembly_Impl::get_state (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "CIAO::Assembly_Impl::get_state %d\n",
              this->serial_number_));
  return this->state_;
}

void
CIAO::Assembly_Impl::make_connection (CIAO::Assembly_Connection::Connect_Info *info
                                      ACE_ENV_ARG_DECL)
{
  switch (info->type_)
    {
    case CIAO::Assembly_Connection::INTERFACE:
      {
        CORBA::Object_var source
          = this->resolve_interface (info->interface_
                                     ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        Components::CCMObject_var comp
          = this->resolve_component (info->component_
                                     ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        comp->connect (info->name_.c_str (),
                       source.in ()
                       ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        // @@ Register the connection?  How?
      }
      break;

    case CIAO::Assembly_Connection::EMITTER_CONSUMER:
      {
        Components::EventConsumerBase_var source
          = this->resolve_consumer (info->interface_
                                    ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        Components::CCMObject_var comp
          = this->resolve_component (info->component_
                                     ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        comp->connect_consumer (info->name_.c_str (),
                                source.in ()
                                ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }
      break;

    case CIAO::Assembly_Connection::PUBLISHER_CONSUMER:
      {
        Components::EventConsumerBase_var source
          = this->resolve_consumer (info->interface_
                                    ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        Components::CCMObject_var comp
          = this->resolve_component (info->component_
                                     ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        comp->subscribe (info->name_.c_str (),
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
CIAO::Assembly_Impl::resolve_interface (CIAO::Assembly_Connection::IF_Resolver_Info *info
                                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
  switch (info->resolver_type ())
    {
    case CIAO::Assembly_Connection::PROVIDER:
      {
        Components::CCMObject_var comp =
          this->resolve_component (info->nested_resolver ()
                                   ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        return comp->provide_facet (info->resolver_info ()
                                    ACE_ENV_ARG_PARAMETER);
      }

    case CIAO::Assembly_Connection::CONSUMER:
      return this->resolve_consumer (info
                                     ACE_ENV_ARG_PARAMETER);

    case CIAO::Assembly_Connection::COMP_IDREF:
      return this->resolve_component (info
                                      ACE_ENV_ARG_PARAMETER);

    case CIAO::Assembly_Connection::HOME_IDREF:
      return this->resolve_home (info
                                 ACE_ENV_ARG_PARAMETER);

    case CIAO::Assembly_Connection::NAMINGSERVICE:
      ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);

    case CIAO::Assembly_Connection::STRINGIFIEDOBJECTREF:
      return this->orb_->string_to_object (info->resolver_info ()
                                           ACE_ENV_ARG_PARAMETER);

    case CIAO::Assembly_Connection::HOMEFINDER:
      return this->resolve_home (info
                                 ACE_ENV_ARG_PARAMETER);

    case CIAO::Assembly_Connection::TRADERQUERY:
      ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);

    default:
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }
}

Components::CCMObject_ptr
CIAO::Assembly_Impl::resolve_component (CIAO::Assembly_Connection::IF_Resolver_Info *info
                                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
  if (info->resolver_type () == CIAO::Assembly_Connection::COMP_IDREF)
    {
      CIAO::Assembly_Context::COMP_MAP::ENTRY *entry;

      if (this->assembly_context_.instantiated_components_.find (info->resolver_info (),
                                                                 entry) != 0)
        // This isn't really a good exception to throw unless we can
        // specify FailureReason somehow.
        ACE_THROW_RETURN (Components::CreateFailure (), 0);

      return Components::CCMObject::_duplicate (entry->int_id_.in ());
    }

  CORBA::Object_var obj = this->resolve_interface (info
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  Components::CCMObject_var comp
    = Components::CCMObject::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return comp._retn ();
}

Components::CCMHome_ptr
CIAO::Assembly_Impl::resolve_home (CIAO::Assembly_Connection::IF_Resolver_Info *info
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
  switch (info->resolver_type ())
    {
    case CIAO::Assembly_Connection::HOME_IDREF:
      {
        CIAO::Assembly_Context::HOME_MAP::ENTRY *entry;

        if (this->assembly_context_.installed_homes_.find (info->resolver_info (),
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
        CORBA::Object_var obj = this->resolve_interface (info
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
CIAO::Assembly_Impl::resolve_consumer (CIAO::Assembly_Connection::IF_Resolver_Info *info
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
  if (info->resolver_type () == CIAO::Assembly_Connection::CONSUMER)
    {
      Components::CCMObject_var comp =
        this->resolve_component (info->nested_resolver ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return comp->get_consumer (info->resolver_info ()
                                 ACE_ENV_ARG_PARAMETER);
    }

  CORBA::Object_var obj = this->resolve_interface (info
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  Components::EventConsumerBase_var consumer
    = Components::EventConsumerBase::_narrow (obj.in ()
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return consumer._retn ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)  || \
    defined (ACE_HAS_GNU_REPO)
template class ACE_Active_Map_Manager<::Components::Deployment::Assembly_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Active_Map_Manager<::Components::Deployment::Assembly_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
