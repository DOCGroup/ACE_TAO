/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Upcall_Wrapper.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_UPCALL_WRAPPER_H
#define TAO_UPCALL_WRAPPER_H

#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "tao/PortableServer/ServerRequestInfo.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/ServerInterceptorAdapter.h"
#include "tao/SArgument_T.h"

/**
 * @class TAO_Upcall_Wrapper
 *
 * @brief Wraps the activities of the _skel operations.
 *
 */
class TAO_PortableServer_Export TAO_Upcall_Wrapper
{
public:
  TAO_Upcall_Wrapper (TAO::SArgument * _tao_arguments,
                      size_t nargs,
                      TAO::SArgument * _tao_retval,
                      TAO_ServerRequest * _tao_server_request,
                      void * _tao_servant_upcall)
    : arglist_ (_tao_arguments),
      nargs_ (nargs),
      retval (_tao_retval),
      tao_server_request (_tao_server_request),
      servant_upcall_ (_tao_servant_upcall)
  {}

  virtual ~TAO_Upcall_Wrapper (void);

  void pre_upcall (void)
  {
  }

  void post_upcall (void)
  {
  }

private:
  TAO::SArgument * arglist_;
  size_t nargs_;
  TAO::SArgument * retval_;
  TAO_ServerRequest * tao_server_request_;
  void * servant_upcall_;
};

#include /**/ "ace/post.h"

#endif /* TAO_UPCALL_WRAPPER_H */
