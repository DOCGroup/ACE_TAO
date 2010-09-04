// -*- C++ -*-

//=============================================================================
/**
 *  @file   ForwardCtrlServerInterceptor.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef FORWARDCTRLSERVERINTERCEPTOR_H
#define FORWARDCTRLSERVERINTERCEPTOR_H

#include "tao/PI_Server/PI_Server.h"
#include "tao/PortableInterceptorC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class ForwardCtrlServerInterceptor
  : public PortableInterceptor::ServerRequestInterceptor
{
public:
  ForwardCtrlServerInterceptor();
  ~ForwardCtrlServerInterceptor();

  virtual char * name (void);

  virtual void destroy (void);

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr);

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
