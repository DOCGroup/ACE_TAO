// -*- C++ -*-

//=============================================================================
/**
 *  @file   Portable_Group_Map.h
 *
 *  $Id$
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_PORTABLE_GROUP_MAP_H
#define TAO_PORTABLE_GROUP_MAP_H

#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroupC.h"

#include "tao/Object_KeyC.h"

#include "ace/Null_Mutex.h"
#include "ace/Hash_Map_Manager_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ServerRequest;

/**
 * @class TAO_GroupId_Hash
 *
 * @brief Hashing class for Group Ids.
 *
 * Define the hash() method for Group Ids.
 */
class TAO_PortableGroup_Export TAO_GroupId_Hash
{
public:

  /// Returns hash value.
  u_long operator () (const PortableGroup::TagGroupTaggedComponent *id) const;
};

/**
 * @class TAO_GroupId_Equal_To
 *
 * @brief Hashing class for Group Ids.
 *
 * Define the Equal_To () method for Object Ids.
 */
class TAO_PortableGroup_Export TAO_GroupId_Equal_To
{
public:

  /// Returns 1 if equal.
  int operator () (const PortableGroup::TagGroupTaggedComponent *lhs,
                   const PortableGroup::TagGroupTaggedComponent *rhs) const;
};

/**
 * @class TAO_Portable_Group_Map
 *
 * @brief Map of GroupIds to ObjectKeys.
 */
class TAO_PortableGroup_Export TAO_Portable_Group_Map
{
public:

  /**
   * @struct Map_Entry
   *
   * @brief Value field of the portable group map.
   */
  struct Map_Entry
  {
    /// The key.
    TAO::ObjectKey key;

    /// next ObjectKey in the group.
    struct Map_Entry *next;
  };

  /// Constructor.
  TAO_Portable_Group_Map ();

  /// Destructor.
  ~TAO_Portable_Group_Map (void);

  /// Add a GroupId->ObjectKey mapping to the map.
  /// This function takes ownership of the memory pointed to be group_id
  void add_groupid_objectkey_pair (PortableGroup::TagGroupTaggedComponent *group_id,
                                   const TAO::ObjectKey &key);


  /// Remove a GroupId->ObjectKey mapping from the map.
  void remove_groupid_objectkey_pair (const PortableGroup::TagGroupTaggedComponent* group_id,
                                      const TAO::ObjectKey &key);

  /// Dispatch a request to all of the ObjectIds that belong to
  /// the specified group.
  void dispatch (PortableGroup::TagGroupTaggedComponent* group_id,
                                    TAO_ORB_Core *orb_core,
                                    TAO_ServerRequest &request,
                                    CORBA::Object_out forward_to);

  /// Id hash map.
  typedef ACE_Hash_Map_Manager_Ex<
    PortableGroup::TagGroupTaggedComponent *,
    Map_Entry *,
    TAO_GroupId_Hash,
    TAO_GroupId_Equal_To,
    ACE_Null_Mutex> GroupId_Table;
  typedef GroupId_Table::iterator Iterator;

protected:
  /// Lock used to synchronize access to map_.
  TAO_SYNCH_RW_MUTEX lock_;

  /// Id map.
  GroupId_Table map_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

//#if defined (__ACE_INLINE__)
//# include "Portable_Group_Map.i"
//#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLE_GROUP_MAP_H */
