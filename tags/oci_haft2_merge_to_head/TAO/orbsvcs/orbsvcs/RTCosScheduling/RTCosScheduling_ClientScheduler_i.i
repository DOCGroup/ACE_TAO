/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    RTCosScheduling_ClientScheduler_i.i
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 */
//=============================================================================

ACE_INLINE char *
TAO::RTCosScheduling_ClientScheduler_Interceptor::name (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
return CORBA::string_dup(this->name_);
}

ACE_INLINE void
TAO::RTCosScheduling_ClientScheduler_Interceptor::destroy (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
}

ACE_INLINE void
TAO::RTCosScheduling_ClientScheduler_Interceptor::send_poll (
  PortableInterceptor::ClientRequestInfo_ptr ri
  ACE_ENV_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG(ri);
}

ACE_INLINE void
TAO::RTCosScheduling_ClientScheduler_Interceptor::receive_reply (
  PortableInterceptor::ClientRequestInfo_ptr ri
  ACE_ENV_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG(ri);
}

ACE_INLINE void
TAO::RTCosScheduling_ClientScheduler_Interceptor::receive_exception (
  PortableInterceptor::ClientRequestInfo_ptr ri
  ACE_ENV_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
                 PortableInterceptor::ForwardRequest))
{
  ACE_UNUSED_ARG(ri);
}

ACE_INLINE void
TAO::RTCosScheduling_ClientScheduler_Interceptor::receive_other (
  PortableInterceptor::ClientRequestInfo_ptr ri
  ACE_ENV_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
                 PortableInterceptor::ForwardRequest))
{
  ACE_UNUSED_ARG(ri);
}
