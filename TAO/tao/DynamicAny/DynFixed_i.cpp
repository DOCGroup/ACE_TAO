// -*- C++ -*-

//=============================================================================
/**
 *  @file    DynFixed_i.cpp
 *
 *  @author Johnny Willemsen <jwillemsen@remedy.nl>
 */
//=============================================================================

#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/DynamicAny/DynFixed_i.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "tao/CDR.h"

#include <cctype>
#include <string>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynFixed_i::TAO_DynFixed_i (CORBA::Boolean allow_truncation)
  : TAO_DynCommon (allow_truncation)
  , value_ (ACE_CDR::Fixed::from_integer ())
{
}

void
TAO_DynFixed_i::init_common ()
{
  this->ref_to_component_ = false;
  this->container_is_destroying_ = false;
  this->has_components_ = false;
  this->destroyed_ = false;
  this->current_position_ = -1;
  this->component_count_ = 0;
}

bool
TAO_DynFixed_i::read_value (TAO_InputCDR& cdr,
                            ACE_CDR::Fixed& value) const
{
  CORBA::TypeCode_var tc = TAO_DynAnyFactory::strip_alias (this->type_.in ());
  CORBA::UShort const digits = tc->fixed_digits ();
  CORBA::UShort const scale = tc->fixed_scale ();

  if (digits == 0 || digits > ACE_CDR::Fixed::MAX_DIGITS || scale > digits)
    {
      return false;
    }
  int const length = (digits + 2) / 2;
  ACE_CDR::Octet octets[16];
  for (int i = 0; i < length; ++i)
    {
      if (!cdr.read_octet (octets[i]))
        {
          return false;
        }
    }

  // Each decimal digit is a BCD nibble. An even precision has a leading
  // zero nibble; the final low nibble is the sign.
  int const first_digit = (digits % 2 == 0) ? 1 : 0;
  if (first_digit && (octets[0] >> 4) != 0)
    {
      return false;
    }
  for (int digit = first_digit; digit < first_digit + digits; ++digit)
    {
      int const octet = digit / 2;
      ACE_CDR::Octet const nibble = digit % 2 == 0
        ? static_cast<ACE_CDR::Octet> (octets[octet] >> 4)
        : static_cast<ACE_CDR::Octet> (octets[octet] & 0x0f);
      if (nibble > 9)
        {
          return false;
        }
    }
  ACE_CDR::Octet const sign = octets[length - 1] & 0x0f;
  if (sign != ACE_CDR::Fixed::POSITIVE && sign != ACE_CDR::Fixed::NEGATIVE)
    {
      return false;
    }

  // from_octets infers the digit count and can drop a leading zero nibble.
  // Rebuild from the TypeCode-sized digits to preserve the declared scale.
  std::string decimal;
  if (sign == ACE_CDR::Fixed::NEGATIVE)
    {
      decimal += '-';
    }
  for (int digit = 0; digit < digits; ++digit)
    {
      if (scale && digit == digits - scale)
        {
          decimal += '.';
        }
      int const octet = (first_digit + digit) / 2;
      ACE_CDR::Octet const nibble = (first_digit + digit) % 2 == 0
        ? static_cast<ACE_CDR::Octet> (octets[octet] >> 4)
        : static_cast<ACE_CDR::Octet> (octets[octet] & 0x0f);
      decimal += static_cast<char> ('0' + nibble);
    }
  value = ACE_CDR::Fixed::from_string (decimal.c_str ());
  return true;
}

void
TAO_DynFixed_i::init (CORBA::Any const& any)
{
  CORBA::TypeCode_var tc = any.type ();
  if (TAO_DynAnyFactory::unalias (tc.in ()) != CORBA::tk_fixed)
    {
      throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
    }
  this->type_ = tc;
  TAO::Any_Impl *impl = any.impl ();
  if (!impl)
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }
  bool decoded = false;

  if (impl->encoded ())
    {
      TAO::Unknown_IDL_Type *unknown =
        dynamic_cast<TAO::Unknown_IDL_Type *> (impl);
      if (!unknown)
        {
          throw CORBA::INTERNAL ();
        }
      TAO_InputCDR input (unknown->_tao_get_cdr ());
      decoded = this->read_value (input, this->value_);
    }
  else
    {
      TAO_OutputCDR output;
      impl->marshal_value (output);
      TAO_InputCDR input (output);
      decoded = this->read_value (input, this->value_);
    }
  if (!decoded)
    {
      throw CORBA::MARSHAL ();
    }
  this->init_common ();
}

void
TAO_DynFixed_i::init (CORBA::TypeCode_ptr tc)
{
  if (TAO_DynAnyFactory::unalias (tc) != CORBA::tk_fixed)
    {
      throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
    }
  this->type_ = CORBA::TypeCode::_duplicate (tc);
  CORBA::TypeCode_var unaliased = TAO_DynAnyFactory::strip_alias (tc);
  CORBA::UShort const digits = unaliased->fixed_digits ();
  CORBA::UShort const scale = unaliased->fixed_scale ();
  if (digits == 0 || digits > ACE_CDR::Fixed::MAX_DIGITS || scale > digits)
    {
      throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
    }
  std::string zero (digits - scale, '0');
  if (scale)
    {
      zero += '.';
      zero.append (scale, '0');
    }
  this->value_ = ACE_CDR::Fixed::from_string (zero.c_str ());
  this->init_common ();
}

TAO_DynFixed_i *
TAO_DynFixed_i::_narrow (CORBA::Object_ptr obj)
{
  if (CORBA::is_nil (obj))
    {
      return nullptr;
    }
  return dynamic_cast<TAO_DynFixed_i *> (obj);
}

char *
TAO_DynFixed_i::get_value ()
{
  if (this->destroyed_)
    {
      throw CORBA::OBJECT_NOT_EXIST ();
    }
  char buffer[ACE_CDR::Fixed::MAX_STRING_SIZE];
  if (!this->value_.to_string (buffer, sizeof (buffer)))
    {
      throw CORBA::INTERNAL ();
    }
  return CORBA::string_dup (buffer);
}

CORBA::Boolean
TAO_DynFixed_i::set_value (char const* value)
{
  if (this->destroyed_)
    {
      throw CORBA::OBJECT_NOT_EXIST ();
    }
  if (!value)
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }
  CORBA::TypeCode_var tc = TAO_DynAnyFactory::strip_alias (this->type_.in ());
  CORBA::UShort const digits = tc->fixed_digits ();
  CORBA::UShort const scale = tc->fixed_scale ();
  std::string text (value);
  std::string::size_type first = 0;
  while (first < text.size () &&
         std::isspace (static_cast<unsigned char> (text[first])))
    {
      ++first;
    }
  std::string::size_type last = text.size ();
  while (last > first &&
         std::isspace (static_cast<unsigned char> (text[last - 1])))
    {
      --last;
    }
  text = text.substr (first, last - first);

  if (!text.empty () && (text[text.size () - 1] == 'd' ||
                         text[text.size () - 1] == 'D'))
    {
      text.erase (text.size () - 1);
    }
  if (text.empty ())
    {
      throw DynamicAny::DynAny::TypeMismatch ();
    }
  std::string sign;
  if (text[0] == '+' || text[0] == '-')
    {
      if (text[0] == '-')
        {
          sign = "-";
        }
      text.erase (0, 1);
    }
  std::string::size_type const point = text.find ('.');
  if (point != std::string::npos && text.find ('.', point + 1) != std::string::npos)
    {
      throw DynamicAny::DynAny::TypeMismatch ();
    }
  std::string integer = point == std::string::npos ? text : text.substr (0, point);
  std::string fraction = point == std::string::npos ? "" : text.substr (point + 1);
  if ((integer.empty () && fraction.empty ()) ||
      integer.find_first_not_of ("0123456789") != std::string::npos ||
      fraction.find_first_not_of ("0123456789") != std::string::npos)
    {
      throw DynamicAny::DynAny::TypeMismatch ();
    }
  CORBA::Boolean truncated = fraction.size () > scale;
  if (truncated)
    {
      fraction.resize (scale);
    }
  std::string significant_integer = integer;
  std::string::size_type const nonzero =
    significant_integer.find_first_not_of ('0');
  if (nonzero == std::string::npos)
    {
      significant_integer.clear ();
    }
  else
    {
      significant_integer.erase (0, nonzero);
    }
  CORBA::UShort const integer_digits = digits - scale;
  if (significant_integer.size () > integer_digits)
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }
  if (integer_digits == 0)
    {
      if (!significant_integer.empty ())
        {
          throw DynamicAny::DynAny::InvalidValue ();
        }
      integer.clear ();
    }
  else
    {
      if (integer.empty ())
        {
          integer = "0";
        }
      if (integer.size () > integer_digits)
        {
          integer.erase (0, integer.size () - integer_digits);
        }
      integer.insert (0, integer_digits - integer.size (), '0');
    }
  fraction.append (static_cast<std::size_t> (scale - fraction.size ()), '0');
  std::string canonical = sign + integer;
  if (scale)
    {
      canonical += '.';
      canonical += fraction;
    }
  this->value_ = ACE_CDR::Fixed::from_string (canonical.c_str ());
  return truncated ? 0 : 1;
}

void
TAO_DynFixed_i::from_any (CORBA::Any const& any)
{
  if (this->destroyed_)
    {
      throw CORBA::OBJECT_NOT_EXIST ();
    }
  CORBA::TypeCode_var tc = any.type ();
  if (!this->type_->equivalent (tc.in ()))
    {
      throw DynamicAny::DynAny::TypeMismatch ();
    }
  TAO_DynFixed_i temporary (this->allow_truncation_);
  temporary.type_ = this->type_;
  temporary.init (any);
  this->value_ = temporary.value_;
}

CORBA::Any *
TAO_DynFixed_i::to_any ()
{
  if (this->destroyed_)
    {
      throw CORBA::OBJECT_NOT_EXIST ();
    }
  TAO_OutputCDR output;
  if (!output.write_fixed (this->value_))
    {
      throw CORBA::MARSHAL ();
    }
  CORBA::Any *result = nullptr;
  ACE_NEW_THROW_EX (result, CORBA::Any, CORBA::NO_MEMORY ());
  TAO_InputCDR input (output);
  TAO::Unknown_IDL_Type *unknown = nullptr;
  ACE_NEW_THROW_EX (unknown,
                    TAO::Unknown_IDL_Type (this->type_.in (), input),
                    CORBA::NO_MEMORY ());
  result->replace (unknown);
  return result;
}

CORBA::Boolean
TAO_DynFixed_i::equal (DynamicAny::DynAny_ptr dyn_any)
{
  if (this->destroyed_)
    {
      throw CORBA::OBJECT_NOT_EXIST ();
    }
  CORBA::TypeCode_var tc = dyn_any->type ();
  if (!tc->equivalent (this->type_.in ()))
    {
      return false;
    }
  CORBA::Any_var any = dyn_any->to_any ();
  TAO_DynFixed_i temporary (this->allow_truncation_);
  temporary.init (any.in ());
  return this->value_.equal (temporary.value_);
}

void
TAO_DynFixed_i::destroy ()
{
  if (this->destroyed_)
    {
      throw CORBA::OBJECT_NOT_EXIST ();
    }
  if (!this->ref_to_component_ || this->container_is_destroying_)
    {
      this->destroyed_ = true;
    }
}

DynamicAny::DynAny_ptr
TAO_DynFixed_i::current_component ()
{
  if (this->destroyed_)
    {
      throw CORBA::OBJECT_NOT_EXIST ();
    }
  throw DynamicAny::DynAny::TypeMismatch ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
