// -*- C++ -*-

//=============================================================================
/**
 *  @file    PG_Object_Group_Storable.h
 *
 *  $Id$
 *
 * Contains declaration for class PG_Object_Group_Storable.
 *
 *  @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#ifndef TAO_PG_OBJECT_GROUP_STORABLE_H_
#define TAO_PG_OBJECT_GROUP_STORABLE_H_
#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/////////////////////////////////
// Includes needed by this header
#include "orbsvcs/PortableGroup/PG_Object_Group.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/////////////////////
// Forward references

////////////////
// Class declarations
namespace TAO
{
  /**
   * An object group whose state persists to a stream so that its state
   * can be saved/retrieved between processes that use the group.
   */
  class TAO_PortableGroup_Export PG_Object_Group_Storable
    : public PG_Object_Group
  {

    /////////////////////
    // Construct/Destruct
  public:

    PG_Object_Group_Storable (
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      CORBA::Object_ptr empty_group,
      const PortableGroup::TagGroupTaggedComponent & tagged_component,
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      TAO::PG_Property_Set * type_properties);


    /// Destructor
    ~PG_Object_Group_Storable ();

    /////////////////
    // public methods

  public:

    virtual const PortableGroup::Location & get_primary_location () const;

    virtual void add_member (
        const PortableGroup::Location & the_location,
        CORBA::Object_ptr member);

    virtual int set_primary_member (
        TAO_IOP::TAO_IOR_Property * prop,
        const PortableGroup::Location & the_location);

    virtual void remove_member (
        const PortableGroup::Location & the_location);

    virtual void create_member (
        const PortableGroup::Location & the_location,
        const char * type_id,
        const PortableGroup::Criteria & the_criteria);

    virtual PortableGroup::Locations * locations_of_members (void);

    virtual CORBA::Object_ptr get_member_reference (
        const PortableGroup::Location & the_location);

    virtual void initial_populate (void);

    virtual void minimum_populate (void);

    virtual int has_member_at (const PortableGroup::Location & location );

    virtual void distribute (int value);

    virtual void set_name (const char* group_name);

    virtual const char* get_name (void);

  };
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // TAO_PG_OBJECT_GROUP_STORABLE_H_
