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

#include "tao/TAO_Export.h"

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
  extern TypeCode_ptr const _tc_null;
  extern TypeCode_ptr const _tc_void;
  extern TypeCode_ptr const _tc_boolean;
  extern TypeCode_ptr const _tc_char;
  extern TypeCode_ptr const _tc_wchar;
  extern TypeCode_ptr const _tc_short;
  extern TypeCode_ptr const _tc_ushort;
  extern TypeCode_ptr const _tc_long;
  extern TypeCode_ptr const _tc_ulong;
  extern TypeCode_ptr const _tc_longlong;
  extern TypeCode_ptr const _tc_ulonglong;
  extern TypeCode_ptr const _tc_float;
  extern TypeCode_ptr const _tc_double;
  extern TypeCode_ptr const _tc_longdouble;
  extern TypeCode_ptr const _tc_octet;
  extern TypeCode_ptr const _tc_any;
  extern TypeCode_ptr const _tc_TypeCode;
  extern TypeCode_ptr const _tc_Principal;

  extern TypeCode_ptr const _tc_string;
  extern TypeCode_ptr const _tc_wstring;

  extern TypeCode_ptr const _tc_Object;
  extern TypeCode_ptr const _tc_Component;
  extern TypeCode_ptr const _tc_Home;

  //@}
}

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_CONSTANTS_H */
