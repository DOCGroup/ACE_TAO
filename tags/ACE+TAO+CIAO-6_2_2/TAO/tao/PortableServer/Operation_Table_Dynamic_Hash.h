// -*- C++ -*-

//=============================================================================
/**
 *  @file    Operation_Table_Dynamic_Hash.h
 *
 *  $Id$
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef TAO_OPERATION_TABLE_DYNAMIC_HASH_H
#define TAO_OPERATION_TABLE_DYNAMIC_HASH_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Operation_Table.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Dynamic_Hash_OpTable
 *
 * @brief Dynamic Hashing scheme for CORBA IDL operation name lookup.
 */
class TAO_PortableServer_Export TAO_Dynamic_Hash_OpTable
  : public TAO_Operation_Table
{
public:
  // = Initialization and termination methods.
  /**
   * Initialize the dynamic hash operation table with a database of
   * operation names. The hash table size may be different from the
   * size of the database. Hence we use the third argument to specify
   * the size of the internal hash table.  The @a alloc argument is
   * used to determine where the memory comes from (usually from
   * ACE_Static_Allocator_Base).
   */
  TAO_Dynamic_Hash_OpTable (const TAO_operation_db_entry *db,
                            CORBA::ULong dbsize,
                            CORBA::ULong hashtblsize,
                            ACE_Allocator *alloc);

  /// Destructor
  ~TAO_Dynamic_Hash_OpTable (void);

  /// See the documentation in the base class for details.
  virtual int bind (const char *opname,
                    const TAO::Operation_Skeletons skel_ptr);

  virtual int find (const char *opname,
                    TAO_Skeleton &skelfunc,
                    const unsigned int length = 0);

  virtual int find (const char *opname,
                    TAO_Collocated_Skeleton &skelfunc,
                    TAO::Collocation_Strategy s,
                    const unsigned int length = 0);
private:
  typedef ACE_Hash_Map_Manager_Ex<const char *,
                                  TAO::Operation_Skeletons,
                                  ACE_Hash<const char *>,
                                  ACE_Equal_To<const char *>,
                                  ACE_Null_Mutex>
        OP_MAP_MANAGER;

  /// The hash table data structure.
  OP_MAP_MANAGER hash_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_OPERATION_TABLE_DYNAMIC_HASH_H */
