// -*- C++ -*-

//=============================================================================
/**
 *  @file    FT_PG_Object_Group_Storable.h
 *
 *
 * Contains declaration for class FT_PG_Object_Group_Storable.
 *
 *  @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#ifndef FT_TAO_PG_OBJECT_GROUP_STORABLE_H_
#define FT_TAO_PG_OBJECT_GROUP_STORABLE_H_
#include /**/ "ace/pre.h"

#include "orbsvcs/Naming/FaultTolerant/ftnaming_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/////////////////////////////////
// Includes needed by this header
#include "orbsvcs/PortableGroup/PG_Object_Group_Storable.h"
#include "orbsvcs/FT_NamingReplicationC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FT_Naming_Replication_Manager;
////////////////
// Class declarations
namespace TAO
{
  /**
   * An object group whose state persists to a stream so that its state
   * can be saved/retrieved between processes that use the group.
   */
  class TAO_FtNaming_Export FT_PG_Object_Group_Storable
    : public PG_Object_Group_Storable
  {
  public:
    /**
     * This constructor is suitable for creating an object group from
     * scratch.
     */
    FT_PG_Object_Group_Storable (
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      CORBA::Object_ptr empty_group,
      const PortableGroup::TagGroupTaggedComponent & tagged_component,
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      const TAO::PG_Property_Set_var & type_properties,
      TAO::Storable_Factory & storable_factory,
      TAO_FT_Naming_Replication_Manager *replicator);

    /**
     * This constructor is suitable for creating an object group from
     * persistent store.
     */
    FT_PG_Object_Group_Storable (
      PortableGroup::ObjectGroupId group_id,
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      TAO::Storable_Factory & storable_factory,
      TAO_FT_Naming_Replication_Manager *replicator);

    /// Destructor
    virtual ~FT_PG_Object_Group_Storable () = default;

  public:
    virtual void stale (bool is_stale);
    virtual bool stale ();

  protected:
    virtual void state_written ();

    virtual bool is_obsolete (time_t stored_time);

    /// Provide support for modifying the object group type_id when first
    /// member is added to the group.
    virtual PortableGroup::ObjectGroup_ptr add_member_to_iogr(
      CORBA::Object_ptr member);

  private:
    FT_PG_Object_Group_Storable () = delete;
    FT_PG_Object_Group_Storable (const FT_PG_Object_Group_Storable &) = delete;
    FT_PG_Object_Group_Storable & operator = (const FT_PG_Object_Group_Storable &) = delete;

    /// Replication persistence support
    bool stale_;

    /// Track if the persistent file used for storage has been created yet
    /// so can know if we should propagate a change type of NEW.
    bool file_created_;

    TAO_FT_Naming_Replication_Manager *replicator_;
    /**
     * Although it is assumed for replication that services
     * using object groups share the persistent store, a
     * CORBA message is sent upon update from one replica to
     * another so the object group data can be marked as stale.
     * This is because using the file time stamp to detect
     * if the data has been updated only provides one second
     * resolution.
     */
    int propagate_update_notification (FT_Naming::ChangeType change_type);
  };
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // FT_TAO_PG_OBJECT_GROUP_STORABLE_H_
