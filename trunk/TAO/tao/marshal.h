// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    marshal.h
//
// = DESCRIPTION
//     Classes that marshal various IDL data types.
//
// = AUTHOR
//     Aniruddha S. Gokhale
// 
// ============================================================================

#if !defined (TAO_MARSHAL_H)
#define TAO_MARSHAL_H

#include "ace/OS.h"
#include "ace/Synch.h"
#include "ace/Singleton.h"

#include "tao/typecode.h"
#include "tao/object.h"  // CORBA_Object::_duplicate() needed by inline method
#include "tao/principa.h"

CORBA_TypeCode::traverse_status
DEEP_COPY (CORBA_TypeCode_ptr  param,
	   const void *source,
	   const void *dest,
	   CORBA_Environment &env);

CORBA_TypeCode::traverse_status
DEEP_FREE (CORBA_TypeCode_ptr  param,
	   const void *source,
	   const void *dest,
	   CORBA_Environment &env);

class TAO_Marshal_Object;
class TAO_Marshal_Primitive;
class TAO_Marshal_Any;
class TAO_Marshal_TypeCode;
class TAO_Marshal_Principal;
class TAO_Marshal_ObjRef;
class TAO_Marshal_Struct;
class TAO_Marshal_Union;
class TAO_Marshal_String;
class TAO_Marshal_Sequence;
class TAO_Marshal_Array;
class TAO_Marshal_Alias;
class TAO_Marshal_Except;
class TAO_Marshal_WString;

class TAO_Marshal_Factory
{
  // = TITLE
  // A Factory for generating specialized marshaling objects
  //
  // = DESCRIPTION
public:
  TAO_Marshal_Factory (void);
  // constructor

  virtual ~TAO_Marshal_Factory (void);
  // destructor

  virtual TAO_Marshal_Object *make_marshal_object (CORBA_TypeCode_ptr tc,
						  CORBA_Environment &env);
  // Factory method that returns the appropriate marshal object.

private:
  struct TAO_Marshal_Object_Entry
  {
    TAO_Marshal_Object *obj_;  
  };

  TAO_Marshal_Object_Entry mobj_table_[TC_KIND_COUNT];
  // A table of specialized marshal objects indexed by the _kind field

#if 0
  // define data members that are instances of various Marshal_Object classes
  TAO_Marshal_Primitive *m_primitive_;
  TAO_Marshal_Any       *m_any_;
  TAO_Marshal_TypeCode  *m_typecode_;
  TAO_Marshal_Principal *m_principal_;
  TAO_Marshal_ObjRef    *m_objref_;
  TAO_Marshal_Struct    *m_struct_;
  TAO_Marshal_Union     *m_union_;
  TAO_Marshal_String    *m_string_;
  TAO_Marshal_Sequence  *m_sequence_;
  TAO_Marshal_Array     *m_array_;
  TAO_Marshal_Alias     *m_alias_;
  TAO_Marshal_Except    *m_except_;
  TAO_Marshal_WString   *m_wstring_;
#endif
};

// Create a type for the singleton factory
typedef ACE_Singleton<TAO_Marshal_Factory, ACE_SYNCH_MUTEX> 
	TAO_MARSHAL_FACTORY;

// Define a default marshal factory to be used by the interpretive
// marshaling engine.
extern TAO_Marshal_Factory *TAO_DEFAULT_MARSHAL_FACTORY;

class TAO_Marshal_Object
{
  // = TITLE
  // The Marshaling object that provides a common interface to the CDR
  // object for marshaling different IDL data types
  //
  // = DESCRIPTION
  // Provides a set of virtual methods for encoding, decoding,
  // deep_copying, and deep_freeing.
public:
  virtual CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA_Environment &env) = 0;
  // encoding operation

  virtual CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA_Environment &env) = 0;
  // decoding operation

  TAO_Marshal_Object (void);
  // constructor

  virtual ~TAO_Marshal_Object (void);
  // destructor
};

class TAO_Marshal_Primitive: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Primitive (void);

  virtual ~TAO_Marshal_Primitive (void);

  virtual CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA_Environment &env);
  virtual CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_copy (CORBA_TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_free (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA_Environment &env);
};

// Create a type for the singleton
typedef ACE_Singleton<TAO_Marshal_Primitive, ACE_SYNCH_MUTEX>
	TAO_MARSHAL_PRIMITIVE;

class TAO_Marshal_Any: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Any (void);

  virtual ~TAO_Marshal_Any (void);

  virtual CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA_Environment &env);
  virtual CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_copy (CORBA_TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_free (CORBA_TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *,
                                                  CORBA_Environment &env);
};

// Create a type for the singleton
typedef ACE_Singleton<TAO_Marshal_Any, ACE_SYNCH_MUTEX> 
	TAO_MARSHAL_ANY;

class TAO_Marshal_TypeCode: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_TypeCode (void);

  virtual ~TAO_Marshal_TypeCode (void);

  virtual CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA_Environment &env);
  virtual CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_copy (CORBA_TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_free (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA_Environment &env);
};

// Create a type for the singleton
typedef ACE_Singleton<TAO_Marshal_TypeCode, ACE_SYNCH_MUTEX> 
	TAO_MARSHAL_TYPECODE;

class TAO_Marshal_Principal: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Principal (void);

  virtual ~TAO_Marshal_Principal (void);

  virtual CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA_Environment &env);
  virtual CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_copy (CORBA_TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_free (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA_Environment &env);
};

// Create a type for the singleton
typedef ACE_Singleton<TAO_Marshal_Principal, ACE_SYNCH_MUTEX> 
	TAO_MARSHAL_PRINCIPAL;

class TAO_Marshal_ObjRef: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_ObjRef (void);

  virtual ~TAO_Marshal_ObjRef (void);

  virtual CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA_Environment &env);
  virtual CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_copy (CORBA_TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_free (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA_Environment &env);
};

// Create a type for the singleton
typedef ACE_Singleton<TAO_Marshal_ObjRef, ACE_SYNCH_MUTEX> 
	TAO_MARSHAL_OBJREF;

class TAO_Marshal_Struct: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Struct (void);

  virtual ~TAO_Marshal_Struct (void);

  virtual CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA_Environment &env);
  virtual CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_copy (CORBA_TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_free (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA_Environment &env);
};

// Create a type for the singleton
typedef ACE_Singleton<TAO_Marshal_Struct, ACE_SYNCH_MUTEX> 
	TAO_MARSHAL_STRUCT;

class TAO_Marshal_Union: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Union (void);

  virtual ~TAO_Marshal_Union (void);

  virtual CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA_Environment &env);
  virtual CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_copy (CORBA_TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_free (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA_Environment &env);
};

// Create a type for the singleton
typedef ACE_Singleton<TAO_Marshal_Union, ACE_SYNCH_MUTEX>
	TAO_MARSHAL_UNION;

class TAO_Marshal_String: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_String (void);

  virtual ~TAO_Marshal_String (void);

  virtual CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA_Environment &env);
  virtual CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_copy (CORBA_TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_free (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA_Environment &env);
};

// Create a type for the singleton
typedef ACE_Singleton<TAO_Marshal_String, ACE_SYNCH_MUTEX> 
	TAO_MARSHAL_STRING;

class TAO_Marshal_Sequence: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Sequence (void);

  virtual ~TAO_Marshal_Sequence (void);

  virtual CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA_Environment &env);
  virtual CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_copy (CORBA_TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_free (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA_Environment &env);
};

// Create a type for the singleton
typedef ACE_Singleton<TAO_Marshal_Sequence, ACE_SYNCH_MUTEX> 
	TAO_MARSHAL_SEQUENCE;

class TAO_Marshal_Array: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Array (void);

  virtual ~TAO_Marshal_Array (void);

  virtual CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA_Environment &env);
  virtual CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_copy (CORBA_TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_free (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA_Environment &env);
};

// Create a type for the singleton
typedef ACE_Singleton<TAO_Marshal_Array, ACE_SYNCH_MUTEX> 
	TAO_MARSHAL_ARRAY;

class TAO_Marshal_Alias: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Alias (void);

  virtual ~TAO_Marshal_Alias (void);

  virtual CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA_Environment &env);
  virtual CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_copy (CORBA_TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_free (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA_Environment &env);
};

// Create a type for the singleton
typedef ACE_Singleton<TAO_Marshal_Alias, ACE_SYNCH_MUTEX> 
	TAO_MARSHAL_ALIAS;

class TAO_Marshal_Except: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Except (void);
  ~TAO_Marshal_Except (void);

  virtual CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA_Environment &env);
  virtual CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_copy (CORBA_TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_free (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA_Environment &env);
};

// Create a type for the singleton
typedef ACE_Singleton<TAO_Marshal_Except, ACE_SYNCH_MUTEX> 
	TAO_MARSHAL_EXCEPT;

class TAO_Marshal_WString : public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_WString (void);
  ~TAO_Marshal_WString (void);

  virtual CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA_Environment &env);
  virtual CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_copy (CORBA_TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA_Environment &env);
  static CORBA_TypeCode::traverse_status deep_free (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA_Environment &env);
};

// Create a type for the singleton
typedef ACE_Singleton<TAO_Marshal_WString, ACE_SYNCH_MUTEX> 
	TAO_MARSHAL_WSTRING;

// In this case, we make a substantial exception to how inline files
// are included.  Normally, we would conditionally include the inline
// file iff __ACE_INLINE__ is defined.  But, in the original, highly
// optimized Sun IIOP code, much of what is in the inline file was
// here ready to be inlined at a moments notice and ALWAYS.  So, in
// this ONE file, we defer to David Brownell's considerable prowess at
// creating typecode interpreters as well as to the ACE convention of
// placing inline functions into separate files.
#  if !defined (__ACE_INLINE__)
#    undef ACE_INLINE
#    define ACE_INLINE inline
#    define do_undef_on_ACE_INLINE
#  endif
#  include "marshal.i"
#  if defined (do_undef_on_ACE_INLINE)
#    undef do_undef_on_ACE_INLINE
#    undef ACE_INLINE
#    define ACE_INLINE
#  endif

#endif
