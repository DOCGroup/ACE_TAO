// -*- C++ -*-
//
// $Id$

ACE_INLINE CORBA::Request_ptr
CORBA::Request::_duplicate (CORBA::Request_ptr x)
{
  if (x != 0)
    {
      x->_incr_refcnt ();
    }

  return x;
}

ACE_INLINE CORBA::Request_ptr
CORBA::Request::_nil (void)
{
  return 0;
}

ACE_INLINE CORBA::Object_ptr
CORBA::Request::target (void) const
{
  return this->target_;
}

// Return the operation name for the request.
ACE_INLINE const CORBA::Char *
CORBA::Request::operation (void) const
{
  return this->opname_;
}

// Return the arguments for the request.
ACE_INLINE CORBA::NVList_ptr
CORBA::Request::arguments (void)
{
  return this->args_;
}

// Return the result for the request.
ACE_INLINE CORBA::NamedValue_ptr
CORBA::Request::result (void)
{
  return this->result_;
}

// Return the exceptions resulting from this request.
ACE_INLINE CORBA::ExceptionList_ptr
CORBA::Request::exceptions (void)
{
  return this->exceptions_.in ();
}

// Return the request's contexts
ACE_INLINE CORBA::ContextList_ptr
CORBA::Request::contexts (void)
{
  return this->contexts_;
}

//// *** DEPRECATED ***  Return the <Environment> for this request.
// ACE_INLINE CORBA::Environment *
// CORBA::Request::env (void)
// {
//   return &this->env_;
// }

// The argument manipulation helper functions

ACE_INLINE CORBA::Any &
CORBA::Request::add_in_arg (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_element (CORBA::ARG_IN ACE_ENV_ARG_PARAMETER)->any_;
}

ACE_INLINE CORBA::Any &
CORBA::Request::add_in_arg (const CORBA::Char *name)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_item (name, CORBA::ARG_IN ACE_ENV_ARG_PARAMETER)->any_;
}

ACE_INLINE CORBA::Any &
CORBA::Request::add_inout_arg (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_element (CORBA::ARG_INOUT ACE_ENV_ARG_PARAMETER)->any_;
}

ACE_INLINE CORBA::Any &
CORBA::Request::add_inout_arg (const CORBA::Char *name)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_item (name, CORBA::ARG_INOUT ACE_ENV_ARG_PARAMETER)->any_;
}

ACE_INLINE CORBA::Any &
CORBA::Request::add_out_arg (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_element (CORBA::ARG_OUT ACE_ENV_ARG_PARAMETER)->any_;
}

ACE_INLINE CORBA::Any &
CORBA::Request::add_out_arg (const CORBA::Char *name)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  return this->args_->add_item (name, CORBA::ARG_OUT ACE_ENV_ARG_PARAMETER)->any_;
}

ACE_INLINE void
CORBA::Request::set_return_type (CORBA::TypeCode_ptr tc)
{
  CORBA::Any newtype (CORBA::TypeCode::_duplicate (tc),
                      0);
  this->result_->any_ = newtype;
}

ACE_INLINE CORBA::Any &
CORBA::Request::return_value (void )
{
  return this->result_->any_;
}

ACE_INLINE CORBA::Context_ptr
CORBA::Request::ctx (void) const
{
  return this->ctx_;
}

ACE_INLINE void
CORBA::Request::ctx (CORBA::Context_ptr ctx)
{
  this->ctx_ = ctx;
}

ACE_INLINE void
CORBA::Request::_tao_lazy_evaluation (int lazy_evaluation)
{
  this->lazy_evaluation_ = lazy_evaluation;
}

ACE_INLINE int
CORBA::Request::_tao_byte_order (void) const
{
  return this->byte_order_;
}

ACE_INLINE void
CORBA::Request::_tao_byte_order (int byte_order)
{
  this->byte_order_ = byte_order;
}

ACE_INLINE void
CORBA::Request::raw_user_exception (TAO_InputCDR &cdr)
{
  this->raw_user_exception_.set (cdr.start ()->rd_ptr (),
                                 cdr.start ()->length (),
                                 1);
}

ACE_INLINE ACE_CString &
CORBA::Request::raw_user_exception (void)
{
  return this->raw_user_exception_;
}
