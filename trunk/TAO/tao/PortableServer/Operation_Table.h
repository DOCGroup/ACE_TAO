// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Operation_Table.h
 *
 *  $Id$
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef TAO_OPTABLE_H
#define TAO_OPTABLE_H
#include "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Abstract_Servant_Base.h"
#include "tao/TAO_Singleton.h"
#include "ace/Synch.h"
#include "ace/Hash_Map_Manager.h"

/**
 * @class TAO_operation_db_entry
 *
 * @brief Define a table entry that holds an operation name and its
 * corresponding skeleton.  A table of such entries is used to
 * initialize the different lookup strategies.
 */
class TAO_operation_db_entry
{
public:
  /// Operation name
  const char* opname_;

  /// Skeleton pointer
  TAO_Skeleton skel_ptr_;
};

/**
 * @class TAO_Operation_Table
 *
 * @brief Abstract class for maintaining and lookup of CORBA IDL
 * operation names.
 */
class TAO_PortableServer_Export TAO_Operation_Table
{
public:
  /**
   * Uses <{opname}> to look up the skeleton function and pass it back
   * in <{skelfunc}>.  Returns non-negative integer on success, or -1
   * on failure.
   */
  virtual int find (const char *opname,
                    TAO_Skeleton &skelfunc,
                    const unsigned int length = 0) = 0;

  /// Associate the skeleton <{skel_ptr}> with an operation named
  /// <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.
  virtual int bind (const char *opname,
                    const TAO_Skeleton skel_ptr) = 0;

  virtual ~TAO_Operation_Table (void);
};

/**
 * @class TAO_Operation_Table_Factory
 *
 * @brief Factory for producing operation table lookup objects based on
 * the enumerated value of strategy held by the parameters.
 */
class TAO_PortableServer_Export TAO_Operation_Table_Factory
{
public:
  /// Return an instance of the specified lookup strategy
  TAO_Operation_Table *opname_lookup_strategy (void);

  /// Constructor
  TAO_Operation_Table_Factory (void);

  /// Destructor
  ~TAO_Operation_Table_Factory (void);
};

/**
 * @class TAO_Operation_Table_Parameters
 *
 * @brief Parameters used to create the operation table.
 */
class TAO_PortableServer_Export TAO_Operation_Table_Parameters
{
public:
  /// various lookup strategies
  enum DEMUX_STRATEGY
  {
    TAO_LINEAR_SEARCH,
    TAO_DYNAMIC_HASH,
    TAO_PERFECT_HASH,
    TAO_BINARY_SEARCH,
    TAO_ACTIVE_DEMUX,
    TAO_USER_DEFINED
  };

  /// Set the lookup strategy from the list of enumerated values
  void lookup_strategy (DEMUX_STRATEGY s);

  /// Return the enumerated value for the lookup strategy. Default is
  /// Dynamic Hashing.
  DEMUX_STRATEGY lookup_strategy (void) const;

  /// Provide a data structure that will do the lookup. This is useful
  /// for user-defined lookup strategies.
  void concrete_strategy (TAO_Operation_Table *ot);

  /// Return the operation table that is being used to do the lookup.
  TAO_Operation_Table *concrete_strategy (void);

  /// Constructor.
  TAO_Operation_Table_Parameters (void);

  /// Destructor
  ~TAO_Operation_Table_Parameters (void);
private:
  /// Pointer to the object that implements a lookup strategy
  TAO_Operation_Table *strategy_;

  /// The enumerated value indicating the lookup strategy
  DEMUX_STRATEGY type_;
};

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
   * the size of the internal hash table.  The <alloc> argument is
   * used to determine where the memory comes from (usually from
   * <ACE_Static_Allocator_Base>).
   */
  TAO_Dynamic_Hash_OpTable (const TAO_operation_db_entry *db,
                            CORBA::ULong dbsize,
                            CORBA::ULong hashtblsize,
                            ACE_Allocator *alloc);

  /// Destructor
  ~TAO_Dynamic_Hash_OpTable (void);

  /// Associate the skeleton <{skel_ptr}> with an operation named
  /// <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.
  virtual int bind (const char *opname,
                    const TAO_Skeleton skel_ptr);

  /**
   * Uses <{opname}> to look up the skeleton function and pass it back
   * in <{skelfunc}>.  Returns non-negative integer on success, or -1
   * on failure.
   */
  virtual int find (const char *opname,
                    TAO_Skeleton &skelfunc,
                    const unsigned int length = 0);

private:
  typedef ACE_Hash_Map_Manager_Ex<const char *,
                                  TAO_Skeleton,
                                  ACE_Hash<const char *>,
                                  ACE_Equal_To<const char *>,
                                  ACE_Null_Mutex>
        OP_MAP_MANAGER;

  /// The hash table data structure.
  OP_MAP_MANAGER hash_;
};

/**
 * @class TAO_Linear_Search_OpTable
 *
 * @brief Operation table lookup strategy based on
 * linear search.  Not efficient, but it works.
 */
class TAO_PortableServer_Export TAO_Linear_Search_OpTable
  : public TAO_Operation_Table
{
public:
  // = Initialization and termination methods.
  /// Default constructor.
  TAO_Linear_Search_OpTable (void);

  /// Destructor.
  ~TAO_Linear_Search_OpTable (void);

  /**
   * Uses <{opname}> to look up the skeleton function and pass it back
   * in <{skelfunc}>.  Returns non-negative integer on success, or -1
   * on failure.
   */
  virtual int find (const char *opname,
                    TAO_Skeleton &skel_ptr,
                    const unsigned int length = 0);

  /// Associate the skeleton <{skel_ptr}> with an operation named
  /// <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.
  virtual int bind (const char *opname,
                    const TAO_Skeleton skelptr);

private:
  // = Method that should defined by the subclasses. GPERF program
  //   will generate this routine routines.
  virtual const TAO_operation_db_entry* lookup (const char *str) = 0;
};

/**
 * @class TAO_Active_Demux_OpTable_Entry
 *
 * @brief Active Demux lookup table entry.
 */
class TAO_PortableServer_Export TAO_Active_Demux_OpTable_Entry
{
public:
  // = Initialization and termination methods.
  /// Constructor
  TAO_Active_Demux_OpTable_Entry (void);

  /// Destructor
  ~TAO_Active_Demux_OpTable_Entry (void);

  /// Skeleton pointer corresponding to the index.
  TAO_Skeleton skel_ptr_;
};

/**
 * @class TAO_Active_Demux_OpTable
 *
 * @brief Implements the active demultiplexed lookup strategy. The key is
 * assumed to provide an index directly into the internal table.
 */
class TAO_PortableServer_Export TAO_Active_Demux_OpTable
  : public TAO_Operation_Table
{
public:
  // = Initialization and termination methods.
  /// Initializes the internal table with the database of operations
  TAO_Active_Demux_OpTable (const TAO_operation_db_entry *db,
                            CORBA::ULong dbsize);

  /// Destructor.
  ~TAO_Active_Demux_OpTable (void);

  /**
   * Uses <{opname}> to look up the skeleton function and pass it back
   * in <{skelfunc}>.  Returns non-negative integer on success, or -1
   * on failure.
   */
  virtual int find (const char *opname,
                    TAO_Skeleton &skel_ptr,
                    const unsigned int length = 0);

  /// Associate the skeleton <{skel_ptr}> with an operation named
  /// <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.
  virtual int bind (const char *opname,
                    const TAO_Skeleton skelptr);

private:
  /// The next available free slot
  CORBA::ULong next_;

  /// Size of the internal table
  CORBA::ULong tablesize_;

  /// Internal lookup table
  TAO_Active_Demux_OpTable_Entry *tbl_;
};

/**
 * @class TAO_Perfect_Hash_OpTable
 *
 * @brief Helper class  for use of perfect hashing operation lookup
 * strategy.
 *
 * This class declares pure virtual methods called 'lookup ()'
 * and 'hash ()' which will be generated by the GPERF
 * program. These methods are used by 'bind ()' and 'find ()'
 * methods. Subclasses will define the lookup and hash
 * functions.
 */
class TAO_PortableServer_Export TAO_Perfect_Hash_OpTable
  : public TAO_Operation_Table
{
public:
  /// Do nothing constructor.
  TAO_Perfect_Hash_OpTable (void);

  /// Do nothing destrctor.
  virtual ~TAO_Perfect_Hash_OpTable (void);

  /**
   * Uses <{opname}> to look up the skeleton function and pass it back
   * in <{skelfunc}>.  Returns non-negative integer on success, or -1
   * on failure.
   */
  virtual int find (const char *opname,
                    TAO_Skeleton &skelfunc,
                    const unsigned int length = 0);

  /// Associate the skeleton <{skel_ptr}> with an operation named
  /// <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.
  virtual int bind (const char *opname,
                    const TAO_Skeleton skel_ptr);

private:
  // = Methods that should defined by the subclasses. GPERF program
  //   will generate these routines.

  virtual unsigned int hash (const char *str, unsigned int len) = 0;

  virtual const TAO_operation_db_entry* lookup (const char *str,
                                                unsigned int len) = 0;
};

/**
 * @class TAO_Binary_Search_OpTable
 *
 * @brief Helper class for using binary search operatin lookup strategy
 * in the server skeletons.
 *
 * This class declares pure virtual method called 'lookup ()'
 * which will be generated by the GPERF program. This method is
 * used by 'bind ()' and 'find ()' methods. Subclasses will
 * define the lookup method.
 */
class TAO_PortableServer_Export TAO_Binary_Search_OpTable
  : public TAO_Operation_Table
{
public:
  /// Do nothing constructor.
  TAO_Binary_Search_OpTable (void);

  /// Do nothing destrctor.
  virtual ~TAO_Binary_Search_OpTable (void);

  /**
   * Uses <{opname}> to look up the skeleton function and pass it back
   * in <{skelfunc}>.  Returns non-negative integer on success, or -1
   * on failure.
   */
  virtual int find (const char *opname,
                    TAO_Skeleton &skelfunc,
                    const unsigned int length = 0);

  /// Associate the skeleton <{skel_ptr}> with an operation named
  /// <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.
  virtual int bind (const char *opname,
                    const TAO_Skeleton skel_ptr);

private:
  // = Method that should defined by the subclasses. GPERF program
  //   will generate this routine routines.
  virtual const TAO_operation_db_entry* lookup (const char *str) = 0;
};


// Define a singleton instance of operation table parameters.
typedef TAO_Singleton<TAO_Operation_Table_Parameters,
                      TAO_SYNCH_RECURSIVE_MUTEX>
        TAO_OP_TABLE_PARAMETERS;


// Define a singleton instance of the operation table factory.
typedef TAO_Singleton<TAO_Operation_Table_Factory,
                      TAO_SYNCH_RECURSIVE_MUTEX>
        TAO_OP_TABLE_FACTORY;

#include "ace/post.h"
#endif /* TAO_OPTABLE_H */
