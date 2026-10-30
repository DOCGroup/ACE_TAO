// -*- C++ -*-
#include "tao/PortableServer/POAManagerFactory.h"
#include "tao/PortableServer/POAManager.h"

#include "ace/OS_NS_string.h"
#include "ace/CORBA_macros.h"

#include <algorithm>
#include <new>

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_POAManager_Factory::TAO_POAManager_Factory (TAO_Object_Adapter &object_adapter) :
  object_adapter_ (object_adapter)
{
}

TAO_POAManager_Factory::~TAO_POAManager_Factory ()
{
  this->remove_all_poamanagers ();
}

::PortableServer::POAManager_ptr
TAO_POAManager_Factory::create_POAManager (
  const char * id,
  const ::CORBA::PolicyList & policies
  )
{
  // Validate the policy.
  TAO_POA_Policy_Set tao_policies (TAO_POA_Policy_Set (this->object_adapter_.default_poa_policies ()));

  // Merge policies from the ORB level.
  this->object_adapter_.validator ().merge_policies (tao_policies.policies ());

  // Merge in any policies that the user may have specified.
  tao_policies.merge_policies (policies);

  // If any of the policy objects specified are not valid for the ORB
  // implementation, if conflicting policy objects are specified, or
  // if any of the specified policy objects require prior
  // administrative action that has not been performed, an
  // InvalidPolicy exception is raised containing the index in the
  // policies parameter value of the first offending policy object.
  tao_policies.validate_policies (this->object_adapter_.validator (),
                                  this->object_adapter_.orb_core ());

  PortableServer::POAManager_var poamanager;

  if (id != nullptr)
  {
    poamanager = this->find (id);

    // If we already have a manager with the same name throw an exception
    if (!CORBA::is_nil (poamanager.in()))
      {
        throw ::PortableServer::POAManagerFactory::ManagerAlreadyExists ();
      }
  }

  // this indirection brought to you by borland's compiler and its refusal
  // to directly assign the newly created TAO_POA_Manager to a POAManager_var.
  {
    PortableServer::POAManager_ptr pm = nullptr;
    ACE_NEW_THROW_EX (pm,
                      TAO_POA_Manager (object_adapter_, id, policies, this),
                      CORBA::NO_MEMORY
                      (CORBA::SystemException::_tao_minor_code (0, ENOMEM),
                       CORBA::COMPLETED_NO));
    poamanager = pm;
  }

  this->register_poamanager (poamanager.in ());

  return poamanager._retn ();
}

::PortableServer::POAManagerFactory::POAManagerSeq *
TAO_POAManager_Factory::list ()
{
  ::PortableServer::POAManagerFactory::POAManagerSeq_var poamanagers;
  CORBA::ULong number_of_poamanagers = static_cast <CORBA::ULong>
                                              (this->poamanager_set_.size ());
  ACE_NEW_THROW_EX (poamanagers,
                    PortableServer::POAManagerFactory::POAManagerSeq (
                      number_of_poamanagers),
                    CORBA::NO_MEMORY ());

  poamanagers->length (number_of_poamanagers);

  CORBA::ULong index = 0;
  for (::PortableServer::POAManager_ptr poamanager : this->poamanager_set_)
    {
      poamanagers[index] =
        PortableServer::POAManager::_duplicate (poamanager);
      ++index;
    }

  return poamanagers._retn ();
}

::PortableServer::POAManager_ptr
TAO_POAManager_Factory::find (const char * id)
{
  ::PortableServer::POAManager_ptr poamanager =
    ::PortableServer::POAManager::_nil();

  for (::PortableServer::POAManager_ptr manager : this->poamanager_set_)
    {
      CORBA::String_var poamanagerid = manager->get_id ();

      if (ACE_OS::strcmp (id, poamanagerid.in()) == 0)
        {
          poamanager = PortableServer::POAManager::_duplicate (manager);
          break;
        }
    }

  return poamanager;
}

void
TAO_POAManager_Factory::remove_all_poamanagers ()
{
  for (::PortableServer::POAManager_ptr poamanager : this->poamanager_set_)
    {
      CORBA::release (poamanager);
    }
  this->poamanager_set_.clear ();
}

int
TAO_POAManager_Factory::remove_poamanager (
  ::PortableServer::POAManager_ptr poamanager)
{
  POAMANAGERSET::iterator const position =
    std::find (this->poamanager_set_.begin (), this->poamanager_set_.end (), poamanager);

  if (position == this->poamanager_set_.end ())
    return -1;

  this->poamanager_set_.erase (position);
  CORBA::release (poamanager);
  return 0;
}

int
TAO_POAManager_Factory::register_poamanager (
  ::PortableServer::POAManager_ptr poamanager)
{
  if (std::find (this->poamanager_set_.begin (), this->poamanager_set_.end (), poamanager)
      != this->poamanager_set_.end ())
    return -1;

  ::PortableServer::POAManager_ptr duplicate =
    PortableServer::POAManager::_duplicate (poamanager);
  try
    {
      this->poamanager_set_.push_back (duplicate);
    }
  catch (std::bad_alloc const&)
    {
      CORBA::release (duplicate);
      return -1;
    }

  return 0;
}


TAO_END_VERSIONED_NAMESPACE_DECL

#endif
