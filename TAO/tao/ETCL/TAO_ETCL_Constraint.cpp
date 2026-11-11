// -*- C++ -*-
#include "ace/ETCL/ETCL_Constraint_Visitor.h"

#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"

#include "tao/ETCL/TAO_ETCL_Constraint.h"

#include <utility>

#if ! defined (__ACE_INLINE__)
#include "tao/ETCL/TAO_ETCL_Constraint.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  TAO_ETCL_Literal_Constraint const &lit)
  : ETCL_Literal_Constraint ()
{
  this->copy (lit);
}

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (CORBA::Any *any)
{
  CORBA::Any const &any_ref = *any;
  CORBA::TypeCode_var const type = any_ref.type ();
  CORBA::TCKind corba_type = CORBA::tk_null;
  Literal_Type const literal_type =
    TAO_ETCL_Literal_Constraint::comparable_type (type.in ());

  try
    {
      corba_type = type->kind ();
    }
  catch (CORBA::Exception const &)
    {
      // @@ Seth: Don't know what else to do. Make sure we can tell
      // when this constructor fails.
      return;
    }

  switch (literal_type)
    {
    case ACE_ETCL_SIGNED:
      if (corba_type == CORBA::tk_short)
        {
          CORBA::Short sh {};
          any_ref >>= sh;
          this->op_ = static_cast<CORBA::Long> (sh);
        }
      else
        {
          this->op_.emplace<CORBA::Long> ();
          any_ref >>= std::get<CORBA::Long> (this->op_);
        }

      break;
    case ACE_ETCL_UNSIGNED:
      if (corba_type == CORBA::tk_ushort)
        {
          CORBA::UShort sh {};
          any_ref >>= sh;
          this->op_ = static_cast<CORBA::ULong> (sh);
        }
      else if (corba_type == CORBA::tk_enum)
        {
          TAO::Any_Impl * const impl = any->impl ();

          if (impl->encoded ())
            {
              TAO::Unknown_IDL_Type * const unk =
                dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

              if (unk == nullptr)
                {
                  throw CORBA::INTERNAL ();
                }

              // We don't want unk's rd_ptr to move, in case we are shared by
              // another Any, so we use this to copy the state, not the buffer.
              TAO_InputCDR for_reading (unk->_tao_get_cdr ());
              CORBA::ULong uinteger {};
              for_reading.read_ulong (uinteger);
              this->op_ = uinteger;
            }
          else
            {
              TAO_OutputCDR out;
              impl->marshal_value (out);
              TAO_InputCDR in (out);
              CORBA::ULong uinteger {};
              in.read_ulong (uinteger);
              this->op_ = uinteger;
            }
        }
      else
        {
          this->op_.emplace<CORBA::ULong> ();
          any_ref >>= std::get<CORBA::ULong> (this->op_);
        }

      break;
    case ACE_ETCL_DOUBLE:
      if (corba_type == CORBA::tk_float)
        {
          CORBA::Float fl {};
          (*any) >>= fl;
          this->op_ = static_cast<CORBA::Double> (fl);
        }
      else
        {
          this->op_.emplace<CORBA::Double> ();
          (*any) >>= std::get<CORBA::Double> (this->op_);
        }

      break;
    case ACE_ETCL_BOOLEAN:
      {
        CORBA::Boolean boolean {};
        CORBA::Any::to_boolean tmp (boolean);
        (*any) >>= tmp;
        this->op_ = boolean;
      }
      break;
    case ACE_ETCL_STRING:
      {
        char const *s {};
        any_ref >>= s;
        this->op_ = String_Owner (CORBA::string_dup (s));
      }
      break;
    case ACE_ETCL_COMPONENT:
      this->any_ = any->impl ();
      this->any_->_add_ref ();
      break;
    }
}

TAO_ETCL_Literal_Constraint::~TAO_ETCL_Literal_Constraint ()
{
  if (this->any_ != nullptr)
    {
      this->any_->_remove_ref ();
    }
}

void
TAO_ETCL_Literal_Constraint::operator= (TAO_ETCL_Literal_Constraint const &co)
{
  this->copy (co);
}

TAO_ETCL_Literal_Constraint::operator CORBA::Boolean () const
{
  return this->ETCL_Literal_Constraint::operator ACE_CDR::Boolean ();
}

TAO_ETCL_Literal_Constraint::operator CORBA::ULong () const
{
  switch (this->expr_type ())
  {
    case ACE_ETCL_COMPONENT:
      {
        CORBA::ULong retval {};
        CORBA::Any tmp;
        tmp.replace (this->any_);
        this->any_->_add_ref ();
        tmp >>= retval;
        return retval;
      }
    default:
      return this->ETCL_Literal_Constraint::operator ACE_CDR::ULong ();
  }
}

TAO_ETCL_Literal_Constraint::operator CORBA::Long () const
{
  switch (this->expr_type ())
  {
    case ACE_ETCL_COMPONENT:
      {
        CORBA::Long retval {};
        CORBA::Any tmp;
        tmp.replace (this->any_);
        this->any_->_add_ref ();
        tmp >>= retval;
        return retval;
      }
    default:
      return this->ETCL_Literal_Constraint::operator ACE_CDR::Long ();
  }
}

TAO_ETCL_Literal_Constraint::operator CORBA::Double () const
{
  switch (this->expr_type ())
  {
    case ACE_ETCL_COMPONENT:
      {
        CORBA::Double retval {};
        CORBA::Any tmp;
        tmp.replace (this->any_);
        this->any_->_add_ref ();
        tmp >>= retval;
        return retval;
      }
    default:
      return this->ETCL_Literal_Constraint::operator ACE_CDR::Double ();
  }
}

TAO_ETCL_Literal_Constraint::operator char const* () const
{
  switch (this->expr_type ())
  {
    case ACE_ETCL_COMPONENT:
      {
        char const* retval {};
        CORBA::Any tmp;
        tmp.replace (this->any_);
        this->any_->_add_ref ();
        tmp >>= retval;
        return retval;
      }
    default:
      return this->ETCL_Literal_Constraint::operator char const* ();
  }
}

TAO_ETCL_Literal_Constraint::operator TAO::Any_Impl* () const
{
  return (this->expr_type () == ACE_ETCL_COMPONENT) ? this->any_ : nullptr;
}

Literal_Type
TAO_ETCL_Literal_Constraint::expr_type () const
{
  return this->any_ != nullptr
    ? static_cast<Literal_Type> (ACE_ETCL_COMPONENT)
    : ETCL_Literal_Constraint::expr_type ();
}

Literal_Type
TAO_ETCL_Literal_Constraint::comparable_type (CORBA::TypeCode_ptr type)
{
  // Convert a CORBA::TCKind into a Literal_Type
  Literal_Type return_value = ACE_ETCL_UNKNOWN;
  CORBA::TCKind kind = CORBA::tk_null;

  try
  {
    kind = type->kind ();
    CORBA::TypeCode_var tmp = CORBA::TypeCode::_duplicate (type);

    while (kind == CORBA::tk_alias)
      {
        tmp = tmp->content_type ();

        kind = tmp->kind ();
      }
  }
  catch (CORBA::Exception const &)
  {
    return return_value;
  }
  // Since this is a top level try block, no need to check again.

  switch (kind)
    {
    case CORBA::tk_ushort:
    case CORBA::tk_ulong:
    case CORBA::tk_enum:
    case CORBA::tk_ulonglong:
      return_value = ACE_ETCL_UNSIGNED;
      break;
    case CORBA::tk_short:
    case CORBA::tk_long:
    case CORBA::tk_longlong:
      return_value = ACE_ETCL_SIGNED;
      break;
    case CORBA::tk_boolean:
      return_value = ACE_ETCL_BOOLEAN;
      break;
    case CORBA::tk_float:
    case CORBA::tk_double:
      return_value = ACE_ETCL_DOUBLE;
      break;
    case CORBA::tk_string:
      return_value = ACE_ETCL_STRING;
      break;
    default:
      return_value = ACE_ETCL_COMPONENT;
      break;
    }

  return return_value;
}

TAO_ETCL_Literal_Constraint
TAO_ETCL_Literal_Constraint::operator+ (TAO_ETCL_Literal_Constraint const &rhs)
{
  Literal_Type const widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case ACE_ETCL_DOUBLE:
      {
        CORBA::Double const result = static_cast<CORBA::Double> (*this)
          + static_cast<CORBA::Double> (rhs);
        return TAO_ETCL_Literal_Constraint (result);
      }
    case ACE_ETCL_INTEGER:
    case ACE_ETCL_SIGNED:
      {
        CORBA::Long const result = static_cast<CORBA::Long> (*this)
          + static_cast<CORBA::Long> (rhs);
        return TAO_ETCL_Literal_Constraint (result);
      }
    case ACE_ETCL_UNSIGNED:
      {
        CORBA::ULong const result = static_cast<CORBA::ULong> (*this)
          + static_cast<CORBA::ULong> (rhs);
        return TAO_ETCL_Literal_Constraint (result);
      }
    default:
      return TAO_ETCL_Literal_Constraint (CORBA::Long {});
    }
}

TAO_ETCL_Literal_Constraint
TAO_ETCL_Literal_Constraint::operator- (TAO_ETCL_Literal_Constraint const &rhs)
{
  Literal_Type const widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case ACE_ETCL_DOUBLE:
      {
        CORBA::Double const result = static_cast<CORBA::Double> (*this)
          - static_cast<CORBA::Double> (rhs);
        return TAO_ETCL_Literal_Constraint (result);
      }
    case ACE_ETCL_INTEGER:
    case ACE_ETCL_SIGNED:
      {
        CORBA::Long const result = static_cast<CORBA::Long> (*this)
          - static_cast<CORBA::Long> (rhs);
        return TAO_ETCL_Literal_Constraint (result);
      }
    case ACE_ETCL_UNSIGNED:
      {
        CORBA::ULong const result = static_cast<CORBA::ULong> (*this)
          - static_cast<CORBA::ULong> (rhs);
        return TAO_ETCL_Literal_Constraint (result);
      }
    default:
      return TAO_ETCL_Literal_Constraint (CORBA::Long {});
    }
}

TAO_ETCL_Literal_Constraint
TAO_ETCL_Literal_Constraint::operator* (TAO_ETCL_Literal_Constraint const &rhs)
{
  Literal_Type const widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case ACE_ETCL_DOUBLE:
      {
        CORBA::Double const result = static_cast<CORBA::Double> (*this)
          * static_cast<CORBA::Double> (rhs);
        return TAO_ETCL_Literal_Constraint (result);
      }
    case ACE_ETCL_INTEGER:
    case ACE_ETCL_SIGNED:
      {
        CORBA::Long const result = static_cast<CORBA::Long> (*this)
          * static_cast<CORBA::Long> (rhs);
        return TAO_ETCL_Literal_Constraint (result);
      }
    case ACE_ETCL_UNSIGNED:
      {
        CORBA::ULong const result = static_cast<CORBA::ULong> (*this)
          * static_cast<CORBA::ULong> (rhs);
        return TAO_ETCL_Literal_Constraint (result);
      }
    default:
      return TAO_ETCL_Literal_Constraint (CORBA::Long {});
    }
}

TAO_ETCL_Literal_Constraint
TAO_ETCL_Literal_Constraint::operator/ (TAO_ETCL_Literal_Constraint const &rhs)
{
  Literal_Type const widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case ACE_ETCL_DOUBLE:
      {
        if (ACE::is_equal (static_cast<ACE_CDR::Double> (rhs), 0.0))
          return TAO_ETCL_Literal_Constraint (ACE_CDR::Double {});

        CORBA::Double const result = static_cast<CORBA::Double> (*this)
          / static_cast<CORBA::Double> (rhs);
        return TAO_ETCL_Literal_Constraint (result);
      }
    case ACE_ETCL_INTEGER:
    case ACE_ETCL_SIGNED:
      {
        if (static_cast<ACE_CDR::Long> (rhs) == 0)
          return TAO_ETCL_Literal_Constraint (ACE_CDR::Long {});

        CORBA::Long const result = static_cast<CORBA::Long> (*this)
          / static_cast<CORBA::Long> (rhs);
        return TAO_ETCL_Literal_Constraint (result);
      }
    case ACE_ETCL_UNSIGNED:
      {
        if (static_cast<ACE_CDR::ULong> (rhs) == 0)
          return TAO_ETCL_Literal_Constraint (ACE_CDR::ULong {});

        CORBA::ULong const result = static_cast<CORBA::ULong> (*this)
          / static_cast<CORBA::ULong> (rhs);
        return TAO_ETCL_Literal_Constraint (result);
      }
    default:
      return TAO_ETCL_Literal_Constraint (CORBA::Long {});
    }
}

TAO_ETCL_Literal_Constraint
TAO_ETCL_Literal_Constraint::operator- ()
{
  switch (this->expr_type ())
  {
    case ACE_ETCL_DOUBLE:
      return TAO_ETCL_Literal_Constraint (- std::get<CORBA::Double> (this->op_));
    case ACE_ETCL_INTEGER:
    case ACE_ETCL_SIGNED:
      return TAO_ETCL_Literal_Constraint (- std::get<CORBA::Long> (this->op_));
    case ACE_ETCL_UNSIGNED:
      return TAO_ETCL_Literal_Constraint (
        - static_cast<CORBA::Long> (std::get<CORBA::ULong> (this->op_)));
    default:
      return TAO_ETCL_Literal_Constraint (CORBA::Long {});
  }
}

Literal_Type
TAO_ETCL_Literal_Constraint::widest_type (
  ETCL_Literal_Constraint const &rhs)
{
  Literal_Type rhs_type = rhs.expr_type ();
  Literal_Type return_value = rhs_type;

  if (rhs_type == ACE_ETCL_COMPONENT)
    {
      TAO_ETCL_Literal_Constraint const &actual =
        dynamic_cast<TAO_ETCL_Literal_Constraint const &> (rhs);
      CORBA::TypeCode_var tc = actual.any_->type ();
      rhs_type = TAO_ETCL_Literal_Constraint::comparable_type (tc.in ());
      return return_value;
    }

  return this->ETCL_Literal_Constraint::widest_type (rhs);
}

void
TAO_ETCL_Literal_Constraint::copy (TAO_ETCL_Literal_Constraint const &lit)
{
  Value value = ETCL_Literal_Constraint::copy_value (lit.op_);
  if (lit.any_ != nullptr)
    lit.any_->_add_ref ();
  if (this->any_ != nullptr)
    this->any_->_remove_ref ();
  this->op_ = std::move (value);
  this->any_ = lit.any_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
