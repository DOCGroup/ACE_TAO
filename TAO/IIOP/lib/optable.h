// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    objtable.hh
//
// = AUTHOR
//    Aniruddha Gokhale
// 
// ============================================================================

// @@ Can you please comment all the classes and methods in the file?

#if !defined (TAO_OPTABLE_H)
#define TAO_OPTABLE_H

#include "ace/ACE.h"
#include "ace/Synch.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/SString.h"
#include "ace/Singleton.h"

#include "orb.h"

typedef ACE_Hash_Map_Manager<ACE_CString, TAO_Skeleton, ACE_SYNCH_RW_MUTEX> OP_MAP_MANAGER;

class TAO_Operation_Table
  // = TITLE
  //     Abstract class for maintaining and lookup of CORBA IDL
  //     operation names. 
{
public:
  virtual int find(const CORBA_String &opname, 
		   TAO_Skeleton &skelfunc) = 0;
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

  virtual int bind (const CORBA_String &opname,
		    const TAO_Skeleton skel_ptr) = 0;
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

  virtual ~TAO_Operation_Table (void);
};

class TAO_Dynamic_Hash_OpTable : public TAO_Operation_Table
// = TITLE
// Dynamic Hashing scheme
{
public:
  TAO_Dynamic_Hash_OpTable (CORBA_ULong size = 0);

  ~TAO_Dynamic_Hash_OpTable (void);

  virtual int bind (const CORBA_String &opname, 
		    const TAO_Skeleton skel_ptr);
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

  virtual int find (const CORBA_String &opname,
		    TAO_Skeleton &skelfunc);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

private:
  OP_MAP_MANAGER hash_;
};

struct TAO_Linear_OpTable_Entry
// = TITLE
// Linear strategy
{
  CORBA_String_var opname;
  TAO_Skeleton skel_ptr;

  TAO_Linear_OpTable_Entry (void);
  ~TAO_Linear_OpTable_Entry (void);
};

class TAO_Linear_OpTable : public TAO_Operation_Table
{
public:
  TAO_Linear_OpTable (CORBA_ULong size);

  ~TAO_Linear_OpTable (void);

  virtual int find (const CORBA_String &opname,
		    TAO_Skeleton &skel_ptr);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

  virtual int bind (const CORBA_String &opname,
		    TAO_Skeleton skelptr);
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

private:
  CORBA_ULong next_;
  CORBA_ULong tablesize_;
  TAO_Linear_OpTable_Entry *tbl_;
};

struct TAO_Active_Demux_OpTable_Entry
// = TITLE
// Active Demux
{
  TAO_Skeleton skel_ptr;

  TAO_Active_Demux_OpTable_Entry (void);
  ~TAO_Active_Demux_OpTable_Entry (void);
};

class TAO_Active_Demux_OpTable : public TAO_Operation_Table
{
public:
  TAO_Active_Demux_OpTable (CORBA_ULong size);

  ~TAO_Active_Demux_OpTable (void);

  virtual int find (const CORBA_String &opname,
		    TAO_Skeleton &skel_ptr);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

  virtual int bind (const CORBA_String &opname,
		    TAO_Skeleton skelptr);
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

private:
  CORBA_ULong next_;
  CORBA_ULong tablesize_;
  TAO_Active_Demux_OpTable_Entry *tbl_;
};

class TAO_Operation_Table_Parameters
{
public:
  enum DEMUX_STRATEGY
  {
    TAO_LINEAR,
    TAO_DYNAMIC_HASH,
    TAO_ACTIVE_DEMUX,
    TAO_USER_DEFINED
  };

  void lookup_strategy (DEMUX_STRATEGY s);
  DEMUX_STRATEGY lookup_strategy (void) const;
  void concrete_strategy (TAO_Operation_Table *ot);
  TAO_Operation_Table *concrete_strategy (void);

  TAO_Operation_Table_Parameters (void);
  ~TAO_Operation_Table_Parameters (void);

private:
  TAO_Operation_Table *strategy_;
  DEMUX_STRATEGY type_;
};

typedef ACE_Singleton<TAO_Operation_Table_Parameters, ACE_RW_Mutex> TAO_OP_TABLE_PARAMETERS;

class TAO_Operation_Table_Factory
{
public:
  TAO_Operation_Table *opname_lookup_strategy (void);
  TAO_Operation_Table_Factory (void);
  ~TAO_Operation_Table_Factory (void);
};

typedef ACE_Singleton<TAO_Operation_Table_Factory, ACE_RW_Mutex> TAO_OP_TABLE_FACTORY;

#endif /* TAO_OPTABLE_H */
