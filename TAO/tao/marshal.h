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

#if 0
#include "ace/OS.h"
#include "ace/Synch.h"
#include "ace/Singleton.h"

#include "tao/typecode.h"
#include "tao/object.h"  // CORBA::Object::_duplicate() needed by inline method
#include "tao/principa.h"
#endif /* 0 */

CORBA::TypeCode::traverse_status
DEEP_COPY (CORBA::TypeCode_ptr  param,
	   const void *source,
	   const void *dest,
	   CORBA::Environment &env);

CORBA::TypeCode::traverse_status
DEEP_FREE (CORBA::TypeCode_ptr  param,
	   const void *source,
	   const void *dest,
	   CORBA::Environment &env);

// = Forward declarations.
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

  virtual TAO_Marshal_Object *make_marshal_object (CORBA::TypeCode_ptr tc,
						  CORBA::Environment &env);
  // Factory method that returns the appropriate marshal object.

private:
  struct TAO_Marshal_Object_Entry
  {
    TAO_Marshal_Object *obj_;  
  };

  TAO_Marshal_Object_Entry mobj_table_[CORBA::TC_KIND_COUNT];
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
#endif /* 0 */
};

class TAO_Marshal
// = TITLE
//    Namespace in which to put otherwise "global" methods like
//    initialize, etc.
{
public:
  static void initialize (void);

  static TAO_Marshal_Factory* DEFAULT_MARSHAL_FACTORY;
};

class TAO_Marshal_Object
{
  // = TITLE
  //    The Marshaling object that provides a common interface to the
  //    CDR object for marshaling different IDL data types
  //
  // = DESCRIPTION
  //    Provides a set of virtual methods for encoding, decoding,
  //    deep_copying, and deep_freeing.
public:
  virtual CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA::Environment &env) = 0;
  // encoding operation

  virtual CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA::Environment &env) = 0;
  // decoding operation

  TAO_Marshal_Object (void);
  // constructor

  virtual ~TAO_Marshal_Object (void);
  // destructor
};

class TAO_Marshal_Primitive: public TAO_Marshal_Object
{
  // @@ Andy, please fill in here and add comments to this class.
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Primitive (void);

  virtual ~TAO_Marshal_Primitive (void);

  virtual CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA::Environment &env);
  virtual CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_copy (CORBA::TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_free (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA::Environment &env);
};

class TAO_Marshal_Any: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Any (void);

  virtual ~TAO_Marshal_Any (void);

  virtual CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA::Environment &env);
  virtual CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_copy (CORBA::TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_free (CORBA::TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *,
                                                  CORBA::Environment &env);
};

class TAO_Marshal_TypeCode: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_TypeCode (void);

  virtual ~TAO_Marshal_TypeCode (void);

  virtual CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA::Environment &env);
  virtual CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_copy (CORBA::TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_free (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA::Environment &env);
};

class TAO_Marshal_Principal: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Principal (void);

  virtual ~TAO_Marshal_Principal (void);

  virtual CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA::Environment &env);
  virtual CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_copy (CORBA::TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_free (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA::Environment &env);
};

class TAO_Marshal_ObjRef: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_ObjRef (void);

  virtual ~TAO_Marshal_ObjRef (void);

  virtual CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA::Environment &env);
  virtual CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_copy (CORBA::TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_free (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA::Environment &env);
};

class TAO_Marshal_Struct: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Struct (void);

  virtual ~TAO_Marshal_Struct (void);

  virtual CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA::Environment &env);
  virtual CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_copy (CORBA::TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_free (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA::Environment &env);
};

class TAO_Marshal_Union: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Union (void);

  virtual ~TAO_Marshal_Union (void);

  virtual CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA::Environment &env);
  virtual CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_copy (CORBA::TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_free (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA::Environment &env);
};

class TAO_Marshal_String: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_String (void);

  virtual ~TAO_Marshal_String (void);

  virtual CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA::Environment &env);
  virtual CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_copy (CORBA::TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_free (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA::Environment &env);
};

class TAO_Marshal_Sequence: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Sequence (void);

  virtual ~TAO_Marshal_Sequence (void);

  virtual CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA::Environment &env);
  virtual CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_copy (CORBA::TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_free (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA::Environment &env);
};

class TAO_Marshal_Array: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Array (void);

  virtual ~TAO_Marshal_Array (void);

  virtual CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA::Environment &env);
  virtual CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_copy (CORBA::TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_free (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA::Environment &env);
};

class TAO_Marshal_Alias: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Alias (void);

  virtual ~TAO_Marshal_Alias (void);

  virtual CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA::Environment &env);
  virtual CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_copy (CORBA::TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_free (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA::Environment &env);
};

class TAO_Marshal_Except: public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_Except (void);
  ~TAO_Marshal_Except (void);

  virtual CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA::Environment &env);
  virtual CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_copy (CORBA::TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_free (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA::Environment &env);
};

class TAO_Marshal_WString : public TAO_Marshal_Object
{
  // = TITLE
  // = DESCRIPTION
public:
  TAO_Marshal_WString (void);
  ~TAO_Marshal_WString (void);

  virtual CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  void *context,
                                                  CORBA::Environment &env);
  virtual CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *parent_typecode,
                                                  void *context,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_copy (CORBA::TypeCode_ptr tc,
                                                  const void *source,
                                                  const void *dest,
                                                  CORBA::Environment &env);
  static CORBA::TypeCode::traverse_status deep_free (CORBA::TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA::Environment &env);
};

#endif /* TAO_MARSHAL_H */
