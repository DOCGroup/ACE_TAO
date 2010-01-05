// -*- C++ -*-

//=============================================================================
/**
 *  @file   TypeCode_Constants.h
 *
 *  $Id$
 *
 *  Declare the @c TypeCode constants available to the ORB and user
 *  applications.
 *
 *  @author Jeff Parsons
 *  @author Ossama Othman
 */
//=============================================================================

#ifndef TAO_TYPECODE_CONSTANTS_H
#define TAO_TYPECODE_CONSTANTS_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TAO_AnyTypeCode_Export.h"

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class TypeCode;
  typedef TypeCode * TypeCode_ptr;

  /**
   * @name TypeCode Constants
   *
   * All the TypeCode constants
   */
  //@{
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_null;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_void;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_boolean;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_char;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_wchar;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_short;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_ushort;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_long;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_ulong;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_longlong;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_ulonglong;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_float;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_double;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_longdouble;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_octet;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_any;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_TypeCode;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_Principal;

  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_string;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_wstring;

  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_Object;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_Component;
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_Home;
  //@}

#define TAO_SYSTEM_EXCEPTION_LIST \
  TAO_SYSTEM_EXCEPTION(UNKNOWN); \
  TAO_SYSTEM_EXCEPTION(BAD_PARAM); \
  TAO_SYSTEM_EXCEPTION(NO_MEMORY); \
  TAO_SYSTEM_EXCEPTION(IMP_LIMIT); \
  TAO_SYSTEM_EXCEPTION(COMM_FAILURE); \
  TAO_SYSTEM_EXCEPTION(INV_OBJREF); \
  TAO_SYSTEM_EXCEPTION(OBJECT_NOT_EXIST); \
  TAO_SYSTEM_EXCEPTION(NO_PERMISSION); \
  TAO_SYSTEM_EXCEPTION(INTERNAL); \
  TAO_SYSTEM_EXCEPTION(MARSHAL); \
  TAO_SYSTEM_EXCEPTION(INITIALIZE); \
  TAO_SYSTEM_EXCEPTION(NO_IMPLEMENT); \
  TAO_SYSTEM_EXCEPTION(BAD_TYPECODE); \
  TAO_SYSTEM_EXCEPTION(BAD_OPERATION); \
  TAO_SYSTEM_EXCEPTION(NO_RESOURCES); \
  TAO_SYSTEM_EXCEPTION(NO_RESPONSE); \
  TAO_SYSTEM_EXCEPTION(PERSIST_STORE); \
  TAO_SYSTEM_EXCEPTION(BAD_INV_ORDER); \
  TAO_SYSTEM_EXCEPTION(TRANSIENT); \
  TAO_SYSTEM_EXCEPTION(FREE_MEM); \
  TAO_SYSTEM_EXCEPTION(INV_IDENT); \
  TAO_SYSTEM_EXCEPTION(INV_FLAG); \
  TAO_SYSTEM_EXCEPTION(INTF_REPOS); \
  TAO_SYSTEM_EXCEPTION(BAD_CONTEXT); \
  TAO_SYSTEM_EXCEPTION(OBJ_ADAPTER); \
  TAO_SYSTEM_EXCEPTION(DATA_CONVERSION); \
  TAO_SYSTEM_EXCEPTION(INV_POLICY); \
  TAO_SYSTEM_EXCEPTION(REBIND); \
  TAO_SYSTEM_EXCEPTION(TIMEOUT); \
  TAO_SYSTEM_EXCEPTION(TRANSACTION_UNAVAILABLE); \
  TAO_SYSTEM_EXCEPTION(TRANSACTION_MODE); \
  TAO_SYSTEM_EXCEPTION(TRANSACTION_REQUIRED); \
  TAO_SYSTEM_EXCEPTION(TRANSACTION_ROLLEDBACK); \
  TAO_SYSTEM_EXCEPTION(INVALID_TRANSACTION); \
  TAO_SYSTEM_EXCEPTION(CODESET_INCOMPATIBLE); \
  TAO_SYSTEM_EXCEPTION(BAD_QOS); \
  TAO_SYSTEM_EXCEPTION(INVALID_ACTIVITY); \
  TAO_SYSTEM_EXCEPTION(ACTIVITY_COMPLETED); \
  TAO_SYSTEM_EXCEPTION(ACTIVITY_REQUIRED); \
  TAO_SYSTEM_EXCEPTION(THREAD_CANCELLED);

  // = Typecode constants for system exceptions.
#define TAO_SYSTEM_EXCEPTION(name) \
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_ ## name
  TAO_SYSTEM_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION
  //@}

  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_Current;

  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_NamedValue;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_CONSTANTS_H */
