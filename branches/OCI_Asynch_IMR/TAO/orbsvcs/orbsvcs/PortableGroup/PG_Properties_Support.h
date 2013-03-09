// -*- C++ -*-

//=============================================================================
/**
 *  @file    PG_Properties_Support.h
 *
 *  $Id$
 *
 *  This file declares classes to help manage the PortableGroup::Properties
 *  It serves roughly the same purpose as PG_PropertiesManager, but takes a
 *  different approach that suits the needs of FT CORBA.
 *  It would be possible to replace PG_PropertiesManager, or implement it in
 *  terms of PG_Properties_Support at some time in the future.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#ifndef TAO_PG_PROPERTIES_SUPPORT_H
#define TAO_PG_PROPERTIES_SUPPORT_H

#include "orbsvcs/PortableGroup/PG_Property_Set.h"
#include "orbsvcs/PortableGroup/portablegroup_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   *  This Properties Support object manages Property Sets (TAO::PG_Property_Set).
   *
   *  One set, default_properties_,
   *  acts as a "global" default set of properties.
   *
   *  The collection, properties_map_, contains a set of properties for each
   *  PortableGroup::type_id.  The default set acts as a "parent" for each of
   *  these type_id sets.
   *
   *  Expected use: When an object group is created that implements the interface
   *  identified by type_id, the corresponding typed_id propery set acts as a
   *  parent to the Property set contained in the PG_Object_Group.
   *
   *  This heirarchy of property sets provides the correct property behavior.  A
   *  request for a propery to an ObjectGroup will be satisfied from:
   *    by the object group group property set, or
   *    the typed_id property set, or
   *    the default property set, or
   *    the request will fail..
   *
   *  Note that changes to type_id or default properties will be visible
   *  immediately at the ObjectGroup level.
   */

  class TAO_PortableGroup_Export PG_Properties_Support
  {
    typedef ACE_Hash_Map_Manager<
      ACE_CString,
      ::TAO::PG_Property_Set_var,
      TAO_SYNCH_MUTEX> Properties_Map;
    typedef ACE_Hash_Map_Iterator<
      ACE_CString,
      ::TAO::PG_Property_Set_var,
      TAO_SYNCH_MUTEX> Properties_Map_Iterator;

  public:
    PG_Properties_Support ();
    ~PG_Properties_Support ();

    /**
     * Set a single default property.
     * Overwriting any value previously set for that property.
     * Leaving all other properties untouched.
     * @param name the name of the property to set
     * @value an Any containing the value.
     */
    void set_default_property (const char * name,
          const PortableGroup::Value & value);

    /**
     * Update the default property set.
     *
     * Properties that appear in props are replaced in or added to the default
     * property set.  Properties that do not appear in props are unchanged.
     *
     * @param props the set of properties to update the defaults.
     */
    void set_default_properties (const PortableGroup::Properties & props);

    /**
     * Export the default properties in PortableGroup::Properties format.
     *
     * This produces the properties in a format suitable for use across
     * a CORBA interface.
     * The caller owns the resulting Properties and must release it to avoid
     * resource leaks.
     * @returns a newly allocated PortableGroup::Properties.
     */
    PortableGroup::Properties * get_default_properties (void);

    /**
     * Undefine default properties that appear in props.
     *
     * Properties that are defined in props are removed from the default
     * property set.  Removal is done by name.  The property values do not
     * have to match.  There is no penalty for attempting to remove a property
     * that does not exist.
     * @param props a set of propertys to be removed by name.
     */
    void remove_default_properties (
        const PortableGroup::Properties & props);

    /**
     * Override or define properties associated with a type_id.
     *
     * If a property set does not exist for type_id, a new one will be created.
     * Any property included in overrides will be set or replaced in the type_id
     * property set.  Any property not in overrides will be unchanged.
     *
     * Contrary to the "throws" specification, this method does not attempt
     * to validate the properties because doing so would unnecessarily constrain
     * the uses to which this class could be put (although one could strategize the
     * validation.)
     */
    void set_type_properties (
        const char *type_id,
        const PortableGroup::Properties & overrides);

    /**
     * Export the property set in a PortableGroup::Properties format.
     *
     * This produces the properties associated with a type_id -- including
     * any default properties that have not been overridden at the type_id level
     * in a format suitable for use across a CORBA interface.
     *
     * The caller owns the resulting Properties and must release it to avoid
     * resource leaks.
     *
     * Compare this method to find_typeid_properties which returns a pointer
     * to the internal representation of the properties in TAO::PG_Property_Set
     * format.  This is more efficient, but suitable only for internal use.
     *
     * @param type_id identifies the set of properties to be exported.
     * @returns a newly allocated PortableGroup::Properties that must be released by the caller.
     */
    PortableGroup::Properties * get_type_properties (
        const char *type_id);

    /**
     * Export the property set in a PortableGroup::Properties format.
     *
     * This method is intended to provide a parent
     * for the property set in a newly-created Object Group of the given
     * type_id.
     *
     * Callers who intend to send the property set across a CORBA interface
     * should use the get_type_properties method.
     *
     * @param type_id identifies the set of properties to be found.
     * @returns a pointer to a Property_Set owned by this Properties_Support object.
     */
    TAO::PG_Property_Set_var  find_typeid_properties (
        const char *type_id);

    /**
     * Undefine default properties that appear in props.
     *
     * Properties that are defined in props are removed from the type_id
     * property set.  Removal is done by name.  The property values do not
     * have to match.  There is no penalty for attempting to remove a property
     * that does not exist.
     * @param props a set of propertys to be removed by name from the type_id set.
     */
    void remove_type_properties (
        const char *type_id,
        const PortableGroup::Properties & props);

    ///////////////
    // Data Members
  private:
    /**
     * Protect internal state.
     */
    TAO_SYNCH_MUTEX internals_;

    /// The default property set.
    TAO::PG_Property_Set_var default_properties_;

    /// A collection of property sets indexed by type_id.
    Properties_Map properties_map_;
  };
} //namespace TAO_PG

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAO_PG_PROPERTIES_SUPPORT_H
