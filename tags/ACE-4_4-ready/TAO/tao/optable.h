// This may look like C, but it's really -*- C++ -*-
//
// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    optable.h
//
// = AUTHOR
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (TAO_OPTABLE_H)
#define TAO_OPTABLE_H

struct TAO_operation_db_entry
  // = TITLE
  //   Define a table entry that holds an operation name and its
  //   corresponding skeleton.  A table of such entries is used to
  //   initialize the different lookup strategies.
{
  CORBA::String opname_;
  // operation name

  TAO_Skeleton skel_ptr_;
  // skeleton pointer
};

class TAO_Export TAO_Operation_Table
  // = TITLE
  //     Abstract class for maintaining and lookup of CORBA IDL
  //     operation names. 
{
public:
  virtual int find (const CORBA::String &opname, 
		   TAO_Skeleton &skelfunc) = 0;
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

  virtual int bind (const CORBA::String &opname,
		    const TAO_Skeleton skel_ptr) = 0;
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

  virtual ~TAO_Operation_Table (void);
};

// Dynamic hashing.  We use template specialization here to use const
// char* as the external ID. The template specialization is needed
// since the "hash" method is not defined on type "char *".
typedef ACE_Hash_Map_Manager<const char *, TAO_Skeleton, ACE_SYNCH_RW_MUTEX> 
        OP_MAP_MANAGER;

class TAO_Export TAO_Dynamic_Hash_OpTable : public TAO_Operation_Table
  // = TITLE
  // Dynamic Hashing scheme for CORBA IDL operation name lookup
{
public:
  TAO_Dynamic_Hash_OpTable (const TAO_operation_db_entry *db, CORBA::ULong
			    dbsize, CORBA::ULong hashtblsize = 0);
  // Constructor.
  // Initialize the dynamic hash operation table with a database of operation
  // names. The hash table size may be different from the size of the
  // database. Hence we use the third argument to specify the size of the
  // internal hash table. 

  ~TAO_Dynamic_Hash_OpTable (void);
  // destructor

  virtual int bind (const CORBA::String &opname, 
		    const TAO_Skeleton skel_ptr);
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

  virtual int find (const CORBA::String &opname,
		    TAO_Skeleton &skelfunc);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

private:
  OP_MAP_MANAGER hash_;
  // The hash table data structure
};

struct TAO_Export TAO_Linear_OpTable_Entry
  // = TITLE
  //   Table entry for linear search lookup strategy.
{
  CORBA::String opname_;
  // holds the operation name

  TAO_Skeleton skel_ptr_;
  // holds a pointer to the skeleton corresponding to the operation name

  TAO_Linear_OpTable_Entry (void);
  // constructor.

  ~TAO_Linear_OpTable_Entry (void);
  // destructor
};

class TAO_Export TAO_Linear_OpTable : public TAO_Operation_Table
// @@ Please comment me.
{
public:
  TAO_Linear_OpTable (const TAO_operation_db_entry *db, CORBA::ULong dbsize);
  // constructor.
  // Initialize the linear search operation table with a database of operation
  // names 

  ~TAO_Linear_OpTable (void);
  // destructor

  virtual int find (const CORBA::String &opname,
		    TAO_Skeleton &skel_ptr);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

  virtual int bind (const CORBA::String &opname,
		    const TAO_Skeleton skelptr);
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

private:
  CORBA::ULong next_;
  // keeps track of the next available slot to be filled.

  CORBA::ULong tablesize_;
  // size of the internal table

  TAO_Linear_OpTable_Entry *tbl_;
  // the table itself
};

struct TAO_Export TAO_Active_Demux_OpTable_Entry
  // = TITLE
  //   Active Demux lookup table entry.
{
  TAO_Skeleton skel_ptr_;
  // skeleton pointer corresponding to the index

  TAO_Active_Demux_OpTable_Entry (void);
  // constructor

  ~TAO_Active_Demux_OpTable_Entry (void);
  // destructor
};

class TAO_Export TAO_Active_Demux_OpTable : public TAO_Operation_Table
  // = TITLE
  //   Implements the active demultiplexed lookup strategy. The key is
  //   assumed to provide an index directly into the internal table.
{
public:
  TAO_Active_Demux_OpTable (const TAO_operation_db_entry *db, CORBA::ULong dbsize);
  // Constructor
  // Initializes the internal table with the database of operations

  ~TAO_Active_Demux_OpTable (void);
  // destructor

  virtual int find (const CORBA::String &opname,
		    TAO_Skeleton &skel_ptr);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

  virtual int bind (const CORBA::String &opname,
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
};

class TAO_Export TAO_Operation_Table_Parameters
// @@ Please comment me.
{
public:
  enum DEMUX_STRATEGY
  {
    // various lookup strategies
    TAO_LINEAR,
    TAO_DYNAMIC_HASH,
    TAO_PERFECT_HASH,
    TAO_ACTIVE_DEMUX,
    TAO_USER_DEFINED
  };

  void lookup_strategy (DEMUX_STRATEGY s);
  // set the lookup strategy from the list of enumerated values

  DEMUX_STRATEGY lookup_strategy (void) const;
  // return the enumerated value for the lookup strategy. Default is Dynamic
  // Hashing. 

  void concrete_strategy (TAO_Operation_Table *ot);
  // Provide a data structure that will do the lookup. This is useful for
  // user-defined lookup strategies.

  TAO_Operation_Table *concrete_strategy (void);
  // return the 

  TAO_Operation_Table_Parameters (void);
  // constructor

  ~TAO_Operation_Table_Parameters (void);
  // destructor
private:
  TAO_Operation_Table *strategy_;
  // pointer to the object that implements a lookup strategy

  DEMUX_STRATEGY type_;
  // the enumerated value indicating the lookup strategy
};

// Define a singleton instance of operation table parameters.
typedef ACE_Singleton<TAO_Operation_Table_Parameters, ACE_SYNCH_RECURSIVE_MUTEX> 
        TAO_OP_TABLE_PARAMETERS;

class TAO_Export TAO_Operation_Table_Factory
  // = TITLE
  //   Factory for producing operation table lookup objects based on
  //   the enumerated value of strategy held by the parameters.
{
public:
  TAO_Operation_Table *opname_lookup_strategy (void);
  // return an instance of the specified lookup strategy

  TAO_Operation_Table_Factory (void);
  // constructor

  ~TAO_Operation_Table_Factory (void);
  // destructor
};

// Define a singleton instance of the operation table factory.
typedef ACE_Singleton<TAO_Operation_Table_Factory, ACE_SYNCH_RECURSIVE_MUTEX> 
        TAO_OP_TABLE_FACTORY;

#endif /* TAO_OPTABLE_H */
