/* -*- C++ -*- */
/**
 *  @file Method_Request_Dispatch_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_METHOD_REQUEST_DISPATCH_T_H
#define TAO_Notify_METHOD_REQUEST_DISPATCH_T_H
#include /**/ "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"

/**
 * @class TAO_Notify_Method_Request_Dispatch_T
 *
 * @brief
 *
 */
template <class EVENT, class PROXY, class EVENT_PARAM, class PROXY_PARAM>
class TAO_Notify_Export TAO_Notify_Method_Request_Dispatch_T
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Dispatch_T (EVENT_PARAM event, PROXY_PARAM proxy, CORBA::Boolean filtering);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Dispatch_T ();

  /// Execute the dispatch operation.
  int execute_i (ACE_ENV_SINGLE_ARG_DECL);

protected:
  /// The Event
  EVENT event_;

  /// The Proxy
  PROXY proxy_supplier_;

  /// Flag is true if we want to do fintering else false.
  CORBA::Boolean filtering_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Dispatch_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Method_Request_Dispatch_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Method_Request_Dispatch_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_METHOD_REQUEST_DISPATCH_T_H */
