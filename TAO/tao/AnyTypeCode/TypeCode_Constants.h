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
}

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_CONSTANTS_H */
