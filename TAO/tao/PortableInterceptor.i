// -*- C++ -*-
//
// $Id$

#if (TAO_HAS_INTERCEPTORS == 1)
ACE_INLINE
TAO_ClientRequestInterceptor_Adapter::TAO_ClientRequestInterceptor_Adapter
  (TAO_ClientRequestInterceptor_List::TYPE &interceptors)
    : interceptors_ (interceptors),
      len_ (0)
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
  for (size_t i = 0; i < this->len_; ++i)
    {
      this->interceptors_[i]->send_request (ri
                                            TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

ACE_INLINE void
TAO_ClientRequestInterceptor_Adapter::
receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri,
               CORBA::Environment &ACE_TRY_ENV)
{
  for (size_t i = 0; i < this->len_; ++i)
    {
      this->interceptors_[i]->receive_reply (ri
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
  for (size_t i = 0; i < this->len_; ++i)
    {
      this->interceptors_[i]->receive_exception (ri
                                                 TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

ACE_INLINE
TAO_ServerRequestInterceptor_Adapter::TAO_ServerRequestInterceptor_Adapter
  (TAO_ServerRequestInterceptor_List::TYPE &interceptors)
    : interceptors_ (interceptors),
      len_ (0)
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
  for (size_t i = 0; i < this->len_; ++i)
    {
      this->interceptors_[i]->receive_request (ri
                                               TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

ACE_INLINE void
TAO_ServerRequestInterceptor_Adapter::
send_reply (PortableInterceptor::ServerRequestInfo_ptr ri,
            CORBA::Environment &ACE_TRY_ENV)
{
  for (size_t i = 0; i < this->len_; ++i)
    {
      this->interceptors_[i]->send_reply (ri
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
  for (size_t i = 0; i < this->len_; ++i)
    {
      this->interceptors_[i]->send_exception (ri
                                              TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  ACE_UNUSED_ARG (ACE_TRY_ENV);

}
#endif /* TAO_HAS_INTERCEPTORS */
