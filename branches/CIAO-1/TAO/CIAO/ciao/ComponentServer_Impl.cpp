// $Id$

#include "ComponentServer_Impl.h"
#include "Container_Impl.h"

#if !defined (__ACE_INLINE__)
# include "ComponentServer_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::ComponentServer_Impl::~ComponentServer_Impl ()
{
  // @@ remove all Containers?
}

PortableServer::POA_ptr
CIAO::ComponentServer_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

int
CIAO::ComponentServer_Impl::init (::Components::ConfigValues &options
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Initialize ComponentServer and create the internal container
  // implementation that actually interacts with installed
  // homes/components.

  ACE_NEW_THROW_EX (this->config_,
                    ::Components::ConfigValues (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  *this->config_ = options;

  // We will probably need two ORBs in this process.  One for the
  // deployment framework, and one for the actual components.
  return 0;
}

::Components::ConfigValues *
CIAO::ComponentServer_Impl::configuration (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Components::ConfigValues *retval;

  ACE_NEW_THROW_EX (retval,
                    Components::ConfigValues (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  *retval = this->config_;

  return retval;
}

::Components::Deployment::ServerActivator_ptr
CIAO::ComponentServer_Impl::get_server_activator (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Components::Deployment::ServerActivator::_duplicate (this->activator_.in ());
}

::Components::Deployment::Container_ptr
CIAO::ComponentServer_Impl::create_container (const Components::ConfigValues & config
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure,
                   Components::InvalidConfiguration))
{
  CIAO::Container_Impl *container_servant = 0;

  ACE_NEW_THROW_EX (container_servant,
                    CIAO::Container_Impl (this->orb_.in (),
                                          this->poa_.in (),
                                          this->get_objref ()),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  PortableServer::ServantBase_var safe_servant (container_servant);
  container_servant->init (config,
                           this->get_component_installation ()
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

  Components::Deployment::Container_var ci
    = Components::Deployment::Container::_narrow (obj.in ()
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Cached the objref in its servant.
  container_servant->set_objref (ci.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

    this->container_set_.add (ci.in ());
  }

  return ci._retn ();
}

void
CIAO::ComponentServer_Impl::remove_container (Components::Deployment::Container_ptr cref
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->container_set_.object_in_set (cref) == 0)
    ACE_THROW (Components::RemoveFailure());

  cref->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // @@ Deactivate object.
  PortableServer::ObjectId_var oid
    = this->poa_->reference_to_id (cref
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_->deactivate_object (oid
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Should we remove the server still, even if the previous call failed.

  if (this->container_set_.remove (cref) == -1)
    ACE_THROW (::Components::RemoveFailure ());
}

Components::Deployment::Containers *
CIAO::ComponentServer_Impl::get_containers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

  Components::Deployment::Containers_var retval;

  ACE_NEW_THROW_EX (retval.out (),
                    Components::Deployment::Containers (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  CORBA::ULong len = this->container_set_.size ();
  retval->length (len);          // resize

#if 0
  // TAO is broken here.  Both <replace>, <get_buffer> and friends are missing.
  this->container_set_.copy (len, retval->get_buffer (0));
#else
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      retval[i] = this->container_set_.at (i);
    }
#endif

  return retval._retn ();
}

void
CIAO::ComponentServer_Impl::remove (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  // @@ Need to remove all containers/homes/components.

  this->orb_->shutdown (ACE_ENV_ARG_PARAMETER);
}
