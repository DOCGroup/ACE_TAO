// $Id$

#include "TypeCode_Constants.h"


ACE_RCSID (tao,
           TypeCode_Constants,
           "$Id$")


#include "Null_RefCount_Policy.h"

#include "Empty_Param_TypeCode.h"
#include "Objref_TypeCode.h"

namespace TAO
{
  namespace TypeCode
  {
    // Notice that these are all statically instantiated, nor are they
    // exported.

    Empty_Param tc_null       (CORBA::tk_null);
    Empty_Param tc_void       (CORBA::tk_void);
    Empty_Param tc_boolean    (CORBA::tk_boolean);
    Empty_Param tc_char       (CORBA::tk_char);
    Empty_Param tc_wchar      (CORBA::tk_wchar);
    Empty_Param tc_short      (CORBA::tk_short);
    Empty_Param tc_ushort     (CORBA::tk_ushort);
    Empty_Param tc_long       (CORBA::tk_long);
    Empty_Param tc_ulong      (CORBA::tk_ulong);
    Empty_Param tc_longlong   (CORBA::tk_longlong);
    Empty_Param tc_ulonglong  (CORBA::tk_ulonglong);
    Empty_Param tc_float      (CORBA::tk_float);
    Empty_Param tc_double     (CORBA::tk_double);
    Empty_Param tc_longdouble (CORBA::tk_longdouble);
    Empty_Param tc_octet      (CORBA::tk_octet);
    Empty_Param tc_any        (CORBA::tk_any);
    Empty_Param tc_TypeCode   (CORBA::tk_TypeCode);
    Empty_Param tc_Principal  (CORBA::tk_Principal);

    char const * const tc_object_id   = "IDL:omg.org/CORBA/Object:1.0";
    char const * const tc_object_name = "Object";
    Objref tc_object (tc_object_id, tc_object_name);

  }  // End TypeCode namespace
}  // End TAO namespace

// ------------------------------------------------------------------
// OMG defined TypeCode constants
// ------------------------------------------------------------------

namespace CORBA
{
  // Notice that these are constant TypeCode references/pointers, not
  // constant TypeCodes.  TypeCodes are effectively read-only since
  // all non-static TypeCode operations are const.

  TypeCode_ptr const _tc_null       = &TAO::TypeCode::tc_null;
  TypeCode_ptr const _tc_void       = &TAO::TypeCode::tc_void;
  TypeCode_ptr const _tc_boolean    = &TAO::TypeCode::tc_boolean;
  TypeCode_ptr const _tc_char       = &TAO::TypeCode::tc_char;
  TypeCode_ptr const _tc_wchar      = &TAO::TypeCode::tc_wchar;
  TypeCode_ptr const _tc_short      = &TAO::TypeCode::tc_short;
  TypeCode_ptr const _tc_ushort     = &TAO::TypeCode::tc_ushort;
  TypeCode_ptr const _tc_long       = &TAO::TypeCode::tc_long;
  TypeCode_ptr const _tc_ulong      = &TAO::TypeCode::tc_ulong;
  TypeCode_ptr const _tc_longlong   = &TAO::TypeCode::tc_longlong;
  TypeCode_ptr const _tc_ulonglong  = &TAO::TypeCode::tc_ulonglong;
  TypeCode_ptr const _tc_float      = &TAO::TypeCode::tc_float;
  TypeCode_ptr const _tc_double     = &TAO::TypeCode::tc_double;
  TypeCode_ptr const _tc_longdouble = &TAO::TypeCode::tc_longdouble;
  TypeCode_ptr const _tc_octet      = &TAO::TypeCode::tc_octet;
  TypeCode_ptr const _tc_any        = &TAO::TypeCode::tc_any;
  TypeCode_ptr const _tc_TypeCode   = &TAO::TypeCode::tc_TypeCode;
  TypeCode_ptr const _tc_Principal  = &TAO::TypeCode::tc_Principal;

  TypeCode_ptr const _tc_Object     = &TAO::TypeCode::tc_object;
}
