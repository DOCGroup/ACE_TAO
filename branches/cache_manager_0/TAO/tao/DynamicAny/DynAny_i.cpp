/* -*- C++ -*- */
// $Id$

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
TAO_DynAny_i::check_typecode (CORBA::TypeCode_ptr tc
                              TAO_ENV_ARG_DECL)
{
  // Check to see if it's a simple type.
  CORBA::TCKind tk = TAO_DynAnyFactory::unalias (tc TAO_ENV_ARG_PARAMETER);
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
TAO_DynAny_i::set_to_default_value (CORBA::TypeCode_ptr tc
                                    TAO_ENV_ARG_DECL)
{
  CORBA::TCKind tk = TAO_DynAnyFactory::unalias (tc TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  switch (tk)
  {
    case CORBA::tk_null:
      break;
    case CORBA::tk_void:
      this->any_ = CORBA_Any (CORBA::_tc_void);
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
      {
        TAO_OutputCDR stream;
        stream << CORBA::Object::_nil ();
        this->any_._tao_replace (tc,
                                 TAO_ENCAP_BYTE_ORDER,
                                 stream.begin ());
        break;
      }
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
TAO_DynAny_i::init_common (void)
{
  this->ref_to_component_ = 0;
  this->container_is_destroying_ = 0;
  this->has_components_ = 0;
  this->destroyed_ = 0;
  this->current_position_ = -1;
  this->component_count_ = 0;
}

void
TAO_DynAny_i::init (CORBA_TypeCode_ptr tc
                    TAO_ENV_ARG_DECL)
{
  this->check_typecode (tc
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->set_to_default_value (tc
                              TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->init_common ();

  this->type_ = CORBA::TypeCode::_duplicate (tc);
}

void
TAO_DynAny_i::init (const CORBA_Any& any
                    TAO_ENV_ARG_DECL)
{
  this->type_ = any.type ();
  this->check_typecode (this->type_.in ()
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->init_common ();

  this->any_ = any;
}

// ****************************************************************

TAO_DynAny_i *
TAO_DynAny_i::_narrow (CORBA::Object_ptr obj
                       TAO_ENV_ARG_DECL_NOT_USED)
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

void
TAO_DynAny_i::from_any (const CORBA_Any &any
                        TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  CORBA::TypeCode_var any_tc = any.type ();

  CORBA::Boolean equiv =
    this->type_->equivalent (any_tc.in ()
                             TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!equiv)
    {
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }

// @@@ (JP) Spec also says we should check for illegal Any
// value here, and throw InvalidValue if we find one.
// Something like a null string will be caught in the constructor.

  this->any_ = any;
}

CORBA::Any_ptr
TAO_DynAny_i::to_any (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  CORBA_Any_ptr retval;

  ACE_NEW_THROW_EX (retval,
                    CORBA::Any (this->any_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return retval;
}

CORBA::Boolean
TAO_DynAny_i::equal (DynamicAny::DynAny_ptr rhs
                     TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  TAO_DynAny_i *rhs_n = TAO_DynAny_i::_narrow (rhs
                                               TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (rhs_n == 0)
    {
      return 0;
    }

  CORBA::Boolean equiv = this->type_->equivalent (rhs_n->type_.in ()
                                                  TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (!equiv)
    {
      return 0;
    }

  int tk = this->type_->kind ();
  ACE_CHECK_RETURN (0);

  switch (tk)
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
      return 1;
    case CORBA::tk_short:
      {
        CORBA::Short rhs_v;
        rhs_n->any_ >>= rhs_v;
        CORBA::Short lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_long:
      {
        CORBA::Long rhs_v;
        rhs_n->any_ >>= rhs_v;
        CORBA::Long lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_ushort:
      {
        CORBA::UShort rhs_v;
        rhs_n->any_ >>= rhs_v;
        CORBA::UShort lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_ulong:
      {
        CORBA::ULong rhs_v;
        rhs_n->any_ >>= rhs_v;
        CORBA::ULong lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_float:
      {
        CORBA::Float rhs_v;
        rhs_n->any_ >>= rhs_v;
        CORBA::Float lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_double:
      {
        CORBA::Double rhs_v;
        rhs_n->any_ >>= rhs_v;
        CORBA::Double lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_longlong:
      {
        CORBA::LongLong rhs_v;
        rhs_n->any_ >>= rhs_v;
        CORBA::LongLong lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_ulonglong:
      {
        CORBA::ULongLong rhs_v;
        rhs_n->any_ >>= rhs_v;
        CORBA::ULongLong lhs_v;
        this->any_ >>= lhs_v;
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_boolean:
      {
        CORBA::Boolean rhs_v;
        rhs_n->any_ >>= CORBA::Any::to_boolean (rhs_v);
        CORBA::Boolean lhs_v;
        this->any_ >>= CORBA::Any::to_boolean (lhs_v);
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_char:
      {
        CORBA::Char rhs_v;
        rhs_n->any_ >>= CORBA::Any::to_char (rhs_v);
        CORBA::Char lhs_v;
        this->any_ >>= CORBA::Any::to_char (lhs_v);
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_wchar:
      {
        CORBA::WChar rhs_v;
        rhs_n->any_ >>= CORBA::Any::to_wchar (rhs_v);
        CORBA::WChar lhs_v;
        this->any_ >>= CORBA::Any::to_wchar (lhs_v);
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_octet:
      {
        CORBA::Octet rhs_v;
        rhs_n->any_ >>= CORBA::Any::to_octet (rhs_v);
        CORBA::Octet lhs_v;
        this->any_ >>= CORBA::Any::to_octet (lhs_v);
        return (lhs_v == rhs_v);
      }
    case CORBA::tk_any:
      {
        CORBA::Any rhs_v;
        rhs_n->any_ >>= rhs_v;
        CORBA::Any lhs_v;
        this->any_ >>= lhs_v;

        DynamicAny::DynAny_var rhs_dyn =
          TAO_DynAnyFactory::make_dyn_any (rhs_v
                                           TAO_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        DynamicAny::DynAny_var lhs_dyn =
          TAO_DynAnyFactory::make_dyn_any (lhs_v
                                           TAO_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        CORBA::Boolean b = rhs_dyn->equal (lhs_dyn.in ()
                                           TAO_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        rhs_dyn->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        lhs_dyn->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        return b;
      }
    case CORBA::tk_TypeCode:
      {
        CORBA::TypeCode_ptr rhs_v;
        rhs_n->any_ >>= rhs_v;
        CORBA::TypeCode_ptr lhs_v;
        this->any_ >>= lhs_v;
        // See CORBA 2.4.2 - must use equal() here.
        return lhs_v->equal (lhs_v
                             TAO_ENV_ARG_PARAMETER);
      }
    case CORBA::tk_objref:
      {
        CORBA::Object_ptr rhs_v;
        rhs_n->any_ >>= CORBA::Any::to_object (rhs_v);
        CORBA::Object_ptr lhs_v;
        this->any_ >>= CORBA::Any::to_object (lhs_v);
        return lhs_v->_is_equivalent (lhs_v
                                      TAO_ENV_ARG_PARAMETER);
      }
    case CORBA::tk_string:
      {
        CORBA::TypeCode_var unaliased_tc =
          TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                          TAO_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        CORBA::ULong bound = unaliased_tc->length (TAO_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        const char* rhs_v;
        rhs_n->any_ >>= CORBA::Any::to_string (rhs_v,
                                               bound);

        // @@@ (JP) On Windows (only), this any does not get the bound
        // set in its type code when copied from another DynAny. So we
        // dispense with the to_string here, since the type codes have
        // already been checked for equivalence above (done with this
        // DynAny's type_ member, which does have the bound set correctly).
        const char* lhs_v;
        this->any_ >>= lhs_v;

        return ACE_OS::strcmp (rhs_v, lhs_v) == 0;
      }
    case CORBA::tk_wstring:
      {
        CORBA::TypeCode_var unaliased_tc =
          TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                          TAO_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        CORBA::ULong bound = unaliased_tc->length (TAO_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        CORBA::WChar * rhs_v;
        rhs_n->any_ >>= CORBA::Any::to_wstring (rhs_v,
                                                bound);
        CORBA::WChar * lhs_v;
        this->any_ >>= CORBA::Any::to_wstring (lhs_v,
                                               bound);
        return ACE_OS::wscmp (rhs_v, lhs_v) == 0;
      }
    default:
      break; // Cannot happen...
    }

  return 0;
}

void
TAO_DynAny_i::destroy (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (!this->ref_to_component_ || this->container_is_destroying_)
    {
      this->destroyed_ = 1;
    }
}


DynamicAny::DynAny_ptr
TAO_DynAny_i::current_component (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        DynamicAny::DynAny::_nil ());
    }

  ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                    DynamicAny::DynAny::_nil ());
}
