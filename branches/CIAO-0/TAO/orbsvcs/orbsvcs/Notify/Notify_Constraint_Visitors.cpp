/* -*- C++ -*- */
/* $Id$ */

#include "Notify_Constraint_Visitors.h"
#include "orbsvcs/ETCL/ETCL_Constraint.h"
#include "orbsvcs/ETCL/ETCL_y.h"
#include "tao/DynamicAny/DynArray_i.h"
#include "tao/DynamicAny/DynSequence_i.h"
#include "tao/DynamicAny/DynStruct_i.h"
#include "tao/DynamicAny/DynUnion_i.h"
#include "tao/DynamicAny/DynEnum_i.h"
#include "tao/DynamicAny/DynAnyFactory.h"

TAO_Notify_Constraint_Visitor::TAO_Notify_Constraint_Visitor (void)
  : implicit_id_ (NONE)
{
  (void) this->implicit_ids_.bind (ACE_CString ("filterable_data",
                                                0,
                                               0),
                                   FILTERABLE_DATA);
  (void) this->implicit_ids_.bind (ACE_CString ("header",
                                                0,
                                                0),
                                   HEADER);
  (void) this->implicit_ids_.bind (ACE_CString ("remainder_of_body",
                                                0,
                                                0),
                                   REMAINDER_OF_BODY);
  (void) this->implicit_ids_.bind (ACE_CString ("fixed_header",
                                                0,
                                                0),
                                   FIXED_HEADER);
  (void) this->implicit_ids_.bind (ACE_CString ("variable_header",
                                                0,
                                                0),
                                   VARIABLE_HEADER);
  (void) this->implicit_ids_.bind (ACE_CString ("event_name",
                                                0,
                                                         0),
                                   EVENT_NAME);
  (void) this->implicit_ids_.bind (ACE_CString ("event_type",
                                                0,
                                                         0),
                                   EVENT_TYPE);
  (void) this->implicit_ids_.bind (ACE_CString ("domain_name",
                                                0,
                                                0),
                                   DOMAIN_NAME);
  (void) this->implicit_ids_.bind (ACE_CString ("type_name",
                                                0,
                                                0),
                                   TYPE_NAME);
}

int
TAO_Notify_Constraint_Visitor::bind_structured_event (
    const CosNotification::StructuredEvent &s_event
  )
{
  // The two sequences contained in a structured event are
  // copied into hash tables so iteration is done only once.

  CORBA::ULong length = s_event.filterable_data.length ();
  CORBA::ULong index = 0;

  for (index = 0; index < length; ++index)
    {
      ACE_CString name_str (s_event.filterable_data[index].name, 0, 0);

      int status =
        this->filterable_data_.bind (
            name_str,
            ACE_const_cast (CORBA::Any *,
                            &s_event.filterable_data[index].value)
          );

      if (status != 0)
        {
          return -1;
        }
    }

  length = s_event.header.variable_header.length ();

  for (index = 0; index < length; ++index)
    {
      ACE_CString name_str (s_event.header.variable_header[index].name, 0, 0);

      int status =
        this->variable_header_.bind (
            name_str,
            ACE_const_cast (CORBA::Any *,
                            &s_event.header.variable_header[index].value)
          );

      if (status != 0)
        {
          return -1;
        }
    }

  this->domain_name_ = 
    CORBA::string_dup (s_event.header.fixed_header.event_type.domain_name);

  this->type_name_ = 
    CORBA::string_dup (s_event.header.fixed_header.event_type.type_name);

  this->event_name_ =
    CORBA::string_dup (s_event.header.fixed_header.event_name);

  this->remainder_of_body_ = s_event.remainder_of_body;

  return 0;
}

CORBA::Boolean
TAO_Notify_Constraint_Visitor::evaluate_constraint (
    TAO_ETCL_Constraint* root
  )
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
TAO_Notify_Constraint_Visitor::visit_literal (
    TAO_ETCL_Literal_Constraint *literal
  )
{
  this->queue_.enqueue_head (*literal);
  return 0;
}

int
TAO_Notify_Constraint_Visitor::visit_identifier (TAO_ETCL_Identifier *ident)
{
  int return_value = -1;
  const char *name = ident->value ();
  ACE_CString key (name, 0, 0);

  CORBA::Any *any = 0;

  if (this->filterable_data_.find (key, any) == 0)
    {
      if (any != 0)
        {
          this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (any));
          return_value = 0;
        }
    }

  return return_value;
}

int
TAO_Notify_Constraint_Visitor::visit_union_value (
    TAO_ETCL_Union_Value *union_value
  )
{
  switch (union_value->sign ())
  {
    case 0:
      this->queue_.enqueue_head (*union_value->string ());
      break;
    case -1:
      this->queue_.enqueue_head (-(*union_value->integer ()));
      break;
    case 1:
      this->queue_.enqueue_head (*union_value->integer ());
      break;
    default:
      return -1;
  }

  return 0;
}

int
TAO_Notify_Constraint_Visitor::visit_union_pos (
    TAO_ETCL_Union_Pos *union_pos
  )
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (union_pos->union_value ()->accept (this) == 0)
        {
          TAO_ETCL_Literal_Constraint disc_val;
          this->queue_.dequeue_head (disc_val);

          TAO_DynUnion_i dyn_union;
          dyn_union.init (this->current_value_.in ()
                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::TypeCode_var tc = this->current_value_->type ();

          switch (disc_val.expr_type ())
          {
            case TAO_ETCL_INTEGER:
            case TAO_ETCL_SIGNED:
            case TAO_ETCL_UNSIGNED:
              {
                CORBA::Any disc_any;
                CORBA::TypeCode_var disc_tc =
                  tc->discriminator_type (ACE_ENV_SINGLE_ARG_PARAMETER);
                ACE_TRY_CHECK;
                CORBA::TCKind disc_kind =
                  TAO_DynAnyFactory::unalias (disc_tc.in ()
                                              ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK;

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
                      disc_any._tao_replace (disc_tc.in (),
                                             TAO_ENCAP_BYTE_ORDER,
                                             cdr.begin ());
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
                  TAO_DynAnyFactory::make_dyn_any (disc_any
                                                   ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK;
                dyn_union.set_discriminator (dyn_any.in ()
                                             ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK;
                DynamicAny::DynAny_var u_member =
                  dyn_union.member (ACE_ENV_SINGLE_ARG_PARAMETER);
                ACE_TRY_CHECK;
                this->current_value_ =
                  u_member->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
                ACE_TRY_CHECK;

                break;
              }
            case TAO_ETCL_STRING:
              {
                const char *name = (const char *) disc_val;
                CORBA::ULong count =
                  tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
                ACE_TRY_CHECK;

                const char *member_name = 0;
                CORBA::ULong i = 0;

                for (i = 0; i < count; ++i)
                  {
                    member_name = tc->member_name (i
                                                   ACE_ENV_ARG_PARAMETER);
                    ACE_TRY_CHECK;

                    if (ACE_OS::strcmp (name, member_name) == 0)
                      {
                        break;
                      }
                  }

                // If there's no match, member_label will throw
                // CORBA::TypeCode::Bounds and the catch block will
                // return -1;
                this->current_value_ = tc->member_label (i
                                                          ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK;

                break;
              }
            // The TAO_ETCL_Union_Value that was put on the queue
            // shouldn't have any other type.
            default:
              return -1;
          }

          TAO_ETCL_Constraint *nested = union_pos->component ();

          // If there's no nested component, then we just want the
          // union member value on the queue. Otherwise, we want
          // the member value in current_value_ while we visit
          // the nested component.
          if (nested == 0)
            {
              TAO_ETCL_Literal_Constraint lit (this->current_value_);
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
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  ACE_NOTREACHED (return 0);
}

int
TAO_Notify_Constraint_Visitor::visit_component_pos (
    TAO_ETCL_Component_Pos *pos
  )
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // If we are here (from visit_component) the Any containing the
      // component as found in filterable_data_ will be in current_value_.
      CORBA::TypeCode_var tc = this->current_value_->type ();
      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ()
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      DynamicAny::DynAny_var member;
      CORBA::Boolean success = 0;
      CORBA::ULong slot = (CORBA::ULong) *pos->integer ();

      switch (kind)
      {
        case CORBA::tk_enum:
          {
            TAO_DynEnum_i dyn_enum;
            dyn_enum.init (this->current_value_.in ()
                           ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            success = dyn_enum.seek (slot
                                     ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            if (success == 0)
              {
                return -1;
              }

            member =
              dyn_enum.current_component (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            break;
          }
        case CORBA::tk_struct:
          {
            TAO_DynStruct_i dyn_struct;
            dyn_struct.init (this->current_value_.in ()
                             ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            success = dyn_struct.seek (slot
                                       ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            if (success == 0)
              {
                return -1;
              }

            member = dyn_struct.current_component (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            break;
          }
        // @@@ (JP) I think enums and structs are the only two cases handled
        // by Component_Pos, since arrays and sequences are handled by
        // Component_Array, and unions are handled by Union_Pos.
        default:
          return -1;
      }

      CORBA::Any_var value = member->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_ETCL_Constraint *comp = pos->component ();

      if (comp == 0)
        {
          TAO_ETCL_Literal_Constraint result (value);
          this->queue_.enqueue_head (result);
          return 0;
        }
      else
        {
          this->current_value_ = value._retn ();
          return comp->accept (this);
        }
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  ACE_NOTREACHED (return 0);
}

int
TAO_Notify_Constraint_Visitor::visit_component_assoc (
    TAO_ETCL_Component_Assoc *assoc
  )
{
  CORBA::Any *any = 0;
  ACE_CString name (assoc->identifier ()->value (), 
                    0, 
                    0);

  switch (this->implicit_id_)
  {
    case FILTERABLE_DATA:
      if (this->filterable_data_.find (name, any) != 0
          || any == 0)
        {
          return -1;
        }

      break;
    case VARIABLE_HEADER:
      if (this->variable_header_.find (name, any) != 0
          || any == 0)
        {
          return -1;
        }

      break;
    // Only the sequence members of CosNotification::StructuredEvent can be
    // treated as associative arrays.
    default:
      return -1;
  }

  TAO_ETCL_Constraint *comp = assoc->component ();
  CORBA::Any *any_ptr = 0;

  if (comp == 0)
    {
      TAO_ETCL_Literal_Constraint result (any);
      this->queue_.enqueue_head (result);

      // If we're at the end of the line, put the name into
      // current_value_ so visit_exist can use it.
      ACE_NEW_RETURN (any_ptr,
                      CORBA::Any,
                      -1);
      (*any_ptr) <<= name.c_str ();
      this->current_value_ = any_ptr;

      return 0;
    }
  else
    {
      ACE_NEW_RETURN (any_ptr,
                      CORBA::Any (*any),
                      -1);
      this->current_value_ = any_ptr;
      return comp->accept (this);
    }
}

int
TAO_Notify_Constraint_Visitor::visit_component_array (
    TAO_ETCL_Component_Array *array
  )
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // If we are here (from visit_component) the Any containing the
      // component as found in filterable_data_ will be in current_value_.
      CORBA::TypeCode_var tc = this->current_value_->type ();
      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ()
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      DynamicAny::DynAny_var member;
      CORBA::Boolean success = 0;
      CORBA::ULong slot = (CORBA::ULong) *array->integer ();

      switch (kind)
      {
        case CORBA::tk_array:
          {
            TAO_DynEnum_i dyn_array;
            dyn_array.init (this->current_value_.in ()
                            ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            success = dyn_array.seek (slot
                                      ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            if (success == 0)
              {
                return -1;
              }

            member = dyn_array.current_component (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            break;
          }
        case CORBA::tk_sequence:
          {
            TAO_DynStruct_i dyn_sequence;
            dyn_sequence.init (this->current_value_.in ()
                               ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            success = dyn_sequence.seek (slot
                                         ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            if (success == 0)
              {
                return -1;
              }

            member =
              dyn_sequence.current_component (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            break;
          }
        // Enums and sequences are the only two cases handled
        // by Component_Array.
        default:
          return -1;
      }

      CORBA::Any_var value = member->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_ETCL_Constraint *comp = array->component ();

      if (comp == 0)
        {
          TAO_ETCL_Literal_Constraint result (value);
          this->queue_.enqueue_head (result);
          return 0;
        }
      else
        {
          this->current_value_ = value._retn ();
          return comp->accept (this);
        }
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  ACE_NOTREACHED (return 0);
}

int
TAO_Notify_Constraint_Visitor::visit_special (TAO_ETCL_Special *special)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::TypeCode_var tc = this->current_value_->type ();

      switch (special->type ())
      {
        case TAO_ETCL_LENGTH:
          {
            // If the TCKind is not a sequence or an array, the
            // call to length() will raise an exception, and the
            // catch block will return -1;
            CORBA::ULong length = tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            TAO_ETCL_Literal_Constraint lit (length);
            this->queue_.enqueue_head (lit);
            return 0;
          }
        case TAO_ETCL_DISCRIMINANT:
          {
            // If the TCKind is not a union, the
            // call to init() will raise an exception, and the
            // catch block will return -1;
            TAO_DynUnion_i dyn_union;
            dyn_union.init (this->current_value_.in ()
                            ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            DynamicAny::DynAny_var disc =
              dyn_union.get_discriminator (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            CORBA::Any_var disc_any = disc->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            TAO_ETCL_Literal_Constraint lit (disc_any);
            this->queue_.enqueue_head (lit);
            return 0;
          }
        case TAO_ETCL_TYPE_ID:
          {
            const char *name = tc->name (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            TAO_ETCL_Literal_Constraint lit (name);
            this->queue_.enqueue_head (lit);
            return 0;
          }
        case TAO_ETCL_REPOS_ID:
          {
            const char *id = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            TAO_ETCL_Literal_Constraint lit (id);
            this->queue_.enqueue_head (lit);
            return 0;
          }
        default:
          return -1;
      }
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  ACE_NOTREACHED (return 0);
}

int
TAO_Notify_Constraint_Visitor::visit_component (
    TAO_ETCL_Component *component
  )
{
  TAO_ETCL_Constraint *nested = component->component ();
  TAO_ETCL_Identifier *identifier = component->identifier ();
  ACE_CString component_name (identifier->value (),
                              0,
                              0);
  CORBA::Any *any_ptr = 0;

  if (this->implicit_ids_.find (component_name, this->implicit_id_) != 0)
    {
      this->implicit_id_ = NONE;
    }

  // If this component has no sub-component, only an identifier,
  // then we just visit the identifier, which puts a literal on
  // the queue to be handled upon returning from this method call.
  // If there is a sub-component, we store the literal's value
  // in our member _var for possible examination at a more
  // nested level, and visit the sub-component. If the identifier 
  // matches one of the nested field names in 
  // CosNotification::StructuredEvent, we just visit the nested
  // component, if any.
  if (this->implicit_id_ == NONE)
    {
      if (nested == 0)
        {
          // If this is the end of the line, we put the component name
          // into current_value_ so visit_exist can use it.
          ACE_NEW_RETURN (any_ptr,
                          CORBA::Any,
                          -1);
          (*any_ptr) <<= component_name.c_str ();
          this->current_value_ = any_ptr;
          return identifier->accept (this);
        }
      else
        {
          int result = identifier->accept (this);

          if (result != 0)
            {
              return result;
            }

          TAO_ETCL_Literal_Constraint id;
          this->queue_.dequeue_head (id);
          ACE_NEW_RETURN (any_ptr,
                          CORBA::Any (*(const CORBA::Any *) id),
                          -1);
          this->current_value_ = any_ptr;
        }
    }

  if (nested != 0)
    {
      return nested->accept (this);
    }
  else
    {
      switch (this->implicit_id_)
      {
        case TYPE_NAME:
          {
            TAO_ETCL_Literal_Constraint tn (this->type_name_.in ());
            this->queue_.enqueue_head (tn);
            return 0;
          }
        case EVENT_NAME:
          {
            TAO_ETCL_Literal_Constraint en (this->event_name_.in ());
            this->queue_.enqueue_head (en);
            return 0;
          }
        case DOMAIN_NAME:
          {
            TAO_ETCL_Literal_Constraint dn (this->domain_name_.in ());
            this->queue_.enqueue_head (dn);
            return 0;
          }
        case REMAINDER_OF_BODY:
          {
            TAO_ETCL_Literal_Constraint rob (&this->remainder_of_body_);
            this->queue_.enqueue_head (rob);
            return 0;
          }
        // The above cases are the leaves of the 
        // CosNotification::StructuredEvent "tree". Anything else and we
        // should have a nested component. otherwise, it's an error.
        default:
          return -1;
      }
    }
}

int
TAO_Notify_Constraint_Visitor::visit_dot (TAO_ETCL_Dot *dot)
{
  // If we are here, we know we're headed for a more nested
  // level, so we just visit it, there's nothing else in this
  // constraint.
  return dot->component ()->accept (this);
}

int
TAO_Notify_Constraint_Visitor::visit_eval (TAO_ETCL_Eval *eval)
{
  // Nothing to do but visit the contained component.
  return eval->component ()->accept (this);
}

int
TAO_Notify_Constraint_Visitor::visit_default (TAO_ETCL_Default *def)
{
  TAO_ETCL_Constraint *comp = def->component ();

  if (comp == 0)
    {
      return -1;
    }

  if (comp->accept (this) != 0)
    {
      return -1;
    }

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::TypeCode_var tc = this->current_value_->type ();

      // If the current member is not a union, this call will
      // throw BadKind and the catch block will return -1.
      CORBA::Long default_index = tc->default_index (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // No default index.
      if (default_index == -1)
        {
          TAO_ETCL_Literal_Constraint result ((CORBA::Boolean) 0);
          this->queue_.enqueue_head (result);
          return 0;
        }

      // Okay, there's a default index, but is it active?

      TAO_ETCL_Literal_Constraint disc;
      this->queue_.dequeue_head (disc);
      TAO_ETCL_Literal_Constraint default_index_value (default_index);
      return (disc == default_index_value);
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  ACE_NOTREACHED (return 0);
}

int
TAO_Notify_Constraint_Visitor::visit_exist (TAO_ETCL_Exist *exist)
{
  TAO_ETCL_Constraint *component = exist->component ();

  if (component->accept (this) == 0)
    {
      const char *value = 0;
      CORBA::Boolean result = 0;

      // For the two cases below, we don't want the item at the top of
      // the queue, because it's the result of a hash table lookup. For
      // an existence test, we want the key value, which is stored in
      // the current_value_ member.
      if (this->implicit_id_ == FILTERABLE_DATA
          || this->implicit_id_ == VARIABLE_HEADER)
        {
          TAO_ETCL_Literal_Constraint current (
                                          &this->current_value_.inout ()
                                        );
          value = CORBA::string_dup ((const char *) current);
        }

      switch (this->implicit_id_)
      {
        case FILTERABLE_DATA:
          result = 
            (this->filterable_data_.find (ACE_CString (value, 0, 0)) == 0);
          break;
        case VARIABLE_HEADER:
          result = 
            (this->variable_header_.find (ACE_CString (value, 0, 0)) == 0);
          break;
        case TYPE_NAME:
          result = (this->type_name_.in () != 0);
          break;
        case EVENT_NAME:
          result = (this->event_name_.in () != 0);
          break;
        case DOMAIN_NAME:
          result = (this->domain_name_.in () != 0);
          break;
        // Anything other than the above cases is an error.
        default:
          return -1;
      }

      this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));

      return 0;
    }

  return -1;
}

int
TAO_Notify_Constraint_Visitor::visit_unary_expr (
    TAO_ETCL_Unary_Expr *unary_expr
  )
{
  TAO_ETCL_Constraint *subexpr = unary_expr->subexpr ();

  if (subexpr->accept (this) == 0)
    {
      TAO_ETCL_Literal_Constraint subexpr_result;
      CORBA::Boolean result = 0;
      int op_type = unary_expr->type ();

      switch (op_type)
      {
        case TAO_ETCL_NOT:
          this->queue_.dequeue_head (subexpr_result);
          result = ! (CORBA::Boolean) subexpr_result;
          this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
          return 0;
        case TAO_ETCL_MINUS:
          // The leading '-' was parsed separately, so we have to pull
          // the literal constraint off the queue, apply the class' own
          // unary minus operator, and put it back.
          this->queue_.dequeue_head (subexpr_result);
          this->queue_.enqueue_head (-subexpr_result);
          return 0;
        case TAO_ETCL_PLUS:
          // Leave the literal constraint on the queue. The leading
          // '+' was just syntactic sugar - no action is necessary.
          return 0;
        default:
          // The parser should never construct a TAO_ETCL_Unary_Constraint
          // behind any operators except the above three.
          return -1;
      }
    }

  return -1;
}

int
TAO_Notify_Constraint_Visitor::visit_binary_expr (
    TAO_ETCL_Binary_Expr *binary_expr
  )
{
  int bin_op_type = binary_expr->type ();

  switch (bin_op_type)
  {
    case TAO_ETCL_OR:
      return this->visit_or (binary_expr);
    case TAO_ETCL_AND:
      return this->visit_and (binary_expr);
    case TAO_ETCL_LT:
    case TAO_ETCL_LE:
    case TAO_ETCL_GT:
    case TAO_ETCL_GE:
    case TAO_ETCL_EQ:
    case TAO_ETCL_NE:
    case TAO_ETCL_PLUS:
    case TAO_ETCL_MINUS:
    case TAO_ETCL_MULT:
    case TAO_ETCL_DIV:
      return this->visit_binary_op (binary_expr,
                                    bin_op_type);
    case TAO_ETCL_TWIDDLE:
      return this->visit_twiddle (binary_expr);
    case TAO_ETCL_IN:
      return this->visit_in (binary_expr);
    default:
      return -1;
  }
}

int
TAO_Notify_Constraint_Visitor::visit_or (
    TAO_ETCL_Binary_Expr *binary
  )
{
  int return_value = -1;
  CORBA::Boolean result = 0;
  TAO_ETCL_Constraint *lhs = binary->lhs ();

  if (lhs->accept (this) == 0)
    {
      TAO_ETCL_Literal_Constraint lhs_result;
      this->queue_.dequeue_head (lhs_result);
      result = (CORBA::Boolean) lhs_result;

      // Short-circuiting OR.
      if (result == 0)
        {
          TAO_ETCL_Constraint *rhs = binary->rhs ();

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
TAO_Notify_Constraint_Visitor::visit_and (
    TAO_ETCL_Binary_Expr *binary
  )
{
  int return_value = -1;
  CORBA::Boolean result = 0;
  TAO_ETCL_Constraint *lhs = binary->lhs ();

  if (lhs->accept (this) == 0)
    {
      TAO_ETCL_Literal_Constraint lhs_result;
      this->queue_.dequeue_head (lhs_result);
      result = (CORBA::Boolean) lhs_result;

      // Short-circuiting AND.
      if (result == 1)
        {
          TAO_ETCL_Constraint *rhs = binary->rhs ();

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
TAO_Notify_Constraint_Visitor::visit_binary_op (
    TAO_ETCL_Binary_Expr *binary,
    int op_type
  )
{
  int return_value = -1;
  TAO_ETCL_Constraint *lhs = binary->lhs ();
  CORBA::Boolean result = 0;

  // Perform an operation on the results of evaluating the left and
  // right branches of this subtree.
  if (lhs->accept (this) == 0)
    {
      TAO_ETCL_Literal_Constraint left_operand;
      this->queue_.dequeue_head (left_operand);
      TAO_ETCL_Constraint *rhs = binary->rhs ();

      if (rhs->accept (this) == 0)
        {
          TAO_ETCL_Literal_Constraint right_operand;
          this->queue_.dequeue_head (right_operand);
          return_value = 0;

          switch (op_type)
          {
            case TAO_ETCL_LT:
              result = left_operand < right_operand;
              this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
              break;
            case TAO_ETCL_LE:
              result = left_operand <= right_operand;
              this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
              break;
            case TAO_ETCL_GT:
              result = left_operand > right_operand;
              this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
              break;
            case TAO_ETCL_GE:
              result = left_operand >= right_operand;
              this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
              break;
            case TAO_ETCL_EQ:
              result = left_operand == right_operand;
              this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
              break;
            case TAO_ETCL_NE:
              result = left_operand != right_operand;
              this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
              break;
            case TAO_ETCL_PLUS:
              this->queue_.enqueue_head (left_operand + right_operand);
              break;
            case TAO_ETCL_MINUS:
              this->queue_.enqueue_head (left_operand - right_operand);
              break;
            case TAO_ETCL_MULT:
              this->queue_.enqueue_head (left_operand * right_operand);
              break;
            case TAO_ETCL_DIV:
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
TAO_Notify_Constraint_Visitor::visit_twiddle (
    TAO_ETCL_Binary_Expr *binary
  )
{
  int return_value = -1;
  TAO_ETCL_Constraint *lhs = binary->lhs ();

  // Determine if the left operand is a substring of the right.
  if (lhs->accept (this) == 0)
    {
      TAO_ETCL_Literal_Constraint left;
      this->queue_.dequeue_head (left);
      TAO_ETCL_Constraint *rhs = binary->rhs ();

      if (rhs->accept (this) == 0)
        {
          TAO_ETCL_Literal_Constraint right;
          this->queue_.dequeue_head (right);
          CORBA::Boolean result =
            (ACE_OS::strstr ((const char *) right,
                             (const char *) left) != 0);
          this->queue_.enqueue_head (TAO_ETCL_Literal_Constraint (result));
          return_value = 0;
        }
    }

  return return_value;
}

int
TAO_Notify_Constraint_Visitor::visit_in (
    TAO_ETCL_Binary_Expr *binary
  )
{
  int return_value = -1;
  TAO_ETCL_Constraint *lhs = binary->lhs ();

  // Determine if the left operand is contained in the right.

  if (lhs->accept (this) == 0)
    {
      TAO_ETCL_Literal_Constraint left;
      this->queue_.dequeue_head (left);

      TAO_ETCL_Constraint *rhs = binary->rhs ();

      if (rhs->accept (this) == 0)
        {
          TAO_ETCL_Literal_Constraint bag;
          this->queue_.dequeue_head (bag);

          if (bag.expr_type () == TAO_ETCL_COMPONENT)
            {
              const CORBA::Any *component = (const CORBA::Any *) bag;
              CORBA::TCKind kind = CORBA::tk_null;

              ACE_DECLARE_NEW_CORBA_ENV;
              ACE_TRY
                {
                  CORBA::TypeCode_var tc = component->type ();
                  kind = TAO_DynAnyFactory::unalias (tc.in ()
                                                     ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;
                }
              ACE_CATCHANY
                {
                  return return_value;
                }
              ACE_ENDTRY;

              CORBA::Boolean result = 0;

              switch (kind)
              {
                case CORBA::tk_sequence:
                  result = this->sequence_does_contain (component,
                                                        left);
                  break;
                case CORBA::tk_array:
                  result = this->array_does_contain (component,
                                                     left);
                  break;
                case CORBA::tk_struct:
                  result = this->struct_does_contain (component,
                                                      left);
                  break;
                case CORBA::tk_union:
                  result = this->union_does_contain (component,
                                                     left);
                  break;
                case CORBA::tk_any:
                  result = this->any_does_contain (component,
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
TAO_Notify_Constraint_Visitor::visit_preference (
    TAO_ETCL_Preference *
  )
{
  // According to OMG 00-06-20 section 2.4.1, the Notification Service
  // does not use the preference operators. The method must be here
  // because it is called by the ETCL node, which may be used by other
  // CORBA services that do use the preference operators.
  return -1;
}

CORBA::Boolean
TAO_Notify_Constraint_Visitor::sequence_does_contain (
    const CORBA::Any *any,
    TAO_ETCL_Literal_Constraint &item
  )
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::TypeCode_var type = any->type ();
      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (type.in ()
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The literal and the array elements must be
      // of the same simple type.
      CORBA::Boolean match = this->simple_type_match (item.expr_type (),
                                                      kind);

      if (match == 0)
        {
          return 0;
        }

      TAO_DynSequence_i dyn_seq;
      dyn_seq.init (*any
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      DynamicAny::AnySeq_var any_seq =
        dyn_seq.get_elements (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ULong length = any_seq->length ();

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          TAO_ETCL_Literal_Constraint element (&any_seq[i]);

          if (item == element)
            {
              return 1;
            }
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
TAO_Notify_Constraint_Visitor::array_does_contain (
    const CORBA::Any *any,
    TAO_ETCL_Literal_Constraint &item
  )
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::TypeCode_var type = any->type ();
      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (type.in ()
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The literal and the array elements must be
      // of the same simple type.
      CORBA::Boolean match = this->simple_type_match (item.expr_type (),
                                                      kind);

      if (match == 0)
        {
          return 0;
        }

      TAO_DynArray_i dyn_array;
      dyn_array.init (*any
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      DynamicAny::AnySeq_var any_seq =
        dyn_array.get_elements (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ULong length = any_seq->length ();

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          TAO_ETCL_Literal_Constraint element (&any_seq[i]);

          if (item == element)
            {
              return 1;
            }
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
TAO_Notify_Constraint_Visitor::struct_does_contain (
    const CORBA::Any *any,
    TAO_ETCL_Literal_Constraint &item
  )
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_DynStruct_i dyn_struct;
      dyn_struct.init (*any
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      DynamicAny::NameValuePairSeq_var members =
        dyn_struct.get_members (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ULong length = members->length ();
      CORBA::TypeCode_var tc;
      CORBA::TCKind kind;

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          tc = members[i].value.type ();
          kind = TAO_DynAnyFactory::unalias (tc.in ()
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // The literal and the struct member must be
          // of the same simple type.
          CORBA::Boolean match = this->simple_type_match (item.expr_type (),
                                                          kind);

          if (match == 0)
            {
              continue;
            }

          TAO_ETCL_Literal_Constraint element (&members[i].value);

          if (item == element)
            {
              return 1;
            }
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
TAO_Notify_Constraint_Visitor::union_does_contain (
    const CORBA::Any *any,
    TAO_ETCL_Literal_Constraint &item
  )
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_DynUnion_i dyn_union;
      dyn_union.init (*any
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      DynamicAny::DynAny_var cc =
        dyn_union.current_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Any_var member = cc->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_var tc = member->type ();
      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ()
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The literal and the union member must be
      // of the same simple type.
      CORBA::Boolean match = this->simple_type_match (item.expr_type (),
                                                      kind);

      if (match == 0)
        {
          return 0;
        }

      TAO_ETCL_Literal_Constraint element (&member.inout ());

      return (item == element);
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;

  ACE_NOTREACHED (return 0);
}

CORBA::Boolean
TAO_Notify_Constraint_Visitor::any_does_contain (
    const CORBA::Any *any,
    TAO_ETCL_Literal_Constraint &item
  )
{
  const CORBA::Any *result = 0;

  *any >>= result;

  TAO_ETCL_Literal_Constraint element (ACE_const_cast (CORBA::Any *,
                                                       result));

  return (item == element);
}

CORBA::Boolean
TAO_Notify_Constraint_Visitor::simple_type_match (int expr_type,
                                                  CORBA::TCKind tc_kind)
{
  switch (expr_type)
  {
    case TAO_ETCL_STRING:
      if (tc_kind != CORBA::tk_string)
        {
          return 0;
        }
    case TAO_ETCL_DOUBLE:
      if (tc_kind != CORBA::tk_double
          && tc_kind != CORBA::tk_float)
        {
          return 0;
        }
    case TAO_ETCL_INTEGER:
    case TAO_ETCL_SIGNED:
      if (tc_kind != CORBA::tk_short
          && tc_kind != CORBA::tk_long
          && tc_kind != CORBA::tk_longlong)
        {
          return 0;
        }
    case TAO_ETCL_UNSIGNED:
      if (tc_kind != CORBA::tk_ushort
          && tc_kind != CORBA::tk_ulong
          && tc_kind != CORBA::tk_ulonglong)
        {
          return 0;
        }
    case TAO_ETCL_BOOLEAN:
      if (tc_kind != CORBA::tk_boolean)
        {
          return 0;
        }
    default:
      return 0;
  }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Manager<ACE_CString, CORBA::Any*, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Entry <ACE_CString, CORBA::Any*>;
template class ACE_Hash_Map_Iterator<ACE_CString, CORBA::Any*, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, CORBA::Any*, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;
template class ACE_Node<TAO_ETCL_Literal_Constraint>;
template class ACE_Unbounded_Queue<TAO_ETCL_Literal_Constraint>;
template class ACE_Unbounded_Queue_Iterator<TAO_ETCL_Literal_Constraint>;


template class ACE_Hash_Map_Manager<ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Entry <ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field>;
template class ACE_Hash_Map_Iterator<ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, CORBA::Any*, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Entry <ACE_CString, CORBA::Any*>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString, CORBA::Any*, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, CORBA::Any*, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Node<TAO_ETCL_Literal_Constraint>
#pragma instantiate ACE_Unbounded_Queue<TAO_ETCL_Literal_Constraint>
#pragma instantiate ACE_Unbounded_Queue_Iterator<TAO_ETCL_Literal_Constraint>

#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field, TAO_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Entry <ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field>;
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field, TAO_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field, TAO_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_Notify_Constraint_Visitor::structured_event_field, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
