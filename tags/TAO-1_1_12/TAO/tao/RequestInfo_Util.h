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

#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DynamicC.h"

/**
 * @class TAO_RequestInfo_Util
 *
 * @brief Helper class that provides methods useful for both
 * PortableInterceptor::ClientRequestInfo and
 * PortableInterceptor::ServerRequestInfo objects.
 */
class TAO_Export TAO_RequestInfo_Util
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
  static Dynamic::ParameterList * make_parameter_list (
    CORBA::Environment &ACE_TRY_ENV);

  static Dynamic::ExceptionList * make_exception_list (
    CORBA::Environment &ACE_TRY_ENV);

  static Dynamic::ContextList * make_context_list (
    CORBA::Environment &ACE_TRY_ENV);

  static Dynamic::RequestContext * make_request_context (
    CORBA::Environment &ACE_TRY_ENV);

  static CORBA::Any * make_any (CORBA::Boolean tk_void_any,
                                CORBA::Environment &ACE_TRY_ENV);
  //@}

};

#include "ace/post.h"

#endif /* TAO_REQUEST_INFO_UTIL_H */
