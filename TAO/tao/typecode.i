ACE_INLINE CORBA::TCKind
CORBA_TypeCode::kind (CORBA::Environment &env) const
{
  env.clear ();
  return this->kind_;
}

// Returns true if the two typecodes are identical
ACE_INLINE CORBA::Boolean
CORBA_TypeCode::equal (const CORBA::TypeCode_ptr tc,
                       CORBA::Environment &env) const
{
  if (this->kind_ != tc->kind (env))
    // simple case
    return CORBA::B_FALSE;
  else
    // typecode kinds are same
    return this->private_equal (tc, env);
}

// just fetch the 'kind' field out of the typecode
ACE_INLINE void *
CORBA_TypeCode::operator new (size_t s)
{
  return ::operator new (s);
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA_TypeCode::_duplicate (CORBA::TypeCode_ptr tc)
{
  if (tc)
    tc->AddRef ();
  return tc;
}

// returns the Repository ID
ACE_INLINE const char *
CORBA_TypeCode::id (CORBA::Environment &env) const
{
  env.clear ();

  // if already precomputed
  if (this->private_state_->tc_id_known_)
    return this->private_state_->tc_id_;
  else
    return this->private_id (env);
}

// returns the string name
ACE_INLINE const char *
CORBA_TypeCode::name (CORBA::Environment &env) const
{
  env.clear ();

  // if already precomputed
  if (this->private_state_->tc_name_known_)
    return this->private_state_->tc_name_;
  else
    return this->private_name (env);
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
  if (this->private_state_->tc_member_count_known_)
    return this->private_state_->tc_member_count_;
  else
    return this->private_member_count (env);
}

// calculate size of the typecode
ACE_INLINE size_t
CORBA_TypeCode::size (CORBA::Environment &env)
{
  if (this->private_state_->tc_size_known_)
    return this->private_state_->tc_size_;
  else
    return this->private_size (env);
}

// calculate alignment requirements of the typecode
ACE_INLINE size_t
CORBA_TypeCode::alignment (CORBA::Environment &env)
{
  if (this->private_state_->tc_alignment_known_)
    return this->private_state_->tc_alignment_;
  else
    return this->private_alignment (env);
}

ACE_INLINE void *
CORBA_TypeCode::operator new (size_t, void *p)
{
  return p;
}
