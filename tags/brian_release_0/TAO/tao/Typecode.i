// -*- C++ -*-
//
// $Id$

ACE_INLINE CORBA::ULong
CORBA::TypeCode::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->refcount_lock_, 0);
  return this->refcount_++;
}

ACE_INLINE CORBA::ULong
CORBA::TypeCode::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }
  delete this;
  return 0;
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::TypeCode::_duplicate (CORBA::TypeCode_ptr tc)
{
  if (tc)
    tc->_incr_refcnt ();
  return tc;
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::TypeCode::_nil (void)
{
  return (CORBA::TypeCode_ptr)0;
}

ACE_INLINE CORBA::TCKind
CORBA::TypeCode::kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return (CORBA::TCKind) this->kind_;
}

// Returns true if the two typecodes are equivalent.
ACE_INLINE CORBA::Boolean
CORBA::TypeCode::equivalent (CORBA::TypeCode_ptr tc
                            ACE_ENV_ARG_DECL) const
{
  return this->equ_common (tc,
                           1
                           ACE_ENV_ARG_PARAMETER);
}

// Returns true if the two typecodes are identical.
ACE_INLINE CORBA::Boolean
CORBA::TypeCode::equal (CORBA::TypeCode_ptr tc
                       ACE_ENV_ARG_DECL) const
{
  return this->equ_common (tc,
                           0
                           ACE_ENV_ARG_PARAMETER);
}

// returns the Repository ID
ACE_INLINE const char *
CORBA::TypeCode::id (ACE_ENV_SINGLE_ARG_DECL) const
{
  // if already precomputed
  if (this->private_state_->tc_id_known_)
    return this->private_state_->tc_id_;
  else
    return this->private_id (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// returns the string name
ACE_INLINE const char *
CORBA::TypeCode::name (ACE_ENV_SINGLE_ARG_DECL) const
{
  // if already precomputed
  if (this->private_state_->tc_name_known_)
    return this->private_state_->tc_name_;
  else
    return this->private_name (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// Return the number of members defined by this typecode
//
// Applicable to struct, union, enum, alias, and except
// For the rest of the cases, raises the BadKind exception.

ACE_INLINE CORBA::ULong
CORBA::TypeCode::member_count (ACE_ENV_SINGLE_ARG_DECL) const
{
  // if already precomputed
  if (this->private_state_->tc_member_count_known_)
    return this->private_state_->tc_member_count_;
  else
    return this->private_member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE CORBA::TypeCode::OFFSET_MAP *
CORBA::TypeCode::offset_map (void) const
{
  return this->offset_map_;
}

ACE_INLINE void
CORBA::TypeCode::offset_map (CORBA::TypeCode::OFFSET_MAP *map)
{
  this->offset_map_ = map;
}

// ************************************************************
// These are in CORBA namespace

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::TypeCode_ptr obj)
{
  return obj == 0;
}

ACE_INLINE void
CORBA::release (CORBA::TypeCode_ptr obj)
{
  if (obj)
    obj->_decr_refcnt ();
}

