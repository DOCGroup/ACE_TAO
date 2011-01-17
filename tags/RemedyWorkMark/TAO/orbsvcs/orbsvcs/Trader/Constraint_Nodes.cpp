// $Id$

#include "orbsvcs/Trader/Constraint_Nodes.h"
#include "orbsvcs/Trader/Constraint_Visitors.h"
#include "orbsvcs/Trader/Constraint_Tokens.h"

#include "tao/AnyTypeCode/Any.h"
#include "ace/OS_NS_string.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Trader/Constraint_Nodes.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_Noop_Constraint::accept (TAO_Constraint_Visitor* visitor)
{
  int return_value = -1;
  switch (this->type_)
    {
    case TAO_FIRST:
      return_value = visitor->visit_first (this);
      break;
    case TAO_RANDOM:
      return_value = visitor->visit_random (this);
    }

  return return_value;
}

TAO_Binary_Constraint::
TAO_Binary_Constraint (TAO_Expression_Type op_type,
                       TAO_Constraint* left,
                       TAO_Constraint* right)
  : op_ (op_type),
    left_ (left),
    right_ (right)
{
}

TAO_Binary_Constraint::~TAO_Binary_Constraint (void)
{
  delete left_;
  delete right_;
}

// Dispatch table for the accept method
static int (*dispatch_table[]) (TAO_Constraint_Visitor*,
                                 TAO_Binary_Constraint*)=
{
  TAO_Binary_Constraint::visit_greater_than,
  TAO_Binary_Constraint::visit_greater_than_equal,
  TAO_Binary_Constraint::visit_less_than,
  TAO_Binary_Constraint::visit_less_than_equal,
  TAO_Binary_Constraint::visit_equal,
  TAO_Binary_Constraint::visit_not_equal,
  0,
  TAO_Binary_Constraint::visit_and,
  TAO_Binary_Constraint::visit_or,
  0,
  TAO_Binary_Constraint::visit_in,
  TAO_Binary_Constraint::visit_twiddle,
  0,
  TAO_Binary_Constraint::visit_add,
  TAO_Binary_Constraint::visit_sub,
  TAO_Binary_Constraint::visit_mult,
  TAO_Binary_Constraint::visit_div
};

// Simulate the fun of actual double dispatching.

int
TAO_Binary_Constraint::accept (TAO_Constraint_Visitor* visitor)
{
  int offset = this->op_ - TAO_GT,
    return_value = -1;

  if (dispatch_table[offset] != 0)
    return_value = dispatch_table[offset] (visitor, this);

  return return_value;
}

int
TAO_Binary_Constraint::
visit_or (TAO_Constraint_Visitor* visitor,
                  TAO_Binary_Constraint* expr)
{
  return visitor->visit_or (expr);
}

int
TAO_Binary_Constraint::
visit_and (TAO_Constraint_Visitor* visitor,
           TAO_Binary_Constraint* expr)
{
  return visitor->visit_and (expr);
}

int
TAO_Binary_Constraint::
visit_less_than (TAO_Constraint_Visitor* visitor,
                TAO_Binary_Constraint* expr)
{
  return visitor->visit_less_than (expr);
}

int
TAO_Binary_Constraint::
visit_less_than_equal (TAO_Constraint_Visitor* visitor,
                      TAO_Binary_Constraint* expr)
{
  return visitor->visit_less_than_equal (expr);
}

int
TAO_Binary_Constraint::
visit_greater_than (TAO_Constraint_Visitor* visitor,
                   TAO_Binary_Constraint* expr)
{
  return visitor->visit_greater_than (expr);
}

int
TAO_Binary_Constraint::
visit_greater_than_equal (TAO_Constraint_Visitor* visitor,
                         TAO_Binary_Constraint* expr)
{
  return visitor->visit_greater_than_equal (expr);
}

int
TAO_Binary_Constraint::
visit_equal (TAO_Constraint_Visitor* visitor,
            TAO_Binary_Constraint* expr)
{
  return visitor->visit_equal (expr);
}

int
TAO_Binary_Constraint::
visit_not_equal (TAO_Constraint_Visitor* visitor,
                TAO_Binary_Constraint* expr)
{
  return visitor->visit_not_equal (expr);
}

int
TAO_Binary_Constraint::
visit_add (TAO_Constraint_Visitor* visitor,
          TAO_Binary_Constraint* expr)
{
  return visitor->visit_add (expr);
}

int
TAO_Binary_Constraint::
visit_sub (TAO_Constraint_Visitor* visitor,
          TAO_Binary_Constraint* expr)
{
  return visitor->visit_sub (expr);
}

int
TAO_Binary_Constraint::
visit_mult (TAO_Constraint_Visitor* visitor,
           TAO_Binary_Constraint* expr)
{
  return visitor->visit_mult (expr);
}

int
TAO_Binary_Constraint::
visit_div (TAO_Constraint_Visitor* visitor,
          TAO_Binary_Constraint* expr)
{
  return visitor->visit_div (expr);
}

int
TAO_Binary_Constraint::
visit_twiddle (TAO_Constraint_Visitor* visitor,
              TAO_Binary_Constraint* expr)
{
  return visitor->visit_twiddle (expr);
}

int
TAO_Binary_Constraint::
visit_in (TAO_Constraint_Visitor* visitor,
         TAO_Binary_Constraint* expr)
{
  return visitor->visit_in (expr);
}


TAO_Constraint*
TAO_Binary_Constraint::left_operand (void) const
{
  return this->left_;
}

TAO_Constraint*
TAO_Binary_Constraint::right_operand (void) const
{
  return this->right_;
}

TAO_Unary_Constraint::
TAO_Unary_Constraint (TAO_Expression_Type op_type,
                     TAO_Constraint* operand)
  : op_ (op_type),
    operand_ (operand)
{
}

TAO_Unary_Constraint::~TAO_Unary_Constraint (void)
{
  delete operand_;
}


int
TAO_Unary_Constraint::accept (TAO_Constraint_Visitor* visitor)
{
  // Since there are only three unary operators, there's no need for a
  // dispatch table.
  int return_value = -1;
  switch (this->op_)
    {
    case TAO_CONSTRAINT:
      return_value = visitor->visit_constraint (this);
      break;
    case TAO_WITH:
      return_value = visitor->visit_with (this);
      break;
    case TAO_MIN:
      return_value = visitor->visit_min (this);
      break;
    case TAO_MAX:
      return_value = visitor->visit_max (this);
      break;
    case TAO_NOT:
      return_value = visitor->visit_not (this);
      break;
    case TAO_UMINUS:
      return_value = visitor->visit_unary_minus (this);
      break;
    case TAO_EXIST:
      return_value = visitor->visit_exist (this);
      break;
    }

  return return_value;
}

TAO_Constraint*
TAO_Unary_Constraint::operand (void)
{
  return this->operand_;
}

TAO_Property_Constraint::
TAO_Property_Constraint (const char* name)
  : name_ (CORBA::string_dup (name))
{
}

TAO_Property_Constraint::~TAO_Property_Constraint (void)
{
  CORBA::string_free (this->name_);
}

int
TAO_Property_Constraint::accept (TAO_Constraint_Visitor* visitor)
{
  return visitor->visit_property (this);
}

TAO_Expression_Type
TAO_Property_Constraint::expr_type (void) const
{
  return TAO_IDENT;
}

const char*
TAO_Property_Constraint::name (void) const
{
  return name_;
}

TAO_Literal_Constraint::TAO_Literal_Constraint (void)
  : type_ (TAO_UNKNOWN)
{
}

TAO_Literal_Constraint::
TAO_Literal_Constraint (const TAO_Literal_Constraint& lit)
  : TAO_Constraint(),
    type_ (TAO_UNKNOWN)
{
  this->copy (lit);
}


TAO_Literal_Constraint::
TAO_Literal_Constraint (CORBA::Any* any)
{
  CORBA::Any& any_ref = *any;
  CORBA::TypeCode_var type = any_ref.type ();
  // @@ No where to throw exception back.
  CORBA::TCKind corba_type = CORBA::tk_null;
  try
    {
      corba_type = type->kind ();
    }
  catch (const CORBA::Exception&)
    {
      // @@ Seth: Don't know what else to do.  Make sure we can tell
      // when this constructor fails.
      return;
    }

  this->type_ = TAO_Literal_Constraint::comparable_type (type.in ());
  switch (this->type_)
    {
    case TAO_SIGNED:
      this->op_.integer_ = 0;
      if (corba_type == CORBA::tk_short)
        {
          CORBA::Short sh;
          any_ref >>= sh;
          this->op_.integer_ = static_cast<CORBA::LongLong> (sh);
        }
      else if (corba_type == CORBA::tk_long)
        {
          CORBA::Long sh;
          any_ref >>= sh;
          this->op_.integer_ = static_cast<CORBA::LongLong> (sh);
        }
      else
        any_ref >>= this->op_.integer_;
      break;
    case TAO_UNSIGNED:
      this->op_.uinteger_ = 0;
      if (corba_type == CORBA::tk_ushort)
        {
          CORBA::UShort sh;
          any_ref >>= sh;
          this->op_.uinteger_ = static_cast<CORBA::ULongLong> (sh);
        }
      else if (corba_type == CORBA::tk_ulong)
        {
          CORBA::ULong sh;
          any_ref >>= sh;
          this->op_.uinteger_ = static_cast<CORBA::ULongLong> (sh);
        }
      else
        any_ref >>= this->op_.uinteger_;
      break;
    case TAO_DOUBLE:
      if (corba_type == CORBA::tk_float)
        {
          CORBA::Float fl;
          (*any) >>= fl;
          this->op_.double_ = (CORBA::Double) fl;
        }
      else
        (*any) >>= this->op_.double_;
      break;
    case TAO_BOOLEAN:
      {
        CORBA::Any::to_boolean tmp (this->op_.bool_);
        (*any) >>= tmp;
      }
    break;
    case TAO_STRING:
      {
        const char* s;
        any_ref >>= s;
        this->op_.str_ = CORBA::string_dup (s);
      }
    break;
    case TAO_SEQUENCE:
      this->op_.any_ = any;
    }
}

TAO_Literal_Constraint::TAO_Literal_Constraint (CORBA::ULongLong uinteger)
  : type_ (TAO_UNSIGNED)
{
  this->op_.uinteger_ = uinteger;
}

TAO_Literal_Constraint::TAO_Literal_Constraint (CORBA::LongLong integer)
  : type_ (TAO_SIGNED)
{
  this->op_.integer_ = integer;
}

TAO_Literal_Constraint::TAO_Literal_Constraint (CORBA::Boolean boolean)
  : type_ (TAO_BOOLEAN)
{
  this->op_.bool_ = boolean;
}

TAO_Literal_Constraint::TAO_Literal_Constraint (CORBA::Double doub)
  : type_ (TAO_DOUBLE)
{
  this->op_.double_ = doub;
}

TAO_Literal_Constraint::TAO_Literal_Constraint (const char* str)
  : type_ (TAO_STRING)
{
  this->op_.str_ = CORBA::string_dup (str);
}

TAO_Literal_Constraint::~TAO_Literal_Constraint (void)
{
  if (this->type_ == TAO_STRING)
    CORBA::string_free (this->op_.str_);
}

int
TAO_Literal_Constraint::accept (TAO_Constraint_Visitor* visitor)
{
  return visitor->visit_literal (this);
}

void
TAO_Literal_Constraint::operator= (const TAO_Literal_Constraint& co)
{
  this->copy (co);
}

TAO_Literal_Constraint::operator CORBA::Boolean (void) const
{
  return (this->type_ == TAO_BOOLEAN) ? this->op_.bool_ : 0;
}

TAO_Literal_Constraint::operator CORBA::ULongLong (void) const
{
  CORBA::ULongLong return_value = 0;

  if (this->type_ == TAO_UNSIGNED)
    return_value = this->op_.uinteger_;
  else if (this->type_ == TAO_SIGNED)
    return_value =
      (this->op_.integer_ > 0) ?
        static_cast<CORBA::ULongLong> (this->op_.integer_) : 0;
  else if (this->type_ == TAO_DOUBLE)
    return_value =
      (this->op_.double_ > 0) ?
      ((this->op_.double_ > ACE_UINT64_MAX) ?
       ACE_UINT64_MAX :
       static_cast<CORBA::ULongLong> (this->op_.double_)) : 0;

  return return_value;
}

TAO_Literal_Constraint::operator CORBA::LongLong (void) const
{
  CORBA::LongLong return_value = 0;

  if (this->type_ == TAO_SIGNED)
    return_value = this->op_.integer_;
  else if (this->type_ == TAO_UNSIGNED)
    return_value =
      (this->op_.uinteger_ > static_cast<CORBA::ULongLong> (ACE_INT64_MAX)) ?
      ACE_INT64_MAX : static_cast<CORBA::LongLong> (this->op_.uinteger_);
  else if (this->type_ == TAO_DOUBLE)
    return_value  =
      (this->op_.double_ > 0) ?
      ((this->op_.double_ > ACE_INT64_MAX) ?
       ACE_INT64_MAX :
       static_cast<CORBA::LongLong> (this->op_.double_)) :
    ((this->op_.double_ < ACE_INT64_MIN) ?
     ACE_INT64_MIN :
     static_cast<CORBA::LongLong> (this->op_.double_));

  return return_value;
}

TAO_Literal_Constraint::operator CORBA::Double (void) const
{
  CORBA::Double return_value = 0.0;

  if (this->type_ == TAO_DOUBLE)
    return_value = this->op_.double_;
  else if (this->type_ == TAO_SIGNED)
    return_value = (CORBA::Double) this->op_.integer_;
  else if (this->type_ == TAO_UNSIGNED)
    return_value = (CORBA::Double) this->op_.uinteger_;

  return return_value;
}

TAO_Literal_Constraint::operator const char* (void) const
{
  return (this->type_ == TAO_STRING) ? this->op_.str_ : 0;
}

TAO_Literal_Constraint::operator const CORBA::Any* (void) const
{
  return (this->type_ == TAO_SEQUENCE) ? this->op_.any_ : 0;
}

TAO_Expression_Type
TAO_Literal_Constraint::comparable_type (CORBA::TypeCode_ptr type)
{
  // Convert a CORBA::TCKind into a TAO_Literal_Type
  TAO_Expression_Type return_value = TAO_UNKNOWN;
  CORBA::TCKind kind = CORBA::tk_null;
  try
    {
      kind = type->kind ();
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
    case CORBA::tk_ulonglong:
      return_value = TAO_UNSIGNED;
      break;
    case CORBA::tk_long:
    case CORBA::tk_short:
    case CORBA::tk_longlong:
      return_value = TAO_SIGNED;
      break;
    case CORBA::tk_boolean:
      return_value = TAO_BOOLEAN;
      break;
    case CORBA::tk_float:
    case CORBA::tk_double:
      return_value = TAO_DOUBLE;
      break;
    case CORBA::tk_string:
      return_value = TAO_STRING;
      break;
    case CORBA::tk_sequence:
      return_value = TAO_SEQUENCE;
      break;
    case CORBA::tk_alias:
      {
        CORBA::TCKind kind = CORBA::tk_void;
        try
          {
            CORBA::TypeCode_var typecode = type->content_type ();
            kind = typecode->kind ();
            ;
          }
        catch (const CORBA::Exception&)
          {
            return return_value;
          }
        // Since this is a "top level try block, no need to check again.

        if (kind == CORBA::tk_sequence)
          return_value = TAO_SEQUENCE;
      }
    break;
    default:
      return_value = TAO_UNKNOWN;
    }

  return return_value;
}

bool
operator== (const TAO_Literal_Constraint& left,
            const TAO_Literal_Constraint& right)
{
  bool return_value = false;
  TAO_Expression_Type widest_type =
    TAO_Literal_Constraint::widest_type (left, right);

  switch (widest_type)
    {
    case TAO_STRING:
      return_value = (ACE_OS::strcmp ((const char*) left, (const char*) right) == 0);
      break;
    case TAO_DOUBLE:
      return_value = ACE::is_equal ((CORBA::Double) left, (CORBA::Double) right);
      break;
    case TAO_SIGNED:
      return_value = static_cast<CORBA::LongLong> (left) ==
                     static_cast<CORBA::LongLong> (right);
      break;
    case TAO_UNSIGNED:
      return_value = static_cast<CORBA::ULongLong> (left) ==
                     static_cast<CORBA::ULongLong> (right);
      break;
    case TAO_BOOLEAN:
      return_value = (CORBA::Boolean) left == (CORBA::Boolean) right;
      break;
    }

  return return_value;
}



bool
operator< (const TAO_Literal_Constraint& left,
           const TAO_Literal_Constraint& right)
{
  bool return_value = false;
  TAO_Expression_Type widest_type =
    TAO_Literal_Constraint::widest_type (left, right);

  switch (widest_type)
    {
    case TAO_STRING:
      return_value = (ACE_OS::strcmp ((const char*) left, (const char*) right) < 0);
      break;
    case TAO_DOUBLE:
      return_value = (CORBA::Double) left < (CORBA::Double) right;
      break;
    case TAO_SIGNED:
      return_value = static_cast<CORBA::LongLong> (left) <
                     static_cast<CORBA::LongLong> (right);
      break;
    case TAO_UNSIGNED:
      return_value = static_cast<CORBA::ULongLong> (left) <
                     static_cast<CORBA::ULongLong> (right);
      break;
    case TAO_BOOLEAN:
      return_value = (CORBA::Boolean) left < (CORBA::Boolean) right;
      break;
    }

  return return_value;
}

bool
operator> (const TAO_Literal_Constraint& left,
           const TAO_Literal_Constraint& right)
{
  bool return_value = false;
  TAO_Expression_Type widest_type =
    TAO_Literal_Constraint::widest_type (left, right);

  switch (widest_type)
    {
    case TAO_STRING:
      return_value = (ACE_OS::strcmp ((const char*) left, (const char*) right) > 0);
      break;
    case TAO_DOUBLE:
      return_value = (CORBA::Double) left > (CORBA::Double) right;
      break;
    case TAO_SIGNED:
      return_value = static_cast<CORBA::LongLong> (left) >
                     static_cast<CORBA::LongLong> (right);
      break;
    case TAO_UNSIGNED:
      return_value = static_cast<CORBA::ULongLong> (left) >
                     static_cast<CORBA::ULongLong> (right);
      break;
    }

  return return_value;
}

bool
operator== (CORBA::Double left, const TAO_Literal_Constraint& right)
{
  return ACE::is_equal (left, (CORBA::Double) right);
}

bool
operator== (const TAO::String_Manager& left,
            const TAO_Literal_Constraint& right)
{
  bool result = false;

  if ((const char*) right != 0)
    result = ACE_OS::strcmp ((const char*) left,
                             (const char*) right) == 0;
  return result;
}


TAO_Literal_Constraint
operator+ (const TAO_Literal_Constraint& left,
           const TAO_Literal_Constraint& right)
{
  TAO_Expression_Type widest_type =
    TAO_Literal_Constraint::widest_type (left, right);

  switch (widest_type)
    {
    case TAO_DOUBLE:
      {
        CORBA::Double result = (CORBA::Double) left + (CORBA::Double) right;
        return TAO_Literal_Constraint (result);
      }
    case TAO_SIGNED:
      {
        CORBA::LongLong result = static_cast<CORBA::LongLong> (left) +
                                 static_cast<CORBA::LongLong> (right);
        return TAO_Literal_Constraint (result);
      }
    case TAO_UNSIGNED:
      {
        CORBA::ULongLong result = static_cast<CORBA::ULongLong> (left) +
                                  static_cast<CORBA::ULongLong> (right);
        return TAO_Literal_Constraint (result);
      }
    default:
      return TAO_Literal_Constraint (static_cast<CORBA::LongLong> (0));
    }
}

TAO_Literal_Constraint
operator- (const TAO_Literal_Constraint& left,
           const TAO_Literal_Constraint& right)
{
  TAO_Expression_Type widest_type =
    TAO_Literal_Constraint::widest_type (left, right);

  switch (widest_type)
    {
    case TAO_DOUBLE:
      {
        CORBA::Double result = (CORBA::Double) left - (CORBA::Double) right;
        return TAO_Literal_Constraint (result);
      }
    case TAO_SIGNED:
      {
        CORBA::LongLong result = static_cast<CORBA::LongLong> (left) -
                                 static_cast<CORBA::LongLong> (right);
        return TAO_Literal_Constraint (result);
      }
    case TAO_UNSIGNED:
      {
        CORBA::ULongLong result = static_cast<CORBA::ULongLong> (left) -
                                  static_cast<CORBA::ULongLong> (right);
        return TAO_Literal_Constraint (result);
      }
    default:
      return TAO_Literal_Constraint (static_cast<CORBA::LongLong> (0));
    }
}

TAO_Literal_Constraint
operator* (const TAO_Literal_Constraint& left,
           const TAO_Literal_Constraint& right)
{
  TAO_Expression_Type widest_type =
    TAO_Literal_Constraint::widest_type (left, right);

  switch (widest_type)
    {
    case TAO_DOUBLE:
      {
        CORBA::Double result = (CORBA::Double) left * (CORBA::Double) right;
        return TAO_Literal_Constraint (result);
      }
    case TAO_SIGNED:
      {
        CORBA::LongLong result = static_cast<CORBA::LongLong> (left) *
                                 static_cast<CORBA::LongLong> (right);
        return TAO_Literal_Constraint (result);
      }
    case TAO_UNSIGNED:
      {
        CORBA::ULongLong result = static_cast<CORBA::ULongLong> (left) *
                                  static_cast<CORBA::ULongLong> (right);
        return TAO_Literal_Constraint (result);
      }
    default:
      return TAO_Literal_Constraint (static_cast<CORBA::LongLong> (0));
    }
}

TAO_Literal_Constraint
operator/ (const TAO_Literal_Constraint& left,
           const TAO_Literal_Constraint& right)
{
  TAO_Expression_Type widest_type =
    TAO_Literal_Constraint::widest_type (left, right);

  switch (widest_type)
    {
    case TAO_DOUBLE:
      {
        if (ACE::is_equal ((CORBA::Double) right, 0.0))
          return TAO_Literal_Constraint ((CORBA::Double) 0.0);

        CORBA::Double result = (CORBA::Double) left / (CORBA::Double) right;
        return TAO_Literal_Constraint (result);
      }
    case TAO_SIGNED:
      {
        if (static_cast<CORBA::LongLong> (right) == 0)
          return TAO_Literal_Constraint (static_cast<CORBA::LongLong> (0));

        CORBA::LongLong result = static_cast<CORBA::LongLong> (left) /
                                 static_cast<CORBA::LongLong> (right);
        return TAO_Literal_Constraint (result);
      }
    case TAO_UNSIGNED:
      {
        if (static_cast<CORBA::ULongLong> (right) == 0)
          return TAO_Literal_Constraint (static_cast<CORBA::ULongLong> (0));

        CORBA::ULongLong result = static_cast<CORBA::ULongLong> (left) /
                                  static_cast<CORBA::ULongLong> (right);
        return TAO_Literal_Constraint (result);
      }
    default:
      return TAO_Literal_Constraint (static_cast<CORBA::LongLong> (0));
    }
}

TAO_Literal_Constraint
operator- (const TAO_Literal_Constraint& operand)
{
  switch (operand.expr_type ())
    {
    case TAO_DOUBLE:
      {
        CORBA::Double result = - (CORBA::Double) operand;
        return TAO_Literal_Constraint (result);
      }
    case TAO_SIGNED:
      {
        CORBA::LongLong result = - static_cast<CORBA::LongLong> (operand);
        return TAO_Literal_Constraint (result);
      }
    case TAO_UNSIGNED:
      {
        CORBA::LongLong result = - static_cast<CORBA::LongLong> (
                                     static_cast<CORBA::ULongLong> (operand));
        return TAO_Literal_Constraint (static_cast<CORBA::ULongLong> (result));
      }
    default:
      return TAO_Literal_Constraint (static_cast<CORBA::LongLong> (0));
    }
}

TAO_Expression_Type
TAO_Literal_Constraint::widest_type (const TAO_Literal_Constraint& left,
                                     const TAO_Literal_Constraint& right)
{
  TAO_Expression_Type left_type = left.expr_type (),
    right_type = right.expr_type (),
    return_value = right_type;

  if (right_type != left_type)
    {
      if (right_type > left_type)
        return_value = right_type;
      else
        return_value = left_type;
    }

  return return_value;
}

void
TAO_Literal_Constraint::copy (const TAO_Literal_Constraint& lit)
{
  if (this->type_ == TAO_STRING)
    CORBA::string_free (this->op_.str_);

  this->type_ = lit.type_;
  if (this->type_ == TAO_STRING)
    this->op_.str_ = CORBA::string_dup (lit.op_.str_);
  else if (this->type_ == TAO_DOUBLE)
    this->op_.double_ = lit.op_.double_;
  else if (this->type_ == TAO_UNSIGNED)
    this->op_.uinteger_ = lit.op_.uinteger_;
  else if (this->type_ == TAO_SIGNED)
    this->op_.integer_ = lit.op_.integer_;
  else if (this->type_ == TAO_BOOLEAN)
    this->op_.bool_ = lit.op_.bool_;
  else if (this->type_ == TAO_SEQUENCE)
    this->op_.any_ = lit.op_.any_;
  else
    type_ = TAO_UNKNOWN;
}

TAO_END_VERSIONED_NAMESPACE_DECL
