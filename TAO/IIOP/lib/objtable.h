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

#if !defined(TAO_OBJECT_TABLE_H)
#define TAO_OBJECT_TABLE_H

#include <ace/ACE.h>
#include <ace/Synch.h>
#include <ace/Hash_Map_Manager.h>
#include <ace/SString.h>

#include "orb.h"

// Dynamic Hashing scheme
typedef ACE_Hash_Map_Manager<ACE_CString, CORBA_Object_ptr, ACE_RW_Mutex>
                                              OBJ_MAP_MANAGER;

class TAO_Dynamic_Hash_ObjTable: public virtual TAO_Object_Table
{
public:
  TAO_Dynamic_Hash_ObjTable(CORBA_ULong size=0);

  ~TAO_Dynamic_Hash_ObjTable();

  virtual CORBA_Object_ptr lookup(const CORBA_OctetSeq &key);
  // CORBA Object key lookup strategy

  virtual int register_obj(const CORBA_OctetSeq &key, CORBA_Object_ptr obj);
  // registers a CORBA_Object into the object table and associates the key with
  // it
private:
  OBJ_MAP_MANAGER  hash_;
};

// Linear strategy
class TAO_Linear_ObjTable: public virtual TAO_Object_Table
{
public:
  TAO_Linear_ObjTable(CORBA_ULong size);

  ~TAO_Linear_ObjTable();

  virtual CORBA_Object_ptr lookup(const CORBA_OctetSeq &key);
  // CORBA Object key lookup strategy

  virtual int register_obj(const CORBA_OctetSeq &key, CORBA_Object_ptr obj);
  // registers a CORBA_Object into the object table and associates the key with
  // it

private:
  struct Entry {
    CORBA_OctetSeq     key;
    CORBA_Object_ptr   obj;

    Entry();
    ~Entry();
  };

  CORBA_ULong next_;
  CORBA_ULong tablesize_;
  Entry       *tbl_;
};

// Active Demux
class TAO_Active_Demux_ObjTable: public virtual TAO_Object_Table
{
public:
  TAO_Active_Demux_ObjTable(CORBA_ULong size);

  ~TAO_Active_Demux_ObjTable();

  virtual CORBA_Object_ptr lookup(const CORBA_OctetSeq &key);
  // CORBA Object key lookup strategy

  virtual int register_obj(const CORBA_OctetSeq &key, CORBA_Object_ptr obj);
  // registers a CORBA_Object into the object table and associates the key with
  // it

private:
  struct Entry {
    CORBA_Object_ptr   obj;

    Entry();
    ~Entry();
  };

  CORBA_ULong next_;
  CORBA_ULong tablesize_;
  Entry       *tbl_;
};

#endif


