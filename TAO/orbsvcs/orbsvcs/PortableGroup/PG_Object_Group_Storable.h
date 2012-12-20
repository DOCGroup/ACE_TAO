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
namespace TAO
{
  class Storable_Factory;
  class Storable_Base;
}

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

    /**
     * This constructor is suitable for creating an object group from
     * scratch.
     */
    PG_Object_Group_Storable (
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      CORBA::Object_ptr empty_group,
      const PortableGroup::TagGroupTaggedComponent & tagged_component,
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      TAO::PG_Property_Set * type_properties,
      TAO::Storable_Factory & storable_factory);

    /**
     * This constructor is suitable for creating an object group from
     * persistent store.
     */
    PG_Object_Group_Storable (
      PortableGroup::ObjectGroupId group_id,
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      TAO::Storable_Factory & storable_factory);

    /// Destructor
    ~PG_Object_Group_Storable ();

    /////////////////
    // public methods

  public:

    /**
     * Indicate that this object group is to be permanently
     * destroyed. During destruction the persistent store
     * for this will then be removed.
     * This is to distinguish between deletion from a process
     * shutdown verses deletion from a destroy request.
     */
    void set_destroyed (bool destroyed);

    /**
     * Allow for replication persistence support by
     * derived classes.
     */
    virtual void stale (bool is_stale);
    virtual bool stale ();

    virtual const PortableGroup::Location & get_primary_location ();

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

    virtual PortableGroup::ObjectGroupId  get_object_group_id () const;

  private:

    /////////////////////////
    // Forbidden methods
    PG_Object_Group_Storable ();
    PG_Object_Group_Storable (const PG_Object_Group_Storable & rhs);
    PG_Object_Group_Storable & operator = (const PG_Object_Group_Storable & rhs);

    /// Indicate if instance of object group has been restored from store
    bool group_previously_stored_;
    PortableGroup::ObjectGroupId group_id_previously_stored_;

    TAO::Storable_Base * create_stream (const char * mode);

    void read (TAO::Storable_Base & stream);

    void write (TAO::Storable_Base & stream);

    TAO::Storable_Factory & storable_factory_;
    time_t last_changed_;

  protected:

    bool loaded_from_stream_;

    bool destroyed_;

    /// To set when state written out.
    bool write_occurred_;

    /**
     * Signals that this context was updated.
     */
    virtual void state_written (void);

    /**
     * A callback invoked by the object group file guard
     * to determine if this context is obsolete with respect to
     * the peristent store.
     */
    virtual bool is_obsolete (time_t stored_time);

    friend class Object_Group_File_Guard;

  };
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // TAO_PG_OBJECT_GROUP_STORABLE_H_
