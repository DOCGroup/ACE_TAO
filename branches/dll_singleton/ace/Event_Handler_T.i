/* -*- C++ -*- */
// $Id$

template<class T> ACE_INLINE void
ACE_Event_Handler_T<T>::op_handler (T *op)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::op_handler");
  this->op_handler_ = op;
}

template<class T> ACE_INLINE T *
ACE_Event_Handler_T<T>::op_handler (void)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::op_handler");
  return this->op_handler_;
}

template<class T> ACE_INLINE ACE_HANDLE
ACE_Event_Handler_T<T>::get_handle (void) const
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::get_handle");
  return this->get_handle_ == 0 ? -1 : (this->op_handler_->*get_handle_) ();
}

template<class T> ACE_INLINE void
ACE_Event_Handler_T<T>::set_handle (ACE_HANDLE h)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::set_handle");
  if (this->set_handle_ != 0)
    (this->op_handler_->*set_handle_) (h);
}

template<class T> ACE_INLINE int
ACE_Event_Handler_T<T>::handle_input (ACE_HANDLE fd)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::handle_input");
  return this->input_handler_ == 0 ? 0 : (this->op_handler_->*input_handler_) (fd);
}

template<class T> ACE_INLINE int
ACE_Event_Handler_T<T>::handle_output (ACE_HANDLE fd)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::handle_output");
  return this->output_handler_ == 0 ? 0 : (this->op_handler_->*output_handler_) (fd);
}

template<class T> ACE_INLINE int
ACE_Event_Handler_T<T>::handle_exception (ACE_HANDLE fd)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::handle_exception");
  return this->except_handler_ == 0 ? 0 : (this->op_handler_->*except_handler_) (fd);
}

template<class T> ACE_INLINE int
ACE_Event_Handler_T<T>::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::handle_timeout");
  return this->to_handler_ == 0 ? 0 : (this->op_handler_->*to_handler_) (tv, arg);
}

template<class T> ACE_INLINE int
ACE_Event_Handler_T<T>::handle_close (ACE_HANDLE fd, ACE_Reactor_Mask close_mask)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::handle_close");
  return this->cl_handler_ == 0 ? 0 : (this->op_handler_->*cl_handler_) (fd, close_mask);
}

template<class T> ACE_INLINE int
ACE_Event_Handler_T<T>::handle_signal (int signum, siginfo_t *s, ucontext_t *u)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::handle_signal");
  return this->sig_handler_ == 0 ? 0 : (this->op_handler_->*sig_handler_) (signum, s, u);
}

template<class T> ACE_INLINE ACE_TYPENAME ACE_Event_Handler_T<T>::GET_HANDLE
ACE_Event_Handler_T<T>::handle_get (void)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::handle_get");
  return this->get_handle_;
}

template<class T> ACE_INLINE void
ACE_Event_Handler_T<T>::handle_get (ACE_TYPENAME ACE_Event_Handler_T<T>::GET_HANDLE h)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::handle_get");
  this->get_handle_ = h;
}

template<class T> ACE_INLINE ACE_TYPENAME ACE_Event_Handler_T<T>::SET_HANDLE
ACE_Event_Handler_T<T>::handle_set (void)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::handle_set");
  return this->set_handle_;
}

template<class T> ACE_INLINE void
ACE_Event_Handler_T<T>::handle_set (ACE_TYPENAME ACE_Event_Handler_T<T>::SET_HANDLE h)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::handle_set");
  this->set_handle_ = h;
}

template<class T> ACE_INLINE ACE_TYPENAME ACE_Event_Handler_T<T>::IO_HANDLER
ACE_Event_Handler_T<T>::input_handler (void)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::input_handler");
  return this->input_handler_;
}

template<class T> ACE_INLINE void
ACE_Event_Handler_T<T>::input_handler (ACE_TYPENAME ACE_Event_Handler_T<T>::IO_HANDLER h)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::input_handler");
  this->input_handler_ = h;
}

template<class T> ACE_INLINE ACE_TYPENAME ACE_Event_Handler_T<T>::IO_HANDLER
ACE_Event_Handler_T<T>::output_handler (void)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::output_handler");
  return this->output_handler_;
}

template<class T> ACE_INLINE void
ACE_Event_Handler_T<T>::output_handler (ACE_TYPENAME ACE_Event_Handler_T<T>::IO_HANDLER h)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::output_handler");
  this->output_handler_ = h;
}

template<class T> ACE_INLINE ACE_TYPENAME ACE_Event_Handler_T<T>::IO_HANDLER
ACE_Event_Handler_T<T>::except_handler (void)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::except_handler");
  return this->except_handler_;
}

template<class T> ACE_INLINE void
ACE_Event_Handler_T<T>::except_handler (ACE_TYPENAME ACE_Event_Handler_T<T>::IO_HANDLER h)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::except_handler");
  this->except_handler_ = h;
}

template<class T> ACE_INLINE ACE_TYPENAME ACE_Event_Handler_T<T>::TO_HANDLER
ACE_Event_Handler_T<T>::to_handler (void)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::to_handler");
  return this->to_handler_;
}

template<class T> ACE_INLINE void
ACE_Event_Handler_T<T>::to_handler (ACE_TYPENAME ACE_Event_Handler_T<T>::TO_HANDLER h)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::to_handler");
  this->to_handler_ = h;
}

template<class T> ACE_INLINE ACE_TYPENAME ACE_Event_Handler_T<T>::CL_HANDLER
ACE_Event_Handler_T<T>::cl_handler (void)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::cl_handler");
  return this->cl_handler_;
}

template<class T> ACE_INLINE void
ACE_Event_Handler_T<T>::cl_handler (ACE_TYPENAME ACE_Event_Handler_T<T>::CL_HANDLER h)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::cl_handler");
  this->cl_handler_ = h;
}

template<class T> ACE_INLINE ACE_TYPENAME ACE_Event_Handler_T<T>::SIG_HANDLER
ACE_Event_Handler_T<T>::sig_handler (void)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::sig_handler");
  return this->sig_handler_;
}

template<class T> ACE_INLINE void
ACE_Event_Handler_T<T>::sig_handler (ACE_TYPENAME ACE_Event_Handler_T<T>::SIG_HANDLER h)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::sig_handler");
  this->sig_handler_ = h;
}
