// -*- C++ -*-

#include "LB_PropertyManager.h"

ACE_RCSID (LoadBalancing,
           LB_PropertyManager,
           "$Id$")

TAO_LB_PropertyManager::TAO_LB_PropertyManager (
  TAO_LB_ObjectGroup_Map &object_group_map)
  : object_group_map_ (object_group_map),
    default_properties_ (),
    type_properties_ (),
    lock_ ()
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

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

    this->default_properties_ = props;
  }
}

LoadBalancing::Properties *
TAO_LB_PropertyManager::get_default_properties (
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);

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

  
}

LoadBalancing::Properties *
TAO_LB_PropertyManager::get_properties (
    LoadBalancing::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound))
{
  TAO_LB_ObjectGroup_Map *entry =
    this->object_group_map_.get_group_entry (object_group, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  //  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, group_map_guard, entry->lock, 0);

  //  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, property_map_guard, this->lock_, 0);

  // Merge the type-properties into the list.
  LoadBalancing::Properties &type_properties;
  if (this->type_properties_.find (entry.in (), type_properties == 0))
    {
    }

  // Merge the default properties into the property list.

  return properties._retn ();
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

LoadBalancing::InitialNumberReplicas
TAO_LB_PropertyManager::get_initial_number_replicas (
  const char * /* type_id */,
  const LoadBalancing::Criteria &the_criteria,
  CORBA::Environment &ACE_TRY_ENV)
{
  // @@ Hack to get things going.

  // First, check if the given criteria has a desired number of
  // initial replicas.
  CORBA::ULong len = the_criteria.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      if (ACE_OS::strcmp (the_criteria[i].nam.id[0],
                          "InitialNumberReplicas") == 0)
        {
          LoadBalancing::InitialNumberReplicas initial_number_replicas = 0;

          if (the_criteria[i].val >>= initial_number_replicas)
            return initial_number_replicas;
          else
            ACE_THROW_RETURN (LoadBalancing::Property (
                                the_criteria[i].nam,
                                the_criteria[i].val),
                              0);
        }
    }

  // Second, check if a type-specific number of initial replicas was
  // set.

  // Third, check if a default number of initial replicas was set.

  return 0;  // @@ FIXME
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
template class ACE_Hash_Map_Manager_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

// Type-specific property hash map template instantiations
#pragma instantiate ACE_Hash_Map_Entry<const char *, LoadBalancing::Properties>
#pragma instantiate ACE_Hash_Map_Manager_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<const char *, LoadBalancing::Properties, TAO_ObjectId_Hash, ACE_Equal_To<const char *>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
