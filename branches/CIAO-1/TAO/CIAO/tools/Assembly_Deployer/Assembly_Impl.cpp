// $Id$

#include "Assembly_Impl.h"
//#include "ACtive_Objref_Map.h"
#include "Cookies.h"

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
CIAO::AssemblyFactory_Impl::init (ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Initialize AssemblyFactory and create the internal container
  // implementation that actually interacts with installed
  // homes/components.

  // We will probably need two ORBs in this process.  One for the
  // deployment framework, and one for the actual components.

  return 0;
}

::Components::Cookie *
CIAO::AssemblyFactory_Impl::create_assembly (const char * assembly_loc
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::InvalidLocation,
                   Components::CreateFailure))
{
  ACE_UNUSED_ARG (assembly_loc);

  CIAO::Assembly_Impl *servant = 0;
  ACE_NEW_RETURN (servant,
                  CIAO::Assembly_Impl (this->orb_.in (),
                                       this->poa_.in ()),
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
  // @@ remove all Containers?
  ACE_DEBUG ((LM_DEBUG,
              "CIAO::Assembly_Impl::~Assembly_Impl %d\n",
              this->serial_number_));
}

PortableServer::POA_ptr
CIAO::Assembly_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

int
CIAO::Assembly_Impl::init (ACE_ENV_ARG_DECL)
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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "CIAO::Assembly_Impl::build %d\n",
              this->serial_number_));

  // @@ Well, we need to actually buid something here.

  this->state_ = ::Components::Deployment::INSERVICE;
}

void
CIAO::Assembly_Impl::tear_down (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)  || \
    defined (ACE_HAS_GNU_REPO)
template class ACE_Active_Map_Manager<::Components::Deployment::Assembly_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Active_Map_Manager<::Components::Deployment::Assembly_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
