// -*- C++ -*-

//=============================================================================
/**
 * @file PG_PropertyManager.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_PG_PROPERTY_MANAGER_H
#define TAO_PG_PROPERTY_MANAGER_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PG_Default_Property_Validator.h"

#include "orbsvcs/PortableGroupS.h"

#include "ace/Functor.h"
#include "ace/Hash_Map_Manager_T.h"



/// Forward declarations.
class TAO_PG_ObjectGroupManager;


/**
 * @class TAO_PG_PropertyManager
 *
 * @brief Class that implements the PortableGroup::PropertyManager
 *        interface.
 *
 * Only the default and type-specific properties are housed in this
 * class.  The properties used at creation time of an object group and
 * those set dynamically after object group creation are stored in the
 * TAO_PG_ObjectGroup_Map_Entry structure.  However, the
 * PropertyManager is still used to manage those properties.
 */
class TAO_PortableGroup_Export TAO_PG_PropertyManager
  : public virtual POA_PortableGroup::PropertyManager
{
public:

  /// Constructor.
  TAO_PG_PropertyManager (TAO_PG_ObjectGroupManager & object_group_manager);

  /**
   * @name PortableGroup::PropertyManager methods
   *
   * Methods required by the PortableGroup::PropertyManager
   * interface.
   */
  //@{

  /// Set the default properties to be used by all object groups.
  virtual void set_default_properties (
      const PortableGroup::Properties & props
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::InvalidProperty,
                     PortableGroup::UnsupportedProperty));

  /// Get the default properties used by all object groups.
  virtual PortableGroup::Properties * get_default_properties (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Remove default properties.
  virtual void remove_default_properties (
      const PortableGroup::Properties & props
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::InvalidProperty,
                     PortableGroup::UnsupportedProperty));

  /// Set properties associated with a given Member type.  These
  /// properties override the default properties.
  virtual void set_type_properties (
      const char * type_id,
      const PortableGroup::Properties & overrides
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::InvalidProperty,
                     PortableGroup::UnsupportedProperty));

  /**
   * Return the properties associated with a give Replica type.  These
   * properties include the type-specific properties in use, in
   * addition to the default properties that were not overridden.
   */
  virtual PortableGroup::Properties * get_type_properties (
      const char * type_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Remove the given properties associated with the Replica type ID.
  virtual void remove_type_properties (
      const char * type_id,
      const PortableGroup::Properties & props
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::InvalidProperty,
                     PortableGroup::UnsupportedProperty));

  /**
   * Dynamically set the properties associated with a given object
   * group as the load balancer and replicas are being executed.
   * These properties override the type-specific and default
   * properties.
   */
  virtual void set_properties_dynamically (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Properties & overrides
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound,
                     PortableGroup::InvalidProperty,
                     PortableGroup::UnsupportedProperty));

  /**
   * Return the properties currently in use by the given object
   * group.  These properties include those that were set dynamically,
   * type-specific properties that weren't overridden, properties that
   * were used when the replica was created, and default properties
   * that weren't overridden.
   */
  virtual PortableGroup::Properties * get_properties (
      PortableGroup::ObjectGroup_ptr object_group
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound));

  //@}

  /// Type-specific property hash map.
  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    PortableGroup::Properties,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> Type_Prop_Table;

private:

  /// Remove properties "to_be_removed" from the given list of
  /// properties.
  void remove_properties (const PortableGroup::Properties & to_be_removed,
                          PortableGroup::Properties &properties
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::InvalidProperty,
                     PortableGroup::UnsupportedProperty));

private:

  /// Table that maps ObjectId to Object Group related information.
  TAO_PG_ObjectGroupManager & object_group_manager_;

  /// Default properties.
  PortableGroup::Properties default_properties_;

  /// Table of type-specific object group properties.
  Type_Prop_Table type_properties_;

  /// Lock used to synchronize access to the default properties and
  /// the type-specific properties.
  TAO_SYNCH_MUTEX lock_;

  /// The property validator.
  /**
   * @todo Strategize the validator, or use template policies.
   */
  TAO_PG_Default_Property_Validator property_validator_;

};

#include /**/ "ace/post.h"

#endif /* TAO_PG_PROPERTY_MANAGER_H */
