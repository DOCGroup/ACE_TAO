// -*- C++ -*-

//=============================================================================
/**
 *  @file   Crash_Injector_Interceptor.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef CRASH_INJECTOR_INTERCEPTOR_H
#define CRASH_INJECTOR_INTERCEPTOR_H
#include "tao/PortableInterceptorC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Crash_Injector_Interceptor :
  public PortableInterceptor::ServerRequestInterceptor
{
public:
  Crash_Injector_Interceptor();
  ~Crash_Injector_Interceptor();

  virtual char * name (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri
                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr
                           ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));
};
#endif
