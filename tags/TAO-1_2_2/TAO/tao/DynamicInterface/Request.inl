// -*- C++ -*-
// $Id$

ACE_INLINE CORBA_Request_ptr
CORBA_Request::_duplicate (CORBA_Request_ptr x)
{
  if (x != 0)
    {
      x->_incr_refcnt ();
    }

  return x;
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
  return this->exceptions_.in ();
}

// Return the request's contexts
ACE_INLINE CORBA::ContextList_ptr
CORBA_Request::contexts (void)
{
  return this->contexts_;
}

//// *** DEPRECATED ***  Return the <Environment> for this request.
// ACE_INLINE CORBA::Environment *
// CORBA_Request::env (void)
// {
//   return &this->env_;
// }

// The argument manipulation helper functions

ACE_INLINE CORBA_Any &
CORBA_Request::add_in_arg (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_element (CORBA::ARG_IN ACE_ENV_ARG_PARAMETER)->any_;
}

ACE_INLINE CORBA_Any &
CORBA_Request::add_in_arg (const CORBA::Char *name)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_item (name, CORBA::ARG_IN ACE_ENV_ARG_PARAMETER)->any_;
}

ACE_INLINE CORBA_Any &
CORBA_Request::add_inout_arg (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_element (CORBA::ARG_INOUT ACE_ENV_ARG_PARAMETER)->any_;
}

ACE_INLINE CORBA_Any &
CORBA_Request::add_inout_arg (const CORBA::Char *name)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_item (name, CORBA::ARG_INOUT ACE_ENV_ARG_PARAMETER)->any_;
}

ACE_INLINE CORBA_Any &
CORBA_Request::add_out_arg (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_element (CORBA::ARG_OUT ACE_ENV_ARG_PARAMETER)->any_;
}

ACE_INLINE CORBA_Any &
CORBA_Request::add_out_arg (const CORBA::Char *name)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_item (name, CORBA::ARG_OUT ACE_ENV_ARG_PARAMETER)->any_;
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
  return this->ctx_;
}

ACE_INLINE void
CORBA_Request::ctx (CORBA::Context_ptr ctx)
{
  this->ctx_ = ctx;
}

ACE_INLINE void
CORBA_Request::_tao_lazy_evaluation (int lazy_evaluation)
{
  this->lazy_evaluation_ = lazy_evaluation;
}

ACE_INLINE int
CORBA_Request::_tao_byte_order (void) const
{
  return this->byte_order_;
}

ACE_INLINE void
CORBA_Request::_tao_byte_order (int byte_order)
{
  this->byte_order_ = byte_order;
}

ACE_INLINE void
CORBA_Request::raw_user_exception (TAO_InputCDR &cdr)
{
  this->raw_user_exception_.set (cdr.start ()->rd_ptr (),
                                 cdr.start ()->length (),
                                 1);
}

ACE_INLINE ACE_CString &
CORBA_Request::raw_user_exception (void)
{
  return this->raw_user_exception_;
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
