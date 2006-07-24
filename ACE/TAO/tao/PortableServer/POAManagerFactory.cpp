// $Id$

#include "tao/PortableServer/POAManagerFactory.h"
#include "tao/PortableServer/POAManager.h"
#include "tao/EndpointPolicy/EndpointPolicyTypeC.h"

#include "ace/OS_NS_string.h"

ACE_RCSID (PortableServer,
           POAManagerFactory,
           "$Id$")

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_POAManager_Factory::TAO_POAManager_Factory (TAO_Object_Adapter &object_adapter) :
  object_adapter_ (object_adapter)
{
}

TAO_POAManager_Factory::~TAO_POAManager_Factory (void)
{
  for (POAMANAGERSET::iterator iterator = this->poamanager_set_.begin ();
        iterator != this->poamanager_set_.end ();
        ++iterator)
    {
      ::PortableServer::POAManager_ptr poamanager = (*iterator);
      CORBA::release (poamanager);
    }
}

::PortableServer::POAManager_ptr
TAO_POAManager_Factory::create_POAManager (
  const char * id,
  const ::CORBA::PolicyList & policies
  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ::PortableServer::POAManagerFactory::ManagerAlreadyExists,
                     ::CORBA::PolicyError))
{
  if (policies.length () > 1
    || (policies.length () == 1 &&
        policies[0]->policy_type () != EndpointPolicy::ENDPOINT_POLICY_TYPE))
  {
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY),
                      ::PortableServer::POAManager::_nil ());
  }

  PortableServer::POAManager_var poamanager =
    PortableServer::POAManager::_nil ();
  if (id != 0)
  {
    poamanager = this->find (id ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (::PortableServer::POAManager::_nil ());

    // If we already have a manager with the same name throw an exception
    if (!CORBA::is_nil (poamanager.in()))
      {
        ACE_THROW_RETURN (
          ::PortableServer::POAManagerFactory::ManagerAlreadyExists (),
          ::PortableServer::POAManager::_nil ());
      }
  }

  // this indirection brought to you by borland's compiler and its refusal
  // to directly assign the newly created TAO_POA_Manager to a POAManager_var.
  {
    PortableServer::POAManager_ptr pm = 0;
    ACE_NEW_THROW_EX (pm,
                      TAO_POA_Manager (object_adapter_, id, policies, this),
                      CORBA::NO_MEMORY
                      (CORBA::SystemException::_tao_minor_code (0, ENOMEM),
                       CORBA::COMPLETED_NO));
    ACE_CHECK_RETURN (::PortableServer::POAManager::_nil ());
    poamanager = pm;
  }

  this->register_poamanager (poamanager.in ());

  return PortableServer::POAManager::_duplicate (poamanager.in ());
}

::PortableServer::POAManagerFactory::POAManagerSeq *
TAO_POAManager_Factory::list (
  ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::PortableServer::POAManagerFactory::POAManagerSeq_var poamanagers;
  CORBA::ULong number_of_poamanagers = static_cast <CORBA::ULong>
                                              (this->poamanager_set_.size ());
  ACE_NEW_THROW_EX (poamanagers,
                    PortableServer::POAManagerFactory::POAManagerSeq (
                      number_of_poamanagers),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  poamanagers->length (number_of_poamanagers);

  CORBA::ULong index = 0;
  for (POAMANAGERSET::iterator iterator = this->poamanager_set_.begin ();
        iterator != this->poamanager_set_.end ();
        ++iterator, ++index)
    {
      ::PortableServer::POAManager_ptr poamanager = (*iterator);
      poamanagers[index] =
        PortableServer::POAManager::_duplicate (poamanager);
    }

  return poamanagers._retn ();
}

::PortableServer::POAManager_ptr
TAO_POAManager_Factory::find (
  const char * id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::PortableServer::POAManager_ptr poamanager =
    ::PortableServer::POAManager::_nil();

  for (POAMANAGERSET::iterator iterator = this->poamanager_set_.begin ();
        iterator != this->poamanager_set_.end ();
        ++iterator)
    {
      CORBA::String_var poamanagerid =
        (*iterator)->get_id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (::PortableServer::POAManager::_nil());

      if (ACE_OS::strcmp (id, poamanagerid.in()) == 0)
        {
          poamanager = PortableServer::POAManager::_duplicate (*iterator);
          break;
        }
    }

  return poamanager;
}

int
TAO_POAManager_Factory::remove_poamanager (
  ::PortableServer::POAManager_ptr poamanager)
{
  int retval = 0;
  retval = this->poamanager_set_.remove (poamanager);

  if (retval == 0)
    {
      CORBA::release (poamanager);
    }

  return retval;
}

int
TAO_POAManager_Factory::register_poamanager (
  ::PortableServer::POAManager_ptr poamanager)
{
  return this->poamanager_set_.insert (
    PortableServer::POAManager::_duplicate (poamanager));
}


TAO_END_VERSIONED_NAMESPACE_DECL

#endif
