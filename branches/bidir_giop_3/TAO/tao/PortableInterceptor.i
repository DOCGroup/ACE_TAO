// -*- C++ -*-
//
// $Id$

ACE_INLINE
TAO_ClientRequestInterceptor_Adapter::TAO_ClientRequestInterceptor_Adapter
  (TAO_ClientRequestInterceptor_List::TYPE &interceptors)
    : interceptors_ (interceptors),
      len_ (0),
      stack_size_ (0)
{
  this->len_ = interceptors.size ();
}

ACE_INLINE
TAO_ClientRequestInterceptor_Adapter::~TAO_ClientRequestInterceptor_Adapter
  (void)
{
}

ACE_INLINE void
TAO_ClientRequestInterceptor_Adapter::
send_request (PortableInterceptor::ClientRequestInfo_ptr ri,
              CORBA::Environment &ACE_TRY_ENV)
{
  // This method implements one of the "starting" client side
  // interception points.
  for (size_t i = 0 ; i < this->len_; ++i)
    {
      this->interceptors_[i]->send_request (ri
                                            TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // The interception point completed successfully.  Push the
      // interceptor on to the flow stack.
      ++this->stack_size_;
    }

  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

ACE_INLINE void
TAO_ClientRequestInterceptor_Adapter::
receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri,
               CORBA::Environment &ACE_TRY_ENV)
{
  // This is an "ending" interception point so we only process the
  // interceptors pushed on to the flow stack.

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending"
  // interception point.

  // Unwind the stack.
  size_t len = this->stack_size_;
  for (size_t i = 0; i < len; ++i)
    {
      // Pop the interceptor off of the flow stack before it is
      // invoked.  This is necessary to prevent an interceptor already
      // invoked in this "ending" interception point from being
      // invoked in another "ending" interception point.
      --this->stack_size_;

      this->interceptors_[this->stack_size_]->receive_reply (
        ri
        TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  ACE_UNUSED_ARG (ACE_TRY_ENV);

}

ACE_INLINE void
TAO_ClientRequestInterceptor_Adapter::
receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri,
                   CORBA::Environment &ACE_TRY_ENV)
{
  // This is an "ending" interception point so we only process the
  // interceptors pushed on to the flow stack.

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending"
  // interception point.

  // Unwind the flow stack.
  size_t len = this->stack_size_;
  for (size_t i = 0; i < len; ++i)
    {
      // Pop the interceptor off of the flow stack before it is
      // invoked.  This is necessary to prevent an interceptor already
      // invoked in this "ending" interception point from being
      // invoked in another "ending" interception point.
      --this->stack_size_;

      this->interceptors_[this->stack_size_]->receive_exception (
        ri
        TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

ACE_INLINE void
TAO_ClientRequestInterceptor_Adapter::
receive_other (PortableInterceptor::ClientRequestInfo_ptr ri,
               CORBA::Environment &ACE_TRY_ENV)
{
  // This is an "ending" interception point so we only process the
  // interceptors pushed on to the flow stack.

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending"
  // interception point.

  // Unwind the stack.
  size_t len = this->stack_size_;
  for (size_t i = 0; i < len; ++i)
    {
      // Pop the interceptor off of the flow stack before it is
      // invoked.  This is necessary to prevent an interceptor already
      // invoked in this "ending" interception point from being
      // invoked in another "ending" interception point.
      --this->stack_size_;

      this->interceptors_[this->stack_size_]->receive_other (
        ri
        TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  ACE_UNUSED_ARG (ACE_TRY_ENV);

}

// -------------------------------------------------------------------

ACE_INLINE
TAO_ServerRequestInterceptor_Adapter::TAO_ServerRequestInterceptor_Adapter
  (TAO_ServerRequestInterceptor_List::TYPE &interceptors)
    : interceptors_ (interceptors),
      len_ (0),
      stack_size_ (0)
{
  this->len_ = interceptors.size ();
}

ACE_INLINE
TAO_ServerRequestInterceptor_Adapter::~TAO_ServerRequestInterceptor_Adapter
  (void)
{
}

ACE_INLINE void
TAO_ServerRequestInterceptor_Adapter::
receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                 CORBA::Environment &ACE_TRY_ENV)
{
  // This method implements an "intermediate" server side interception
  // point.  Since it is a receiving server side interception point,
  // interceptors are invoked in the same order they were pushed on to
  // the flow stack.

  // @@ The receive_request_service_contexts() interception point
  //    should be the starting one, so interceptors should only be
  //    pushed on to the stack there.  Once that interception point is
  //    implemented, move the "stack pushing" code (i.e. the
  //    "++this->stack_size_" there.

  for (size_t i = 0; i < this->len_; ++i)
    {
      this->interceptors_[i]->receive_request (ri
                                               TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // The interception point completed successfully.  Push the
      // interceptor on to the flow stack.
      ++this->stack_size_;
    }

  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

ACE_INLINE void
TAO_ServerRequestInterceptor_Adapter::
send_reply (PortableInterceptor::ServerRequestInfo_ptr ri,
            CORBA::Environment &ACE_TRY_ENV)
{
  // This is an "ending" interception point so we only process the
  // interceptors pushed on to the flow stack.

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending"
  // interception point.

  // Unwind the stack.
  size_t len = this->stack_size_;
  for (size_t i = 0; i < len; ++i)
    {
      // Pop the interceptor off of the flow stack before it is
      // invoked.  This is necessary to prevent an interceptor already
      // invoked in this "ending" interception point from being
      // invoked in another "ending" interception point.
      --this->stack_size_;

      this->interceptors_[this->stack_size_]->send_reply (
        ri
        TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

ACE_INLINE void
TAO_ServerRequestInterceptor_Adapter::
send_exception (PortableInterceptor::ServerRequestInfo_ptr ri,
                CORBA::Environment &ACE_TRY_ENV)
{
  // This is an "ending" server side interception point so we only
  // process the interceptors pushed on to the flow stack.

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending" server
  // side interception point.

  // Unwind the flow stack.
  size_t len = this->stack_size_;
  for (size_t i = 0; i < len; ++i)
    {
      // Pop the interceptor off of the flow stack before it is
      // invoked.  This is necessary to prevent an interceptor already
      // invoked in this "ending" interception point from being
      // invoked in another "ending" interception point.
      --this->stack_size_;

      this->interceptors_[this->stack_size_]->send_exception (
        ri
        TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  ACE_UNUSED_ARG (ACE_TRY_ENV);
}
