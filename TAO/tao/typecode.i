// just fetch the 'kind' field out of the typecode
ACE_INLINE void *
CORBA_TypeCode::operator new (size_t s)
{ 
  return ::operator new (s); 
}

ACE_INLINE CORBA::TCKind
CORBA_TypeCode::kind (CORBA::Environment &env) const
{
  env.clear ();
  return _kind;
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA_TypeCode::_duplicate (CORBA::TypeCode_ptr tc)
{
  if (tc)
    tc->AddRef ();
  return tc;
}

// Constructor for CONSTANT typecodes with empty parameter lists.
// These are only created once, and those constants are shared.

ACE_INLINE 
CORBA_TypeCode::CORBA_TypeCode (CORBA::TCKind kind)
  : _length (0),
    _buffer (0),
    _kind (kind),
    _parent (0),
    refcount_ (1),
    _delete_flag (CORBA::B_FALSE),
    _orb_owns (CORBA::B_TRUE),
    _private_state (new TC_Private_State (kind)),
    non_aligned_buffer_ (0)
{
}

// Returns true if the two typecodes are identical
ACE_INLINE CORBA::Boolean 
CORBA_TypeCode::equal (const CORBA::TypeCode_ptr tc,
                       CORBA::Environment &env) const
{
  if (this->_kind != tc->kind (env))
    // simple case
    return CORBA::B_FALSE;
  else
    return this->private_equal (tc, env);
}

// returns the Repository ID
ACE_INLINE TAO_CONST CORBA::String
CORBA_TypeCode::id (CORBA::Environment &env) const
{
  env.clear ();

  // if already precomputed
  if (_private_state->tc_id_known_)
    return _private_state->tc_id_;
  else
    return private_id (env);
}

// returns the string name
ACE_INLINE TAO_CONST CORBA::String
CORBA_TypeCode::name (CORBA::Environment &env) const
{
  env.clear ();

  // if already precomputed
  if (_private_state->tc_name_known_)
    return _private_state->tc_name_;
  else
    return private_name (env);
}

// Return the number of members defined by this typecode
//
// Applicable to struct, union, enum, alias, and except
// For the rest of the cases, raises the BadKind exception.

ACE_INLINE CORBA::ULong
CORBA_TypeCode::member_count (CORBA::Environment &env) const
{
  env.clear ();

  // if already precomputed
  if (_private_state->tc_member_count_known_)
    return _private_state->tc_member_count_;
  else
    return private_member_count (env);
}

// Return the i-th member typecode if it exists, else raise an
// exception. Possible exceptions are BadKind and Bounds.
//
// Applicable only to struct, union, and except

ACE_INLINE CORBA::TypeCode_ptr
CORBA_TypeCode::member_type (CORBA::ULong index, 
			     CORBA::Environment &env) const
{
  if (_private_state->tc_member_count_known_ 
      && _private_state->tc_member_type_list_known_)
    {
      if (index < _private_state->tc_member_count_)
        return _private_state->tc_member_type_list_[index];
      else
        {
          env.exception (new CORBA::TypeCode::Bounds ());
          return 0;
        }
    }
  else
    return private_member_type (index, env);
}


// Return the label of the i-th member.  Applicable only to CORBA::tk_union
ACE_INLINE CORBA::Any_ptr
CORBA_TypeCode::member_label (CORBA::ULong index,
                              CORBA::Environment &env) const
{
  if (_private_state->tc_member_count_known_ 
      && _private_state->tc_member_label_list_known_)
    {
      if (index < _private_state->tc_member_count_)
        return _private_state->tc_member_label_list_[index];
      else
        {
          env.exception (new CORBA::TypeCode::Bounds ());
          return 0;
        }
    }
  else
    return private_member_label (index, env);
}

// only applicable to CORBA::tk_unions
ACE_INLINE CORBA::TypeCode_ptr
CORBA_TypeCode::discriminator_type (CORBA::Environment &env) const
{
  if (_kind == CORBA::tk_union)
    {
      if (_private_state->tc_discriminator_type_known_)
        return _private_state->tc_discriminator_type_;
      else
          return private_discriminator_type (env);
    }
  else
    {
      env.exception (new CORBA::TypeCode::BadKind ());
      return (CORBA::TypeCode_ptr)0;
    }
}

// only applicable to CORBA::tk_unions
ACE_INLINE CORBA::Long
CORBA_TypeCode::default_index (CORBA::Environment &env) const
{
  if (_kind == CORBA::tk_union)
    {
      if (_private_state->tc_default_index_used_known_)
        return _private_state->tc_default_index_used_;
      else
        return private_default_index (env);
    }
  else
    {
      env.exception (new CORBA::TypeCode::BadKind ());
      return 0;
    }
}

// returns the length. Applicable only to string, sequence, and arrays
ACE_INLINE CORBA::ULong
CORBA_TypeCode::length (CORBA::Environment &env) const
{
  // a switch stmt, unfortunately, doesn't get inlined
  if (_kind == CORBA::tk_sequence 
      || _kind == CORBA::tk_array 
      || _kind == CORBA::tk_string 
      || _kind == CORBA::tk_wstring)
    {
      if (_private_state->tc_length_known_)
        return _private_state->tc_length_;
      else
        return private_length (env);
    }
  else
    {
      env.exception (new CORBA::TypeCode::BadKind ());
      return 0;
    }
}

// returns the length. Applicable only to string, sequence, and arrays
ACE_INLINE CORBA::TypeCode_ptr
CORBA_TypeCode::content_type (CORBA::Environment &env) const
{
  if (_kind == CORBA::tk_sequence 
      || _kind == CORBA::tk_array 
      || _kind == CORBA::tk_alias)
    {
      if (_private_state->tc_content_type_known_)
        return _private_state->tc_content_type_;
      else
        return private_content_type (env);
    }
  else
    {
      env.exception (new CORBA::TypeCode::BadKind ());
      return 0;
    }
}

// calculate size of the typecode
ACE_INLINE size_t
CORBA_TypeCode::size (CORBA::Environment &env)
{
  if (_private_state->tc_size_known_)
    return _private_state->tc_size_;
  else
    return private_size (env);
}

// calculate alignment requirements of the typecode
ACE_INLINE size_t
CORBA_TypeCode::alignment (CORBA::Environment &env)
{
  if (_private_state->tc_alignment_known_)
    return _private_state->tc_alignment_;
  else
    return private_alignment (env);
}

// compute the padded size of the discriminant
ACE_INLINE CORBA::ULong
CORBA_TypeCode::TAO_discrim_pad_size (CORBA::Environment &env)
{
  if (_kind == CORBA::tk_union)
    {
      if (_private_state->tc_discrim_pad_size_known_)
        return _private_state->tc_discrim_pad_size_;
      else
        return private_discrim_pad_size (env);
    }
  else
    {
      env.exception (new CORBA::TypeCode::BadKind ());
      return 0;
    }
}

ACE_INLINE void *
CORBA_TypeCode::operator new (size_t, void *p)
{ 
  return p; 
}
