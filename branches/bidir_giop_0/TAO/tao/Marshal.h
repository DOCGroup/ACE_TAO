// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Marshal.h
//
// = DESCRIPTION
//     Classes that marshal various IDL data types.
//
// = AUTHOR
//     Aniruddha S. Gokhale
//
// ============================================================================

#ifndef TAO_MARSHAL_H
#define TAO_MARSHAL_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Typecode.h"
#include "tao/Principal.h"
#include "tao/Any.h"
#include "tao/ORB.h"

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

class TAO_Export TAO_Marshal_Object
{
  // = TITLE
  //   TAO_Marshal_Object
  //
  // = DESCRIPTION
  //    The Marshaling object that provides a common interface to the
  //    CDR object for marshaling different IDL data types
  //
  //    Provides a set of virtual methods for appending and skipping
  //    data over a CDR stream.
public:
  TAO_Marshal_Object (void);
  // constructor

  virtual ~TAO_Marshal_Object (void);
  // destructor

  static CORBA::TypeCode::traverse_status
    perform_skip (CORBA::TypeCode_ptr tc,
                  TAO_InputCDR *context,
                  CORBA_Environment &ACE_TRY_ENV);
  // skip entry point, it allocates the right kind of Marshal_Object
  // and invokes skip on it.

  virtual CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                                 TAO_InputCDR *context,
                                                 CORBA_Environment &ACE_TRY_ENV) = 0;
  // skip operation

  static CORBA::TypeCode::traverse_status
    perform_append (CORBA::TypeCode_ptr tc,
                    TAO_InputCDR *src,
                    TAO_OutputCDR *dest,
                    CORBA_Environment &ACE_TRY_ENV);
  // append entry point, it allocates the right kind of Marshal_Object
  // and invokes skip on it.

  virtual CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                                   TAO_InputCDR *src,
                                                   TAO_OutputCDR *dest,
                                                   CORBA_Environment &ACE_TRY_ENV) = 0;
  // append operation
};

class TAO_Export TAO_Marshal_Primitive: public TAO_Marshal_Object
{
  // = TITLE
  //   TAO_Marshal_Primitive
  // = DESCRIPTION
  //   marshaling primitives
public:
  TAO_Marshal_Primitive (void);

  virtual CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                                 TAO_InputCDR *context,
                                                 CORBA_Environment &ACE_TRY_ENV);
  // skip operation

  virtual CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                                   TAO_InputCDR *src,
                                                   TAO_OutputCDR *dest,
                                                   CORBA_Environment &ACE_TRY_ENV);
  // append operation

};

class TAO_Export TAO_Marshal_Any: public TAO_Marshal_Object
{
  // = TITLE
  //   TAO_Marshal_Any
  // = DESCRIPTION
  //   Marshal an Any
public:
  TAO_Marshal_Any (void);

  virtual CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                                 TAO_InputCDR *context,
                                                 CORBA_Environment &ACE_TRY_ENV);
  // skip operation

  virtual CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                                   TAO_InputCDR *src,
                                                   TAO_OutputCDR *dest,
                                                   CORBA_Environment &ACE_TRY_ENV);
  // append operation

};

class TAO_Export TAO_Marshal_TypeCode: public TAO_Marshal_Object
{
  // = TITLE
  //   TAO_Marshal_TypeCode
  // = DESCRIPTION
  //   marshal a typecode
public:
  TAO_Marshal_TypeCode (void);

  virtual CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                                 TAO_InputCDR *context,
                                                 CORBA_Environment &ACE_TRY_ENV);
  // skip operation

  virtual CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                                   TAO_InputCDR *src,
                                                   TAO_OutputCDR *dest,
                                                   CORBA_Environment &ACE_TRY_ENV);
  // append operation

};

class TAO_Export TAO_Marshal_Principal: public TAO_Marshal_Object
{
  // = TITLE
  //   TAO_Marshal_Principal
  // = DESCRIPTION
  //   marshal a principal
public:
  TAO_Marshal_Principal (void);

  virtual CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                                 TAO_InputCDR *context,
                                                 CORBA_Environment &ACE_TRY_ENV);
  // skip operation

  virtual CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                                   TAO_InputCDR *src,
                                                   TAO_OutputCDR *dest,
                                                   CORBA_Environment &ACE_TRY_ENV);
  // append operation

};

class TAO_Export TAO_Marshal_ObjRef: public TAO_Marshal_Object
{
  // = TITLE
  //   TAO_Marshal_ObjRef
  // = DESCRIPTION
  //   marshal an object reference
public:
  TAO_Marshal_ObjRef (void);

  virtual CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                                 TAO_InputCDR *context,
                                                 CORBA_Environment &ACE_TRY_ENV);
  // skip operation

  virtual CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                                   TAO_InputCDR *src,
                                                   TAO_OutputCDR *dest,
                                                   CORBA_Environment &ACE_TRY_ENV);
  // append operation

};

class TAO_Export TAO_Marshal_Struct: public TAO_Marshal_Object
{
  // = TITLE
  //   TAO_Marshal_Struct
  // = DESCRIPTION
  //   marshal a struct
public:
  TAO_Marshal_Struct (void);

  virtual CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                                 TAO_InputCDR *context,
                                                 CORBA_Environment &ACE_TRY_ENV);
  // skip operation

  virtual CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                                   TAO_InputCDR *src,
                                                   TAO_OutputCDR *dest,
                                                   CORBA_Environment &ACE_TRY_ENV);
  // append operation

};

class TAO_Export TAO_Marshal_Union: public TAO_Marshal_Object
{
  // = TITLE
  //   TAO_Marshal_Union
  // = DESCRIPTION
  //   marshal a union
public:
  TAO_Marshal_Union (void);

  virtual CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                                 TAO_InputCDR *context,
                                                 CORBA_Environment &ACE_TRY_ENV);
  // skip operation

  virtual CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                                   TAO_InputCDR *src,
                                                   TAO_OutputCDR *dest,
                                                   CORBA_Environment &ACE_TRY_ENV);
  // append operation

};

class TAO_Export TAO_Marshal_String: public TAO_Marshal_Object
{
  // = TITLE
  //   TAO_Marshal_String
  // = DESCRIPTION
  //   marshal a string
public:
  TAO_Marshal_String (void);

  virtual CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                                 TAO_InputCDR *context,
                                                 CORBA_Environment &ACE_TRY_ENV);
  // skip operation

  virtual CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                                   TAO_InputCDR *src,
                                                   TAO_OutputCDR *dest,
                                                   CORBA_Environment &ACE_TRY_ENV);
  // append operation

};

class TAO_Export TAO_Marshal_Sequence: public TAO_Marshal_Object
{
  // = TITLE
  //   TAO_Marshal_Sequence
  // = DESCRIPTION
  //   marshal a sequence
public:
  TAO_Marshal_Sequence (void);

  virtual CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                                 TAO_InputCDR *context,
                                                 CORBA_Environment &ACE_TRY_ENV);
  // skip operation

  virtual CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                                   TAO_InputCDR *src,
                                                   TAO_OutputCDR *dest,
                                                   CORBA_Environment &ACE_TRY_ENV);
  // append operation

};

class TAO_Export TAO_Marshal_Array: public TAO_Marshal_Object
{
  // = TITLE
  //   TAO_Marshal_Array
  // = DESCRIPTION
  //   marshal an array
public:
  TAO_Marshal_Array (void);

  virtual CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                                 TAO_InputCDR *context,
                                                 CORBA_Environment &ACE_TRY_ENV);
  // skip operation

  virtual CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                                   TAO_InputCDR *src,
                                                   TAO_OutputCDR *dest,
                                                   CORBA_Environment &ACE_TRY_ENV);
  // append operation

};

class TAO_Export TAO_Marshal_Alias: public TAO_Marshal_Object
{
  // = TITLE
  //   TAO_Marshal_Alias
  // = DESCRIPTION
  //   marshal an alias
public:
  TAO_Marshal_Alias (void);

  virtual CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                                 TAO_InputCDR *context,
                                                 CORBA_Environment &ACE_TRY_ENV);
  // skip operation

  virtual CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                                   TAO_InputCDR *src,
                                                   TAO_OutputCDR *dest,
                                                   CORBA_Environment &ACE_TRY_ENV);
  // append operation

};

class TAO_Export TAO_Marshal_Except: public TAO_Marshal_Object
{
  // = TITLE
  //   TAO_Marshal_Except
  // = DESCRIPTION
  //   marshal an exception
public:
  TAO_Marshal_Except (void);

  virtual CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                                 TAO_InputCDR *context,
                                                 CORBA_Environment &ACE_TRY_ENV);
  // skip operation

  virtual CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                                   TAO_InputCDR *src,
                                                   TAO_OutputCDR *dest,
                                                   CORBA_Environment &ACE_TRY_ENV);
  // append operation

};

class TAO_Export TAO_Marshal_WString : public TAO_Marshal_Object
{
  // = TITLE
  //   TAO_Marshal_WString
  // = DESCRIPTION
  //   marshal a wide string
public:
  TAO_Marshal_WString (void);

  virtual CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                                 TAO_InputCDR *context,
                                                 CORBA_Environment &ACE_TRY_ENV);
  // skip operation

  virtual CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                                   TAO_InputCDR *src,
                                                   TAO_OutputCDR *dest,
                                                   CORBA_Environment &ACE_TRY_ENV);
  // append operation

};

#if defined (__ACE_INLINE__)
# include "tao/Marshal.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_MARSHAL_H */
