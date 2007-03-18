// -*- C++ -*-

//=============================================================================
/**
 *  @file    Marshal.h
 *
 *  $Id$
 *
 *   Classes that marshal various IDL data types.
 *
 *
 *  @author  Aniruddha S. Gokhale
 */
//=============================================================================


#ifndef TAO_MARSHAL_H
#define TAO_MARSHAL_H

#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/TAO_AnyTypeCode_Export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class TypeCode;
  typedef TypeCode *TypeCode_ptr;
}

class TAO_OutputCDR;
class TAO_InputCDR;

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
class TAO_Marshal_Value;

namespace TAO
{
  enum traverse_status
    {
      TRAVERSE_STOP,
      TRAVERSE_CONTINUE
    };
}

/**
 * @class TAO_Marshal_Object
 *
 * @brief TAO_Marshal_Object
 *
 * The Marshaling object that provides a common interface to the
 * CDR object for marshaling different IDL data types
 * Provides a set of virtual methods for appending and skipping
 * data over a CDR stream.
 */
class TAO_AnyTypeCode_Export TAO_Marshal_Object
{
public:
  /// constructor
  TAO_Marshal_Object (void);

  /// destructor
  virtual ~TAO_Marshal_Object (void);

  /// skip entry point, it allocates the right kind of Marshal_Object
  /// and invokes skip on it.
  static TAO::traverse_status perform_skip (CORBA::TypeCode_ptr tc,
                                            TAO_InputCDR *context);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context) = 0;

  /// append entry point, it allocates the right kind of Marshal_Object
  /// and invokes skip on it.
  static TAO::traverse_status perform_append (CORBA::TypeCode_ptr tc,
                                              TAO_InputCDR *src,
                                              TAO_OutputCDR *dest);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest) = 0;
};

/**
 * @class TAO_Marshal_Primitive:
 *
 * @brief TAO_Marshal_Primitive
 *
 * marshaling primitives
 */
class TAO_AnyTypeCode_Export TAO_Marshal_Primitive: public TAO_Marshal_Object
{
public:
  TAO_Marshal_Primitive (void);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest);
};

/**
 * @class TAO_Marshal_Any:
 *
 * @brief TAO_Marshal_Any
 *
 * Marshal an Any
 */
class TAO_AnyTypeCode_Export TAO_Marshal_Any: public TAO_Marshal_Object
{
public:
  TAO_Marshal_Any (void);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest);
};

/**
 * @class TAO_Marshal_TypeCode
 *
 * @brief TAO_Marshal_TypeCode
 *
 * marshal a typecode
 */
class TAO_AnyTypeCode_Export TAO_Marshal_TypeCode: public TAO_Marshal_Object
{
public:
  TAO_Marshal_TypeCode (void);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest);
};

/**
 * @class TAO_Marshal_Principal
 *
 * @brief TAO_Marshal_Principal
 *
 * marshal a principal
 */
class TAO_AnyTypeCode_Export TAO_Marshal_Principal: public TAO_Marshal_Object
{
public:
  TAO_Marshal_Principal (void);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest);
};

/**
 * @class TAO_Marshal_ObjRef
 *
 * @brief TAO_Marshal_ObjRef
 *
 * marshal an object reference
 */
class TAO_AnyTypeCode_Export TAO_Marshal_ObjRef: public TAO_Marshal_Object
{
public:
  TAO_Marshal_ObjRef (void);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest);
};

/**
 * @class TAO_Marshal_Struct
 *
 * @brief TAO_Marshal_Struct
 *
 * marshal a struct
 */
class TAO_AnyTypeCode_Export TAO_Marshal_Struct : public TAO_Marshal_Object
{
public:
  TAO_Marshal_Struct (void);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest);
};

/**
 * @class TAO_Marshal_Union
 *
 * @brief TAO_Marshal_Union
 *
 * marshal a union
 */
class TAO_AnyTypeCode_Export TAO_Marshal_Union: public TAO_Marshal_Object
{
public:
  TAO_Marshal_Union (void);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest);
};

/**
 * @class TAO_Marshal_String
 *
 * @brief TAO_Marshal_String
 *
 * marshal a string
 */
class TAO_AnyTypeCode_Export TAO_Marshal_String: public TAO_Marshal_Object
{
public:
  TAO_Marshal_String (void);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest);
};

/**
 * @class TAO_Marshal_Sequence
 *
 * @brief TAO_Marshal_Sequence
 *
 * marshal a sequence
 */
class TAO_AnyTypeCode_Export TAO_Marshal_Sequence: public TAO_Marshal_Object
{
public:
  TAO_Marshal_Sequence (void);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest);
};

/**
 * @class TAO_Marshal_Array
 *
 * @brief TAO_Marshal_Array
 *
 * marshal an array
 */
class TAO_AnyTypeCode_Export TAO_Marshal_Array: public TAO_Marshal_Object
{
public:
  TAO_Marshal_Array (void);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest);
};

/**
 * @class TAO_Marshal_Alias
 *
 * @brief TAO_Marshal_Alias
 *
 * marshal an alias
 */
class TAO_AnyTypeCode_Export TAO_Marshal_Alias: public TAO_Marshal_Object
{
public:
  TAO_Marshal_Alias (void);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest);
};

/**
 * @class TAO_Marshal_Except
 *
 * @brief TAO_Marshal_Except
 *
 * marshal an exception
 */
class TAO_AnyTypeCode_Export TAO_Marshal_Except: public TAO_Marshal_Object
{
public:
  TAO_Marshal_Except (void);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest);
};

/**
 * @class TAO_Marshal_WString
 *
 * @brief TAO_Marshal_WString
 *
 * marshal a wide string
 */
class TAO_AnyTypeCode_Export TAO_Marshal_WString : public TAO_Marshal_Object
{
public:
  TAO_Marshal_WString (void);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest);
};

/**
 * @class TAO_Marshal_Value
 *
 * @brief TAO_Marshal_Value
 *
 * marshal a valuetype
 */
class TAO_AnyTypeCode_Export TAO_Marshal_Value: public TAO_Marshal_Object
{
public:
  TAO_Marshal_Value (void);

  /// skip operation
  virtual TAO::traverse_status skip (CORBA::TypeCode_ptr tc,
                                     TAO_InputCDR *context);

  /// append operation
  virtual TAO::traverse_status append (CORBA::TypeCode_ptr tc,
                                       TAO_InputCDR *src,
                                       TAO_OutputCDR *dest);

private:
  CORBA::Boolean nested_processing_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/AnyTypeCode/Marshal.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_MARSHAL_H */
