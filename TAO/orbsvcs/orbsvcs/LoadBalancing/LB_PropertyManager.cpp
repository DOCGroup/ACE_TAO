// -*- C++ -*-

#include "LB_PropertyManager.h"

ACE_RCSID (LoadBalancing,
           LB_PropertyManager,
           "$Id$")

TAO_LB_PropertyManager::TAO_LB_PropertyManager (void)
  : default_properties_ (),
    type_properties_ (),
    creation_properties_ (),
    dynamic_properties_ ()
{
}

void
TAO_LB_PropertyManager::set_default_properties (
    const LoadBalancing::Properties & props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
  this->validate_properties (props, ACE_TRY_ENV);
  ACE_CHECK;

  this->default_properties_ = props;
}

LoadBalancing::Properties *
TAO_LB_PropertyManager::get_default_properties (
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  LoadBalancing::Properties *props = 0;
  ACE_NEW_THROW_EX (props,
                    LoadBalancing::Properties (this->default_properties_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  return props;
}

void
TAO_LB_PropertyManager::remove_default_properties (
    const LoadBalancing::Properties &props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
  this->validate_properties (props, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_LB_PropertyManager::set_type_properties (
    const char * /* type_id */,
    const LoadBalancing::Properties & overrides,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
  this->validate_properties (overrides, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

LoadBalancing::Properties *
TAO_LB_PropertyManager::get_type_properties (
    const char * /* type_id */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
TAO_LB_PropertyManager::remove_type_properties (
    const char * /* type_id */,
    const LoadBalancing::Properties & props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
  this->validate_properties (props, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_LB_PropertyManager::set_properties_dynamically (
    LoadBalancing::ObjectGroup_ptr object_group,
    const LoadBalancing::Properties & overrides,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
  this->validate_properties (overrides, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

LoadBalancing::Properties *
TAO_LB_PropertyManager::get_properties (
    LoadBalancing::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
TAO_LB_PropertyManager::validate_properties (
    const LoadBalancing::Properties & props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
  // @todo FIXME!  Just a hack to get things going...

  CORBA::ULong len = props.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      if (ACE_OS::strcmp (props[i].nam.id[0],
                          "Factories") == 0)
        return;
      else if (ACE_OS::strcmp (props[i].nam.id[0],
                          "InitialNumberReplicas") == 0)
        return;
      else if (ACE_OS::strcmp (props[i].nam.id[0],
                          "MinimumNumberReplicas") == 0)
        return;
      else
        ACE_THROW (LoadBalancing::UnsupportedProperty (props[i].nam));
    }
}

// -----------------------------------------------------------

CORBA::Boolean
operator== (const LoadBalancing::Property &lhs,
            const LoadBalancing::Property &rhs)
{
  // @todo Interim implementation until we figure out what to do when a
  //       property has multiple components.
  CORBA::ULong lhs_len = lhs.length ();
  CORBA::ULong rhs_len = rhs.length ();

  if (lhs_len != rhs_len)
    return 0;

  for (CORBA::ULong i = 0; i < lhs_len; ++i)
    if (ACE_OS::strcmp (lhs[i].id, rhs[i].id) != 0
        || ACE_OS::strcmp (lhs[i].kind, rhs[i].kind) != 0)
      return 0;

  return 1;
}

// -----------------------------------------------------------

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

// Type-specific property hash map template instantiations
template class ACE_Hash_Map_Entry<const char *, LoadBalancing::Properties>;
template class ACE_Hash_Map_Manager_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, TAO_SYNCH_MUTEX>;

// ObjectGroup-specific property hash map template instantiations
template class ACE_Hash_Map_Entry<PortableServer::ObjectId, LoadBalancing::Properties>;
template class ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<PortableServer::ObjectId, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<PortableServer::ObjectId, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<PortableServer::ObjectId, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>;

template class ACE_Equal_To<PortableServer::ObjectId>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

// Type-specific property hash map template instantiations
#pragma instantiate ACE_Hash_Map_Entry<const char *, LoadBalancing::Properties>
#pragma instantiate ACE_Hash_Map_Manager_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, TAO_SYNCH_MUTEX>

// ObjectGroup-specific property hash map template instantiations
#pragma instantiate ACE_Hash_Map_Entry<PortableServer::ObjectId, LoadBalancing::Properties>
#pragma instantiate ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<PortableServer::ObjectId, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<PortableServer::ObjectId, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<PortableServer::ObjectId, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>

#pragma instantiate  ACE_Equal_To<PortableServer::ObjectId>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
