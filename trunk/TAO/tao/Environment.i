// This may look like C, but it's really -*- C++ -*-
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

ACE_INLINE CORBA_Exception*
CORBA_Environment::exception (void) const
{
  return this->exception_;
}

ACE_INLINE CORBA_Environment *
CORBA_Environment::_duplicate (CORBA_Environment *x)
{
  if (x == 0)
    return 0;
  return new CORBA_Environment (*x);
}

ACE_INLINE CORBA_Environment_ptr
CORBA_Environment::_nil (void)
{
  return (CORBA_Environment_ptr)0;
}

// Inline operations for class CORBA_Environment_var

ACE_INLINE
CORBA_Environment_var::CORBA_Environment_var (void)
  : ptr_ (0)
{}

ACE_INLINE
CORBA_Environment_var::CORBA_Environment_var (CORBA_Environment_ptr p)
  : ptr_ (p)
{}

ACE_INLINE CORBA_Environment_ptr
CORBA_Environment_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_Environment_var::CORBA_Environment_var (const CORBA_Environment_var &p)
  : ptr_ (new CORBA_Environment (*p.ptr ()))
{}

ACE_INLINE
CORBA_Environment_var::~CORBA_Environment_var (void)
{
  delete this->ptr_;
}

ACE_INLINE
CORBA_Environment_var::operator const CORBA_Environment_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_Environment_var::operator CORBA_Environment_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_Environment_ptr
CORBA_Environment_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_Environment_ptr
CORBA_Environment_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_Environment_ptr &
CORBA_Environment_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_Environment_ptr &
CORBA_Environment_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE CORBA_Environment_ptr
CORBA_Environment_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA_Environment_ptr val = this->ptr_;
  this->ptr_ = 0;
  return val;
}

// Inline operations for class CORBA_Environment_out

ACE_INLINE
CORBA_Environment_out::CORBA_Environment_out (CORBA_Environment_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA_Environment_out::CORBA_Environment_out (CORBA_Environment_var &p)
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
CORBA_Environment_out::CORBA_Environment_out (const CORBA_Environment_out &p)
  : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA_Environment_out &
CORBA_Environment_out::operator= (const CORBA_Environment_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA_Environment_out &
CORBA_Environment_out::operator= (CORBA_Environment_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_Environment_out::operator CORBA_Environment_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_Environment_ptr &
CORBA_Environment_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_Environment_ptr
CORBA_Environment_out::operator-> (void)
{
  return this->ptr_;
}
