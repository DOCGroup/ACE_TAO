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
  return this->target_; 
}

// Return the operation name for the request.
ACE_INLINE const CORBA::Char *
CORBA_Request::operation (void) const 
{
  return this->opname_; 
}

// Return the arguments for the request.
ACE_INLINE CORBA::NVList_ptr 
CORBA_Request::arguments (void) 
{ 
  return this->args_; 
}

// Return the result for the request.
ACE_INLINE CORBA::NamedValue_ptr 
CORBA_Request::result (void) 
{
  return this->result_; 
}

// Return the exceptions resulting from this request.
ACE_INLINE CORBA::ExceptionList_ptr 
CORBA_Request::exceptions (void) 
{ 
  return &this->exceptions_; 
}

// Return the request's contexts
ACE_INLINE CORBA::ContextList_ptr
CORBA_Request::contexts (void)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                    0);
}

// Return the <Environment> for this request.
ACE_INLINE CORBA::Environment *
CORBA_Request::env (void) 
{
  return &this->env_; 
}

// The argument manipulation helper functions

ACE_INLINE CORBA_Any &
CORBA_Request::add_in_arg (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_element (CORBA::ARG_IN, ACE_TRY_ENV)->any_;
}

ACE_INLINE CORBA_Any &
CORBA_Request::add_in_arg (const CORBA::Char *name)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_item (name, CORBA::ARG_IN, ACE_TRY_ENV)->any_;
}

ACE_INLINE CORBA_Any &
CORBA_Request::add_inout_arg (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_element (CORBA::ARG_INOUT, ACE_TRY_ENV)->any_;
}

ACE_INLINE CORBA_Any &
CORBA_Request::add_inout_arg (const CORBA::Char *name)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_item (name, CORBA::ARG_INOUT, ACE_TRY_ENV)->any_;
}

ACE_INLINE CORBA_Any &
CORBA_Request::add_out_arg (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_element (CORBA::ARG_OUT, ACE_TRY_ENV)->any_;
}

ACE_INLINE CORBA_Any &
CORBA_Request::add_out_arg (const CORBA::Char *name)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_item (name, CORBA::ARG_OUT, ACE_TRY_ENV)->any_;
}

ACE_INLINE void
CORBA_Request::set_return_type (CORBA::TypeCode_ptr tc)
{
  CORBA::Any newtype (tc);
  this->result_->any_ = newtype;
}

ACE_INLINE CORBA_Any &
CORBA_Request::return_value (void )
{
  return this->result_->any_;
}

ACE_INLINE CORBA::Context_ptr
CORBA_Request::ctx (void) const
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                    0);
}

ACE_INLINE void
CORBA_Request::ctx (CORBA::Context_ptr ctx)
{
  if (ctx)
    ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                    CORBA::COMPLETED_NO));
}

// *************************************************************
// Inline operations for class CORBA_Request_var
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
