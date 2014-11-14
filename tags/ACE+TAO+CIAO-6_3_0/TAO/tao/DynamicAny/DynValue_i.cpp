// -*- C++ -*-
// $Id$

#include "tao/DynamicAny/DynValue_i.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "tao/DynamicAny/DynAnyUtils_T.h"

#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"

#include "tao/CDR.h"
#include "tao/Valuetype/ValueBase.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynValue_i::TAO_DynValue_i (CORBA::Boolean allow_truncation)
  : TAO_DynCommon (allow_truncation)
  , TAO_DynAny_i (allow_truncation)
  , TAO_DynValueCommon_i (allow_truncation)
{
}

TAO_DynValue_i::~TAO_DynValue_i (void)
{
}

void
TAO_DynValue_i::init (const CORBA::Any & any)
{
  this->init_helper (any._tao_get_typecode ());
  this->from_any_helper (any);

  // init() is called only from DynAnyFactory create_*
  // methods. These are the only functions that can
  // throw MustTruncate, so once init () has completed
  // revert to always allowing truncation.
  this->allow_truncation_ = true;
}

void
TAO_DynValue_i::init (CORBA::TypeCode_ptr tc, TAO_InputCDR &in)
{
  this->init_helper (tc);
  this->from_inputCDR (in);

  // init() is called only from DynAnyFactory create_*
  // methods. These are the only functions that can
  // throw MustTruncate, so once init () has completed
  // revert to always allowing truncation.
  this->allow_truncation_ = true;
}

void
TAO_DynValue_i::init (CORBA::TypeCode_ptr tc)
{
  this->init_helper (tc);

  // Each member is now (recursively) set-up
  // with its own correct typecode.

  for (CORBA::ULong i = 0u;
       i < this->component_count_;
       ++i)
    {
      CORBA::TypeCode_var
        member_type (
          get_member_type (this->da_base_types_, i));

      this->da_members_[i] = TAO::MakeDynAnyUtils::
        make_dyn_any_t<CORBA::TypeCode_ptr>
          (member_type.in (), member_type.in (), this->allow_truncation_);
    }

  this->set_to_null ();

  // init() is called only from DynAnyFactory create_*
  // methods. These are the only functions that can
  // throw MustTruncate, so once init () has completed
  // revert to always allowing truncation.
  this->allow_truncation_ = true;
}

void
TAO_DynValue_i::init_helper (CORBA::TypeCode_ptr tc)
{
  // Ensure we have been given a valid ValueType
  // typecode and then store a copy of the original.

  this->check_typecode (tc);
  this->type_ = CORBA::TypeCode::_duplicate (tc);

  // Work out how many total members and types there
  // are in total in this derived->base hiarchy.

  get_base_types (
    tc,
    this->da_base_types_,
    &this->component_count_);
  this->da_members_.size (this->component_count_);

  // And initalize all of the DynCommon mix-in

  this->init_common ();
}

void
TAO_DynValue_i::get_base_types (
  CORBA::TypeCode_ptr tc,
  BaseTypesList_t &base_types,
  CORBA::ULong *total_member_count)
{
  // First initalize to the fully derived type we are
  // starting with.

  CORBA::ULong numberOfBases = 1u;
  base_types.size (numberOfBases);
  base_types[0] = TAO_DynAnyFactory::strip_alias (tc);
  if (total_member_count)
    {
      *total_member_count =
        base_types[0]->member_count ();
    }

  // Obtain each derived type's basetype and add this to
  // the list.

  CORBA::TypeCode_var
    base (base_types[0]->concrete_base_type());
  while (base.in() &&
         CORBA::tk_value ==
           (base= // assignment
             TAO_DynAnyFactory::strip_alias (base.in()))
           ->kind ())
    {
      if (total_member_count)
        {
          *total_member_count += base->member_count ();
        }

      base_types.size (numberOfBases + 1);
      base_types[numberOfBases++] =
        CORBA::TypeCode::_duplicate (base.in ());
      base = base->concrete_base_type();
    }
}

CORBA::TypeCode_ptr
TAO_DynValue_i::get_correct_base_type (
  const BaseTypesList_t &base_types,
  CORBA::ULong &index)
{
  // We work backwards through the list of derived types,
  // so index 0 is the first member of the extreme base type
  // (assuming it has any members) once we run out of that
  // base types members we move up the list to the next
  // derived type until that type's members are exhausted
  // and so on until we reach the member we have asked for.

  CORBA::ULong
    currentBase = base_types.size ();
  if (!currentBase)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::get_correct_base_type () ")
        ACE_TEXT ("BaseTypesList_t is not initialised\n")));
      return 0;
    }

  while (base_types[--currentBase]->member_count () <= index)
    {
      index -= base_types[currentBase]->member_count ();
      if (!currentBase)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
            ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::get_correct_base_type () ")
            ACE_TEXT ("BaseTypesList_t is not large enough\n")));
          return 0;
        }
    }

  // Note that the "index" has now been reduced to the range
  // of the returning base_type.
  return base_types[currentBase].in ();
}

CORBA::TypeCode_ptr
TAO_DynValue_i::get_member_type (
  const BaseTypesList_t &base_types,
  CORBA::ULong index)
{
  const CORBA::TypeCode_ptr
    base = get_correct_base_type (base_types, index);
  return base->member_type (index);
}

const char *
TAO_DynValue_i::get_member_name (
  const BaseTypesList_t &base_types,
  CORBA::ULong index)
{
  const CORBA::TypeCode_ptr
    base = get_correct_base_type (base_types, index);
  return base->member_name (index);
}

void
TAO_DynValue_i::set_to_value (void)
{
  this->component_count_ =
    static_cast <CORBA::ULong> (this->da_members_.size ());
  this->current_position_ =
    this->component_count_ ? 0 : -1;
  this->is_null_ = false;
}

void
TAO_DynValue_i::check_typecode (CORBA::TypeCode_ptr tc)
{
  if (CORBA::tk_value == TAO_DynAnyFactory::unalias (tc))
    {
      return;
    }

  throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
}

TAO_DynValue_i *
TAO_DynValue_i::_narrow (CORBA::Object_ptr _tao_objref)
{
  return (CORBA::is_nil (_tao_objref)) ?
         0 :
         dynamic_cast<TAO_DynValue_i *> (_tao_objref);
}

DynamicAny::FieldName
TAO_DynValue_i::current_member_name (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  if (-1 == this->current_position_)
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  return CORBA::string_dup (
    this->get_member_name (
      this->da_base_types_,
      this->current_position_));
}

CORBA::TCKind
TAO_DynValue_i::current_member_kind (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  if (-1 == this->current_position_)
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  CORBA::TypeCode_var tc (
    get_member_type (
      this->da_base_types_,
      this->current_position_));
  return TAO_DynAnyFactory::unalias (tc.in ());
}

DynamicAny::NameValuePairSeq *
TAO_DynValue_i::get_members (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // Create the return NameValuePairSeq
  DynamicAny::NameValuePairSeq *members = 0;
  ACE_NEW_THROW_EX (
    members,
    DynamicAny::NameValuePairSeq (this->component_count_),
    CORBA::NO_MEMORY ());
  members->length (this->component_count_);
  DynamicAny::NameValuePairSeq_var
    safe_retval (members);

  // Assign member name and value to each slot.
  for (CORBA::ULong i = 0u;
       i < this->component_count_;
       ++i)
    {
      safe_retval[i].id = CORBA::string_dup (
        this->get_member_name (this->da_base_types_, i));
      CORBA::Any_var
        temp (this->da_members_[i]->to_any ());
      safe_retval[i].value = temp.in ();
    }

  return safe_retval._retn ();
}

void
TAO_DynValue_i::set_members (
  const DynamicAny::NameValuePairSeq &values)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // Check lengths match.
  const CORBA::ULong length = values.length ();
  if (length !=
      static_cast <CORBA::ULong>
        (this->da_members_.size ()))
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  // Check each member typecodes match.
  CORBA::ULong i;
  for (i = 0u; i < length; ++i)
    {
      CORBA::TypeCode_var my_member (
        get_member_type (this->da_base_types_, i));
      CORBA::TypeCode_var value_member (
        values[i].value.type ());
      if (!my_member->equivalent (value_member.in ()))
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }
    }

  // Copy in the new values to each member ()
  for (i = 0u; i < length; ++i)
    {
      this->da_members_[i] = TAO::MakeDynAnyUtils::
        make_dyn_any_t<const CORBA::Any&> (
          values[i].value._tao_get_typecode (),
          values[i].value,
          this->allow_truncation_);
    }

  this->set_to_value ();
}

DynamicAny::NameDynAnyPairSeq *
TAO_DynValue_i::get_members_as_dyn_any (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // Create the return NameDynAnyPairSeq
  DynamicAny::NameDynAnyPairSeq *members = 0;
  ACE_NEW_THROW_EX (
    members,
    DynamicAny::NameDynAnyPairSeq (this->component_count_),
    CORBA::NO_MEMORY ());
  members->length (this->component_count_);
  DynamicAny::NameDynAnyPairSeq_var
    safe_retval (members);

  // Assign name and value to each pearl on the string.
  for (CORBA::ULong i = 0u;
       i < this->component_count_;
       ++i)
    {
      safe_retval[i].id = CORBA::string_dup (
        this->get_member_name (this->da_base_types_, i));

      // A deep copy is made only by copy()
      // (CORBA 2.4.2 section 9.2.3.6).
      // Set the flag so the caller can't destroy.
      this->set_flag (this->da_members_[i].in (), 0);

      safe_retval[i].value =
        DynamicAny::DynAny::_duplicate (
          this->da_members_[i].in ());
    }

  return safe_retval._retn ();
}

void
TAO_DynValue_i::set_members_as_dyn_any (
  const DynamicAny::NameDynAnyPairSeq & values)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // Check lengths match.
  CORBA::ULong length = values.length ();
  if (length !=
      static_cast <CORBA::ULong> (
        this->da_members_.size ()))
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  // Check each member typecodes match.
  CORBA::ULong i = 0u;
  for (; i < length; ++i)
    {
      CORBA::TypeCode_var
        my_member (
          get_member_type (this->da_base_types_, i)),
        value_member (
          values[i].value->type ());
      if (!my_member->equivalent (value_member.in ()))
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }
    }

  // Copy in the new values to each member ()
  for (i = 0u; i < length; ++i)
    {
      this->da_members_[i] = values[i].value->copy ();
    }

  this->set_to_value ();
}

void
TAO_DynValue_i::from_any (const CORBA::Any &any)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::TypeCode_var
    tc (any.type ());
  if (!this->type_->equivalent (tc.in ()))
    {
      throw DynamicAny::DynAny::TypeMismatch ();
    }

  this->from_any_helper (any);
}

CORBA::Boolean
TAO_DynValue_i::equal (DynamicAny::DynAny_ptr rhs)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::TypeCode_var tc (rhs->type ());
  if (!tc->equivalent (this->type_.in ()) ||
      this->component_count_ !=
        rhs->component_count ()  )
    {
      return false;
    }

  TAO_DynValue_i *rhs_v=
    dynamic_cast<TAO_DynValue_i *> (rhs);

  if (!rhs_v || this->is_null () != rhs_v->is_null ())
    {
      return false;
    }

  if (!this->is_null ())
    {
      for (CORBA::ULong i = 0u;
           i < this->component_count_;
           ++i)
        {
          if (!rhs_v->da_members_[i]
              ->equal (this->da_members_[i].in ()))
            {
              return false;
            }
        }
    }

  return true;
}

void
TAO_DynValue_i::destroy (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  if (!this->ref_to_component_ ||
      this->container_is_destroying_)
    {
      // Do a deep destroy.
      this->component_count_ =
        static_cast <CORBA::ULong> (
          this->da_members_.size () );

      for (CORBA::ULong i = 0u;
           i < this->component_count_;
           ++i)
        {
          this->set_flag (da_members_[i].in (), 1);
          this->da_members_[i]->destroy ();
        }

      this->destroyed_ = 1;
    }
}

DynamicAny::DynAny_ptr
TAO_DynValue_i::current_component (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  if (-1 == this->current_position_)
    {
      return DynamicAny::DynAny::_nil ();
    }

  const CORBA::ULong index =
    static_cast <CORBA::ULong> (this->current_position_);
  this->set_flag (this->da_members_[index].in (), 0);

  return DynamicAny::DynAny::_duplicate (
    this->da_members_[index].in () );
}

// Overrides TAO_DynCommon::insert_val (). Since we ARE
// a ValueType, it does not make sense to pass the value
// type input down to the current_component to deal with
// (even though we are a constructed type and should do
// so with any other type of input). If we don't assume
// the value type is for us, it will get passed down
// (recursivly) to the terminal non-valuetype member
// which then will be wrong type for the valuetype input
// we started with.
void
TAO_DynValue_i::insert_val (CORBA::ValueBase *value)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // If the arg is 0 we can't do this kind of type
  // checking, and the call to _tao_marshal() below
  // will handle the null value correctly in any case.
  if (value)
    {
      const char
        *value_id = value->_tao_obv_repository_id (),
        *my_id    = this->type_->id ();

      // Valuetypes, unlike interfaces, don't have a
      // virtual method to check for a more derived
      // type when the repo ids don't match. Valuetypes
      // have only a static _downcast() method, which
      // can't be used here, so if the repo ids don't
      // match, we're hosed.
      if (ACE_OS::strcmp (value_id, my_id))
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }
    }

  // This will handle null values correctly, or
  // otherwise make a virtual call to marshal the state.
  TAO_OutputCDR out;
  if (!CORBA::ValueBase::_tao_marshal (out, value))
    {
      TAOLIB_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::insert_val ")
        ACE_TEXT ("can not marshal value\n") ));
      throw DynamicAny::DynAny::InvalidValue ();
    }

  TAO_InputCDR in (out);
  this->from_inputCDR (in);
}

// Overrides TAO_DynCommon::get_val (). Since we ARE
// a ValueType, it does not make sense to pass the get
// request down to the current_component to deal with
// (even though we are a constructed type and should
// do so with any other type of output). If we don't
// assume the value type is us, it will get passed down
// (recursivly) to the terminal non-valuetype member
// which then will be wrong type for the valuetype
// output we want.
CORBA::ValueBase *
TAO_DynValue_i::get_val (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // Convert this value into an output stream
  TAO_OutputCDR out_cdr;
  this->to_outputCDR (out_cdr);

  // Now read in this stream to create the actual value.
  TAO_InputCDR for_reading (out_cdr);
  CORBA::ValueBase *retval = 0;
  if (!CORBA::ValueBase::_tao_unmarshal (
        for_reading, retval ))
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  return retval;
}

CORBA::Any_ptr
TAO_DynValue_i::to_any (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // First create the value as an output stream
  TAO_OutputCDR out_cdr;
  this->to_outputCDR (out_cdr);

  // Convert the out_cdr into a new any.
  TAO_InputCDR in_cdr (out_cdr);
  TAO::Unknown_IDL_Type *unk = 0;
  ACE_NEW_THROW_EX (
    unk,
    TAO::Unknown_IDL_Type (this->type_.in (), in_cdr),
    CORBA::NO_MEMORY () );
  CORBA::Any_ptr retval = 0;
  ACE_NEW_THROW_EX (
    retval,
    CORBA::Any,
    CORBA::NO_MEMORY ());
  retval->replace (unk);
  return retval;
}

// This code is common to from_any() and the init(any).
// Basically the type information for our DynValue has
// already been set-up, we are now just copying the any
// value into our type.
void
TAO_DynValue_i::from_any_helper (const CORBA::Any & any)
{
  // Get the CDR stream of the Any, if there isn't one,
  // make one by marshalling the value into a new stream.

  TAO_OutputCDR out;
  TAO_InputCDR in (static_cast<ACE_Message_Block *> (0));
  TAO::Any_Impl *impl = any.impl ();
  if (impl->encoded ())
    {
      TAO::Unknown_IDL_Type *unk =
        dynamic_cast<TAO::Unknown_IDL_Type *> (impl);
      if (!unk)
        {
          throw CORBA::INTERNAL ();
        }

      in = unk->_tao_get_cdr ();
    }
  else
    {
      impl->marshal_value (out);
      TAO_InputCDR tmp_in (out);
      in = tmp_in;
    }

  this->from_inputCDR (in);
}

void
TAO_DynValue_i::to_outputCDR (TAO_OutputCDR &out_cdr)
{
  // Is this an NULL Valuetype? If so we need to
  // output the special NULL value type header.
  // Note that we don't actually have a CORBA::ValueBase *
  // which this call expects as we are actually dealing
  // with DynValue * instead. However the pointer isn't
  // actually dereferanced by the _tao_write_special_value()
  // call, its address (as a void *) is just used to
  // check for the null value and any previous writen
  // value for the indirection header and the saving of
  // this current location for this new valuetype if it
  // is not indirected (this time).
  if (!CORBA::ValueBase::_tao_write_special_value (
        out_cdr,
        reinterpret_cast <CORBA::ValueBase *>
          (this->is_null_ ? 0 : this)             ))
    {
      // OK since a special_value hasn't been written we are
      // NOT a NULL or indirect ValueType. We need to create
      // a normal valuetype header, which states if chunking
      // is being used and if CodeBaseURL and type information
      // ((none, single or list of) repository id(s)) is being
      // provided. TAO NEVER sends a codebase URL and ALWAYS
      // lists the full list of repository ids that are
      // VM_TRUNCATABLE.

      CORBA::Long valuetag =
        TAO_OBV_GIOP_Flags::Value_tag_base |
        TAO_OBV_GIOP_Flags::Type_info_single;

      const CORBA::ULong num_ids = this->da_base_types_.size ();
      CORBA::ULong trunc_ids;
      for (trunc_ids= 0u; trunc_ids < num_ids - 1u; ++trunc_ids)
        {
          if (CORBA::VM_TRUNCATABLE !=
              this->da_base_types_[trunc_ids]->type_modifier ())
            {
              break; // Found the first type that is not truncatable
            }
        }
      ++trunc_ids; // Include non-truncatable id that stopped us.

      if (1u < trunc_ids)
        {
          valuetag |=
            TAO_OBV_GIOP_Flags::Type_info_list;
        }

      CORBA::Boolean we_are_chunking = (1u < trunc_ids);
      if (!we_are_chunking)
        {
          for (CORBA::ULong i= trunc_ids - 1u; i < num_ids; ++i)
            {
              if (CORBA::VM_CUSTOM ==
                  this->da_base_types_[i]->type_modifier ())
                {
                  we_are_chunking = true;
                  break;
                }
            }
        }

      if (we_are_chunking)
        {
          valuetag |=
            TAO_OBV_GIOP_Flags::Chunking_tag_sigbits;
        }

      // Start writing out the value header (and if
      // necessary, the number of repo_ids we are
      // going to write).
      if (!out_cdr.write_long (valuetag) ||
          (1u < trunc_ids && !out_cdr.write_ulong (trunc_ids)) )
        {
          TAOLIB_DEBUG ((LM_DEBUG,
            ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::to_outputCDR() ")
            ACE_TEXT ("problem writing <value-tag> header\n")));
          throw CORBA::INTERNAL ();
        }

      // Marshal out the list of repo_ids we are sending from
      // the most derived type_id to the most base type_id in
      // order. NOTE these type_ids are the real / full /
      // non-dealliased versions not the dealliased ones that
      // are stored in the da_base_types_[] list.
      CORBA::ULong i= 0u;
      for (CORBA::TypeCode_var
             next (CORBA::TypeCode::_duplicate (this->type_.in ()));
           i < trunc_ids;
           ++i)
        {
          ACE_CString repo_id (next->id ());
          if (!CORBA::ValueBase::
                _tao_write_repository_id (out_cdr, repo_id) )
            {
              TAOLIB_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::to_outputCDR() ")
                ACE_TEXT ("problem writing header repo_ids\n")));
              throw CORBA::INTERNAL ();
            }

          // Using the dealliased tc for this current type, find
          // the next non-dealliased base typecode.
          next = this->da_base_types_[i]->concrete_base_type ();
        }

      // Write out the start chunking markers for the number
      // of base types we are writing. If we are not a truncatable
      // type there are none to write (i.e. we are writing one
      // less than the trunc_ids we actually have).
      TAO_ChunkInfo ci (we_are_chunking);
      for (i= 0u; i < trunc_ids - 1u; ++i)
        {
          if (!ci.start_chunk (out_cdr))
            {
              TAOLIB_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::to_outputCDR() ")
                ACE_TEXT ("problem writing basetype start chucks\n")));
              throw CORBA::INTERNAL ();
            }
        }

      // Now write out every member's value (add further chunking
      // marks for each seporate base-type's state).
      CORBA::Boolean need_first = true;
      CORBA::ULong
        currentBase= num_ids,  // Note NOT just the trunc_ids
        currentBaseMember = 0u;
      for (CORBA::ULong currentMember= 0u;
           currentMember < this->component_count_;
           ++currentMember)
        {
          // Are we starting a new base-type
          if (!currentBaseMember)
            {
              // Move on to the next derived type in the
              // list of our type hyarchy
              while (!this->da_base_types_[--currentBase]
                      ->member_count ())
                {
                  // Skipping over all types that have no
                  // state (i.e. no members to write).
                }

              if (currentBase < trunc_ids || need_first)
                {
                  need_first = false;
                  // Start chunk for this base-type's STATE
                  if (!ci.start_chunk (out_cdr))
                    {
                      TAOLIB_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::to_outputCDR() ")
                        ACE_TEXT ("problem writing base-type ")
                        ACE_TEXT ("%u state start chuck\n"),
                        currentBase ));
                      throw CORBA::INTERNAL ();
                    }
                }
            }

          // Recursive step - Add this member to the out_cdr
          if (TAO_DynValue_i *member= // Assignment
              dynamic_cast<TAO_DynValue_i *>
                (this->da_members_[currentMember].in ()))
            {
              member->to_outputCDR (out_cdr);
            }
          else
            {
              CORBA::Any_var
                any (this->da_members_[currentMember]->to_any ());
              TAO::Any_Impl *impl = any->impl ();
              if (impl->encoded ())
                {
                  // Already encoded as a stream, convert this
                  // into an unknown and copy the stream over.
                  TAO::Unknown_IDL_Type
                    *unk= dynamic_cast<TAO::Unknown_IDL_Type *> (impl);
                  if (!unk)
                    {
                      TAOLIB_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::to_outputCDR() ")
                        ACE_TEXT ("problem obtaining Unknown_IDL_Type\n")));
                      throw CORBA::INTERNAL ();
                    }

                  // Create a new input CDR for this member's
                  // marshaled representation.
                  TAO_InputCDR
                    in_cdr (unk->_tao_get_cdr ());

                  // Ok append this member's value to the output stream.
                  CORBA::TypeCode_var
                    tc (this->da_members_[currentMember]->type ());

                  (void) TAO_Marshal_Object::perform_append (
                    tc.in (),
                    &in_cdr,
                    &out_cdr);
                }
              else
                {
                  // Just need to marshal this value into the stream.
                  impl->marshal_value (out_cdr);
                }
            }

          // Are we ending the current base-type?
          if (this->da_base_types_[currentBase]->member_count ()
              <= ++currentBaseMember)
            {
              // Remind us to start again with the next derived type
              // for the next member to be writen.
              currentBaseMember= 0u;

              // We must end the chunk we started for this
              // base-type's STATE if we have written the whole state.
              if (currentBase < trunc_ids && !ci.end_chunk (out_cdr))
                {
                  TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::to_outputCDR() ")
                    ACE_TEXT ("problem writing base-type ")
                    ACE_TEXT ("%u state end chuck\n"),
                    currentBase ));
                  throw CORBA::INTERNAL ();
                }
            }
        }

      // Write out the end chunking markers for the number
      // of base types we have writen.
      for (i= 1u; i < trunc_ids; ++i)
        {
          if (!ci.end_chunk (out_cdr))
            {
              TAOLIB_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::to_outputCDR() ")
                ACE_TEXT ("problem writing basetype end chucks\n")));
              throw CORBA::INTERNAL ();
            }
        }
    }
}

void
TAO_DynValue_i::from_inputCDR (TAO_InputCDR &strm)
{
  // Save the start of this ValueType position in the input stream
  // to allow caching for later indirections.
  VERIFY_MAP (TAO_InputCDR, value_map, Value_Map);
  if (strm.align_read_ptr (ACE_CDR::LONG_SIZE))
    {
      this->set_to_null ();
      throw CORBA::INTERNAL ();
    }
  void *const start_of_valuetype = strm.rd_ptr();

  // Read in the ValueType header
  CORBA::ValueBase::Repository_Id_List ids;
  CORBA::Boolean
    is_null_object= false,
    is_indirected= false,
    is_chunked = false,
    result = CORBA::ValueBase::_tao_unmarshal_header (
      strm, this->type_.in ()->id (), ids,
      is_null_object, is_indirected, is_chunked);

  if (!result)
    {
      TAOLIB_DEBUG ((LM_ERROR,
        ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::from_inputCDR() ")
        ACE_TEXT ("_tao_unmarshal_header() failed\n") ));
      this->set_to_null ();
      throw CORBA::INTERNAL ();
    }
  if (is_null_object)
    {
      this->set_to_null ();
      return;
    }
  if (is_indirected)
    {
      // Effectivly this member? is the same ValueType as previous
      // seen either in another member of this container OR the
      // whole container itself. (Possiably can happen as a
      // circular linked list?)
      if (TAO_debug_level)
        {
          TAOLIB_DEBUG ((LM_ERROR,
            ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::from_inputCDR() ")
            ACE_TEXT ("Any contains Indirected ValueType\n") ));
        }

      // Force the originally created container to empty, it is up
      // to the caller to delete it and replace it with the duplicate
      // we are now about to find.
      this->set_to_null ();

      // Read the offset from the stream (should be negative to point
      // back to original)
      CORBA::Long offset = 0;
      if (!strm.read_long (offset) ||  0 <= offset)
        {
          TAOLIB_DEBUG ((LM_ERROR,
            ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::from_inputCDR() ")
            ACE_TEXT ("Can't read/understand ")
            ACE_TEXT ("Indirected ValueType offset\n") ));
          throw CORBA::INTERNAL ();
        }

      // Work out the input stream location of the original valuetype
      // and find the address of the original TAO_DynValue_i that we
      // created last time and stored in the map.
      void
        *pos = strm.rd_ptr () + offset - sizeof (CORBA::Long),
        *original = 0;
      if (strm.get_value_map()->get()->find (pos, original))
        {
          TAOLIB_DEBUG ((LM_ERROR,
            ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::from_inputCDR() ")
            ACE_TEXT ("Can't find Indirected ValueType ")
            ACE_TEXT ("offset in map\n") ));
          throw CORBA::INTERNAL ();
        }

      // Since this is a void * convert it back to our real type and
      // throw it for the caller to catch and replace "this"
      // TAO_DynValue_i.
      TAO_DynValue_i *this_one_instead=
        reinterpret_cast<TAO_DynValue_i *> (original);
      this_one_instead->_add_ref ();
      throw this_one_instead;
    }

  // Ok since we are not indirected (this time), record "this"
  // DynValue_i for later possiable indirections to use.
  if (strm.get_value_map ()->get()
        ->bind (
          start_of_valuetype,
          reinterpret_cast<void *> (this)))
    {
      TAOLIB_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::from_inputCDR() ")
        ACE_TEXT ("Failed to record this into value_map\n") ));
      throw CORBA::INTERNAL ();
    }

  // Work out how many total types there
  // are in this derived->base hiarchy.
  const CORBA::ULong
    num_fields = static_cast <CORBA::ULong> (this->da_members_.size ()),
    num_ids =    static_cast <CORBA::ULong> (ids.size ());

  // Work out if the encoded valuetype inside the any is
  // required to be truncated into our DynValue.
  CORBA::Boolean requires_truncation = false;
  const char *const our_id = this->da_base_types_[0]->id ();
  CORBA::ULong i;
  for (i= 0u; i < num_ids; ++i)
    {
      // Does the current id match our DynValue id?
      if (!strcmp (our_id, ids[i].c_str ()))
        {
          break; // Found a match, we have our answer
        }

      if (!this->allow_truncation_)
        {
          throw DynamicAny::MustTruncate ();
        }

      // Since this does not match we must be attempting
      // to truncated to a base-type, thus the incomming
      // any must be chuncked and this outer type must
      // allow truncation.
      if (!is_chunked)
        {
          TAOLIB_DEBUG ((LM_ERROR,
            ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::from_inputCDR()\n")
            ACE_TEXT ("  type %C requires truncation to %C but is not chunked type.\n"),
            ids[i].c_str (),
            our_id ));
          this->set_to_null ();
          throw DynamicAny::DynAny::TypeMismatch ();
        }

      requires_truncation = true;
    }
  if (i == num_ids)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::from_inputCDR() ")
        ACE_TEXT ("couldn't find matching repo_id!\n")));
      this->set_to_null ();
      throw DynamicAny::DynAny::TypeMismatch ();
    }

  // Read in the start chunking markers for the number
  // of base types we are reading. If we are not a derived
  // type there are none to read (i.e. we are reading one
  // less than the num_ids we actually have received).

  TAO_ChunkInfo ci (is_chunked, 1);
  for (i= 0u; i < num_ids - 1u; ++i)
    {
      if (!ci.handle_chunking (strm))
        {
          TAOLIB_DEBUG ((LM_DEBUG,
            ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::from_inputCDR() ")
            ACE_TEXT ("problem reading basetype start chucks\n")));
          this->set_to_null ();
          throw DynamicAny::DynAny::InvalidValue ();
        }
    }

  // Now read in every member's value (reading further chunking
  // marks for each seporate base-type's state we pass).
  CORBA::Boolean need_first = true;
  CORBA::ULong
    currentBase= this->da_base_types_.size (),
    currentBaseMember = 0u;
  for (CORBA::ULong currentMember= 0u;
       currentMember < num_fields;
       ++currentMember)
    {
      // Are we starting a new base-type
      if (!currentBaseMember)
        {
          // Move on to the next derived type in the
          // list of our type hyarchy
          while (!this->da_base_types_[--currentBase]
                  ->member_count ())
            {
              // Skipping over all types that have no
              // state (i.e. no members to write).
            }

          if (currentBase < num_ids || need_first)
            {
              // Read past the start chunk for this base-type's state
              if (!ci.handle_chunking (strm))
                {
                  TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::from_inputCDR() ")
                    ACE_TEXT ("problem reading base-type ")
                    ACE_TEXT ("%u state start chuck\n"),
                    currentBase ));
                  this->set_to_null ();
                  throw DynamicAny::DynAny::InvalidValue ();
                }
            }
        }

      // OK read in the current member
      CORBA::TypeCode_var
        field_tc (this->da_base_types_[currentBase]
                  ->member_type (currentBaseMember));
      if (CORBA::tk_value == field_tc->kind ())
        {
          // This is recursive, keep reading from our inputCDR
          // this allows for indirection
          this->da_members_[currentMember]=
            TAO::CreateDynAnyUtils<TAO_DynValue_i, TAO_InputCDR &>
              ::create_dyn_any_t (
                field_tc.in (),
                strm,
                this->allow_truncation_);
        }
      else
        {
          // Need to create an any for this field.
          TAO_InputCDR unk_in (strm);
          TAO::Unknown_IDL_Type *unk= 0;
          ACE_NEW_THROW_EX (
            unk,
            TAO::Unknown_IDL_Type (field_tc.in (), unk_in),
            CORBA::NO_MEMORY ());

          CORBA::Any field_any;
          field_any.replace (unk);

          // This recursive step will call the correct
          // constructor based on the type of field_any.
          this->da_members_[currentMember] = TAO::MakeDynAnyUtils::
            make_dyn_any_t<const CORBA::Any&> (
              field_any._tao_get_typecode (),
              field_any,
              this->allow_truncation_);

          // Since the above Unknown used its own inputCDR we need
          // to move onto the next field in the real CDR stream as
          // IF we had just read the actual value from it.
          (void) TAO_Marshal_Object::perform_skip (
            field_tc.in (),
            &strm);
        }

      // Are we ending the current base-type?
      if (this->da_base_types_[currentBase]->member_count ()
          <= ++currentBaseMember)
        {
          // Remind us to start again with the next derived type
          // for the next member to be writen.
          currentBaseMember= 0u;

          if (currentBase < num_ids)
            {
              // We must end the chunk that was started for this
              // base-type's state.
              if (!( (currentBase || !requires_truncation) ?
                     ci.handle_chunking (strm) :
                     ci.skip_chunks (strm)                  ))
                {
                  TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - %N:%l TAO_DynValue_i::from_inputCDR() ")
                    ACE_TEXT ("problem reading base-type ")
                    ACE_TEXT ("%u state end chuck\n"),
                    currentBase ));
                  this->set_to_null ();
                  throw DynamicAny::DynAny::InvalidValue ();
                }
            }
        }
    }

  this->set_to_value ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
