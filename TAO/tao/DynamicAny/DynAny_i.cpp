/* -*- C++ -*- */
// $Id$
// =================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynAny_i.cpp
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// =================================================================

#include "DynAny_i.h"

#include "DynAnyFactory.h"

ACE_RCSID(DynamicAny, DynAny_i, "$Id$")

// Constructor from typecode
TAO_DynAny_i::TAO_DynAny_i (void)
{
}

// Destructor
TAO_DynAny_i::~TAO_DynAny_i (void)
{
}

void
TAO_DynAny_i::check_typecode (CORBA::TypeCode_ptr tc,
                              CORBA::Environment &ACE_TRY_ENV)
{
  // Check to see if it's a simple type.
  CORBA::TCKind tk = TAO_DynAnyFactory::unalias (tc, ACE_TRY_ENV);
  ACE_CHECK;
  switch (tk)
  {
    case CORBA::tk_null:
    case CORBA::tk_void:
    case CORBA::tk_short:
    case CORBA::tk_long:
    case CORBA::tk_ushort:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_double:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
    case CORBA::tk_boolean:
    case CORBA::tk_char:
    case CORBA::tk_wchar:
    case CORBA::tk_octet:
    case CORBA::tk_any:
    case CORBA::tk_TypeCode:
    case CORBA::tk_objref:
    case CORBA::tk_string:
    case CORBA::tk_wstring:
      break;
    default:
      ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
  }
}

void
TAO_DynAny_i::set_to_default_value (CORBA::TypeCode_ptr tc,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TCKind tk = TAO_DynAnyFactory::unalias (tc, ACE_TRY_ENV);
  ACE_CHECK;
  switch (tk)
  {
    case CORBA::tk_null:
    case CORBA::tk_void:
      break;
    case CORBA::tk_short:
      this->any_ <<= ACE_static_cast (CORBA::Short, 0);
      break;
    case CORBA::tk_long:
      this->any_ <<= ACE_static_cast (CORBA::Long, 0);
      break;
    case CORBA::tk_ushort:
      this->any_ <<= ACE_static_cast (CORBA::UShort, 0);
      break;
    case CORBA::tk_ulong:
      this->any_ <<= ACE_static_cast (CORBA::ULong, 0);
      break;
#if !defined (ACE_LACKS_LONGLONG_T)
    case CORBA::tk_longlong:
      this->any_ <<= ACE_static_cast (CORBA::LongLong, 0);
      break;
    case CORBA::tk_ulonglong:
      this->any_ <<= ACE_static_cast (CORBA::ULongLong, 0);
      break;
#endif /* ACE_LACKS_LONGLONG_T */
    case CORBA::tk_boolean:
      this->any_ <<= CORBA::Any::from_boolean (0);
      break;
    case CORBA::tk_octet:
      this->any_ <<= CORBA::Any::from_octet (0);
      break;
    case CORBA::tk_char:
      this->any_ <<= CORBA::Any::from_char (0);
      break;
    case CORBA::tk_wchar:
      this->any_ <<= CORBA::Any::from_wchar (0);
      break;
    case CORBA::tk_float:
      this->any_ <<= ACE_static_cast (CORBA::Float, 0);
      break;
    case CORBA::tk_double:
      this->any_ <<= ACE_static_cast (CORBA::Double, 0);
      break;
    case CORBA::tk_any:
      this->any_ <<= CORBA::Any (CORBA::_tc_null);
      break;
    case CORBA::tk_TypeCode:
      {
        CORBA::TypeCode tc (CORBA::tk_null);
        CORBA::TypeCode_ptr tc_ptr = &tc;
        this->any_ <<= tc_ptr;
        break;
      }
    case CORBA::tk_objref:
      this->any_ <<= CORBA::Object::_nil ();
      break;
    case CORBA::tk_string:
      this->any_ <<= "";
      break;
    case CORBA::tk_wstring:
      {
        CORBA::WChar wstr[1];
        wstr[0] = 0;
        this->any_ <<= wstr;
        break;
      }
    default:
      // Should never get here - check_typecode() has already been called.
      break;
  }
}

void
TAO_DynAny_i::init (CORBA_TypeCode_ptr tc,
                    CORBA::Environment &ACE_TRY_ENV)
{
  this->check_typecode (tc, 
                        ACE_TRY_ENV);
  ACE_CHECK;

  this->set_to_default_value (tc,
                              ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_DynAny_i::init (const CORBA_Any& any,
                    CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TypeCode_var tc = any.type ();
  this->check_typecode (tc.in (), 
                        ACE_TRY_ENV);
  ACE_CHECK;

  this->any_ = any;
}

// ****************************************************************

TAO_DynAny_i *
TAO_DynAny_i::_narrow (CORBA::Object_ptr obj,
                       CORBA::Environment &)
{
  if (CORBA::is_nil (obj))
    {
      return 0;
    }

  return ACE_reinterpret_cast (
             TAO_DynAny_i*,
             obj->_tao_QueryInterface (
                      ACE_reinterpret_cast (
                          ptr_arith_t,
                          &TAO_DynAny_i::_narrow
                        )
                    )
           );
}

void*
TAO_DynAny_i::_tao_QueryInterface (ptr_arith_t type)
{
  ptr_arith_t mytype =
    ACE_reinterpret_cast (ptr_arith_t,
                          &TAO_DynAny_i::_narrow);
  if (type == mytype)
    {
      this->_add_ref ();
      return this;
    }

  return 
    this->ACE_NESTED_CLASS (DynamicAny, DynAny::_tao_QueryInterface) (type);
}

// ****************************************************************

CORBA::TypeCode_ptr
TAO_DynAny_i::type (CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  return this->any_.type ();
}

void
TAO_DynAny_i::assign (DynamicAny::DynAny_ptr dyn_any,
                      CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch
      ))
{
  TAO_DynAny_i *rhs =
    TAO_DynAny_i::_narrow (dyn_any, ACE_TRY_ENV);
  ACE_CHECK;
  if (rhs == 0)
    ACE_THROW (DynamicAny::DynAny::TypeMismatch ());

  this->any_ = rhs->any_;
  rhs->_remove_ref ();
}

void
TAO_DynAny_i::from_any (const CORBA_Any& any,
                        CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var any_tc = any.type ();
  this->check_typecode (any_tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  this->any_ = any;
}

CORBA::Any_ptr
TAO_DynAny_i::to_any (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  CORBA_Any_ptr retval;

  ACE_NEW_THROW_EX (retval,
                    CORBA::Any (this->any_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return retval;
}

CORBA::Boolean
TAO_DynAny_i::equal (
        DynamicAny::DynAny_ptr /* rhs */,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
#if 0
  CORBA::TypeCode_var rhs_tc = rhs->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA::TypeCode_var lhs_tc = this->any_.type ();

  CORBA::Boolean equivalent =
    lhs_tc->equivalent (rhs_tc.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  if (!equivalent)
    return 0;

  int tk = lhs_tc->kind ();
  ACE_TRY_CHECK;

  CORBA::Any_var rhs_any =
    rhs->to_any (ACE_TRY_ENV);
  ACE_TRY_CHECK;

  switch (tk)
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
      return 1;

    case CORBA::tk_short:
      {
        CORBA::Short rhs_v;
        rhs_any >>= rhs_v;
        CORBA::Short lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_long:
      {
        CORBA::Long rhs_v;
        rhs_any >>= rhs_v;
        CORBA::Long lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_ushort:
      {
        CORBA::UShort rhs_v;
        rhs_any >>= rhs_v;
        CORBA::UShort lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_ulong:
      {
        CORBA::ULong rhs_v;
        rhs_any >>= rhs_v;
        CORBA::ULong lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_float:
      {
        CORBA::Float rhs_v;
        rhs_any >>= rhs_v;
        CORBA::Float lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_double:
      {
        CORBA::Double rhs_v;
        rhs_any >>= rhs_v;
        CORBA::Double lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_longlong:
      {
        CORBA::LongLong rhs_v;
        rhs_any >>= rhs_v;
        CORBA::LongLong lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_ulonglong:
      {
        CORBA::ULongLong rhs_v;
        rhs_any >>= rhs_v;
        CORBA::ULongLong lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_boolean:
      {
        CORBA::Boolean rhs_v;
        rhs_any >>= CORBA::Any::to_boolean(rhs_v);
        CORBA::Boolean lhs_v;
        this->any_ >>= CORBA::Any::to_boolean(lhs_v);
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_char:
      {
        CORBA::Char rhs_v;
        rhs_any >>= CORBA::Any::to_char(rhs_v);
        CORBA::Char lhs_v;
        this->any_ >>= CORBA::Any::to_char(lhs_v);
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_wchar:
      {
        CORBA::WChar rhs_v;
        rhs_any >>= CORBA::Any::to_wchar(rhs_v);
        CORBA::WChar lhs_v;
        this->any_ >>= CORBA::Any::to_wchar(lhs_v);
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_octet:
      {
        CORBA::Octet rhs_v;
        rhs_any >>= CORBA::Any::to_octet(rhs_v);
        CORBA::Octet lhs_v;
        this->any_ >>= CORBA::Any::to_octet(lhs_v);
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_any:
      {
        CORBA::Any rhs_v;
        rhs_any >>= rhs_v;
        CORBA::Any lhs_v;
        this->any_ >>= lhs_v;

        DynamicAny::DynAny_var rhs_dyn =
          TAO_DynAnyFactory::create_basic_dyn_any (rhs_v, ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);

        DynamicAny::DynAny_var lhs_dyn =
          TAO_DynAnyFactory::create_basic_dyn_any (lhs_v, ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);

        // @@ What about destroy?
        CORBA::Boolean b = 0;
        ACE_TRY
          {
            b = rhs_dyn->equal (lhs_dyn.in (), ACE_TRY_ENV);
            ACE_TRY_CHECK;
          }
        ACE_CATCHANY { }
        ACE_ENDTRY;

        // @@ Should we catch exceptions here too?
        rhs_dyn->destroy (ACE_TRY_ENV);
        ACE_CHECK;
        lhs_dyn->destroy (ACE_TRY_ENV);
        ACE_CHECK;

        return b;
      }
    case CORBA::tk_TypeCode:
      {
        CORBA::TypeCode_ptr rhs_v;
        rhs_any >>= rhs_v;
        CORBA::TypeCode_ptr lhs_v;
        this->any_ >>= lhs_v;
        return lhs_v->equivalent (lhs_v, ACE_TRY_ENV);
      }
    case CORBA::tk_objref:
      {
        CORBA::Object_ptr rhs_v;
        rhs_any >>= CORBA::Any::to_object (rhs_v);
        CORBA::Object_ptr lhs_v;
        this->any_ >>= CORBA::Any::to_object (lhs_v);
        return lhs_v->_is_equivalent (lhs_v, ACE_TRY_ENV);
      }
    case CORBA::tk_string:
      {
        const CORBA::Char * rhs_v;
        rhs_any >>= rhs_v;
        const CORBA::Char * lhs_v;
        this->any_ >>= lhs_v;
        return ACE_OS::strcmp (rhs_v, lhs_v) == 0;
      }
      break;
    case CORBA::tk_wstring:
      {
        const CORBA::WChar * rhs_v;
        rhs_any >>= rhs_v;
        const CORBA::WChar * lhs_v;
        this->any_ >>= lhs_v;
        // @@ return ACE_OS::strcmp (rhs_v, lhs_v) == 0;
        return 0;
      }
      break;
    default:
      break;// @@ Cannot happen...
    }
  return 0;
#endif /* 0 */
}

void
TAO_DynAny_i::destroy (CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  // @@ It is possible that we should set a flag to raise
  // CORBA::OBJECT_NOT_EXIST if any method is invoked after this
  // call.
}


DynamicAny::DynAny_ptr
TAO_DynAny_i::copy (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  TAO_DynAny_i* tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_DynAny_i (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());
  tmp->init (this->any_, ACE_TRY_ENV);
  ACE_CHECK_RETURN (tmp);

  return tmp;
}

// Insert functions

void
TAO_DynAny_i::insert_boolean (CORBA::Boolean value,
                              CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  this->any_ <<= CORBA::Any::from_boolean (value);
}

void
TAO_DynAny_i::insert_octet (CORBA::Octet value,
                            CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                   ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_octet)
    {
      CORBA::Any::from_octet tmp (value);
      this->any_ <<= tmp;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_char (CORBA::Char value,
                           CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_char)
    {
      CORBA::Any::from_char tmp (value);
      this->any_ <<= tmp;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_short (CORBA::Short value,
                            CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_short)
    {
      this->any_ <<= value;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_ushort (CORBA::UShort value,
                             CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_ushort)
    {
      this->any_ <<= value;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_long (CORBA::Long value,
                           CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_long)
    {
      this->any_ <<= value;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_ulong (CORBA::ULong value,
                            CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_ulong)
    {
      this->any_ <<= value;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_float (CORBA::Float value,
                            CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_float)
    {
      this->any_ <<= value;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_double (CORBA::Double value,
                             CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_double)
    {
      this->any_ <<= value;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_string (const char * value,
                             CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_string)
    {
      this->any_ <<= value;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_reference (CORBA::Object_ptr value,
                                CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_objref)
    {
      CORBA::Object_var obj = CORBA::Object::_duplicate (value);

      TAO_OutputCDR stream;
      if (!(stream << obj.in ()))
          ACE_THROW (CORBA::MARSHAL ());

      CORBA::TypeCode_var tc = this->any_.type ();

      this->any_._tao_replace (tc.in (),
                                 TAO_ENCAP_BYTE_ORDER,
                                 stream.begin (),
                                 1,
                                 obj._retn (),
                                 CORBA::Object::_tao_any_destructor);
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_typecode (CORBA::TypeCode_ptr value,
                               CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_TypeCode)
    {
      this->any_ <<= value;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_longlong (CORBA::LongLong value,
                               CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_longlong)
    {
      this->any_ <<= value;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_ulonglong (CORBA::ULongLong value,
                                CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_ulonglong)
    {
      this->any_ <<= value;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_longdouble (
        CORBA::LongDouble,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_DynAny_i::insert_wchar (CORBA::WChar value,
                            CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_wchar)
    {
      CORBA::Any::from_wchar tmp (value);
      this->any_ <<= tmp;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_wstring (
        const CORBA::WChar *,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_DynAny_i::insert_any (const CORBA::Any& value,
                          CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->any_.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_any)
    {
      this->any_ <<= value;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynAny_i::insert_dyn_any (
        DynamicAny::DynAny_ptr,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

#ifdef TAO_HAS_VALUETYPE
void
TAO_DynAny_i::insert_val (
        CORBA::ValueBase_ptr,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}
#endif /* TAO_HAS_VALUETYPE */

// ****************************************************************

CORBA::Boolean
TAO_DynAny_i::get_boolean (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Boolean val;
  CORBA::Any::to_boolean tmp (val);

  if (!(this->any_ >>= tmp))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        val);
    }

  return val;
}

CORBA::Octet
TAO_DynAny_i::get_octet (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Octet val;
  CORBA::Any::to_octet tmp (val);

  if (!(this->any_ >>= tmp))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        val);
    }

  return val;
}

CORBA::Char
TAO_DynAny_i::get_char (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Char val;
  CORBA::Any::to_char tmp (val);

  if (!(this->any_ >>= tmp))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        val);
    }

  return val;
}

CORBA::Short
TAO_DynAny_i::get_short (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Short val;

  if (!(this->any_ >>= val))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        val);
    }

  return val;
}

CORBA::UShort
TAO_DynAny_i::get_ushort (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::UShort val;

  if (!(this->any_ >>= val))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        val);
    }

  return val;
}

CORBA::Long
TAO_DynAny_i::get_long (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Long val;

  if (!(this->any_ >>= val))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        val);
    }

  return val;
}

CORBA::ULong
TAO_DynAny_i::get_ulong (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::ULong val;

  if (!(this->any_ >>= val))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        val);
    }

  return val;
}

CORBA::Float
TAO_DynAny_i::get_float (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Float val;

  if (!(this->any_ >>= val))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        val);
    }

  return val;
}

CORBA::Double
TAO_DynAny_i::get_double (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Double val;

  if (!(this->any_ >>= val))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        val);
    }

  return val;
}

char *
TAO_DynAny_i::get_string (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  const char *val;

  if (!(this->any_ >>= val))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (), 0);
    }

  return CORBA::string_dup (val);
}

CORBA::Object_ptr
TAO_DynAny_i::get_reference (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA_Object_ptr val;
  CORBA::Any::to_object tmp (val);

  if (!(this->any_ >>= tmp))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        val);
    }

  return CORBA::Object::_duplicate (val);
}

CORBA::TypeCode_ptr
TAO_DynAny_i::get_typecode (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA_TypeCode_ptr val;

  if (!(this->any_ >>= val))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        val);
    }

  return CORBA::TypeCode::_duplicate (val);
}

CORBA::LongLong
TAO_DynAny_i::get_longlong (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::LongLong val;

  if (!(this->any_ >>= val))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        val);
    }

  return val;
}

CORBA::ULongLong
TAO_DynAny_i::get_ulonglong (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::ULongLong val;

  if (!(this->any_ >>= val))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        val);
    }

  return val;
}

CORBA::LongDouble
TAO_DynAny_i::get_longdouble (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::LongDouble ret = ACE_CDR_LONG_DOUBLE_INITIALIZER;
  ACE_UNUSED_ARG (ret);
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), ret);
}

CORBA::WChar
TAO_DynAny_i::get_wchar (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::WChar val;
  CORBA::Any::to_wchar tmp (val);

  if (!(this->any_ >>= tmp))
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        val);
    }

  return val;
}

CORBA::WChar *
TAO_DynAny_i::get_wstring (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Any_ptr
TAO_DynAny_i::get_any (CORBA::Environment& ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA_Any_ptr tmp = 0;

  ACE_NEW_THROW_EX (tmp,
                    CORBA_Any,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA_Any_var val = tmp;

  if (!(this->any_ >>= *tmp))
    {
      val = 0;
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        0);
    }

  return val._retn ();
}

DynamicAny::DynAny_ptr
TAO_DynAny_i::get_dyn_any (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), DynamicAny::DynAny::_nil ());
}

#ifdef TAO_HAS_VALUETYPE
CORBA::ValueBase_ptr
TAO_DynAny_i::get_val (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}
#endif /* TAO_HAS_VALUETYPE */

// ****************************************************************

CORBA::Boolean
TAO_DynAny_i::seek (CORBA::Long slot,
                    CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  return (CORBA::Boolean) (slot == 0 ? 1 : 0);
}

void
TAO_DynAny_i::rewind (CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
}

CORBA::Boolean
TAO_DynAny_i::next (CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  return 0;
}

CORBA::ULong
TAO_DynAny_i::component_count (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

DynamicAny::DynAny_ptr
TAO_DynAny_i::current_component (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch
      ))
{
  ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                    DynamicAny::DynAny::_nil ());
}
