ACE_INLINE CORBA_TypeCode_ptr
CORBA_TypeCode::_duplicate(CORBA_TypeCode_ptr tc)
{
  if (tc)
    {
      tc->AddRef();
    }
  return tc;
}

// Constructor for CONSTANT typecodes with empty parameter lists.
// These are only created once, and those constants are shared.

ACE_INLINE CORBA_TypeCode::CORBA_TypeCode (CORBA_TCKind kind)
  : _length (0),
    _buffer (0),
    _kind (kind),
    _parent (0),
    _refcount (1),
    _orb_owns (CORBA_B_FALSE),
    _prv_state (new TC_PRV_State)
{
}

// Destructor.  For "indirected" typecodes, the typecode reuses the
// buffer owned by its parent, and so rather than deleting the buffer
// it just drops the parent's refcount.

ACE_INLINE CORBA_TypeCode::~CORBA_TypeCode (void)
{
  delete _prv_state;

  if (_parent)
    _parent->Release ();
  else if (_orb_owns)
    delete _buffer;
}

// Returns true if the two typecodes are identical
ACE_INLINE CORBA_Boolean 
CORBA_TypeCode::equal(const CORBA_TypeCode_ptr tc,
				    CORBA_Environment &env) const
{
  if (this->_kind != tc->kind(env))
    {
      // simple case
      return CORBA_B_FALSE;
    }
  else
    {
      return this->prv_equal(tc, env);
    }
}

// just fetch the 'kind' field out of the typecode
ACE_INLINE CORBA_TCKind
CORBA_TypeCode::kind (CORBA_Environment	&env) const
{
  env.clear ();
  return _kind;
}

// returns the Repository ID
ACE_INLINE const CORBA_String
CORBA_TypeCode::id(CORBA_Environment &env) const
{
  env.clear();

  // if already precomputed
  if (_prv_state->tc_id_known_)
    {
      return _prv_state->tc_id_;
    }
  else
    {
      return prv_id(env);
    }
}

// returns the string name
ACE_INLINE const CORBA_String
CORBA_TypeCode::name(CORBA_Environment &env) const
{
  env.clear();

  // if already precomputed
  if (_prv_state->tc_name_known_)
    {
      return _prv_state->tc_name_;
    }
  else
    {
      return prv_name(env);
    }
}

// Return the number of members defined by this typecode
//
// Applicable to struct, union, enum, alias, and except
// For the rest of the cases, raises the BadKind exception.

ACE_INLINE CORBA_ULong
CORBA_TypeCode::member_count (CORBA_Environment &env) const
{
  env.clear ();

  // if already precomputed
  if (_prv_state->tc_member_count_known_)
    {
      return _prv_state->tc_member_count_;
    }
  else
    {
      return prv_member_count(env);
    }
}

// Return the i-th member typecode if it exists, else raise an
// exception. Possible exceptions are BadKind and Bounds.
//
// Applicable only to struct, union, and except
//

ACE_INLINE CORBA_TypeCode_ptr
CORBA_TypeCode::member_type (CORBA_ULong index, CORBA_Environment &env) const
{
  if (_prv_state->tc_member_count_known_ && _prv_state->tc_member_type_list_known_)
    {
      if (index >= 0 && index < _prv_state->tc_member_count_)
	{
	  return CORBA_TypeCode::_duplicate(_prv_state->tc_member_type_list_[index]);
	}
      else
	{
	  env.exception(new CORBA_Bounds());
	  return 0;
	}
    }
  else
    {
      return prv_member_type(index, env);
    }
}


// Return the label of the i-th member. 
// Applicable only to tk_union
ACE_INLINE CORBA_Any_ptr
CORBA_TypeCode::member_label (CORBA_ULong index, CORBA_Environment &env) const
{
  if (_prv_state->tc_member_count_known_ && _prv_state->tc_member_label_list_known_)
    {
      if (index >= 0 && index < _prv_state->tc_member_count_)
	{
	  (void)_prv_state->tc_member_label_list_[index]->AddRef();
	  return _prv_state->tc_member_label_list_[index];
	}
      else
	{
	  env.exception(new CORBA_Bounds());
	  return 0;
	}
    }
  else
    {
      return prv_member_label(index, env);
    }
}

// only applicable to tk_unions
ACE_INLINE CORBA_TypeCode_ptr
CORBA_TypeCode::discriminator_type (CORBA_Environment &env) const
{
  if (_kind == tk_union)
    {
      if (_prv_state->tc_discriminator_type_known_)
	{
	  return CORBA_TypeCode::_duplicate(_prv_state->tc_discriminator_type_);
	}
      else
	{
	  return prv_discriminator_type(env);
	}
    }
  else
    {
      env.exception( new CORBA_BadKind());
      return (CORBA_TypeCode_ptr)0;
    }
}

// only applicable to tk_unions
ACE_INLINE CORBA_Long
CORBA_TypeCode::default_index (CORBA_Environment &env) const
{
  if (_kind == tk_union)
    {
      if (_prv_state->tc_default_index_used_known_)
	{
	  return _prv_state->tc_default_index_used_;
	}
      else
	{
	  return prv_default_index (env);
	}
    }
  else
    {
      env.exception( new CORBA_BadKind());
      return 0;
    }
}

// returns the length. Applicable only to string, sequence, and arrays
ACE_INLINE CORBA_ULong
CORBA_TypeCode::length (CORBA_Environment &env) const
{
  switch (_kind)
    {
    case tk_sequence:
    case tk_array:
    case tk_string:
    case tk_wstring:
      if (_prv_state->tc_length_known_)
	{
	  return _prv_state->tc_length_;
	}
      else
	{
	  return prv_length(env);
	}
    default:
      env.exception( new CORBA_BadKind());
      return 0;
    }
}

// returns the length. Applicable only to string, sequence, and arrays
ACE_INLINE CORBA_TypeCode_ptr
CORBA_TypeCode::content_type (CORBA_Environment &env) const
{
  switch (_kind)
    {
    case tk_sequence:
    case tk_array:
    case tk_alias:
      if (_prv_state->tc_content_type_known_)
	{
	  return CORBA_TypeCode::_duplicate(_prv_state->tc_content_type_);
	}
      else
	{
	  return prv_content_type(env);
	}
    default:
      env.exception( new CORBA_BadKind());
      return 0;
    }
}

// calculate size of the typecode
ACE_INLINE size_t
CORBA_TypeCode::size (CORBA_Environment &env)
{
  if (_prv_state->tc_size_known_)
    {
      return _prv_state->tc_size_;
    }
  else
    {
      return prv_size (env);
    }
}

// calculate alignment requirements of the typecode
ACE_INLINE size_t
CORBA_TypeCode::alignment (CORBA_Environment &env)
{
  if (_prv_state->tc_alignment_known_)
    {
      return _prv_state->tc_alignment_;
    }
  else
    {
      return prv_alignment (env);
    }
}

// compute the padded size of the discriminant
ACE_INLINE CORBA_ULong
CORBA_TypeCode::TAO_discrim_pad_size (CORBA_Environment &env)
{
  if (_kind == tk_union)
    {
      if (_prv_state->tc_discrim_pad_size_known_)
	{
	  return _prv_state->tc_discrim_pad_size_;
	}
      else
	{
	  return prv_discrim_pad_size (env);
	}
    }
  else
    {
      env.exception (new CORBA_BadKind());
      return 0;
    }
}
