// $Id$

// This may look like C, but it's really -*- C++ -*-

// Return the target of this request.

ACE_INLINE void
CORBA::release (CORBA::Request_ptr req)
{
  if (req)
    req->_decr_refcnt ();
}

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::Request_ptr req)
{
  return (CORBA::Boolean) (req == 0);
}

ACE_INLINE CORBA_Request*
CORBA_Request::_duplicate (CORBA_Request* x)
{
  if (x != 0)
    x->_incr_refcnt ();
  return x;
}

ACE_INLINE CORBA_Request*
CORBA_Request::_nil (void)
{
  return 0;
}

ACE_INLINE CORBA::Object_ptr 
CORBA_Request::target (void) const 
{ 
  return target_; 
}

// Return the operation name for the request.
ACE_INLINE const CORBA::Char *
CORBA_Request::operation (void) const 
{
  return opname_; 
}

// Return the arguments for the request.
ACE_INLINE CORBA::NVList_ptr 
CORBA_Request::arguments (void) 
{ 
  return args_; 
}

// Return the result for the request.
ACE_INLINE CORBA::NamedValue_ptr 
CORBA_Request::result (void) 
{
  return result_; 
}

// Return the exceptions resulting from this request.
ACE_INLINE CORBA::ExceptionList_ptr 
CORBA_Request::exceptions (void) 
{ 
  return &exceptions_; 
}

// Return the <Environment> for this request.
ACE_INLINE CORBA::Environment *
CORBA_Request::env (void) 
{
  return &env_; 
}

// *************************************************************
// Inline operations for class CORBA_NVList_var
// *************************************************************

ACE_INLINE
CORBA_Request_var::CORBA_Request_var (void)
  : ptr_ (CORBA_Request::_nil ())
{
}

ACE_INLINE
CORBA_Request_var::CORBA_Request_var (CORBA_Request_ptr p)
  : ptr_ (p)
{}

ACE_INLINE
CORBA_Request_var::~CORBA_Request_var (void)
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA_Request_ptr
CORBA_Request_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_Request_var::CORBA_Request_var (const CORBA_Request_var &p)
  : ptr_ (CORBA_Request::_duplicate (p.ptr ()))
{}

ACE_INLINE CORBA_Request_var &
CORBA_Request_var::operator= (CORBA_Request_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_Request_var &
CORBA_Request_var::operator= (const CORBA_Request_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = CORBA_Request::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA_Request_var::operator const CORBA_Request_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_Request_var::operator CORBA_Request_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_Request_ptr
CORBA_Request_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_Request_ptr
CORBA_Request_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_Request_ptr &
CORBA_Request_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_Request_ptr &
CORBA_Request_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_Request::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA_Request_ptr
CORBA_Request_var::_retn (void)
{
  // yield ownership
  CORBA_Request_ptr val = this->ptr_;
  this->ptr_ = CORBA_Request::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_Request_out
// *************************************************************

ACE_INLINE
CORBA_Request_out::CORBA_Request_out (CORBA_Request_ptr &p)
        : ptr_ (p)
{
  this->ptr_ = CORBA_Request::_nil ();
}

ACE_INLINE
CORBA_Request_out::CORBA_Request_out (CORBA_Request_var &p)
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_Request::_nil ();
}

ACE_INLINE
CORBA_Request_out::CORBA_Request_out (CORBA_Request_out &p)
  : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA_Request_out &
CORBA_Request_out::operator= (CORBA_Request_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA_Request_out &
CORBA_Request_out::operator= (const CORBA_Request_var &p)
{
  this->ptr_ = CORBA_Request::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_Request_out &
CORBA_Request_out::operator= (CORBA_Request_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_Request_out::operator CORBA_Request_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_Request_ptr &
CORBA_Request_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_Request_ptr
CORBA_Request_out::operator-> (void)
{
  return this->ptr_;
}
