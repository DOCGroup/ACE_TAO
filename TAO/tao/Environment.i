// -*- C++ -*-
//
// $Id$

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::Environment_ptr env)
{
  return env == 0;
}

ACE_INLINE void
CORBA::release (CORBA::Environment_ptr env)
{
  if (env == 0)
    return;
  delete env;
}

ACE_INLINE CORBA::Exception*
CORBA::Environment::exception (void) const
{
  return this->exception_;
}

ACE_INLINE CORBA::Environment *
CORBA::Environment::_duplicate (CORBA::Environment *x)
{
  if (x == 0)
    return 0;
  return new CORBA::Environment (*x);
}

ACE_INLINE CORBA::Environment_ptr
CORBA::Environment::_nil (void)
{
  return (CORBA::Environment_ptr)0;
}

// Inline operations for class CORBA::Environment_var

ACE_INLINE
CORBA::Environment_var::Environment_var (void)
  : ptr_ (0)
{}

ACE_INLINE
CORBA::Environment_var::Environment_var (CORBA::Environment_ptr p)
  : ptr_ (p)
{}

ACE_INLINE CORBA::Environment_ptr
CORBA::Environment_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA::Environment_var::Environment_var (const CORBA::Environment_var &p)
  : ptr_ (new CORBA::Environment (*p.ptr ()))
{}

ACE_INLINE
CORBA::Environment_var::~Environment_var (void)
{
  delete this->ptr_;
}

ACE_INLINE
CORBA::Environment_var::operator const CORBA::Environment_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
CORBA::Environment_var::operator CORBA::Environment_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA::Environment_ptr
CORBA::Environment_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::Environment_ptr
CORBA::Environment_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::Environment_ptr &
CORBA::Environment_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::Environment_ptr &
CORBA::Environment_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE CORBA::Environment_ptr
CORBA::Environment_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA::Environment_ptr val = this->ptr_;
  this->ptr_ = 0;
  return val;
}

// Inline operations for class CORBA::Environment_out

ACE_INLINE
CORBA::Environment_out::Environment_out (CORBA::Environment_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA::Environment_out::Environment_out (CORBA::Environment_var &p)
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
CORBA::Environment_out::Environment_out (const CORBA::Environment_out &p)
  : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA::Environment_out &
CORBA::Environment_out::operator= (const CORBA::Environment_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA::Environment_out &
CORBA::Environment_out::operator= (CORBA::Environment_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA::Environment_out::operator CORBA::Environment_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA::Environment_ptr &
CORBA::Environment_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::Environment_ptr
CORBA::Environment_out::operator-> (void)
{
  return this->ptr_;
}
