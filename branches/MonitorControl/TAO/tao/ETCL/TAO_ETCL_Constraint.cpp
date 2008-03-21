// -*- C++ -*-
// $Id$

#include "ETCL_Constraint_Visitor.h"

#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"

#include "TAO_ETCL_Constraint.h"

#if ! defined (__ACE_INLINE__)
#include "TAO_ETCL_Constraint.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  const TAO_ETCL_Literal_Constraint & lit)
  : ETCL_Literal_Constraint (),
    any_ (0)
{
  this->copy (lit);
}

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::Any * any)
{
  CORBA::Any& any_ref = *any;
  CORBA::TypeCode_var type = any_ref.type ();
  CORBA::TCKind corba_type = CORBA::tk_null;

  try
    {
      corba_type = type->kind ();
    }
  catch (const CORBA::Exception&)
    {
      // @@ Seth: Don't know what else to do. Make sure we can tell
      // when this constructor fails.
      return;
    }

  this->type_ =
    TAO_ETCL_Literal_Constraint::comparable_type (type.in ());

  switch (this->type_)
    {
    case ACE_ETCL_SIGNED:
      this->op_.integer_ = 0;

      if (corba_type == CORBA::tk_short)
        {
          CORBA::Short sh;
          any_ref >>= sh;
          this->op_.integer_ = (CORBA::Long) sh;
        }
      else
        {
          any_ref >>= this->op_.integer_;
        }

      break;
    case ACE_ETCL_UNSIGNED:
      this->op_.uinteger_ = 0;

      if (corba_type == CORBA::tk_ushort)
        {
          CORBA::UShort sh;
          any_ref >>= sh;
          this->op_.uinteger_ = (CORBA::ULong) sh;
        }
      else if (corba_type == CORBA::tk_enum)
        {
          TAO::Any_Impl *impl = any->impl ();

          if (impl->encoded ())
            {
              TAO::Unknown_IDL_Type *unk =
                dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

              if (!unk)
                throw CORBA::INTERNAL ();

              // We don't want unk's rd_ptr to move, in case we are shared by
              // another Any, so we use this to copy the state, not the buffer.
              TAO_InputCDR for_reading (unk->_tao_get_cdr ());
              for_reading.read_ulong (this->op_.uinteger_);
            }
          else
            {
              TAO_OutputCDR out;
              impl->marshal_value (out);
              TAO_InputCDR in (out);
              in.read_ulong (this->op_.uinteger_);
            }
        }
      else
        {
          any_ref >>= this->op_.uinteger_;
        }

      break;
    case ACE_ETCL_DOUBLE:
      if (corba_type == CORBA::tk_float)
        {
          CORBA::Float fl;
          (*any) >>= fl;
          this->op_.double_ = (CORBA::Double) fl;
        }
      else
        {
          (*any) >>= this->op_.double_;
        }

      break;
    case ACE_ETCL_BOOLEAN:
      {
        CORBA::Any::to_boolean tmp (this->op_.bool_);
        (*any) >>= tmp;
      }
      break;
    case ACE_ETCL_STRING:
      {
        const char* s;
        any_ref >>= s;
        this->op_.str_ = CORBA::string_dup (s);
      }
      break;
    case ACE_ETCL_COMPONENT:
      this->any_ = any->impl ();
      this->any_->_add_ref ();
      break;
    }
}

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::ULong uinteger)
  : ETCL_Literal_Constraint (uinteger)
{
}

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::Long integer)
  : ETCL_Literal_Constraint (integer)
{
}

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::Boolean boolean)
  : ETCL_Literal_Constraint (boolean)
{
}

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::Double doub)
  : ETCL_Literal_Constraint (doub)
{
}

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  const char* str)
  : ETCL_Literal_Constraint (str)
{
}

TAO_ETCL_Literal_Constraint::~TAO_ETCL_Literal_Constraint (void)
{
  if (this->any_ != 0)
    {
      this->any_->_remove_ref ();
    }
}

void
TAO_ETCL_Literal_Constraint::operator= (const TAO_ETCL_Literal_Constraint& co)
{
  this->copy (co);
}

TAO_ETCL_Literal_Constraint::operator CORBA::Boolean (void) const
{
  return this->ETCL_Literal_Constraint::operator ACE_CDR::Boolean ();
}

TAO_ETCL_Literal_Constraint::operator CORBA::ULong (void) const
{
  switch (this->type_)
  {
    case ACE_ETCL_COMPONENT:
      {
        CORBA::ULong retval = 0;
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

TAO_ETCL_Literal_Constraint::operator CORBA::Long (void) const
{
  switch (this->type_)
  {
    case ACE_ETCL_COMPONENT:
      {
        CORBA::Long retval = 0;
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

TAO_ETCL_Literal_Constraint::operator CORBA::Double (void) const
{
  switch (this->type_)
  {
    case ACE_ETCL_COMPONENT:
      {
        CORBA::Double retval = 0.0;
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

TAO_ETCL_Literal_Constraint::operator const char* (void) const
{
  switch (this->type_)
  {
    case ACE_ETCL_COMPONENT:
      {
        const char *retval = 0;
        CORBA::Any tmp;
        tmp.replace (this->any_);
        this->any_->_add_ref ();
        tmp >>= retval;
        return retval;
      }
    default:
      return this->ETCL_Literal_Constraint::operator const char * ();
  }
}

TAO_ETCL_Literal_Constraint::operator TAO::Any_Impl* (void) const
{
  return (this->type_ == ACE_ETCL_COMPONENT) ? this->any_ : 0;
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
  catch (const CORBA::Exception&)
  {
    return return_value;
  }
  // Since this is a "top level try block, no need to check again.

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
TAO_ETCL_Literal_Constraint::operator+ (const TAO_ETCL_Literal_Constraint & rhs)
{
  Literal_Type widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case ACE_ETCL_DOUBLE:
      {
        CORBA::Double result = (CORBA::Double) *this + (CORBA::Double) rhs;
        return TAO_ETCL_Literal_Constraint ((CORBA::Double) result);
      }
    case ACE_ETCL_INTEGER:
    case ACE_ETCL_SIGNED:
      {
        CORBA::Long result = (CORBA::Long) *this + (CORBA::Long) rhs;
        return TAO_ETCL_Literal_Constraint ((CORBA::Long) result);
      }
    case ACE_ETCL_UNSIGNED:
      {
        CORBA::ULong result = (CORBA::ULong) *this + (CORBA::ULong) rhs;
        return TAO_ETCL_Literal_Constraint ((CORBA::ULong) result);
      }
    default:
      return TAO_ETCL_Literal_Constraint ((CORBA::Long) 0);
    }
}

TAO_ETCL_Literal_Constraint
TAO_ETCL_Literal_Constraint::operator- (const TAO_ETCL_Literal_Constraint & rhs)
{
  Literal_Type widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case ACE_ETCL_DOUBLE:
      {
        CORBA::Double result = (CORBA::Double) *this - (CORBA::Double) rhs;
        return TAO_ETCL_Literal_Constraint ((CORBA::Double) result);
      }
    case ACE_ETCL_INTEGER:
    case ACE_ETCL_SIGNED:
      {
        CORBA::Long result = (CORBA::Long) *this - (CORBA::Long) rhs;
        return TAO_ETCL_Literal_Constraint ((CORBA::Long) result);
      }
    case ACE_ETCL_UNSIGNED:
      {
        CORBA::ULong result = (CORBA::ULong) *this - (CORBA::ULong) rhs;
        return TAO_ETCL_Literal_Constraint ((CORBA::ULong) result);
      }
    default:
      return TAO_ETCL_Literal_Constraint ((CORBA::Long) 0);
    }
}

TAO_ETCL_Literal_Constraint
TAO_ETCL_Literal_Constraint::operator* (const TAO_ETCL_Literal_Constraint & rhs)
{
  Literal_Type widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case ACE_ETCL_DOUBLE:
      {
        CORBA::Double result = (CORBA::Double) *this * (CORBA::Double) rhs;
        return TAO_ETCL_Literal_Constraint ((CORBA::Double) result);
      }
    case ACE_ETCL_INTEGER:
    case ACE_ETCL_SIGNED:
      {
        CORBA::Long result = (CORBA::Long) *this * (CORBA::Long) rhs;
        return TAO_ETCL_Literal_Constraint ((CORBA::Long) result);
      }
    case ACE_ETCL_UNSIGNED:
      {
        CORBA::ULong result = (CORBA::ULong) *this * (CORBA::ULong) rhs;
        return TAO_ETCL_Literal_Constraint ((CORBA::ULong) result);
      }
    default:
      return TAO_ETCL_Literal_Constraint ((CORBA::Long) 0);
    }
}

TAO_ETCL_Literal_Constraint
TAO_ETCL_Literal_Constraint::operator/ (const TAO_ETCL_Literal_Constraint & rhs)
{
  Literal_Type widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case ACE_ETCL_DOUBLE:
      {
        CORBA::Double result = (CORBA::Double) *this / (CORBA::Double) rhs;
        return TAO_ETCL_Literal_Constraint ((CORBA::Double) result);
      }
    case ACE_ETCL_INTEGER:
    case ACE_ETCL_SIGNED:
      {
        CORBA::Long result = (CORBA::Long) *this / (CORBA::Long) rhs;
        return TAO_ETCL_Literal_Constraint ((CORBA::Long) result);
      }
    case ACE_ETCL_UNSIGNED:
      {
        CORBA::ULong result = (CORBA::ULong) *this / (CORBA::ULong) rhs;
        return TAO_ETCL_Literal_Constraint ((CORBA::ULong) result);
      }
    default:
      return TAO_ETCL_Literal_Constraint ((CORBA::Long) 0);
    }
}

TAO_ETCL_Literal_Constraint
TAO_ETCL_Literal_Constraint::operator- (void)
{
  switch (this->type_)
  {
    case ACE_ETCL_DOUBLE:
      return TAO_ETCL_Literal_Constraint (- this->op_.double_);
    case ACE_ETCL_INTEGER:
    case ACE_ETCL_SIGNED:
      return TAO_ETCL_Literal_Constraint (- this->op_.integer_);
    case ACE_ETCL_UNSIGNED:
      return TAO_ETCL_Literal_Constraint (- (CORBA::Long) this->op_.uinteger_);
    default:
      return TAO_ETCL_Literal_Constraint ((CORBA::Long) 0);
  }
}

Literal_Type
TAO_ETCL_Literal_Constraint::widest_type (
  const ETCL_Literal_Constraint & rhs)
{
  Literal_Type rhs_type = rhs.expr_type ();
  Literal_Type return_value = rhs_type;

  if (rhs_type == ACE_ETCL_COMPONENT)
    {
      const TAO_ETCL_Literal_Constraint& actual =
        dynamic_cast<const TAO_ETCL_Literal_Constraint&> (rhs);
      CORBA::TypeCode_var tc = actual.any_->type ();
      rhs_type = this->comparable_type (tc.in ());
      return return_value;
    }
    
  return this->ETCL_Literal_Constraint::widest_type (rhs);
}

void
TAO_ETCL_Literal_Constraint::copy (const TAO_ETCL_Literal_Constraint &lit)
{
  // Save the older type_ before assignment.
  unsigned long prev_type = this->type_;

  this->type_ = lit.type_;

  switch (this->type_)
  {
  case ACE_ETCL_COMPONENT:
    if (prev_type == ACE_ETCL_COMPONENT && this->any_ != 0)
      {
        this->any_->_remove_ref ();
      }
      
    this->any_ = lit.any_;
    this->any_->_add_ref ();
    break;
  default:
    this->ETCL_Literal_Constraint::copy (lit);
    break;
  }
}

// ****************************************************************

TAO_ETCL_Union_Value::~TAO_ETCL_Union_Value (void)
{
  delete this->string_;
  delete this->integer_;
}

int
TAO_ETCL_Union_Value::accept (ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_union_value (this);
}

// ****************************************************************

TAO_ETCL_Union_Pos::~TAO_ETCL_Union_Pos (void)
{
  delete this->component_;
  delete this->union_value_;
}

int
TAO_ETCL_Union_Pos::accept (ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_union_pos (this);
}

// ****************************************************************

TAO_ETCL_Component_Pos::~TAO_ETCL_Component_Pos (void)
{
  delete this->component_;
  delete this->integer_;
}

int
TAO_ETCL_Component_Pos::accept (ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_component_pos (this);
}

// ****************************************************************

TAO_ETCL_Component_Assoc::~TAO_ETCL_Component_Assoc (void)
{
  delete this->component_;
  delete this->identifier_;
}

int
TAO_ETCL_Component_Assoc::accept (ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_component_assoc (this);
}

// ****************************************************************

TAO_ETCL_Component_Array::~TAO_ETCL_Component_Array (void)
{
  delete this->component_;
  delete this->integer_;
}

int
TAO_ETCL_Component_Array::accept (ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_component_array (this);
}

// ****************************************************************

TAO_ETCL_Special::~TAO_ETCL_Special (void)
{
}

int
TAO_ETCL_Special::accept (ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_special (this);
}

// ****************************************************************

TAO_ETCL_Component::~TAO_ETCL_Component (void)
{
  delete this->component_;
  delete this->identifier_;
}

int
TAO_ETCL_Component::accept (ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_component (this);
}

// ****************************************************************

TAO_ETCL_Dot::~TAO_ETCL_Dot (void)
{
  delete this->component_;
}

int
TAO_ETCL_Dot::accept (ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_dot (this);
}

// ****************************************************************

TAO_ETCL_Eval::~TAO_ETCL_Eval (void)
{
  delete this->component_;
}

int
TAO_ETCL_Eval::accept (ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_eval (this);
}

// ****************************************************************

TAO_ETCL_Default::~TAO_ETCL_Default (void)
{
  delete this->component_;
}

int
TAO_ETCL_Default::accept (ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_default (this);
}

// ****************************************************************

TAO_ETCL_Exist::~TAO_ETCL_Exist (void)
{
  delete this->component_;
}

int
TAO_ETCL_Exist::accept (ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_exist (this);
}

// ****************************************************************

TAO_ETCL_Unary_Expr::~TAO_ETCL_Unary_Expr (void)
{
}

// ****************************************************************

TAO_ETCL_Binary_Expr::~TAO_ETCL_Binary_Expr (void)
{
}

// ****************************************************************

TAO_ETCL_Preference::~TAO_ETCL_Preference (void)
{
  delete this->subexpr_;
}

int
TAO_ETCL_Preference::accept (ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_preference (this);
}

TAO_END_VERSIONED_NAMESPACE_DECL
