/* -*- C++ -*- */
/**
 *  @file Method_Request_Lookup.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_LOOKUP_METHOD_REQUEST_H
#define TAO_Notify_LOOKUP_METHOD_REQUEST_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Worker.h"
#include "Method_Request.h"
#include "ProxyConsumer.h"
#include "Consumer_Map.h"
#include "Method_Request_Lookup_T.h"

class TAO_Notify_ProxyConsumer;

typedef TAO_Notify_Method_Request_Lookup_T<const TAO_Notify_Event_var
                                       , TAO_Notify_ProxyConsumer_Guard
                                       , const TAO_Notify_Event_var&
                                       , TAO_Notify_ProxyConsumer*>  TAO_Notify_Method_Request_Lookup_Base;

/**
 * @class TAO_Notify_Method_Request_Lookup
 *
 * @brief Lookup command object looks up the event type of the given event in the consumer map and send the event to each proxysupplier.
 *
 */
class TAO_Notify_Export TAO_Notify_Method_Request_Lookup : public TAO_Notify_Method_Request_Lookup_Base
                                                       , public TAO_Notify_Method_Request
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Lookup (const TAO_Notify_Event_var& event, TAO_Notify_ProxyConsumer* proxy_consumer);

  /// Destructor
  ~TAO_Notify_Method_Request_Lookup ();

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);
};

/*****************************************************************************************************************************/

typedef TAO_Notify_Method_Request_Lookup_T<const TAO_Notify_Event*
                                       , TAO_Notify_ProxyConsumer*
                                       , const TAO_Notify_Event*
                                       , TAO_Notify_ProxyConsumer*>  TAO_Notify_Method_Request_Lookup_No_Copy_Base;

/**
 * @class TAO_Notify_Method_Request_Lookup_No_Copy
 *
 * @brief Lookup command object looks up the event type of the given event in the consumer map and send the event to each proxysupplier.
 *
 */
class TAO_Notify_Export TAO_Notify_Method_Request_Lookup_No_Copy : public TAO_Notify_Method_Request_Lookup_No_Copy_Base
                                                               , public TAO_Notify_Method_Request_No_Copy
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Lookup_No_Copy (const TAO_Notify_Event* event, TAO_Notify_ProxyConsumer* proxy_consumer);

  /// Destructor
  ~TAO_Notify_Method_Request_Lookup_No_Copy ();

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

  /// Create a copy of this object.
  virtual TAO_Notify_Method_Request* copy (ACE_ENV_SINGLE_ARG_DECL);
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Lookup.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_Notify_LOOKUP_METHOD_REQUEST_H */
