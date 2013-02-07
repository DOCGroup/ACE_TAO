// -*- C++ -*-

//=============================================================================
/**
 * @file PG_Group_List_Store.h
 *
 * $Id$
 *
 * @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#ifndef PG_GROUP_LIST_STORE_H
#define PG_GROUP_LIST_STORE_H

#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroupC.h"

#include <set>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class Storable_Base;
  class Storable_Factory;
}

namespace TAO
{
  /**
   * @class PG_Group_List_Store
   * Persists a list of object groups that are themselves persisted.
   * All creation and deletion of persistent object groups should
   * result in the list store being updated.
   * This is done to support multiple group factories using the same
   * persistent object groups.
   * @see PG_Object_Group_Storable
   */
  class TAO_PortableGroup_Export PG_Group_List_Store
  {
  public:
    PG_Group_List_Store (Storable_Factory & storable_factory);

    ~PG_Group_List_Store ();

    /**
     * Return a group ID that can be used for creating a new object group.
     */
    PortableGroup::ObjectGroupId get_next_group_id ();

    int add (PortableGroup::ObjectGroupId id);

    int remove (PortableGroup::ObjectGroupId id);

    typedef std::set<PortableGroup::ObjectGroupId> Group_Ids;

    typedef std::set<PortableGroup::ObjectGroupId>::iterator
      Group_Id_Iterator;

    typedef std::set<PortableGroup::ObjectGroupId>::reverse_iterator
      Group_Id_Revers_Iterator;

    typedef std::set<PortableGroup::ObjectGroupId>::const_iterator
      Group_Id_Const_Iterator;

    Group_Ids & get_group_ids ();

    /**
     * Make explicit that state is stale instead of relying only
     * on persistent file time stamp.
     */
    void stale (bool is_stale);

    bool stale ();

    /**
     * Answer if the list is obsolete because the persistent store has been updated.
     * Used by PG_Group_Factory to avoid having to compare IDs in group_map_ with
     * the list contained here.
     */
    bool list_obsolete ();

  private:

    Group_Ids group_ids_;

    /// Ensure ID is monotonically increasing even when groups gets
    /// removed.
    PortableGroup::ObjectGroupId next_group_id_;

    TAO::Storable_Base * create_stream (const char * mode);

    Storable_Factory & storable_factory_;
    bool loaded_from_stream_;
    time_t last_changed_;
    bool stale_;
    void read (TAO::Storable_Base & stream);
    void write (TAO::Storable_Base & stream);

    bool is_obsolete (time_t stored_time);

    friend class PG_Group_List_Store_File_Guard;

  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* PG_GROUP_LIST_STORE_H */
