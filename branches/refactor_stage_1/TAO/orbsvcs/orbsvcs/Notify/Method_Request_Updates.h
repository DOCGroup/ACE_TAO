/* -*- C++ -*- */
/**
 *  @file Method_Request_Updates.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_METHOD_REQUEST_UPDATES_H
#define TAO_NS_METHOD_REQUEST_UPDATES_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Method_Request.h"
#include "EventTypeSeq.h"
#include "Refcountable.h"

class TAO_NS_Proxy;

/**
 * @class TAO_NS_Method_Request_Updates
 *
 * @brief Command Object to send updates to proxys.
 *
 */
class TAO_Notify_Export TAO_NS_Method_Request_Updates : public TAO_NS_Method_Request
{
public:
  /// Constuctor
  TAO_NS_Method_Request_Updates (const TAO_NS_EventTypeSeq& added, const TAO_NS_EventTypeSeq& removed, TAO_NS_Proxy* proxy);

  /// Destructor
  ~TAO_NS_Method_Request_Updates ();

  /// Create a copy of this object.
  TAO_NS_Method_Request* copy (void);

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

private:
  /// The Updates
  const TAO_NS_EventTypeSeq added_;
  const TAO_NS_EventTypeSeq removed_;

  /// The proxy that will receive the updates.
  TAO_NS_Proxy* proxy_;

  /// Guard to automatically inc/decr ref count on the proxy.
  TAO_NS_Refcountable_Guard refcountable_guard_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Updates.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_METHOD_REQUEST_UPDATES_H */
