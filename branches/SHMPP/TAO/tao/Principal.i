// $Id$

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::Principal_ptr principal)
{
  return (CORBA::Boolean) (principal == 0);
}

ACE_INLINE CORBA::ULong
CORBA_Principal::_decr_refcnt (void)
{
  {
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

ACE_INLINE void
CORBA::release (CORBA::Principal_ptr principal)
{
  if (principal)
    principal->_decr_refcnt ();
}

ACE_INLINE CORBA::ULong
CORBA_Principal::_incr_refcnt (void)
{
  return this->refcount_++;
}


ACE_INLINE CORBA_Principal*
CORBA_Principal::_duplicate (CORBA_Principal* x)
{
  if (x != 0)
    x->_incr_refcnt ();
  return x;
}


ACE_INLINE CORBA_Principal*
CORBA_Principal::_nil (void)
{
  return 0;
}

// *************************************************************
// Inline operations for class CORBA_Principal_var
// *************************************************************

ACE_INLINE
CORBA_Principal_var::CORBA_Principal_var (void)
  : ptr_ (CORBA_Principal::_nil ())
{
}

ACE_INLINE
CORBA_Principal_var::CORBA_Principal_var (CORBA_Principal_ptr p)
  : ptr_ (p)
{}

ACE_INLINE
CORBA_Principal_var::~CORBA_Principal_var (void)
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA_Principal_ptr
CORBA_Principal_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_Principal_var::CORBA_Principal_var (const CORBA_Principal_var &p)
  : ptr_ (CORBA_Principal::_duplicate (p.ptr ()))
{}

ACE_INLINE CORBA_Principal_var &
CORBA_Principal_var::operator= (CORBA_Principal_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_Principal_var &
CORBA_Principal_var::operator= (const CORBA_Principal_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = CORBA_Principal::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA_Principal_var::operator const CORBA_Principal_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_Principal_var::operator CORBA_Principal_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_Principal_ptr
CORBA_Principal_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_Principal_ptr
CORBA_Principal_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_Principal_ptr &
CORBA_Principal_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_Principal_ptr &
CORBA_Principal_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_Principal::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA_Principal_ptr
CORBA_Principal_var::_retn (void)
{
  // yield ownership
  CORBA_Principal_ptr val = this->ptr_;
  this->ptr_ = CORBA_Principal::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_Principal_out
// *************************************************************

ACE_INLINE
CORBA_Principal_out::CORBA_Principal_out (CORBA_Principal_ptr &p)
        : ptr_ (p)
{
  this->ptr_ = CORBA_Principal::_nil ();
}

ACE_INLINE
CORBA_Principal_out::CORBA_Principal_out (CORBA_Principal_var &p)
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_Principal::_nil ();
}

ACE_INLINE
CORBA_Principal_out::CORBA_Principal_out (CORBA_Principal_out &p)
  : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA_Principal_out &
CORBA_Principal_out::operator= (CORBA_Principal_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA_Principal_out &
CORBA_Principal_out::operator= (const CORBA_Principal_var &p)
{
  this->ptr_ = CORBA_Principal::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_Principal_out &
CORBA_Principal_out::operator= (CORBA_Principal_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_Principal_out::operator CORBA_Principal_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_Principal_ptr &
CORBA_Principal_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_Principal_ptr
CORBA_Principal_out::operator-> (void)
{
  return this->ptr_;
}
