// This may look like C, but it's really -*- C++ -*-
//
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

#if !defined(TAO_OPERATION_TABLE_H)
#define TAO_OPERATION_TABLE_H

#include "ace/ACE.h"
#include "ace/Synch.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/SString.h"
#include "ace/Singleton.h"

#include "orb.h"

// Dynamic Hashing scheme
typedef ACE_Hash_Map_Manager<ACE_CString, TAO_Skeleton, ACE_SYNCH_RW_MUTEX> OP_MAP_MANAGER;

class TAO_Dynamic_Hash_OpTable: public TAO_Operation_Table
{
public:
  TAO_Dynamic_Hash_OpTable(CORBA_ULong size=0);

  ~TAO_Dynamic_Hash_OpTable();

  virtual int bind(const CORBA_String &opname, const TAO_Skeleton skel_ptr);
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

  virtual int find(const CORBA_String &opname, TAO_Skeleton &skelfunc);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

private:
  OP_MAP_MANAGER  hash_;
};

// Linear strategy
struct TAO_Linear_OpTable_Entry
{
  CORBA_String_var opname;
  TAO_Skeleton skel_ptr;

  TAO_Linear_OpTable_Entry();
  ~TAO_Linear_OpTable_Entry();
};

class TAO_Linear_OpTable: public TAO_Operation_Table
{
public:
  TAO_Linear_OpTable(CORBA_ULong size);

  ~TAO_Linear_OpTable();

  virtual int find(const CORBA_String &opname, TAO_Skeleton &skel_ptr);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

  virtual int bind(const CORBA_String &opname, TAO_Skeleton skelptr);
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

private:

  CORBA_ULong next_;
  CORBA_ULong tablesize_;
  TAO_Linear_OpTable_Entry *tbl_;
};

// Active Demux
struct TAO_Active_Demux_OpTable_Entry
{
  TAO_Skeleton  skel_ptr;

  TAO_Active_Demux_OpTable_Entry();
  ~TAO_Active_Demux_OpTable_Entry();
};

class TAO_Active_Demux_OpTable: public TAO_Operation_Table
{
public:
  TAO_Active_Demux_OpTable(CORBA_ULong size);

  ~TAO_Active_Demux_OpTable();

  virtual int find(const CORBA_String &opname, TAO_Skeleton &skel_ptr);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

  virtual int bind(const CORBA_String &opname, TAO_Skeleton skelptr);
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.

private:
  CORBA_ULong next_;
  CORBA_ULong tablesize_;
  TAO_Active_Demux_OpTable_Entry *tbl_;
};


class OpTable_Parameters
{
public:
  enum DEMUX_STRATEGY
  {
    TAO_LINEAR,
    TAO_DYNAMIC_HASH,
    TAO_ACTIVE_DEMUX,
    TAO_USER_DEFINED
  };
  void lookup_strategy(OpTable_Parameters::DEMUX_STRATEGY s);
  OpTable_Parameters::DEMUX_STRATEGY lookup_strategy() const;
  void concrete_strategy(TAO_Operation_Table *ot);
  TAO_Operation_Table* concrete_strategy();

  OpTable_Parameters();
  ~OpTable_Parameters();
private:
  TAO_Operation_Table *strategy_;
  OpTable_Parameters::DEMUX_STRATEGY type_;
};

typedef ACE_Singleton<OpTable_Parameters, ACE_RW_Mutex> TAO_OP_TABLE_PARAMETERS;

class OpTable_Factory
{
public:
  TAO_Operation_Table *opname_lookup_strategy();
  OpTable_Factory();
  ~OpTable_Factory();
};

typedef ACE_Singleton<OpTable_Factory, ACE_RW_Mutex> TAO_OP_TABLE_FACTORY;

#endif
