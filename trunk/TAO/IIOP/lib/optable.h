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

#include <ace/ACE.h>
#include <ace/Synch.h>
#include <ace/Hash_Map_Manager.h>
#include <ace/SString.h>
#include <ace/Singleton.h>

#include "orb.h"

// Dynamic Hashing scheme
typedef ACE_Hash_Map_Manager<ACE_CString, skeleton, ACE_RW_Mutex>
                                              OP_MAP_MANAGER;

class TAO_Dynamic_Hash_OpTable: public virtual TAO_Operation_Table
{
public:
  TAO_Dynamic_Hash_OpTable(CORBA_ULong size=0);

  ~TAO_Dynamic_Hash_OpTable();

  virtual int register_op(const CORBA_String &opname, skeleton skel_ptr);
  // registers a operation skeleton into the operation table and associates the key with
  // it
  virtual skeleton lookup(const CORBA_String &opname);
  // CORBA operation name lookup strategy

private:
  OP_MAP_MANAGER  hash_;
};

// Linear strategy
class TAO_Linear_OpTable: public virtual TAO_Operation_Table
{
public:
  TAO_Linear_OpTable(CORBA_ULong size);

  ~TAO_Linear_OpTable();

  virtual skeleton lookup(const CORBA_String &opname);
  // CORBA Object key lookup strategy

  virtual int register_op(const CORBA_String &opname, skeleton skelptr);
  // registers a CORBA_Object into the object table and associates the key with
  // it

private:
  struct Entry {
    CORBA_String_var   opname;
    skeleton           skel_ptr;

    Entry();
    ~Entry();
  };

  CORBA_ULong next_;
  CORBA_ULong tablesize_;
  Entry       *tbl_;
};

// Active Demux
class TAO_Active_Demux_OpTable: public virtual TAO_Operation_Table
{
public:
  TAO_Active_Demux_OpTable(CORBA_ULong size);

  ~TAO_Active_Demux_OpTable();

  virtual skeleton lookup(const CORBA_String &opname);
  // CORBA Object key lookup strategy

  virtual int register_op(const CORBA_String &opname, skeleton skel_ptr);
  // registers a CORBA_Object into the object table and associates the key with
  // it

private:
  struct Entry {
    skeleton  skel_ptr;

    Entry();
    ~Entry();
  };

  CORBA_ULong next_;
  CORBA_ULong tablesize_;
  Entry       *tbl_;
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
