// $Id$

ACE_INLINE CORBA::ULong
CORBA_TypeCode::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->refcount_lock_, 0);
  return this->refcount_++;
}

ACE_INLINE CORBA::ULong
CORBA_TypeCode::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }
  delete this;
  return 0;
}

ACE_INLINE void
CORBA::release (CORBA::TypeCode_ptr tc)
{
  if (tc)
    tc->_decr_refcnt ();
}

// returns true if the typecode is NULL
ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::TypeCode_ptr tc)
{
  return tc == 0;
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA_TypeCode::_duplicate (CORBA::TypeCode_ptr tc)
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
CORBA_TypeCode::kind (CORBA::Environment &) const
{
  return this->kind_;
}

// Returns true if the two typecodes are identical
ACE_INLINE CORBA::Boolean
CORBA_TypeCode::equal (const CORBA::TypeCode_ptr tc,
                       CORBA::Environment &env) const
{
  if (this->kind_ != tc->kind (env))
    // simple case
    return 0;
  else
    // typecode kinds are same
    return this->private_equal (tc, env);
}

// returns the Repository ID
ACE_INLINE const char *
CORBA_TypeCode::id (CORBA::Environment &env) const
{
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

// ****************************************************************

ACE_INLINE
CORBA_TypeCode_var::CORBA_TypeCode_var (void)
  : ptr_ (CORBA_TypeCode::_nil ())
{}

ACE_INLINE
CORBA_TypeCode_var::CORBA_TypeCode_var (CORBA_TypeCode_ptr p)
  : ptr_ (p)
{}

ACE_INLINE CORBA_TypeCode_ptr
CORBA_TypeCode_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_TypeCode_var::CORBA_TypeCode_var (const CORBA_TypeCode_var &p)
  : ptr_ (CORBA_TypeCode::_duplicate (p.ptr ()))
{}

ACE_INLINE
CORBA_TypeCode_var::~CORBA_TypeCode_var (void)
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA_TypeCode_var &
CORBA_TypeCode_var::operator= (CORBA_TypeCode_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_TypeCode_var &
CORBA_TypeCode_var::operator= (const CORBA_TypeCode_var &p)
{
  if (this != &p)
    {
      CORBA::release (this->ptr_);
      this->ptr_ = CORBA_TypeCode::_duplicate (p.ptr ());
    }
  return *this;
}

ACE_INLINE
CORBA_TypeCode_var::operator const CORBA_TypeCode_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_TypeCode_var::operator CORBA_TypeCode_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_TypeCode_ptr
CORBA_TypeCode_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_TypeCode_ptr
CORBA_TypeCode_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_TypeCode_ptr &
CORBA_TypeCode_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_TypeCode_ptr &
CORBA_TypeCode_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_TypeCode::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA_TypeCode_ptr
CORBA_TypeCode_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA_TypeCode_ptr val = this->ptr_;
  this->ptr_ = CORBA_TypeCode::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_TypeCode_out
// *************************************************************

ACE_INLINE
CORBA_TypeCode_out::CORBA_TypeCode_out (CORBA_TypeCode_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = CORBA_TypeCode::_nil ();
}

ACE_INLINE
CORBA_TypeCode_out::CORBA_TypeCode_out (CORBA_TypeCode_var &p)
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_TypeCode::_nil ();
}

ACE_INLINE
CORBA_TypeCode_out::CORBA_TypeCode_out (const CORBA_TypeCode_out &p)
  : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA_TypeCode_out &
CORBA_TypeCode_out::operator= (const CORBA_TypeCode_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA_TypeCode_out &
CORBA_TypeCode_out::operator= (const CORBA_TypeCode_var &p)
{
  this->ptr_ = CORBA_TypeCode::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_TypeCode_out &
CORBA_TypeCode_out::operator= (CORBA_TypeCode_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_TypeCode_out::operator CORBA_TypeCode_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_TypeCode_ptr &
CORBA_TypeCode_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_TypeCode_ptr
CORBA_TypeCode_out::operator-> (void)
{
  return this->ptr_;
}
