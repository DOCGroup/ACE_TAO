// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/Struct_TypeCode_Static.h"
#include "tao/AnyTypeCode/Null_RefCount_Policy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {
    // Notice that these are all statically instantiated and not
    // exported.

    char const tc_bounds_id[]   = "IDL:omg.org/CORBA/TypeCode/Bounds:1.0";
    char const tc_bounds_name[] = "Bounds";
    Struct<char const *,
           CORBA::TypeCode_ptr const *,
           Struct_Field<char const *,
                        CORBA::TypeCode_ptr const *> const *,
           TAO::Null_RefCount_Policy> tc_Bounds (CORBA::tk_except,
                                                 tc_bounds_id,
                                                 tc_bounds_name,
                                                 0,
                                                 0);

    char const tc_bad_kind_id[]   = "IDL:omg.org/CORBA/TypeCode/BadKind:1.0";
    char const tc_bad_kind_name[] = "BadKind";
    Struct<char const *,
           CORBA::TypeCode_ptr const *,
           Struct_Field<char const *,
                        CORBA::TypeCode_ptr const *> const *,
           TAO::Null_RefCount_Policy> tc_BadKind (CORBA::tk_except,
                                                  tc_bad_kind_id,
                                                  tc_bad_kind_name,
                                                  0,
                                                  0);
  }
}


// ------------------------------------------------------------------
// OMG defined TypeCode constants
// ------------------------------------------------------------------

// Notice that these are constant TypeCode references/pointers, not
// constant TypeCodes.  TypeCodes are effectively read-only since
// all non-static TypeCode operations are const.


CORBA::TypeCode_ptr const CORBA::TypeCode::_tc_BadKind =
  &TAO::TypeCode::tc_BadKind;
CORBA::TypeCode_ptr const CORBA::TypeCode::_tc_Bounds =
  &TAO::TypeCode::tc_Bounds;


TAO_END_VERSIONED_NAMESPACE_DECL
