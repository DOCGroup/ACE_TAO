// -*- C++ -*-

// ===================================================================
/**
 *  @file   RequestInfo_Util.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_REQUEST_INFO_UTIL_H
#define TAO_REQUEST_INFO_UTIL_H

#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI/pi_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class StringSeq;
  class Any;
}

namespace Dynamic
{
  class ParameterList;
  class ExceptionList;
  typedef CORBA::StringSeq ContextList;
  typedef CORBA::StringSeq RequestContext;
}

/**
 * @class TAO_RequestInfo_Util
 *
 * @brief Helper class that provides methods useful for both
 * PortableInterceptor::ClientRequestInfo and
 * PortableInterceptor::ServerRequestInfo objects.
 */
class TAO_PI_Export TAO_RequestInfo_Util
{
public:

  /**
   * @name Factory Methods
   *
   * These are methods called by TAO_{Client,Server}RequestInfo and
   * their subclasses.  Rather than having the same factory code in
   * each subclass, we factor the common code in to this utility
   * class.
   */
  //@{
  static Dynamic::ParameterList * make_parameter_list (void);

  static Dynamic::ExceptionList * make_exception_list (void);

  static Dynamic::ContextList * make_context_list (void);

  static Dynamic::RequestContext * make_request_context (void);

  static CORBA::Any * make_any (CORBA::Boolean tk_void_any);
  //@}
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif

#include /**/ "ace/post.h"

#endif /* TAO_REQUEST_INFO_UTIL_H */
