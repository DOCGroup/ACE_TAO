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

// @@ Can you please document all the methods and classes in this file?

#if !defined (TAO_OBJTABLE_H)
#  define TAO_OBJTABLE_H

#  include "ace/ACE.h"
#  include "ace/Synch.h"
#  include "ace/Hash_Map_Manager.h"
#  include "ace/SString.h"

#  include "orb.h"

// Dynamic Hashing scheme.
typedef ACE_Hash_Map_Manager<ACE_CString, CORBA_Object_ptr, ACE_SYNCH_RW_MUTEX> OBJ_MAP_MANAGER;

class TAO_Dynamic_Hash_ObjTable: public TAO_Object_Table
{
public:
  TAO_Dynamic_Hash_ObjTable (CORBA_ULong size = 0);

  ~TAO_Dynamic_Hash_ObjTable (void);

  virtual int bind (const CORBA_OctetSeq &key, 
		    CORBA_Object_ptr obj);
  // Registers a CORBA_Object into the object table and associates the
  // key with it.  Returns -1 on failure, 0 on success, 1 on
  // duplicate.

  virtual int find (const CORBA_OctetSeq &key, 
		    CORBA_Object_ptr &obj);
  // Looks up an object in the object table using <{key}>.  Returns
  // non-negative integer on success, or -1 on failure.

private:
  OBJ_MAP_MANAGER hash_;
};

// Linear strategy
struct TAO_Linear_ObjTable_Entry
{
  CORBA_OctetSeq key;
  CORBA_Object_ptr obj;

  TAO_Linear_ObjTable_Entry (void);
  ~TAO_Linear_ObjTable_Entry (void);
};

class TAO_Linear_ObjTable: public TAO_Object_Table
{
public:
  TAO_Linear_ObjTable (CORBA_ULong size);

  ~TAO_Linear_ObjTable (void);

  virtual int bind (const CORBA_OctetSeq &key,
		    CORBA_Object_ptr obj);
  // Registers a CORBA_Object into the object table and associates the
  // key with it.  Returns -1 on failure, 0 on success, 1 on
  // duplicate.

  virtual int find (const CORBA_OctetSeq &key, 
		    CORBA_Object_ptr &obj);
  // Looks up an object in the object table using <{key}>.  Returns
  // non-negative integer on success, or -1 on failure.

private:
  CORBA_ULong next_;
  CORBA_ULong tablesize_;
  TAO_Linear_ObjTable_Entry *tbl_;
};

// Active Demux
struct TAO_Active_Demux_Entry
{
  CORBA_Object_ptr obj;

  TAO_Active_Demux_Entry (void);
  ~TAO_Active_Demux_Entry (void);
};

class TAO_Active_Demux_ObjTable : public TAO_Object_Table
{
public:
  TAO_Active_Demux_ObjTable (CORBA_ULong size);

  ~TAO_Active_Demux_ObjTable (void);

  virtual int bind (const CORBA_OctetSeq &key,
		    CORBA_Object_ptr obj);
  // Registers a CORBA_Object into the object table and associates the
  // key with it.  Returns -1 on failure, 0 on success, 1 on
  // duplicate.

  virtual int find (const CORBA_OctetSeq &key,
		    CORBA_Object_ptr &obj);
  // Looks up an object in the object table using <{key}>.  Returns
  // non-negative integer on success, or -1 on failure.

private:
  CORBA_ULong next_;
  CORBA_ULong tablesize_;
  TAO_Active_Demux_Entry *tbl_;
};

#endif /* TAO_OBJTABLE_H */


