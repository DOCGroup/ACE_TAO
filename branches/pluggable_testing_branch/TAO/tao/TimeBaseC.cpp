/* -*- C++ -*- */
//
// $Id$
//
// ================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   TimeBaseC.cpp
//
// = DESCRIPTION
//   Generated from TimeBase.pidl
//
// ================================================================

#include "TimeBaseC.h"

#include "TimeBaseS.h"

#if !defined (__ACE_INLINE__)
#include "TimeBaseC.i"
#endif /* !defined INLINE */

ACE_RCSID(tao, TimeBaseC, "$Id$")

static const CORBA::Long _oc_TimeBase_TimeT[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x54696d65), ACE_NTOHL (0x42617365), ACE_NTOHL (0x2f54696d), ACE_NTOHL (0x65543a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/TimeBase/TimeT:1.0
  6, ACE_NTOHL (0x54696d65), ACE_NTOHL (0x54000000),  // name = TimeT
  CORBA::tk_ulonglong,

};
static CORBA::TypeCode _tc_TAO_tc_TimeBase_TimeT (CORBA::tk_alias, sizeof (_oc_TimeBase_TimeT), (char *) &_oc_TimeBase_TimeT, 0, sizeof (TimeBase::TimeT));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (TimeBase)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_TimeT, &_tc_TAO_tc_TimeBase_TimeT)
TAO_NAMESPACE_END
static const CORBA::Long _oc_TimeBase_InaccuracyT[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  37, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x54696d65), ACE_NTOHL (0x42617365), ACE_NTOHL (0x2f496e61), ACE_NTOHL (0x63637572), ACE_NTOHL (0x61637954), ACE_NTOHL (0x3a312e30), ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/TimeBase/InaccuracyT:1.0
  12, ACE_NTOHL (0x496e6163), ACE_NTOHL (0x63757261), ACE_NTOHL (0x63795400),  // name = InaccuracyT
  CORBA::tk_alias, // typecode kind for typedefs
  56, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x54696d65), ACE_NTOHL (0x42617365), ACE_NTOHL (0x2f54696d), ACE_NTOHL (0x65543a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/TimeBase/TimeT:1.0
    6, ACE_NTOHL (0x54696d65), ACE_NTOHL (0x54000000),  // name = TimeT
    CORBA::tk_ulonglong,

};
static CORBA::TypeCode _tc_TAO_tc_TimeBase_InaccuracyT (CORBA::tk_alias, sizeof (_oc_TimeBase_InaccuracyT), (char *) &_oc_TimeBase_InaccuracyT, 0, sizeof (TimeBase::InaccuracyT));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (TimeBase)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_InaccuracyT, &_tc_TAO_tc_TimeBase_InaccuracyT)
TAO_NAMESPACE_END
static const CORBA::Long _oc_TimeBase_TdfT[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  30, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x54696d65), ACE_NTOHL (0x42617365), ACE_NTOHL (0x2f546466), ACE_NTOHL (0x543a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/TimeBase/TdfT:1.0
  5, ACE_NTOHL (0x54646654), ACE_NTOHL (0x0),  // name = TdfT
  CORBA::tk_short,

};
static CORBA::TypeCode _tc_TAO_tc_TimeBase_TdfT (CORBA::tk_alias, sizeof (_oc_TimeBase_TdfT), (char *) &_oc_TimeBase_TdfT, 0, sizeof (TimeBase::TdfT));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (TimeBase)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_TdfT, &_tc_TAO_tc_TimeBase_TdfT)
TAO_NAMESPACE_END
static const CORBA::Long _oc_TimeBase_UtcT[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  30, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x54696d65), ACE_NTOHL (0x42617365), ACE_NTOHL (0x2f557463), ACE_NTOHL (0x543a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/TimeBase/UtcT:1.0
  5, ACE_NTOHL (0x55746354), ACE_NTOHL (0x0),  // name = UtcT
  4, // member count
    5, ACE_NTOHL (0x74696d65), ACE_NTOHL (0x0),  // name = time
    CORBA::tk_alias, // typecode kind for typedefs
    56, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x54696d65), ACE_NTOHL (0x42617365), ACE_NTOHL (0x2f54696d), ACE_NTOHL (0x65543a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/TimeBase/TimeT:1.0
      6, ACE_NTOHL (0x54696d65), ACE_NTOHL (0x54000000),  // name = TimeT
      CORBA::tk_ulonglong,

    8, ACE_NTOHL (0x696e6163), ACE_NTOHL (0x636c6f00),  // name = inacclo
    CORBA::tk_ulong,

    8, ACE_NTOHL (0x696e6163), ACE_NTOHL (0x63686900),  // name = inacchi
    CORBA::tk_ushort,

    4, ACE_NTOHL (0x74646600),  // name = tdf
    CORBA::tk_alias, // typecode kind for typedefs
    56, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      30, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x54696d65), ACE_NTOHL (0x42617365), ACE_NTOHL (0x2f546466), ACE_NTOHL (0x543a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/TimeBase/TdfT:1.0
      5, ACE_NTOHL (0x54646654), ACE_NTOHL (0x0),  // name = TdfT
      CORBA::tk_short,

};
static CORBA::TypeCode _tc_TAO_tc_TimeBase_UtcT (CORBA::tk_struct, sizeof (_oc_TimeBase_UtcT), (char *) &_oc_TimeBase_UtcT, 0, sizeof (TimeBase::UtcT));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (TimeBase)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_UtcT, &_tc_TAO_tc_TimeBase_UtcT)
TAO_NAMESPACE_END
static const CORBA::Long _oc_TimeBase_IntervalT[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  35, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x54696d65), ACE_NTOHL (0x42617365), ACE_NTOHL (0x2f496e74), ACE_NTOHL (0x65727661), ACE_NTOHL (0x6c543a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/TimeBase/IntervalT:1.0
  10, ACE_NTOHL (0x496e7465), ACE_NTOHL (0x7276616c), ACE_NTOHL (0x54000000),  // name = IntervalT
  2, // member count
    12, ACE_NTOHL (0x6c6f7765), ACE_NTOHL (0x725f626f), ACE_NTOHL (0x756e6400),  // name = lower_bound
    CORBA::tk_alias, // typecode kind for typedefs
    56, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x54696d65), ACE_NTOHL (0x42617365), ACE_NTOHL (0x2f54696d), ACE_NTOHL (0x65543a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/TimeBase/TimeT:1.0
      6, ACE_NTOHL (0x54696d65), ACE_NTOHL (0x54000000),  // name = TimeT
      CORBA::tk_ulonglong,

    12, ACE_NTOHL (0x75707065), ACE_NTOHL (0x725f626f), ACE_NTOHL (0x756e6400),  // name = upper_bound
    CORBA::tk_alias, // typecode kind for typedefs
    56, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x54696d65), ACE_NTOHL (0x42617365), ACE_NTOHL (0x2f54696d), ACE_NTOHL (0x65543a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/TimeBase/TimeT:1.0
      6, ACE_NTOHL (0x54696d65), ACE_NTOHL (0x54000000),  // name = TimeT
      CORBA::tk_ulonglong,

};
static CORBA::TypeCode _tc_TAO_tc_TimeBase_IntervalT (CORBA::tk_struct, sizeof (_oc_TimeBase_IntervalT), (char *) &_oc_TimeBase_IntervalT, 0, sizeof (TimeBase::IntervalT));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (TimeBase)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_IntervalT, &_tc_TAO_tc_TimeBase_IntervalT)
TAO_NAMESPACE_END
void operator<<= (CORBA::Any &_tao_any, const TimeBase::UtcT &_tao_elem) // copying
{
  TimeBase::UtcT *_any_val;
  ACE_NEW (_any_val, TimeBase::UtcT (_tao_elem));
  if (!_any_val) return;
  ACE_TRY_NEW_ENV
  {
    _tao_any.replace (TimeBase::_tc_UtcT, _any_val, 1, ACE_TRY_ENV); // copy the value
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY
  {
    delete _any_val;
  }
  ACE_ENDTRY;
}

void operator<<= (CORBA::Any &_tao_any, TimeBase::UtcT *_tao_elem) // non copying
{
  ACE_TRY_NEW_ENV
  {
    _tao_any.replace (TimeBase::_tc_UtcT, _tao_elem, 1, ACE_TRY_ENV); // consume it
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY {}
  ACE_ENDTRY;
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, TimeBase::UtcT *&_tao_elem)
{
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equal (TimeBase::_tc_UtcT, ACE_TRY_ENV)) return 0; // not equal
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (TimeBase::UtcT *)_tao_any.value ();
      return 1;
      }
    else
    {
      ACE_NEW_RETURN (_tao_elem, TimeBase::UtcT, 0);
      TAO_InputCDR stream (_tao_any._tao_get_cdr ());
      if (stream.decode (TimeBase::_tc_UtcT, _tao_elem, 0, ACE_TRY_ENV)
        == CORBA::TypeCode::TRAVERSE_CONTINUE)
      {
        ((CORBA::Any *)&_tao_any)->replace (TimeBase::_tc_UtcT, _tao_elem, 1, ACE_TRY_ENV);
        ACE_TRY_CHECK;
        return 1;
      }
      else
      {
        delete _tao_elem;
      }
    }
  }
  ACE_CATCHANY
  {
    delete _tao_elem;
    return 0;
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const TimeBase::IntervalT &_tao_elem) // copying
{
  TimeBase::IntervalT *_any_val;
  ACE_NEW (_any_val, TimeBase::IntervalT (_tao_elem));
  if (!_any_val) return;
  ACE_TRY_NEW_ENV
  {
    _tao_any.replace (TimeBase::_tc_IntervalT, _any_val, 1, ACE_TRY_ENV); // copy the value
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY
  {
    delete _any_val;
  }
  ACE_ENDTRY;
}

void operator<<= (CORBA::Any &_tao_any, TimeBase::IntervalT *_tao_elem) // non copying
{
  ACE_TRY_NEW_ENV
  {
    _tao_any.replace (TimeBase::_tc_IntervalT, _tao_elem, 1, ACE_TRY_ENV); // consume it
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY {}
  ACE_ENDTRY;
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, TimeBase::IntervalT *&_tao_elem)
{
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equal (TimeBase::_tc_IntervalT, ACE_TRY_ENV)) return 0; // not equal
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (TimeBase::IntervalT *)_tao_any.value ();
      return 1;
      }
    else
    {
      ACE_NEW_RETURN (_tao_elem, TimeBase::IntervalT, 0);
      TAO_InputCDR stream (_tao_any._tao_get_cdr ());
      if (stream.decode (TimeBase::_tc_IntervalT, _tao_elem, 0, ACE_TRY_ENV)
        == CORBA::TypeCode::TRAVERSE_CONTINUE)
      {
        ((CORBA::Any *)&_tao_any)->replace (TimeBase::_tc_IntervalT, _tao_elem, 1, ACE_TRY_ENV);
        ACE_TRY_CHECK;
        return 1;
      }
      else
      {
        delete _tao_elem;
      }
    }
  }
  ACE_CATCHANY
  {
    delete _tao_elem;
    return 0;
  }
  ACE_ENDTRY;
  return 0;
}
