// -*- C++ -*-
// $Id$

#include "ETCL_Constraint.h"
#include "ETCL_Constraint_Visitor.h"
#include "ETCL_y.h"
#include "tao/Any.h"
#include "tao/Managed_Types.h"

#if ! defined (__ACE_INLINE__)
#include "ETCL_Constraint.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ETCL, ETCL_Constraint, "$Id$")

TAO_ETCL_Constraint::TAO_ETCL_Constraint (void)
{
}

TAO_ETCL_Constraint::~TAO_ETCL_Constraint (void)
{
}

// ****************************************************************

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
    const TAO_ETCL_Literal_Constraint & lit
  )
  : TAO_ETCL_Constraint ()
{
  this->copy (lit);
}


TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (CORBA::Any * any)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  CORBA::Any& any_ref = *any;
  CORBA::TypeCode_var type = any_ref.type ();
  // @@ No where to throw exception back.
  CORBA::TCKind corba_type = CORBA::tk_null;
  ACE_TRY
    {
      corba_type = type->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // @@ Seth: Don't know what else to do.  Make sure we can tell
      // when this constructor fails.
      return;
    }
  ACE_ENDTRY;

  this->type_ = TAO_ETCL_Literal_Constraint::comparable_type (type.in ());
  switch (this->type_)
    {
    case TAO_ETCL_SIGNED:
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
    case TAO_ETCL_UNSIGNED:
      this->op_.uinteger_ = 0;

      if (corba_type == CORBA::tk_ushort)
	      {
	        CORBA::UShort sh;
	        any_ref >>= sh;
	        this->op_.uinteger_ = (CORBA::ULong) sh;
	      }
      else
        {
	        any_ref >>= this->op_.uinteger_;
        }

      break;
    case TAO_ETCL_DOUBLE:
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
    case TAO_ETCL_BOOLEAN:
      {
	      CORBA_Any::to_boolean tmp (this->op_.bool_);
        (*any) >>= tmp;
      }
      break;
    case TAO_ETCL_STRING:
      {
	      const char* s;
	      any_ref >>= s;
	      this->op_.str_ = CORBA::string_dup (s);
      }
      break;
    case TAO_ETCL_COMPONENT:
      this->op_.any_ = any;
      break;
    }
}

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
    CORBA::ULong uinteger
  )
  : type_ (TAO_ETCL_UNSIGNED)
{
  this->op_.uinteger_ = uinteger;
}

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
    CORBA::Long integer
  )
  : type_ (TAO_ETCL_SIGNED)
{
  this->op_.integer_ = integer;
}

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
    CORBA::Boolean boolean
  )
  : type_ (TAO_ETCL_BOOLEAN)
{
  this->op_.bool_ = boolean;
}

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
    CORBA::Double doub
  )
  : type_ (TAO_ETCL_DOUBLE)
{
  this->op_.double_ = doub;
}

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
    const char* str
  )
  : type_ (TAO_ETCL_STRING)
{
  this->op_.str_ = CORBA::string_dup (str);
}

TAO_ETCL_Literal_Constraint::~TAO_ETCL_Literal_Constraint (void)
{
  if (this->type_ == TAO_ETCL_STRING)
    {
      CORBA::string_free (this->op_.str_);
    }
}

int
TAO_ETCL_Literal_Constraint::accept (TAO_ETCL_Constraint_Visitor* visitor)
{
  return visitor->visit_literal (this);
}

TAO_Literal_Type
TAO_ETCL_Literal_Constraint::expr_type (void) const
{
  return this->type_;
}

void
TAO_ETCL_Literal_Constraint::operator= (const TAO_ETCL_Literal_Constraint& co)
{
  this->copy (co);
}

TAO_ETCL_Literal_Constraint::operator CORBA::Boolean (void) const
{
  return (this->type_ == TAO_ETCL_BOOLEAN) ? this->op_.bool_ : 0;
}

TAO_ETCL_Literal_Constraint::operator CORBA::ULong (void) const
{
  switch (this->type_)
  {
    case TAO_ETCL_UNSIGNED:
      return this->op_.uinteger_;
    case TAO_ETCL_SIGNED:
    case TAO_ETCL_INTEGER:
      return
        (this->op_.integer_ > 0) ? (CORBA::ULong) this->op_.integer_ : 0;
    case TAO_ETCL_DOUBLE:
      return
        (this->op_.double_ > 0) ?
        ((this->op_.double_ > ACE_UINT32_MAX) ?
         ACE_UINT32_MAX :
         (CORBA::ULong) this->op_.double_)
        : 0;
    case TAO_ETCL_COMPONENT:
      {
        CORBA::ULong retval = 0;
        *this->op_.any_ >>= retval;
        return retval;
      }
    default:
      return 0;
  }
}

TAO_ETCL_Literal_Constraint::operator CORBA::Long (void) const
{
  switch (this->type_)
  {
    case TAO_ETCL_SIGNED:
    case TAO_ETCL_INTEGER:
      return this->op_.integer_;
    case TAO_ETCL_UNSIGNED:
      return
        (this->op_.uinteger_ > (CORBA::ULong) ACE_INT32_MAX) ?
        ACE_INT32_MAX : (CORBA::Long) this->op_.uinteger_;
    case TAO_ETCL_DOUBLE:
      return
        (this->op_.double_ > 0) ?
         ((this->op_.double_ > ACE_INT32_MAX) ?
          ACE_INT32_MAX :
          (CORBA::Long) this->op_.double_) :
          ((this->op_.double_ < ACE_INT32_MIN) ?
           ACE_INT32_MIN :
           (CORBA::Long) this->op_.double_);
    case TAO_ETCL_COMPONENT:
      {
        CORBA::Long retval = 0;
        *this->op_.any_ >>= retval;
        return retval;
      }
    default:
      return 0;
  }
}

TAO_ETCL_Literal_Constraint::operator CORBA::Double (void) const
{
  switch (this->type_)
  {
    case TAO_ETCL_DOUBLE:
      return this->op_.double_;
    case TAO_ETCL_SIGNED:
    case TAO_ETCL_INTEGER:
      return (CORBA::Double) this->op_.integer_;
    case TAO_ETCL_UNSIGNED:
      return (CORBA::Double) this->op_.uinteger_;
    case TAO_ETCL_COMPONENT:
      {
        CORBA::Double retval = 0.0;
        *this->op_.any_ >>= retval;
        return retval;
      }
    default:
      return 0.0;
  }
}

TAO_ETCL_Literal_Constraint::operator const char* (void) const
{
  switch (this->type_)
  {
    case TAO_ETCL_STRING:
      return this->op_.str_;
    case TAO_ETCL_COMPONENT:
      {
        char *retval = 0;
        *this->op_.any_ >>= retval;
        return retval;
      }
    default:
      return 0;
  }
}

TAO_ETCL_Literal_Constraint::operator const CORBA::Any* (void) const
{
  return (this->type_ == TAO_ETCL_COMPONENT) ? this->op_.any_ : 0;
}

TAO_Literal_Type
TAO_ETCL_Literal_Constraint::comparable_type (CORBA::TypeCode_ptr type)
{
  // Convert a CORBA::TCKind into a TAO_Literal_Type
  ACE_DECLARE_NEW_CORBA_ENV;
  TAO_Literal_Type return_value = TAO_UNKNOWN;
  CORBA::TCKind kind = CORBA::tk_null;
  ACE_TRY
    {
      kind = type->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_var tmp = CORBA::TypeCode::_duplicate (type);

      while (kind == CORBA::tk_alias)
        {
          tmp = tmp->content_type (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          kind = tmp->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      return return_value;
    }
  ACE_ENDTRY;
  // Since this is a "top level try block, no need to check again.

  switch (kind)
    {
    case CORBA::tk_ushort:
    case CORBA::tk_ulong:
    case CORBA::tk_ulonglong:
      return_value = TAO_ETCL_UNSIGNED;
      break;
    case CORBA::tk_short:
    case CORBA::tk_long:
    case CORBA::tk_longlong:
      return_value = TAO_ETCL_SIGNED;
      break;
    case CORBA::tk_boolean:
      return_value = TAO_ETCL_BOOLEAN;
      break;
    case CORBA::tk_float:
    case CORBA::tk_double:
      return_value = TAO_ETCL_DOUBLE;
      break;
    case CORBA::tk_string:
      return_value = TAO_ETCL_STRING;
      break;
    default:
      return_value = TAO_ETCL_COMPONENT;
      break;
    }

  return return_value;
}

CORBA::Boolean
TAO_ETCL_Literal_Constraint::operator== (const TAO_ETCL_Literal_Constraint & rhs)
{
  int return_value = 0;
  TAO_Literal_Type widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case TAO_ETCL_STRING:
      return_value = (ACE_OS::strcmp ((const char*) *this, (const char*) rhs) == 0);
      break;
    case TAO_ETCL_DOUBLE:
      return_value = (CORBA::Double) *this == (CORBA::Double) rhs;
      break;
    case TAO_ETCL_INTEGER:
    case TAO_ETCL_SIGNED:
      return_value = (CORBA::Long) *this == (CORBA::Long) rhs;
      break;
    case TAO_ETCL_UNSIGNED:
      return_value = (CORBA::ULong) *this == (CORBA::ULong) rhs;
      break;
    case TAO_ETCL_BOOLEAN:
      return_value = (CORBA::Boolean) *this == (CORBA::Boolean) rhs;
      break;
    default:
     break;
    }

  return return_value;
}


CORBA::Boolean
TAO_ETCL_Literal_Constraint::operator!= (const TAO_ETCL_Literal_Constraint & rhs)
{
  int return_value = 0;
  TAO_Literal_Type widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case TAO_ETCL_STRING:
      return_value = (ACE_OS::strcmp ((const char*) *this, (const char*) rhs) != 0);
      break;
    case TAO_ETCL_DOUBLE:
      return_value = (CORBA::Double) *this != (CORBA::Double) rhs;
      break;
    case TAO_ETCL_INTEGER:
    case TAO_ETCL_SIGNED:
      return_value = (CORBA::Long) *this != (CORBA::Long) rhs;
      break;
    case TAO_ETCL_UNSIGNED:
      return_value = (CORBA::ULong) *this != (CORBA::ULong) rhs;
      break;
    case TAO_ETCL_BOOLEAN:
      return_value = (CORBA::Boolean) *this != (CORBA::Boolean) rhs;
      break;
    default:
      break;
    }

  return return_value;
}

CORBA::Boolean
TAO_ETCL_Literal_Constraint::operator< (const TAO_ETCL_Literal_Constraint & rhs)
{
  int return_value = 0;
  TAO_Literal_Type widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case TAO_ETCL_STRING:
      return_value = (ACE_OS::strcmp ((const char*) *this, (const char*) rhs) < 0);
      break;
    case TAO_ETCL_DOUBLE:
      return_value = (CORBA::Double) *this < (CORBA::Double) rhs;
      break;
    case TAO_ETCL_INTEGER:
    case TAO_ETCL_SIGNED:
      return_value = (CORBA::Long) *this < (CORBA::Long) rhs;
      break;
    case TAO_ETCL_UNSIGNED:
      return_value = (CORBA::ULong) *this < (CORBA::ULong) rhs;
      break;
    case TAO_ETCL_BOOLEAN:
      return_value = (CORBA::Boolean) *this < (CORBA::Boolean) rhs;
      break;
    default:
      break;
    }

  return return_value;
}

CORBA::Boolean
TAO_ETCL_Literal_Constraint::operator<= (const TAO_ETCL_Literal_Constraint & rhs)
{
  int return_value = 0;
  TAO_Literal_Type widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case TAO_ETCL_STRING:
      return_value = (ACE_OS::strcmp ((const char*) *this, (const char*) rhs) <= 0);
      break;
    case TAO_ETCL_DOUBLE:
      return_value = (CORBA::Double) *this <= (CORBA::Double) rhs;
      break;
    case TAO_ETCL_INTEGER:
    case TAO_ETCL_SIGNED:
      return_value = (CORBA::Long) *this <= (CORBA::Long) rhs;
      break;
    case TAO_ETCL_UNSIGNED:
      return_value = (CORBA::ULong) *this <= (CORBA::ULong) rhs;
      break;
    default:
      break;
    }

  return return_value;
}

CORBA::Boolean
TAO_ETCL_Literal_Constraint::operator> (const TAO_ETCL_Literal_Constraint & rhs)
{
  int return_value = 0;
  TAO_Literal_Type widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case TAO_ETCL_STRING:
      return_value = (ACE_OS::strcmp ((const char*) *this, (const char*) rhs) > 0);
      break;
    case TAO_ETCL_DOUBLE:
      return_value = (CORBA::Double) *this > (CORBA::Double) rhs;
      break;
    case TAO_ETCL_INTEGER:
    case TAO_ETCL_SIGNED:
      return_value = (CORBA::Long) *this > (CORBA::Long) rhs;
      break;
    case TAO_ETCL_UNSIGNED:
      return_value = (CORBA::ULong) *this > (CORBA::ULong) rhs;
      break;
    default:
      break;
    }

  return return_value;
}

CORBA::Boolean
TAO_ETCL_Literal_Constraint::operator>= (const TAO_ETCL_Literal_Constraint & rhs)
{
  int return_value = 0;
  TAO_Literal_Type widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case TAO_ETCL_STRING:
      return_value = 
        (ACE_OS::strcmp ((const char*) *this, (const char*) rhs) >= 0);
      break;
    case TAO_ETCL_DOUBLE:
      return_value = (CORBA::Double) *this >= (CORBA::Double) rhs;
      break;
    case TAO_ETCL_INTEGER:
    case TAO_ETCL_SIGNED:
      return_value = (CORBA::Long) *this >= (CORBA::Long) rhs;
      break;
    case TAO_ETCL_UNSIGNED:
      return_value = (CORBA::ULong) *this >= (CORBA::ULong) rhs;
      break;
    default:
      break;
    }

  return return_value;
}

TAO_ETCL_Literal_Constraint
TAO_ETCL_Literal_Constraint::operator+ (const TAO_ETCL_Literal_Constraint & rhs)
{
  TAO_Literal_Type widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case TAO_ETCL_DOUBLE:
      {
	      CORBA::Double result = (CORBA::Double) *this + (CORBA::Double) rhs;
	      return TAO_ETCL_Literal_Constraint ((CORBA::Double) result);
      }
    case TAO_ETCL_INTEGER:
    case TAO_ETCL_SIGNED:
      {
	      CORBA::Long result = (CORBA::Long) *this + (CORBA::Long) rhs;
	      return TAO_ETCL_Literal_Constraint ((CORBA::Long) result);
      }
    case TAO_ETCL_UNSIGNED:
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
  TAO_Literal_Type widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case TAO_ETCL_DOUBLE:
      {
	      CORBA::Double result = (CORBA::Double) *this - (CORBA::Double) rhs;
	      return TAO_ETCL_Literal_Constraint ((CORBA::Double) result);
      }
    case TAO_ETCL_INTEGER:
    case TAO_ETCL_SIGNED:
      {
	      CORBA::Long result = (CORBA::Long) *this - (CORBA::Long) rhs;
	      return TAO_ETCL_Literal_Constraint ((CORBA::Long) result);
      }
    case TAO_ETCL_UNSIGNED:
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
  TAO_Literal_Type widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case TAO_ETCL_DOUBLE:
      {
	      CORBA::Double result = (CORBA::Double) *this * (CORBA::Double) rhs;
	      return TAO_ETCL_Literal_Constraint ((CORBA::Double) result);
      }
    case TAO_ETCL_INTEGER:
    case TAO_ETCL_SIGNED:
      {
	      CORBA::Long result = (CORBA::Long) *this * (CORBA::Long) rhs;
	      return TAO_ETCL_Literal_Constraint ((CORBA::Long) result);
      }
    case TAO_ETCL_UNSIGNED:
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
  TAO_Literal_Type widest_type = this->widest_type (rhs);

  switch (widest_type)
    {
    case TAO_ETCL_DOUBLE:
      {
	      CORBA::Double result = (CORBA::Double) *this / (CORBA::Double) rhs;
	      return TAO_ETCL_Literal_Constraint ((CORBA::Double) result);
      }
    case TAO_ETCL_INTEGER:
    case TAO_ETCL_SIGNED:
      {
	      CORBA::Long result = (CORBA::Long) *this / (CORBA::Long) rhs;
	      return TAO_ETCL_Literal_Constraint ((CORBA::Long) result);
      }
    case TAO_ETCL_UNSIGNED:
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
    case TAO_ETCL_DOUBLE:
	    return TAO_ETCL_Literal_Constraint (- this->op_.double_);
    case TAO_ETCL_INTEGER:
    case TAO_ETCL_SIGNED:
	    return TAO_ETCL_Literal_Constraint (- this->op_.integer_);
    case TAO_ETCL_UNSIGNED:
	    return TAO_ETCL_Literal_Constraint (- (CORBA::Long) this->op_.uinteger_);
    default:
      return TAO_ETCL_Literal_Constraint ((CORBA::Long) 0);
  }
}

TAO_Literal_Type
TAO_ETCL_Literal_Constraint::widest_type (
		const TAO_ETCL_Literal_Constraint & rhs
  )
{
  TAO_Literal_Type rhs_type = rhs.expr_type ();
  TAO_Literal_Type return_value = rhs_type;

  if (rhs_type == TAO_ETCL_COMPONENT)
    {
      CORBA::TypeCode_var tc = rhs.op_.any_->type ();
      rhs_type = this->comparable_type (tc.in ());
    }
  else if (rhs_type != this->type_)
    {
      if (rhs_type > this->type_)
        {
	        return_value = rhs_type;
        }
      else
        {
	        return_value = this->type_;
        }
    }

  return return_value;
}

void
TAO_ETCL_Literal_Constraint::copy (const TAO_ETCL_Literal_Constraint &lit)
{
  this->type_ = lit.type_;

  switch (this->type_)
  {
  case TAO_ETCL_STRING:
    this->op_.str_ = CORBA::string_dup (lit.op_.str_);
    break;
  case TAO_ETCL_DOUBLE:
    this->op_.double_ = lit.op_.double_;
    break;
  case TAO_ETCL_UNSIGNED:
    this->op_.uinteger_ = lit.op_.uinteger_;
    break;
  case TAO_ETCL_INTEGER:
  case TAO_ETCL_SIGNED:
    this->op_.integer_ = lit.op_.integer_;
    break;
  case TAO_ETCL_BOOLEAN:
    this->op_.bool_ = lit.op_.bool_;
    break;
  case TAO_ETCL_COMPONENT:
    this->op_.any_ = lit.op_.any_;
    break;
  default:
    this->type_ = TAO_UNKNOWN;
    break;
  }
}

// ****************************************************************

int
TAO_ETCL_Identifier::accept (TAO_ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_identifier (this);
}

// ****************************************************************

TAO_ETCL_Union_Value::~TAO_ETCL_Union_Value (void)
{
  delete this->string_;
  delete this->integer_;
}

int
TAO_ETCL_Union_Value::accept (TAO_ETCL_Constraint_Visitor *visitor)
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
TAO_ETCL_Union_Pos::accept (TAO_ETCL_Constraint_Visitor *visitor)
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
TAO_ETCL_Component_Pos::accept (TAO_ETCL_Constraint_Visitor *visitor)
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
TAO_ETCL_Component_Assoc::accept (TAO_ETCL_Constraint_Visitor *visitor)
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
TAO_ETCL_Component_Array::accept (TAO_ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_component_array (this);
}

// ****************************************************************

TAO_ETCL_Special::~TAO_ETCL_Special (void)
{
}

int
TAO_ETCL_Special::accept (TAO_ETCL_Constraint_Visitor *visitor)
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
TAO_ETCL_Component::accept (TAO_ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_component (this);
}

// ****************************************************************

TAO_ETCL_Dot::~TAO_ETCL_Dot (void)
{
  delete this->component_;
}

int
TAO_ETCL_Dot::accept (TAO_ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_dot (this);
}

// ****************************************************************

TAO_ETCL_Eval::~TAO_ETCL_Eval (void)
{
  delete this->component_;
}

int
TAO_ETCL_Eval::accept (TAO_ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_eval (this);
}

// ****************************************************************

TAO_ETCL_Default::~TAO_ETCL_Default (void)
{
  delete this->component_;
}

int
TAO_ETCL_Default::accept (TAO_ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_default (this);
}

// ****************************************************************

TAO_ETCL_Exist::~TAO_ETCL_Exist (void)
{
  delete this->component_;
}

int
TAO_ETCL_Exist::accept (TAO_ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_exist (this);
}

// ****************************************************************

TAO_ETCL_Unary_Expr::~TAO_ETCL_Unary_Expr (void)
{
  delete this->subexpr_;
}

int
TAO_ETCL_Unary_Expr::accept (TAO_ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_unary_expr (this);
}

// ****************************************************************

TAO_ETCL_Binary_Expr::~TAO_ETCL_Binary_Expr (void)
{
  delete this->lhs_;
  delete this->rhs_;
}

int
TAO_ETCL_Binary_Expr::accept (TAO_ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_binary_expr (this);
}

// ****************************************************************

TAO_ETCL_Preference::~TAO_ETCL_Preference (void)
{
  delete this->subexpr_;
}

int
TAO_ETCL_Preference::accept (TAO_ETCL_Constraint_Visitor *visitor)
{
  return visitor->visit_preference (this);
}

// ****************************************************************

TAO_ETCL_Constraint_Visitor::~TAO_ETCL_Constraint_Visitor (void)
{
}
