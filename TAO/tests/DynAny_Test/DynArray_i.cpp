// $Id$

// ====================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    DynArray_i.cpp
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ====================================================================

#include "DynAny_i.h"
#include "DynArray_i.h"

//////////////////////////////////////////////////////////////////////
// Constructors and destructor

DynArray_i::DynArray_i (const CORBA_Any& any)
  : type_ (CORBA::TypeCode::_duplicate (any.type ())),
    index_ (0),
    da_members_ (0)
{
  CORBA::Environment env;
  CORBA::TypeCode_ptr tc = this->type_.in ();

  // The type will be correct if this constructor called from a 
  // factory function, but it could also be called by the 
  // user, so..... 
  if (DynAny_i::unalias (tc, env) == CORBA::tk_array)
    {
      CORBA::ULong numfields = this->get_arg_length (any.type (),
                                                     env);
      
      // Resize the array.
      this->da_members_ = ACE_Array<DynAny_var> (numfields);

      // Get the CDR stream of the argument.
      ACE_Message_Block* mb =
        ACE_Message_Block::duplicate (any._tao_get_cdr ());

      TAO_InputCDR cdr (mb);

      CORBA::TypeCode_ptr field_tc = this->get_element_type (env);

      for (CORBA::ULong i = 0; i < numfields; i++)
        {
          // This Any constructor is a TAO extension.
          CORBA_Any field_any (field_tc,
                               cdr.start ());
          
          // This recursive step will call the correct constructor 
          // based on the type of field_any.
          this->da_members_[i] = 
            DynAny_i::create_dyn_any (field_any,
                                      env);

          // Move to the next field in the CDR stream.
          cdr.skip (field_tc);
        }
    }
  else
    env.exception (new DynAny_i::InconsistentTypeCode);
}

DynArray_i::DynArray_i (CORBA_TypeCode_ptr tc)
  : type_ (CORBA::TypeCode::_duplicate (tc)),
    index_ (0),
    da_members_ (0)
{
  CORBA::Environment env;

  // Need to check if called by user.
  if (DynAny_i::unalias (tc, env) == CORBA::tk_array)
    {
      CORBA::ULong numfields = this->get_arg_length (tc,
                                                     env);

      // Resize the array.
      this->da_members_ = ACE_Array<DynAny_var> (numfields);

      for (CORBA::ULong i = 0; i < numfields; i++)

        // With a typecode arg, we just create the top level.
        this->da_members_[i] = 0;
    }
  else
    env.exception (new DynAny_i::InconsistentTypeCode);      
}

DynArray_i::~DynArray_i (void)
{
}

///////////////////////////////////////////////////////////////////////
// Functions specific to DynArray

AnySeq_ptr 
DynArray_i::get_elements (CORBA::Environment& env)
{
  CORBA::ULong length = this->da_members_.size ();

  if (length == 0)
    return 0;

  // Arg only sets maximum, so...
  AnySeq_ptr elements = 
    new AnySeq (length);

  // ...we must do this explicitly.
  elements->length (length);

  // Initialize each Any.
  for (CORBA::ULong i = 0; i < length; i++)
    (*elements)[i] = *this->da_members_[i]->to_any (env);

  return elements;
}

void 
DynArray_i::set_elements (const AnySeq& value,
                          CORBA::Environment& env)
{
  CORBA::ULong length = value.length ();
  CORBA::ULong size = this->da_members_.size ();

  if (size == 0)
    // Resize the array, it hasn't yet been initialized.
    this->da_members_ = ACE_Array<DynAny_var> (length);
  else if (size != length)
    {
      env.exception (new DynAny::InvalidSeq);
      return;
    }
    
  CORBA::TypeCode_ptr element_type = this->get_element_type (env);

  for (CORBA::ULong i = 0; i < length; i++)
    {
      // Check each arg element for type match.
      if (value[i].type ()->equal (element_type, env))

        this->da_members_[i] = 
          DynAny_i::create_dyn_any (value[i], env);

      else
        {
          env.exception (new DynAny::InvalidSeq);
          break;
        }
    }
}

//////////////////////////////////////////////////////////////////////
// Common functions

void
DynArray_i::assign (DynAny_ptr dyn_any, CORBA::Environment &env)
{
  // *dyn_any->to_any raises Invalid if arg is bad.
  if (this->type_.in ()->equal (dyn_any->type (env), env))
    this->from_any (*dyn_any->to_any (env), env);
  else
    env.exception (new DynAny::Invalid);
}

DynAny_ptr
DynArray_i::copy (CORBA::Environment &env)
{
  CORBA_Any_ptr a = this->to_any (env);
  return DynAny_i::create_dyn_any (*a, env);
}

void
DynArray_i::destroy (CORBA::Environment &env)
{
  // Do a deep destroy
  for (CORBA::ULong i = 0; i < this->da_members_.size (); i++)
    this->da_members_[i]->destroy (env);

  // Free the top level
  CORBA::release (this->_this (env));
}

void
DynArray_i::from_any (const CORBA_Any& any, CORBA::Environment &env)
{
  if (this->type_.in ()->equal (any.type (), env))
    {
      // Get the CDR stream of the argument.
      ACE_Message_Block* mb =
        ACE_Message_Block::duplicate (any._tao_get_cdr ());
      TAO_InputCDR cdr (mb);

      CORBA::ULong length = this->da_members_.size ();
      CORBA::ULong arg_length = this->get_arg_length (any.type (),
                                                      env);

      if (length == 0)
        // Resize the array, it hasn't yet been initialized.
        this->da_members_ = ACE_Array<DynAny_var> (arg_length);
      else if (length != arg_length)
        {
          env.exception (new DynAny::Invalid);
          return;
        }
    
      CORBA::TypeCode_ptr field_tc = this->get_element_type (env);

      for (CORBA::ULong i = 0; i < arg_length; i++)
        {
          // This Any constructor is a TAO extension.
          CORBA_Any field_any (field_tc,
                               cdr.start ());
          
          // Actually a recursive step. Can't call from_any()
          // recursively because maybe only the top level is created,
          // but create_dyn_any will do the right thing.
          this->da_members_[i] = 
            DynAny_i::create_dyn_any (field_any,
                                      env);

          // Move to the next field in the CDR stream.
          cdr.skip (field_tc);
        }
    }
  else
    env.exception (new DynAny::Invalid);
}

CORBA::Any_ptr
DynArray_i::to_any (CORBA::Environment& env)
{
  TAO_OutputCDR out_cdr;

  CORBA_TypeCode_ptr field_tc = this->get_element_type (env);

  for (CORBA::ULong i = 0; i < this->da_members_.size (); i++)
    {
      // Each component must have been initialized.
      if (!this->da_members_[i].in ())
        {
          env.exception (new DynAny::Invalid);
          return 0;
        }

      // Recursive step
      CORBA_Any_ptr field_any = this->da_members_[i]->to_any (env);

      ACE_Message_Block* field_mb = 
        ACE_Message_Block::duplicate (field_any->_tao_get_cdr ());

      TAO_InputCDR field_cdr (field_mb);

      out_cdr.append (field_tc, &field_cdr, env);
    }

  TAO_InputCDR in_cdr (out_cdr);
  
  return new CORBA_Any (this->type (env),
                        in_cdr.start ());
}

CORBA::TypeCode_ptr
DynArray_i::type (CORBA::Environment &)
{
  return this->type_.in ();
}

// If the DynAny has been initialized but this component has not, the
// first call to current_component will create the pointer and return it.
DynAny_ptr
DynArray_i::current_component (CORBA::Environment &env)
{
  if (this->da_members_.size () == 0)
    return 0;

  if (!this->da_members_[this->index_].in ())
    this->da_members_[this->index_] = 
      DynAny_i::create_dyn_any (this->get_element_type (env), env);

  return this->da_members_[this->index_].in ();
}

CORBA::Boolean
DynArray_i::next (CORBA::Environment &)
{
  CORBA::Long size = (CORBA::Long) this->da_members_.size ();

  if (size == 0 || this->index_ + 1 == size)
    return 0;

  ++this->index_;
  return 1;
}

CORBA::Boolean
DynArray_i::seek (CORBA::Long index, CORBA::Environment &)
{
  if (index < 0 || index >= (CORBA::Long) this->da_members_.size ())
    return 0;

  this->index_ = index;
  return 1;
}

void
DynArray_i::rewind (CORBA::Environment &)
{
  this->index_ = 0;
}

//////////////////////////////////////////////////////////////////////////
// The insert-primitive and get-primitive functions are required 
// by the spec of all types of DynAny, although if the top level
// members aren't primitive types, these functions aren't too helpful.
// Also, while not mentioned in the spec, the example code seems to
// indicate that next() is called in the body of each of these, and
// it has been so implemented here.

//************************INSERT FUNCTIONS****************************

void
DynArray_i::insert_boolean (CORBA::Boolean value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_boolean)
    {
      this->current_component (env)->insert_boolean (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_octet (CORBA::Octet value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_octet)
    {
      this->current_component (env)->insert_octet (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_char (CORBA::Char value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_char)
    {
      this->current_component (env)->insert_char (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_short (CORBA::Short value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_short)
    {
      this->current_component (env)->insert_short (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_ushort (CORBA::UShort value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_ushort)
    {
      this->current_component (env)->insert_ushort (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_long (CORBA::Long value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_long)
    {
      this->current_component (env)->insert_long (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_ulong (CORBA::ULong value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_long)
    {
      this->current_component (env)->insert_ulong (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_float (CORBA::Float value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_float)
    {
      this->current_component (env)->insert_float (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_double (CORBA::Double value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_double)
    {
      this->current_component (env)->insert_double (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_string (char * value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_string)
    {
      this->current_component (env)->insert_string (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_reference (CORBA::Object_ptr value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_objref)
    {
      this->current_component (env)->insert_reference (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_typecode (CORBA::TypeCode_ptr value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_TypeCode)
    {
      this->current_component (env)->insert_typecode (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_longlong (CORBA::LongLong value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_longlong)
    {
      this->current_component (env)->insert_longlong (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_ulonglong (CORBA::ULongLong value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_ulonglong)
    {
      this->current_component (env)->insert_ulonglong (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_wchar (CORBA::WChar value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_wchar)
    {
      this->current_component (env)->insert_wchar (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynArray_i::insert_any (CORBA::Any& value, CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_any)
    {
      this->current_component (env)->insert_any (value, env);
      this->next (env);
    }
  else
    env.exception (new DynAny::InvalidValue);
}

//*************************GET FUNCTIONS**********************

// If the current component has not been intialized, these
// raise Invalid, which is not required by the spec, but which 
// seems like a courteous thing to do.

CORBA::Boolean
DynArray_i::get_boolean (CORBA::Environment &env)
{
  CORBA::Boolean val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_boolean)
        {
          val = dp->get_boolean (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);
  
  return val;
}

CORBA::Octet
DynArray_i::get_octet (CORBA::Environment &env)
{
  CORBA::Octet val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_octet)
        {
          val = dp->get_octet (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

CORBA::Char
DynArray_i::get_char (CORBA::Environment &env)
{
  CORBA::Char val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_char)
        {
          val = dp->get_char (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

CORBA::Short
DynArray_i::get_short (CORBA::Environment &env)
{
  CORBA::Short val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_short)
        {
          val = dp->get_short (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

CORBA::UShort
DynArray_i::get_ushort (CORBA::Environment &env)
{
  CORBA::UShort val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_ushort)
        {
          val = dp->get_ushort (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

CORBA::Long
DynArray_i::get_long (CORBA::Environment &env)
{
  CORBA::Long val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_long)
        {
          val = dp->get_long (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

CORBA::ULong
DynArray_i::get_ulong (CORBA::Environment &env)
{
  CORBA::ULong val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_ulong)
        {
          val = dp->get_ulong (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

CORBA::Float
DynArray_i::get_float (CORBA::Environment &env)
{
  CORBA::Float val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_float)
        {
          val = dp->get_float (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

CORBA::Double
DynArray_i::get_double (CORBA::Environment &env)
{
  CORBA::Double val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_double)
        {
          val = dp->get_double (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

char *
DynArray_i::get_string (CORBA::Environment &env)
{
  CORBA::Char *val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_string)
        {
          val = dp->get_string (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

CORBA::Object_ptr
DynArray_i::get_reference (CORBA::Environment &env)
{
  CORBA_Object_ptr val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (dp->type (env)->kind (env) == CORBA::tk_objref)
        {
          val = dp->get_reference (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

CORBA::TypeCode_ptr
DynArray_i::get_typecode (CORBA::Environment &env)
{
  CORBA_TypeCode_ptr val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_TypeCode)
        {
          val = dp->get_typecode (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

CORBA::LongLong
DynArray_i::get_longlong (CORBA::Environment &env)
{
  CORBA::LongLong val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_longlong)
        {
          val = dp->get_longlong (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

CORBA::ULongLong
DynArray_i::get_ulonglong (CORBA::Environment &env)
{
  CORBA::ULongLong val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_ulonglong)
        {
          val = dp->get_ulonglong (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

CORBA::WChar
DynArray_i::get_wchar (CORBA::Environment &env)
{
  CORBA::WChar val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_wchar)
        {
          val = dp->get_wchar (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

CORBA::Any_ptr
DynArray_i::get_any (CORBA::Environment &env)
{
  CORBA_Any_ptr val;
  DynAny_ptr dp = this->da_members_[this->index_];

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_any)
        {
          val = dp->get_any (env);
          this->next (env);
        }
      else
        env.exception (new DynAny::TypeMismatch);
    }
  else
    env.exception (new DynAny::Invalid);

  return val;
}

/////////////////////////////////////////////////////////////////////////////////

// Private utility function.
CORBA::TypeCode_ptr
DynArray_i::get_element_type (CORBA::Environment& env)
{
  CORBA::TypeCode_ptr element_type = this->type_.in ();

  // Strip away aliases (if any) on top of the outer type
  while (element_type->kind (env) != CORBA::tk_array)
    element_type = element_type->content_type (env);

  // Return the content type.
  return element_type->content_type (env);
}

// Get the argument length from the (aliased) typecode.
CORBA::ULong
DynArray_i::get_arg_length (CORBA::TypeCode_ptr tc,
                            CORBA::Environment &env)
{
  while (tc->kind (env) == CORBA::tk_alias)
    tc = tc->content_type (env);

  return tc->length (env);
}

