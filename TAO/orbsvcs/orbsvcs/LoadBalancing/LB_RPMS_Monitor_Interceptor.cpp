// -*- C++ -*-

#include "LB_RPMS_Monitor_Interceptor.h"

ACE_RCSID (LoadBalancing,
           LB_RPMS_Monitor_Interceptor,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "LB_RPMS_Monitor_Interceptor.inl"
#endif /* __ACE_INLINE__ */


TAO_LB_RPMS_Monitor_Interceptor::TAO_LB_RPMS_Monitor_Interceptor (void)
  : lock_ (),
    request_count_ (0),
    interval_start_ (ACE_OS::gettimeofday ()),
    redirect_table_ ()
{
}

char *
TAO_LB_RPMS_Monitor_Interceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @todo It might be necessary to use an anonymous interceptor,
  //       i.e. an interceptor whose name is the empty string ("") in
  //       case multiple interceptors of this type are registered with
  //       an ORB.
  return CORBA::string_dup ("TAO_LB_RPMS_Monitor_Interceptor");
}

void
TAO_LB_RPMS_Monitor_Interceptor::destroy (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->redirect_table_.destroy ();
}

void
TAO_LB_RPMS_Monitor_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

  ++this->request_count_;

  // If required to do so by the load balancer, redirect all
  // requests back to the target corresponding to the RepositoryId in
  // redirect table.
  this->redirect_table_.find_redirect (ri,
                                       ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_LB_RPMS_Monitor_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
TAO_LB_RPMS_Monitor_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_LB_RPMS_Monitor_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
TAO_LB_RPMS_Monitor_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}
