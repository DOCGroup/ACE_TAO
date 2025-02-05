// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::Request_ptr
CORBA::Request::_duplicate (CORBA::Request_ptr x)
{
  if (x)
    {
      x->_incr_refcount ();
    }

  return x;
}

ACE_INLINE CORBA::Request_ptr
CORBA::Request::_nil ()
{
  return nullptr;
}

ACE_INLINE CORBA::Object_ptr
CORBA::Request::target () const
{
  return this->target_;
}

// Return the operation name for the request.
ACE_INLINE const CORBA::Char *
CORBA::Request::operation () const
{
  return this->opname_;
}

// Return the arguments for the request.
ACE_INLINE CORBA::NVList_ptr
CORBA::Request::arguments ()
{
  return this->args_;
}

// Return the result for the request.
ACE_INLINE CORBA::NamedValue_ptr
CORBA::Request::result ()
{
  return this->result_;
}

// Return the exceptions resulting from this request.
ACE_INLINE CORBA::ExceptionList_ptr
CORBA::Request::exceptions ()
{
  return this->exceptions_.in ();
}

// Return the request's contexts
ACE_INLINE CORBA::ContextList_ptr
CORBA::Request::contexts ()
{
  return this->contexts_;
}

// The argument manipulation helper functions
ACE_INLINE CORBA::Any &
CORBA::Request::add_in_arg ()
{
  return this->args_->add_element (CORBA::ARG_IN)->any_;
}

ACE_INLINE CORBA::Any &
CORBA::Request::add_in_arg (const CORBA::Char *name)
{
  return this->args_->add_item (name, CORBA::ARG_IN)->any_;
}

ACE_INLINE CORBA::Any &
CORBA::Request::add_inout_arg ()
{
  return this->args_->add_element (CORBA::ARG_INOUT)->any_;
}

ACE_INLINE CORBA::Any &
CORBA::Request::add_inout_arg (const CORBA::Char *name)
{
  return this->args_->add_item (name, CORBA::ARG_INOUT)->any_;
}

ACE_INLINE CORBA::Any &
CORBA::Request::add_out_arg ()
{
  return this->args_->add_element (CORBA::ARG_OUT)->any_;
}

ACE_INLINE CORBA::Any &
CORBA::Request::add_out_arg (const CORBA::Char *name)
{
  return this->args_->add_item (name, CORBA::ARG_OUT)->any_;
}

ACE_INLINE void
CORBA::Request::set_return_type (CORBA::TypeCode_ptr tc)
{
  this->result_->any_._tao_set_typecode (tc);
}

ACE_INLINE CORBA::Any &
CORBA::Request::return_value ()
{
  return this->result_->any_;
}

ACE_INLINE CORBA::Context_ptr
CORBA::Request::ctx () const
{
  return this->ctx_;
}

ACE_INLINE void
CORBA::Request::ctx (CORBA::Context_ptr ctx)
{
  this->ctx_ = ctx;
}

ACE_INLINE void
CORBA::Request::_tao_lazy_evaluation (bool lazy_evaluation)
{
  this->lazy_evaluation_ = lazy_evaluation;
}

ACE_INLINE int
CORBA::Request::_tao_byte_order () const
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
CORBA::Request::raw_user_exception ()
{
  return this->raw_user_exception_;
}

ACE_INLINE CORBA::Boolean
CORBA::Request::response_received ()
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    this->lock_,
                    false);

  return this->response_received_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
