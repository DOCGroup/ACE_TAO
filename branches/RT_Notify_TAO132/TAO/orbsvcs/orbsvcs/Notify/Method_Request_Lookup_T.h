/* -*- C++ -*- */
/**
 *  @file Method_Request_Lookup_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_METHOD_REQUEST_LOOKUP_T_H
#define TAO_NS_METHOD_REQUEST_LOOKUP_T_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Worker.h"
#include "ProxySupplier.h"

/**
 * @class TAO_NS_Method_Request_Lookup_T
 *
 * @brief
 *
 */
template <class EVENT, class PROXY, class EVENT_PARAM, class PROXY_PARAM>
class TAO_Notify_Export TAO_NS_Method_Request_Lookup_T : public TAO_ESF_Worker<TAO_NS_ProxySupplier>
{
public:
  /// Constuctor
  TAO_NS_Method_Request_Lookup_T (EVENT_PARAM event, PROXY_PARAM proxy);

  /// Destructor
  virtual ~TAO_NS_Method_Request_Lookup_T ();

  /// Execute the dispatch operation.
  int execute_i (ACE_ENV_SINGLE_ARG_DECL);

protected:
  ///= TAO_ESF_Worker method
  virtual void work (TAO_NS_ProxySupplier* proxy_supplier ACE_ENV_ARG_DECL);

  /// The Event
  EVENT event_;

  /// The Proxy
  PROXY proxy_consumer_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Lookup_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Method_Request_Lookup_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Method_Request_Lookup_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_NS_METHOD_REQUEST_LOOKUP_T_H */
