/* -*- C++ -*- $Id$ */

#if (TAO_HAS_INTERCEPTORS == 1)
ACE_INLINE
TAO_ClientRequestInterceptor_Adapter::TAO_ClientRequestInterceptor_Adapter
  (PortableInterceptor::ClientRequestInterceptor_ptr interceptor)
    : interceptor_ (interceptor)
{
}

ACE_INLINE
TAO_ClientRequestInterceptor_Adapter::~TAO_ClientRequestInterceptor_Adapter
  (void)
{
}

ACE_INLINE CORBA::Boolean
TAO_ClientRequestInterceptor_Adapter::valid (void) const
{
  return (! CORBA::is_nil (this->interceptor_.in ()));
}

ACE_INLINE void
TAO_ClientRequestInterceptor_Adapter::
        send_request (PortableInterceptor::ClientRequestInfo_ptr ri,
                      CORBA::Environment &ACE_TRY_ENV)
{
  if (this->valid ())
    this->interceptor_->send_request (ri
                                      TAO_ENV_ARG_PARAMETER);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

ACE_INLINE void
TAO_ClientRequestInterceptor_Adapter::
receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri,
               CORBA::Environment &ACE_TRY_ENV)
{
  if (this->valid ())
    this->interceptor_->receive_reply (ri
                                       TAO_ENV_ARG_PARAMETER);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

ACE_INLINE void
TAO_ClientRequestInterceptor_Adapter::
receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri,
                   CORBA::Environment &ACE_TRY_ENV)
{
  if (this->valid ())
    this->interceptor_->receive_exception (ri
                                           TAO_ENV_ARG_PARAMETER);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

ACE_INLINE
TAO_ServerRequestInterceptor_Adapter::TAO_ServerRequestInterceptor_Adapter
  (PortableInterceptor::ServerRequestInterceptor_ptr interceptor)
    : interceptor_ (interceptor)
{
}

ACE_INLINE
TAO_ServerRequestInterceptor_Adapter::~TAO_ServerRequestInterceptor_Adapter
  (void)
{
}

ACE_INLINE CORBA::Boolean
TAO_ServerRequestInterceptor_Adapter::valid (void) const
{
  return (! CORBA::is_nil (this->interceptor_.in ()));
}

ACE_INLINE void
TAO_ServerRequestInterceptor_Adapter::
receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                 CORBA::Environment &ACE_TRY_ENV)
{
  if (this->valid ())
    this->interceptor_->receive_request (ri
                                         TAO_ENV_ARG_PARAMETER);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

ACE_INLINE void
TAO_ServerRequestInterceptor_Adapter::
send_reply (PortableInterceptor::ServerRequestInfo_ptr ri,
            CORBA::Environment &ACE_TRY_ENV)
{
  if (this->valid ())
    this->interceptor_->send_reply (ri
                                    TAO_ENV_ARG_PARAMETER);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

ACE_INLINE void
TAO_ServerRequestInterceptor_Adapter::
send_exception (PortableInterceptor::ServerRequestInfo_ptr ri,
                CORBA::Environment &ACE_TRY_ENV)
{
  if (this->valid ())
    this->interceptor_->send_exception (ri
                                        TAO_ENV_ARG_PARAMETER);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}
#endif /* TAO_HAS_INTERCEPTORS */
