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
//#include "Method_Request_Dispatch_T.h"
#include "Method_Request_Dispatch_Base.h"
#include "ProxySupplier.h"

/**
 * @class TAO_Notify_Method_Request_Dispatch
 *
 * @brief Dispatchs an event to a proxy supplier.
 *
 */

class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Dispatch
    : public TAO_Notify_Method_Request_Dispatch_Base
    , public TAO_Notify_Method_Request
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Dispatch (const TAO_Notify_Event_var& event, TAO_Notify_ProxySupplier* proxy_supplier, CORBA::Boolean filtering);

  /// Destructor
  ~TAO_Notify_Method_Request_Dispatch ();

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
    : public TAO_Notify_Method_Request_Dispatch_Base
    , public TAO_Notify_Method_Request_No_Copy
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Dispatch_No_Copy (const TAO_Notify_Event* event, TAO_Notify_ProxySupplier* proxy_supplier, CORBA::Boolean filtering);

  /// Destructor
  ~TAO_Notify_Method_Request_Dispatch_No_Copy ();

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

  /// Create a copy of this object.
  virtual TAO_Notify_Method_Request* copy (ACE_ENV_SINGLE_ARG_DECL);
};

/*******************************************************************************************************/

/**
 * @class TAO_Notify_Method_Request_Dispatch_No_Copy_Ex
 *
 * @brief Dispatchs an event to a proxy supplier.
 *
 */

class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Dispatch_No_Copy_Ex
  : public TAO_Notify_Method_Request_Dispatch_Base
  , public TAO_Notify_Method_Request_No_Copy
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Dispatch_No_Copy_Ex (const TAO_Notify_Event_var& event, TAO_Notify_ProxySupplier* proxy_supplier, CORBA::Boolean filtering);

  /// Destructor
  ~TAO_Notify_Method_Request_Dispatch_No_Copy_Ex ();

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

  /// Create a copy of this object.
  virtual TAO_Notify_Method_Request* copy (ACE_ENV_SINGLE_ARG_DECL);
private:
  const TAO_Notify_Event_var& event_var_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Dispatch.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_DISPATCH_METHOD_REQUEST_H */
