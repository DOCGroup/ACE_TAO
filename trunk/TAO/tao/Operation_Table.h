// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Operation_Table.h
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef TAO_OPTABLE_H
#define TAO_OPTABLE_H

#include "ace/Hash_Map_Manager.h"
#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "tao/corbafwd.h"

class TAO_operation_db_entry
{
  // = TITLE
  //   Define a table entry that holds an operation name and its
  //   corresponding skeleton.  A table of such entries is used to
  //   initialize the different lookup strategies.
public:
  CORBA::String opname_;
  // operation name

  TAO_Skeleton skel_ptr_;
  // skeleton pointer
};

class TAO_Export TAO_Operation_Table
{
  // = TITLE
  //     Abstract class for maintaining and lookup of CORBA IDL
  //     operation names.
public:
  virtual int find (const char *opname,
                   TAO_Skeleton &skelfunc) = 0;
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

  virtual int bind (const char *opname,
                    const TAO_Skeleton skel_ptr) = 0;
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

  virtual ~TAO_Operation_Table (void);
};

class TAO_Export TAO_Operation_Table_Factory
{
  // = TITLE
  //   Factory for producing operation table lookup objects based on
  //   the enumerated value of strategy held by the parameters.
public:
  TAO_Operation_Table *opname_lookup_strategy (void);
  // return an instance of the specified lookup strategy

  TAO_Operation_Table_Factory (void);
  // constructor

  ~TAO_Operation_Table_Factory (void);
  // destructor
};

class TAO_Export TAO_Operation_Table_Parameters
{
  // = TITLE
  //    Parameters used to create the operation table.
public:
  enum DEMUX_STRATEGY
  {
    // various lookup strategies
    TAO_LINEAR_SEARCH,
    TAO_DYNAMIC_HASH,
    TAO_PERFECT_HASH,
    TAO_BINARY_SEARCH,
    TAO_ACTIVE_DEMUX,
    TAO_USER_DEFINED
  };

  void lookup_strategy (DEMUX_STRATEGY s);
  // set the lookup strategy from the list of enumerated values

  DEMUX_STRATEGY lookup_strategy (void) const;
  // Return the enumerated value for the lookup strategy. Default is
  // Dynamic Hashing.

  void concrete_strategy (TAO_Operation_Table *ot);
  // Provide a data structure that will do the lookup. This is useful
  // for user-defined lookup strategies.

  TAO_Operation_Table *concrete_strategy (void);
  // Return the operation table that is being used to do the lookup.

  TAO_Operation_Table_Parameters (void);
  // constructor.

  ~TAO_Operation_Table_Parameters (void);
  // destructor
private:
  TAO_Operation_Table *strategy_;
  // pointer to the object that implements a lookup strategy

  DEMUX_STRATEGY type_;
  // the enumerated value indicating the lookup strategy
};

class TAO_Export TAO_Dynamic_Hash_OpTable : public TAO_Operation_Table
{
  // = TITLE
  //     Dynamic Hashing scheme for CORBA IDL operation name lookup.
public:
  // = Initialization and termination methods.
  TAO_Dynamic_Hash_OpTable (const TAO_operation_db_entry *db,
                            CORBA::ULong dbsize,
                            CORBA::ULong hashtblsize,
                            ACE_Allocator *alloc);
  // Initialize the dynamic hash operation table with a database of
  // operation names. The hash table size may be different from the
  // size of the database. Hence we use the third argument to specify
  // the size of the internal hash table.  The <alloc> argument is
  // used to determine where the memory comes from (usually from
  // <ACE_Static_Allocator_Base>).

  ~TAO_Dynamic_Hash_OpTable (void);
  // destructor

  virtual int bind (const char *opname,
                    const TAO_Skeleton skel_ptr);
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

  virtual int find (const char *opname,
                    TAO_Skeleton &skelfunc);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

private:
  typedef ACE_Hash_Map_Manager_Ex<const char *,
                                  TAO_Skeleton,
                                  ACE_Hash<const char *>,
                                  ACE_Equal_To<const char *>,
                                  ACE_Null_Mutex>
        OP_MAP_MANAGER;

  OP_MAP_MANAGER hash_;
  // The hash table data structure.
};

class TAO_Export TAO_Linear_Search_OpTable : public TAO_Operation_Table
{
  // = TITLE
  //    Operation table lookup strategy based on
  //    linear search.  Not efficient, but it works.
public:
  // = Initialization and termination methods.
  TAO_Linear_Search_OpTable (void);
  // Default constructor.

  ~TAO_Linear_Search_OpTable (void);
  // Destructor.

  virtual int find (const char *opname,
                    TAO_Skeleton &skel_ptr);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

  virtual int bind (const char *opname,
                    const TAO_Skeleton skelptr);
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

private:
  // = Method that should defined by the subclasses. GPERF program
  //   will generate this routine routines.
  virtual const TAO_operation_db_entry* lookup (const char *str) = 0;
};

class TAO_Export TAO_Active_Demux_OpTable_Entry
{
  // = TITLE
  //   Active Demux lookup table entry.
public:
  // = Initialization and termination methods.
  TAO_Active_Demux_OpTable_Entry (void);
  // constructor

  ~TAO_Active_Demux_OpTable_Entry (void);
  // destructor

  TAO_Skeleton skel_ptr_;
  // Skeleton pointer corresponding to the index.
};

class TAO_Export TAO_Active_Demux_OpTable : public TAO_Operation_Table
{
  // = TITLE
  //   Implements the active demultiplexed lookup strategy. The key is
  //   assumed to provide an index directly into the internal table.
public:
  // = Initialization and termination methods.
  TAO_Active_Demux_OpTable (const TAO_operation_db_entry *db, CORBA::ULong dbsize);
  // Initializes the internal table with the database of operations

  ~TAO_Active_Demux_OpTable (void);
  // destructor.

  virtual int find (const char *opname,
                    TAO_Skeleton &skel_ptr);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

  virtual int bind (const char *opname,
                    const TAO_Skeleton skelptr);
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

private:
  CORBA::ULong next_;
  // the next available free slot

  CORBA::ULong tablesize_;
  // size of the internal table

  TAO_Active_Demux_OpTable_Entry *tbl_;
  // internal lookup table
};

class TAO_Export TAO_Perfect_Hash_OpTable : public TAO_Operation_Table
{
  // = TITLE
  //     Helper class  for use of perfect hashing operation lookup
  //     strategy.
  //
  // = DESCRIPTION
  //     This class declares pure virtual methods called 'lookup ()'
  //     and 'hash ()' which will be generated by the GPERF
  //     program. These methods are used by 'bind ()' and 'find ()'
  //     methods. Subclasses will define the lookup and hash
  //     functions.
public:
  TAO_Perfect_Hash_OpTable (void);
  // Do nothing constructor.

  virtual ~TAO_Perfect_Hash_OpTable (void);
  // Do nothing destrctor.

  virtual int find (const char *opname,
                    TAO_Skeleton &skelfunc);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

  virtual int bind (const char *opname,
                    const TAO_Skeleton skel_ptr);
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

private:
  // = Methods that should defined by the subclasses. GPERF program
  //   will generate these routines.

  virtual unsigned int hash (const char *str, unsigned int len) = 0;

  virtual const TAO_operation_db_entry* lookup (const char *str, unsigned int len) = 0;
};

class TAO_Export TAO_Binary_Search_OpTable : public TAO_Operation_Table
{
  // = TITLE
  //     Helper class for using binary search operatin lookup strategy
  //     in the server skeletons.
  //
  // = DESCRIPTION
  //     This class declares pure virtual method called 'lookup ()'
  //     which will be generated by the GPERF program. This method is
  //     used by 'bind ()' and 'find ()' methods. Subclasses will
  //     define the lookup method.
public:
  TAO_Binary_Search_OpTable (void);
  // Do nothing constructor.

  virtual ~TAO_Binary_Search_OpTable (void);
  // Do nothing destrctor.

  virtual int find (const char *opname,
                    TAO_Skeleton &skelfunc);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

  virtual int bind (const char *opname,
                    const TAO_Skeleton skel_ptr);
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

private:
  // = Method that should defined by the subclasses. GPERF program
  //   will generate this routine routines.
  virtual const TAO_operation_db_entry* lookup (const char *str) = 0;
};


// Define a singleton instance of operation table parameters.
typedef ACE_Singleton<TAO_Operation_Table_Parameters,
                      ACE_SYNCH_RECURSIVE_MUTEX>
        TAO_OP_TABLE_PARAMETERS;


// Define a singleton instance of the operation table factory.
typedef ACE_Singleton<TAO_Operation_Table_Factory,
                      ACE_SYNCH_RECURSIVE_MUTEX>
        TAO_OP_TABLE_FACTORY;

#endif /* TAO_OPTABLE_H */
