// -*- C++ -*-

//=======================================================================
/**
 *  @file     LB_ObjectGroup_Map.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_LB_OBJECTGROUP_MAP_H
#define TAO_LB_OBJECTGROUP_MAP_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager_T.h"
#include "orbsvcs/LoadBalancingC.h"
#include "tao/PortableServer/Key_Adapters.h"
#include "tao/PortableServer/PortableServerC.h"

#include "LB_ReplicaInfo.h"


/**
 * @class TAO_LB_ObjectGroup_Map_Entry
 *
 * @brief Value field of the ObjectGroup map.
 */
struct TAO_LB_ObjectGroup_Map_Entry
{
public:

  /// The RepositoryId corresponding to all Replicas in the
  /// ObjectGroup.
  CORBA::String_var type_id;

  /// Reference to the ObjectGroup.
  PortableGroup::ObjectGroup_var object_group;

  /// Unbounded set containing replica references and all related
  /// information for each replica.
  TAO_LB_ReplicaInfo_Set replica_infos;

  /// Properties used when creating this object group, in addition
  /// those set dynamically after the creation.
  PortableGroup::Properties properties;

  /// Lock used to synchronize access to the ReplicaInfo set.
  TAO_SYNCH_MUTEX lock;
};

/**
 * @class TAO_LB_ObjectGroup_Map
 *
 * @brief Class that encapsulates hash map operations on an underlying
 *        object group map.
 *
 * This class contains all the operations necessary to manipulate the
 * underlying object group map.  The canonical hash map operations are
 * supported, in addition to some other operations needed for mapping
 * object group reference (rather than ObjectId) to object group map
 * entry.
 */
class TAO_LB_ObjectGroup_Map
{
public:

  /// ObjectId hash map typedef.
  typedef ACE_Hash_Map_Manager_Ex<
    PortableServer::ObjectId,
    TAO_LB_ObjectGroup_Map_Entry *,
    TAO_ObjectId_Hash,
    ACE_Equal_To<PortableServer::ObjectId>,
    TAO_SYNCH_MUTEX> Table;

  /// Forward STL-style iterator typedef.
  typedef Table::iterator iterator;

  /// Constructor
  TAO_LB_ObjectGroup_Map (void);

  /**
   * @name The Canonical Map Methods
   */
  //@{

  /// Bind the given ObjectId to the given object group map entry.
  int bind (const PortableServer::ObjectId &oid,
            TAO_LB_ObjectGroup_Map_Entry *entry);

  /// Return the object group entry corresponding to the given
  /// ObjectId.
  int find (const PortableServer::ObjectId &oid,
            TAO_LB_ObjectGroup_Map_Entry *&entry);

  /// Unbind the given ObjectId from the map.
  int unbind (const PortableServer::ObjectId &oid);

  /// Return forward iterator that points to the beginning of the
  /// underlying object group map.
  iterator begin (void);

  /// Return forward iterator that points to the end of the underlying
  /// object group map.
  iterator end (void);

  //@}

  /// Set the reference to the POA that created the object group
  /// references passed to the ObjectGroupManager methods.
  void poa (PortableServer::POA_ptr poa);

private:

  /// Obtain the ObjectGroup hash map entry corresponding to the given
  /// ObjectGroup reference.
  TAO_LB_ObjectGroup_Map_Entry *get_group_entry (
      PortableGroup::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound));

private:

  /// Reference to the POA that created the object group reference.
  PortableServer::POA_var poa_;

  /// Underlying table that maps ObjectId to object group related
  /// information.
  Table table_;

};


#if defined (__ACE_INLINE__)
#include "LB_ObjectGroup_Map.inl"
#endif  /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_LB_OBJECTGROUP_MAP_H */
