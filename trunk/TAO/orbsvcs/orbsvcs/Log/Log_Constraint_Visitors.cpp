// $Id$

#include "orbsvcs/Log/Log_Constraint_Visitors.h"

#include "ace/ETCL/ETCL_Constraint.h"
#include "ace/ETCL/ETCL_y.h"

#include "tao/DynamicAny/DynArray_i.h"
#include "tao/DynamicAny/DynSequence_i.h"
#include "tao/DynamicAny/DynStruct_i.h"
#include "tao/DynamicAny/DynUnion_i.h"
#include "tao/DynamicAny/DynEnum_i.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "tao/DynamicAny/DynAnyUtils_T.h"

#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/CDR.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Log_Constraint_Visitor::TAO_Log_Constraint_Visitor (const DsLogAdmin::LogRecord &rec)
  : property_lookup_ (property_lookup_size_)
{
  CORBA::Any val_id;
#if defined (ACE_LACKS_LONGLONG_T) || defined (ACE_LACKS_UNSIGNEDLONGLONG_T)
  val_id <<= ACE_U64_TO_U32 (rec.id);
#else
  val_id <<= static_cast<ACE_UINT32> (rec.id);
#endif
  this->property_lookup_.bind (ACE_CString("id", 0, false), val_id);


  CORBA::Any val_time;
#if defined (ACE_LACKS_LONGLONG_T) || defined (ACE_LACKS_UNSIGNEDLONGLONG_T)
  val_time <<= ACE_U64_TO_U32 (rec.time);
#else
  val_time <<= static_cast<ACE_UINT32> (rec.time);
#endif
  this->property_lookup_.bind (ACE_CString ("time", 0, false), val_time);

  this->property_lookup_.bind (ACE_CString ("info", 0, false), rec.info);

  // Bind an entry for each item in the record's attribute list.
  CORBA::Long len = rec.attr_list.length ();

  for (CORBA::Long i = 0; i < len; ++i)
    {
      this->property_lookup_.bind (ACE_CString (rec.attr_list[i].name,
                                                0,
                                                false),
                                   rec.attr_list[i].value);
    }
}

CORBA::Boolean
TAO_Log_Constraint_Visitor::evaluate_constraint (ETCL_Constraint* root)
{
  CORBA::Boolean result = 0;
  this->queue_.reset ();

  // Evaluate the constraint in root_;
  if (root != 0)
    {
      if ((root->accept (this) == 0) &&
          (! this->queue_.is_empty ()))
        {
          TAO_ETCL_Literal_Constraint top;
          this->queue_.dequeue_head (top);
          result = (CORBA::Boolean) top;
        }
    }

  // If a property couldn't be evaluated we must return 0.
  return result;
}

int
TAO_Log_Constraint_Visitor::visit_literal (
    ETCL_Literal_Constraint *literal
  )
{
  TAO_ETCL_Literal_Constraint lit (literal);
  this->queue_.enqueue_head (lit);
  return 0;
}

int
TAO_Log_Constraint_Visitor::visit_identifier (ETCL_Identifier *ident)
{
  int return_value = -1;
  const char *name = ident->value ();
  ACE_CString key (name, 0, false);

  CORBA::Any any;

  if (this->property_lookup_.find (key, any) == 0)
    {
      if (any.impl() != 0)
        {
          this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (&any));
          return_value = 0;
        }
    }

  return return_value;
}

int
TAO_Log_Constraint_Visitor::visit_union_value (
    ETCL_Union_Value *union_value
  )
{
  switch (union_value->sign ())
  {
    case 0:
      {
        TAO_ETCL_Literal_Constraint lit (union_value->string ());
        this->queue_.enqueue_head (lit);
      }
      break;
    case -1:
      {
        CORBA::Long value = -(*union_value->integer ());
        TAO_ETCL_Literal_Constraint lit (value);
        this->queue_.enqueue_head (lit);
      }
      break;
    case 1:
      {
        TAO_ETCL_Literal_Constraint lit (union_value->integer ());
        this->queue_.enqueue_head (lit);
      }
      break;
    default:
      return -1;
  }

  return 0;
}

int
TAO_Log_Constraint_Visitor::visit_union_pos (
    ETCL_Union_Pos *union_pos
  )
{
  try
    {
      if (union_pos->union_value ()->accept (this) == 0)
        {
          TAO_ETCL_Literal_Constraint disc_val;
          this->queue_.dequeue_head (disc_val);

          TAO_DynUnion_i dyn_union;
          dyn_union.init (this->current_member_.in ());

          CORBA::TypeCode_var tc = this->current_member_->type ();

          switch (disc_val.expr_type ())
          {
            case ETCL_INTEGER:
            case ETCL_SIGNED:
            case ETCL_UNSIGNED:
              {
                CORBA::Any disc_any;
                CORBA::TypeCode_var disc_tc =
                  tc->discriminator_type ();
                CORBA::TCKind disc_kind =
                  TAO_DynAnyFactory::unalias (disc_tc.in ());

                switch (disc_kind)
                {
                  case CORBA::tk_boolean:
                    disc_any <<= CORBA::Any::from_boolean ((CORBA::Boolean) disc_val);
                    break;
                  case CORBA::tk_short:
                    disc_any <<= (CORBA::Short) ((CORBA::Long) disc_val);
                    break;
                  case CORBA::tk_ushort:
                    disc_any <<= (CORBA::UShort) ((CORBA::ULong) disc_val);
                    break;
                  case CORBA::tk_long:
                    disc_any <<= (CORBA::Long) disc_val;
                    break;
                  case CORBA::tk_ulong:
                    disc_any <<= (CORBA::ULong) disc_val;
                    break;
                  case CORBA::tk_enum:
                    {
                      TAO_OutputCDR cdr;
                      cdr.write_ulong ((CORBA::ULong) disc_val);
                      TAO_InputCDR in_cdr (cdr);
                      TAO::Unknown_IDL_Type *unk = 0;
                      ACE_NEW_RETURN (unk,
                                      TAO::Unknown_IDL_Type (disc_tc.in (),
                                                             in_cdr),
                                      -1);

                      disc_any.replace (unk);
                      break;
                    }
                  // @@@ (JP) I don't think ETCL handles 64-bit
                  // integers at this point, and I also think that
                  // chars and/or wchars will just come out in the
                  // constraint as (w)strings of length 1.
                  case CORBA::tk_longlong:
                  case CORBA::tk_ulonglong:
                  case CORBA::tk_char:
                  case CORBA::tk_wchar:
                  default:
                    return -1;
                }

                DynamicAny::DynAny_var dyn_any =
                  TAO::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any &> (
                    disc_tc.in (),
                    disc_any);

                dyn_union.set_discriminator (dyn_any.in ());
                DynamicAny::DynAny_var u_member =
                  dyn_union.member ();

                this->current_member_ =
                  u_member->to_any ();

                break;
              }
            case ETCL_STRING:
              {
                const char *name = (const char *) disc_val;
                CORBA::ULong count =
                  tc->member_count ();

                const char *member_name = 0;
                CORBA::ULong i = 0;

                for (i = 0; i < count; ++i)
                  {
                    member_name = tc->member_name (i);

                    if (ACE_OS::strcmp (name, member_name) == 0)
                      {
                        break;
                      }
                  }

                // If there's no match, member_label will throw
                // CORBA::TypeCode::Bounds and the catch block will
                // return -1;
                this->current_member_ = tc->member_label (i);

                break;
              }
            // The TAO_ETCL_Union_Value that was put on the queue
            // shouldn't have any other type.
            default:
              return -1;
          }

          ETCL_Constraint *nested = union_pos->component ();

          // If there's no nested component, then we just want the
          // union member value on the queue. Otherwise, we want
          // the member value in current_member_ while we visit
          // the nested component.
          if (nested == 0)
            {
              TAO_ETCL_Literal_Constraint lit (this->current_member_.ptr ());
              this->queue_.enqueue_head (lit);
              return 0;
            }
          else
            {
              return nested->accept (this);
            }
        }
      else
        {
          return -1;
        }
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
}

int
TAO_Log_Constraint_Visitor::visit_component_pos (ETCL_Component_Pos *pos)
{
  try
    {
      // If we are here (from visit_component) the Any containing the
      // component as found in property_lookup_ will be in current_member_.
      CORBA::TypeCode_var tc = this->current_member_->type ();
      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ());

      DynamicAny::DynAny_var member;
      CORBA::Boolean success = 0;
      CORBA::ULong slot = (CORBA::ULong) *pos->integer ();

      switch (kind)
      {
        case CORBA::tk_enum:
          {
            TAO_DynEnum_i dyn_enum;
            dyn_enum.init (this->current_member_.in ());

            success = dyn_enum.seek (slot);

            if (success == 0)
              {
                return -1;
              }

            member =
              dyn_enum.current_component ();

            break;
          }
        case CORBA::tk_struct:
          {
            TAO_DynStruct_i dyn_struct;
            dyn_struct.init (this->current_member_.in ());

            success = dyn_struct.seek (slot);

            if (success == 0)
              {
                return -1;
              }

            member = dyn_struct.current_component ();

            break;
          }
        // @@@ (JP) I think enums and structs are the only two cases handled
        // by Component_Pos, since arrays and sequences are handled by
        // Component_Array, and unions are handled by Union_Pos.
        default:
          return -1;
      }

      CORBA::Any_var value = member->to_any ();

      ETCL_Constraint *comp = pos->component ();

      if (comp == 0)
        {
          TAO_ETCL_Literal_Constraint result (value.ptr ());
          this->queue_.enqueue_head (result);
          return 0;
        }
      else
        {
          this->current_member_ = value._retn ();
          return comp->accept (this);
        }
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
}

int
TAO_Log_Constraint_Visitor::visit_component_assoc (
  ETCL_Component_Assoc *assoc)
{
  // @@@ (JP) The spec reserves this type of constraint for NVLists.
  // Since NVLists don't have type codes or Any operators, there's
  // no way that TAO can put one into the event's filterable data.
  // However, from the looks of the ETCL grammar, I believe that a
  // contruct like 'exist $(foo)' is legal, and is in effect using
  // the event's filterable data as one big NVList. It is
  // equivalent to '$.foo'. I've implemented this method on that
  // basis, while keeping in mind that a clearer interpretation of
  // the spec may come along someday.

  CORBA::Any any;
  ACE_CString key (assoc->identifier ()->value (),
       0,
       false);

  if (this->property_lookup_.find (key, any) != 0
      || any.impl () == 0)
    {
      return -1;
    }

  ETCL_Constraint *comp = assoc->component ();

  if (comp == 0)
    {
      TAO_ETCL_Literal_Constraint result (&any);
      this->queue_.enqueue_head (result);
      return 0;
    }

  CORBA::Any *any_ptr = 0;
  ACE_NEW_RETURN (any_ptr,
                  CORBA::Any (any),
                  -1);
  this->current_member_ = any_ptr;
  return comp->accept (this);
}

int
TAO_Log_Constraint_Visitor::visit_component_array (
    ETCL_Component_Array *array
  )
{
  try
    {
      // If we are here (from visit_component) the Any containing the
      // component as found in property_lookup_ will be in current_member_.
      CORBA::TypeCode_var tc = this->current_member_->type ();
      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ());

      DynamicAny::DynAny_var member;
      CORBA::Boolean success = 0;
      CORBA::ULong slot = (CORBA::ULong) *array->integer ();

      switch (kind)
      {
        case CORBA::tk_array:
          {
            TAO_DynEnum_i dyn_array;
            dyn_array.init (this->current_member_.in ());

            success = dyn_array.seek (slot);

            if (success == 0)
              {
                return -1;
              }

            member = dyn_array.current_component ();

            break;
          }
        case CORBA::tk_sequence:
          {
            TAO_DynStruct_i dyn_sequence;
            dyn_sequence.init (this->current_member_.in ());

            success = dyn_sequence.seek (slot);

            if (success == 0)
              {
                return -1;
              }

            member =
              dyn_sequence.current_component ();

            break;
          }
        // Enums and sequences are the only two cases handled
        // by Component_Array.
        default:
          return -1;
      }

      CORBA::Any_var value = member->to_any ();

      ETCL_Constraint *comp = array->component ();

      if (comp == 0)
        {
          TAO_ETCL_Literal_Constraint result (value.ptr ());
          this->queue_.enqueue_head (result);
          return 0;
        }
      else
        {
          this->current_member_ = value._retn ();
          return comp->accept (this);
        }
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
}

int
TAO_Log_Constraint_Visitor::visit_special (ETCL_Special *special)
{
  try
    {
      CORBA::TypeCode_var tc = this->current_member_->type ();

      switch (special->type ())
      {
        case ETCL_LENGTH:
          {
            // If the TCKind is not a sequence or an array, the
            // call to length() will raise an exception, and the
            // catch block will return -1;
            CORBA::ULong length = tc->length ();

            TAO_ETCL_Literal_Constraint lit (length);
            this->queue_.enqueue_head (lit);
            return 0;
          }
        case ETCL_DISCRIMINANT:
          {
            // If the TCKind is not a union, the
            // call to init() will raise an exception, and the
            // catch block will return -1;
            TAO_DynUnion_i dyn_union;
            dyn_union.init (this->current_member_.in ());

            DynamicAny::DynAny_var disc =
              dyn_union.get_discriminator ();

            CORBA::Any_var disc_any = disc->to_any ();

            TAO_ETCL_Literal_Constraint lit (disc_any.ptr ());
            this->queue_.enqueue_head (lit);
            return 0;
          }
        case ETCL_TYPE_ID:
          {
            const char *name = tc->name ();

            TAO_ETCL_Literal_Constraint lit (name);
            this->queue_.enqueue_head (lit);
            return 0;
          }
        case ETCL_REPOS_ID:
          {
            const char *id = tc->id ();

            TAO_ETCL_Literal_Constraint lit (id);
            this->queue_.enqueue_head (lit);
            return 0;
          }
        default:
          return -1;
      }
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
}

int
TAO_Log_Constraint_Visitor::visit_component (
    ETCL_Component *component
  )
{
  // If this component has no sub-component, only an identifier,
  // then we just visit the identifier, which puts a literal on
  // the queue to be handled upon returning from this method call.
  // If there is a sub-component, we store the literal's value
  // in our member _var for possible examination at a more
  // nested level, and visit the sub-component.

  ETCL_Constraint *nested = component->component ();
  int result = component->identifier ()->accept (this);

  if (nested == 0 || result != 0)
    {
      return result;
    }
  else
    {
      TAO_ETCL_Literal_Constraint id;
      this->queue_.dequeue_head (id);

      CORBA::Any *any_ptr = 0;
      ACE_NEW_RETURN (any_ptr,
                      CORBA::Any,
                      -1);

      any_ptr->replace (id);
      any_ptr->impl ()->_add_ref ();
      this->current_member_ = any_ptr;
      return nested->accept (this);
    }
}

int
TAO_Log_Constraint_Visitor::visit_dot (ETCL_Dot *dot)
{
  // If we are here, we know we're headed for a more nested
  // level, so we just visit it, there's nothing else in this
  // constraint.
  return dot->component ()->accept (this);
}

int
TAO_Log_Constraint_Visitor::visit_eval (ETCL_Eval *eval)
{
  // Nothing to do but visit the contained component.
  return eval->component ()->accept (this);
}

int
TAO_Log_Constraint_Visitor::visit_default (ETCL_Default *def)
{
  ETCL_Constraint *comp = def->component ();

  if (comp == 0)
    {
      return -1;
    }

  if (comp->accept (this) != 0)
    {
      return -1;
    }

  try
    {
      CORBA::TypeCode_var tc = this->current_member_->type ();

      // If the current member is not a union, this call will
      // throw BadKind and the catch block will return -1.
      CORBA::Long default_index = tc->default_index ();

      // No default index.
      if (default_index == -1)
        {
          TAO_ETCL_Literal_Constraint result (false);
          this->queue_.enqueue_head (result);
          return 0;
        }

      // Okay, there's a default index, but is it active?

      TAO_ETCL_Literal_Constraint disc_value;
      this->queue_.dequeue_head (disc_value);
      TAO_ETCL_Literal_Constraint default_index_value (default_index);
      return (disc_value == default_index_value);
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
}

int
TAO_Log_Constraint_Visitor::visit_exist (ETCL_Exist *exist)
{
  ETCL_Constraint *component = exist->component ();

  if (component->accept (this) == 0)
    {
      TAO_ETCL_Literal_Constraint top;
      this->queue_.dequeue_head (top);

      const char *value = (const char *) top;
      ACE_CString key (value, 0, false);

      CORBA::Boolean result = (this->property_lookup_.find (key) == 0);

      this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));

      return 0;
    }

  return -1;
}

int
TAO_Log_Constraint_Visitor::visit_unary_expr (
    ETCL_Unary_Expr *unary_expr
  )
{
  ETCL_Constraint *subexpr = unary_expr->subexpr ();

  if (subexpr->accept (this) == 0)
    {
      TAO_ETCL_Literal_Constraint subexpr_result;
      CORBA::Boolean result = false;
      int op_type = unary_expr->type ();

      switch (op_type)
      {
        case ETCL_NOT:
          this->queue_.dequeue_head (subexpr_result);
          result = ! (CORBA::Boolean) subexpr_result;
          this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
          return 0;
        case ETCL_MINUS:
          // The leading '-' was parsed separately, so we have to pull
          // the literal constraint off the queue, apply the class' own
          // unary minus operator, and put it back.
          this->queue_.dequeue_head (subexpr_result);
          this->queue_.enqueue_head (-subexpr_result);
          return 0;
        case ETCL_PLUS:
          // Leave the literal constraint on the queue. The leading
          // '+' was just syntactic sugar - no action is necessary.
          return 0;
        default:
          // The parser should never construct a ETCL_Unary_Constraint
          // behind any operators except the above three.
          return -1;
      }
    }

  return -1;
}

int
TAO_Log_Constraint_Visitor::visit_binary_expr (
    ETCL_Binary_Expr *binary_expr
  )
{

  // Evaluate the constraint
  int bin_op_type = binary_expr->type ();

  switch (bin_op_type)
  {
    case ETCL_OR:
      return this->visit_or (binary_expr);
    case ETCL_AND:
      return this->visit_and (binary_expr);
    case ETCL_LT:
    case ETCL_LE:
    case ETCL_GT:
    case ETCL_GE:
    case ETCL_EQ:
    case ETCL_NE:
    case ETCL_PLUS:
    case ETCL_MINUS:
    case ETCL_MULT:
    case ETCL_DIV:
      return this->visit_binary_op (binary_expr,
                                    bin_op_type);
    case ETCL_TWIDDLE:
      return this->visit_twiddle (binary_expr);
    case ETCL_IN:
      return this->visit_in (binary_expr);
    default:
      return -1;
  }
}

int
TAO_Log_Constraint_Visitor::visit_or (
    ETCL_Binary_Expr *binary
  )
{
  int return_value = -1;
  CORBA::Boolean result = false;
  ETCL_Constraint *lhs = binary->lhs ();

  if (lhs->accept (this) == 0)
    {
      TAO_ETCL_Literal_Constraint lhs_result;
      this->queue_.dequeue_head (lhs_result);
      result = (CORBA::Boolean) lhs_result;

      // Short-circuiting OR.
      if (result == 0)
        {
          ETCL_Constraint *rhs = binary->rhs ();

          if (rhs->accept (this) == 0)
            {
              TAO_ETCL_Literal_Constraint rhs_result;
              this->queue_.dequeue_head (rhs_result);
              result = (CORBA::Boolean) rhs_result;
              return_value = 0;
            }
        }
      else
        {
          return_value = 0;
        }
    }

  if (return_value == 0)
    {
      this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
    }

  return return_value;
}

int
TAO_Log_Constraint_Visitor::visit_and (
    ETCL_Binary_Expr *binary
  )
{
  int return_value = -1;
  CORBA::Boolean result = false;
  ETCL_Constraint *lhs = binary->lhs ();

  if (lhs->accept (this) == 0)
    {
      TAO_ETCL_Literal_Constraint lhs_result;
      this->queue_.dequeue_head (lhs_result);
      result = (CORBA::Boolean) lhs_result;

      // Short-circuiting AND.
      if (result == 1)
        {
          ETCL_Constraint *rhs = binary->rhs ();

          if (rhs->accept (this) == 0)
            {
              TAO_ETCL_Literal_Constraint rhs_result;
              this->queue_.dequeue_head (rhs_result);
              result = (CORBA::Boolean) rhs_result;
              return_value = 0;
            }
        }
      else
        {
          return_value = 0;
        }
    }

  if (return_value == 0)
    {
      this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
    }

  return return_value;
}

int
TAO_Log_Constraint_Visitor::visit_binary_op (
    ETCL_Binary_Expr *binary,
    int op_type
  )
{
  int return_value = -1;
  ETCL_Constraint *lhs = binary->lhs ();
  CORBA::Boolean result = false;

  // Evaluate the constraint
  // Perform an operation on the results of evaluating the left and
  // right branches of this subtree.
  if (lhs->accept (this) == 0)
    {
      TAO_ETCL_Literal_Constraint left_operand;
      this->queue_.dequeue_head (left_operand);
      ETCL_Constraint *rhs = binary->rhs ();

      if (rhs->accept (this) == 0)
        {
          TAO_ETCL_Literal_Constraint right_operand;
          this->queue_.dequeue_head (right_operand);
          return_value = 0;

          switch (op_type)
          {
            case ETCL_LT:
              result = left_operand < right_operand;
              this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
              break;
            case ETCL_LE:
              //result = left_operand <= right_operand; // Compile error on LynxOS
              result = left_operand.operator<= (right_operand);
              this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
              break;
            case ETCL_GT:
              //result = left_operand > right_operand; // Compile error on LynxOS
              result = left_operand.operator> (right_operand);
              this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
              break;
            case ETCL_GE:
              //result = left_operand >= right_operand; // Compile error on LynxOS
              result = left_operand.operator>= (right_operand);
              this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
              break;
            case ETCL_EQ:
              result = left_operand == right_operand;
              this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
              break;
            case ETCL_NE:
              //result = left_operand != right_operand; // Compile error on LynxOS
              result = left_operand.operator!= (right_operand);
              this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
              break;
            case ETCL_PLUS:
              this->queue_.enqueue_head (left_operand + right_operand);
              break;
            case ETCL_MINUS:
              this->queue_.enqueue_head (left_operand - right_operand);
              break;
            case ETCL_MULT:
              this->queue_.enqueue_head (left_operand * right_operand);
              break;
            case ETCL_DIV:
              this->queue_.enqueue_head (left_operand / right_operand);
              break;
            default:
              return_value = -1;
          }
        }
    }

  return return_value;
}

int
TAO_Log_Constraint_Visitor::visit_twiddle (
    ETCL_Binary_Expr *binary
  )
{
  int return_value = -1;
  ETCL_Constraint *lhs = binary->lhs ();

  // Determine if the left operand is a substring of the right.
  if (lhs->accept (this) == 0)
    {
      TAO_ETCL_Literal_Constraint left;
      this->queue_.dequeue_head (left);
      ETCL_Constraint *rhs = binary->rhs ();

      if (rhs->accept (this) == 0)
        {
          TAO_ETCL_Literal_Constraint right;
          this->queue_.dequeue_head (right);
          CORBA::Boolean result =
            (ACE_OS::strstr ((const char *) left,
                             (const char *) right) != 0);
          this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
          return_value = 0;
        }
    }

  return return_value;
}

int
TAO_Log_Constraint_Visitor::visit_in (
    ETCL_Binary_Expr *binary
  )
{
  int return_value = -1;
  ETCL_Constraint *lhs = binary->lhs ();

  // Determine if the left operand is contained in the right.

  if (lhs->accept (this) == 0)
    {
      TAO_ETCL_Literal_Constraint left;
      this->queue_.dequeue_head (left);

      ETCL_Constraint *rhs = binary->rhs ();

      if (rhs->accept (this) == 0)
        {
          TAO_ETCL_Literal_Constraint bag;
          this->queue_.dequeue_head (bag);

          if (bag.expr_type () == ETCL_COMPONENT)
            {
              CORBA::Any_ptr any_ptr = 0;
              ACE_NEW_RETURN (any_ptr,
                              CORBA::Any,
                              -1);

              CORBA::Any_var component = any_ptr;
              component->replace (bag);
              component->impl ()->_add_ref ();
              CORBA::TCKind kind = CORBA::tk_null;

              try
                {
                  CORBA::TypeCode_var tc = component->type ();
                  kind = TAO_DynAnyFactory::unalias (tc.in ());
                }
              catch (const CORBA::Exception&)
                {
                  return return_value;
                }

              CORBA::Boolean result = 0;

              switch (kind)
              {
                case CORBA::tk_sequence:
                  result = this->sequence_does_contain (&component.in (),
                                                        left);
                  break;
                case CORBA::tk_array:
                  result = this->array_does_contain (&component.in (),
                                                     left);
                  break;
                case CORBA::tk_struct:
                  result = this->struct_does_contain (&component.in (),
                                                      left);
                  break;
                case CORBA::tk_union:
                  result = this->union_does_contain (&component.in (),
                                                     left);
                  break;
                case CORBA::tk_any:
                  result = this->any_does_contain (&component.in (),
                                                   left);
                  break;
                default:
                  return return_value;
              }

              this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
              return_value = 0;
            }
        }
    }

  return return_value;
}

int
TAO_Log_Constraint_Visitor::visit_preference (
    ETCL_Preference *
  )
{
  // According to OMG 00-06-20 section 2.4.1, the Notification Service
  // does not use the preference operators. The method must be here
  // because it is called by the ETCL node, which may be used by other
  // CORBA services that do use the preference operators.
  return -1;
}

CORBA::Boolean
TAO_Log_Constraint_Visitor::sequence_does_contain (
    const CORBA::Any *any,
    TAO_ETCL_Literal_Constraint &item
  )
{
  try
    {
      CORBA::TypeCode_var type = any->type ();
      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (type.in ());

      // The literal and the array elements must be
      // of the same simple type.
      CORBA::Boolean match = this->simple_type_match (item.expr_type (),
                                                      kind);

      if (!match)
        {
          return false;
        }

      TAO_DynSequence_i dyn_seq;
      dyn_seq.init (*any);

      DynamicAny::AnySeq_var any_seq = dyn_seq.get_elements ();

      CORBA::ULong length = any_seq->length ();

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          TAO_ETCL_Literal_Constraint element (&any_seq[i]);

          if (item == element)
            {
              return true;
            }
        }
    }
  catch (const CORBA::Exception&)
    {
      return false;
    }

  return false;
}

CORBA::Boolean
TAO_Log_Constraint_Visitor::array_does_contain (
    const CORBA::Any *any,
    TAO_ETCL_Literal_Constraint &item
  )
{
  try
    {
      CORBA::TypeCode_var type = any->type ();
      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (type.in ());

      // The literal and the array elements must be
      // of the same simple type.
      CORBA::Boolean match = this->simple_type_match (item.expr_type (),
                                                      kind);

      if (!match)
        {
          return false;
        }


      TAO_DynArray_i dyn_array;
      dyn_array.init (*any);

      DynamicAny::AnySeq_var any_seq = dyn_array.get_elements ();

      CORBA::ULong length = any_seq->length ();

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          TAO_ETCL_Literal_Constraint element (&any_seq[i]);

          if (item == element)
            {
              return true;
            }
        }
    }
  catch (const CORBA::Exception&)
    {
      return false;
    }

  return false;
}

CORBA::Boolean
TAO_Log_Constraint_Visitor::struct_does_contain (
    const CORBA::Any *any,
    TAO_ETCL_Literal_Constraint &item
  )
{
  try
    {
      TAO_DynStruct_i dyn_struct;
      dyn_struct.init (*any);

      DynamicAny::NameValuePairSeq_var members =
        dyn_struct.get_members ();

      CORBA::ULong length = members->length ();
      CORBA::TypeCode_var tc;
      CORBA::TCKind kind;

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          tc = members[i].value.type ();
          kind = TAO_DynAnyFactory::unalias (tc.in ());

          // The literal and the struct member must be
          // of the same simple type.
          CORBA::Boolean match = this->simple_type_match (item.expr_type (),
                                                          kind);

      if (!match)
        {
          return false;
        }

          TAO_ETCL_Literal_Constraint element (&members[i].value);

          if (item == element)
            {
              return true;
            }
        }
    }
  catch (const CORBA::Exception&)
    {
      return false;
    }

  return false;
}

CORBA::Boolean
TAO_Log_Constraint_Visitor::union_does_contain (
    const CORBA::Any *any,
    TAO_ETCL_Literal_Constraint &item
  )
{
  try
    {
      TAO_DynUnion_i dyn_union;
      dyn_union.init (*any);

      DynamicAny::DynAny_var cc =
        dyn_union.current_component ();

      CORBA::Any_var member = cc->to_any ();

      CORBA::TypeCode_var tc = member->type ();
      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ());

      // The literal and the union member must be
      // of the same simple type.
      CORBA::Boolean match = this->simple_type_match (item.expr_type (),
                                                      kind);

      if (!match)
        {
          return false;
        }

      TAO_ETCL_Literal_Constraint element (&member.inout ());

      return (item == element);
    }
  catch (const CORBA::Exception&)
    {
      return false;
    }
}

CORBA::Boolean
TAO_Log_Constraint_Visitor::any_does_contain (
    const CORBA::Any *any,
    TAO_ETCL_Literal_Constraint &item
  )
{
  const CORBA::Any *result = 0;

  *any >>= result;

  TAO_ETCL_Literal_Constraint element (const_cast<CORBA::Any *> (result));

  return (item == element);
}

CORBA::Boolean
TAO_Log_Constraint_Visitor::simple_type_match (int expr_type,
                                               CORBA::TCKind tc_kind)
{
  switch (expr_type)
  {
    case ETCL_STRING:
      if (tc_kind != CORBA::tk_string)
        {
          return false;
        }
    case ETCL_DOUBLE:
      if (tc_kind != CORBA::tk_double
          && tc_kind != CORBA::tk_float)
        {
          return false;
        }
    case ETCL_INTEGER:
    case ETCL_SIGNED:
      if (tc_kind != CORBA::tk_short
          && tc_kind != CORBA::tk_long
          && tc_kind != CORBA::tk_longlong)
        {
          return false;
        }
    case ETCL_UNSIGNED:
      if (tc_kind != CORBA::tk_ushort
          && tc_kind != CORBA::tk_ulong
          && tc_kind != CORBA::tk_ulonglong)
        {
          return false;
        }
    case ETCL_BOOLEAN:
      if (tc_kind != CORBA::tk_boolean)
        {
          return false;
        }
    default:
      return false;
  }

  return true;
}

const size_t TAO_Log_Constraint_Visitor::property_lookup_size_ = 31;

TAO_END_VERSIONED_NAMESPACE_DECL
