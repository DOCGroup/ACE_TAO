// $Id$

#ifndef TAO_INTERPRETER_UTILS_H
#include "ace/pre.h"
#define TAO_INTERPRETER_UTILS_H

#include "ace/Hash_Map_Manager.h"
#include "tao/corba.h"
#include "orbsvcs/orbsvcs_export.h"

// =  Classes to deal with the ACE_Hash_Map_Manager.

class TAO_ORBSVCS_Export TAO_String_Hash_Key : public CORBA::String_var
{
  // = TITLE
  //     Key for the Hash Table. The EXT_ID of the
  //     ACE_Hash_Map_Manager.
public:
  // = Initialization and termination methods.
  TAO_String_Hash_Key (void);
  // Default constructor.

  TAO_String_Hash_Key (char * name);
  // Constructor from a const string.

  TAO_String_Hash_Key (const char * name);
  // Constructor from a const string.

  TAO_String_Hash_Key (const CORBA::String_var &hash_key);
  // Copy constructor.

  ~TAO_String_Hash_Key (void);
  // Destructor.

  int operator == (const TAO_String_Hash_Key &hash_key) const;
  // The operator for hash binding and "find"ing.

  friend int operator < (const TAO_String_Hash_Key &left,
                         const TAO_String_Hash_Key &right);
  // The operator for hash binding and "find"ing.

  u_long hash (void) const;
  // The function that computes a hash value.
};

typedef ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, int, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex> TAO_Lookup_Table;
typedef ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, CORBA::TypeCode_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex> TAO_Typecode_Table;


// *************************************************************
// TAO_Sequence_Extracter_Base
// *************************************************************

class TAO_Sequence_Extracter_Base
// = TITLE
//   Static methods used by the TAO_Sequence_Extracter class.
{
public:

  static CORBA::TCKind sequence_type (CORBA::TypeCode* code,
                                      CORBA::Environment& env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return the sequence element type for the sequence whose typecode
  // is <code>.
};

#include "ace/post.h"
#endif /* TAO_INTERPRETER_UTILS_H */
