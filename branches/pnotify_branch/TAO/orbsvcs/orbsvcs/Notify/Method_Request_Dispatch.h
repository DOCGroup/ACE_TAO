/* -*- C++ -*- */
/**
 *  @file Method_Request_Dispatch.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_DISPATCH_METHOD_REQUEST_H
#define TAO_Notify_DISPATCH_METHOD_REQUEST_H
#include /**/ "ace/pre.h"

#include "notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Method_Request.h"
#include "Refcountable.h"
#include "Method_Request_Event.h"
#include "ProxySupplier.h"


/**
 * @class TAO_Notify_Method_Request_Dispatch
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Dispatch
  : public TAO_Notify_Method_Request_Event
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Dispatch (const TAO_Notify_Event *,
                                        TAO_Notify_ProxySupplier* proxy,
                                        bool filtering);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Dispatch ();

protected:
  /// Execute the dispatch operation.
  int execute_i (ACE_ENV_SINGLE_ARG_DECL);

protected:
  /// The Proxy
  TAO_Notify_ProxySupplier * proxy_supplier_;

  /// Flag is true if we want to do fintering else false.
  bool filtering_;
};

/**
 * @class TAO_Notify_Method_Request_Dispatch_Queueable
 *
 * @brief Dispatchs an event to a proxy supplier.
 *
 */

class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Dispatch_Queueable
    : public TAO_Notify_Method_Request_Dispatch
    , public TAO_Notify_Method_Request_Queueable
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Dispatch_Queueable (const TAO_Notify_Event_var& event, TAO_Notify_ProxySupplier* proxy_supplier, CORBA::Boolean filtering);

  /// Destructor
  ~TAO_Notify_Method_Request_Dispatch_Queueable ();

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

private:
  const TAO_Notify_Event_var event_var_;
  TAO_Notify_ProxySupplier_Guard proxy_guard_;
};

/*******************************************************************************************************/

/**
 * @class TAO_Notify_Method_Request_Dispatch_No_Copy
 *
 * @brief Dispatchs an event to a proxy supplier.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Dispatch_No_Copy
    : public TAO_Notify_Method_Request_Dispatch
    , public TAO_Notify_Method_Request
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Dispatch_No_Copy (const TAO_Notify_Event* event, TAO_Notify_ProxySupplier* proxy_supplier, CORBA::Boolean filtering);

  /// Destructor
  ~TAO_Notify_Method_Request_Dispatch_No_Copy ();

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

  /// Create a copy of this method request
  virtual TAO_Notify_Method_Request_Queueable* copy (ACE_ENV_SINGLE_ARG_DECL);
};

/*******************************************************************************************************/

#if defined (__ACE_INLINE__)
#include "Method_Request_Dispatch.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_DISPATCH_METHOD_REQUEST_H */
