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

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Method_Request.h"
#include "Refcountable.h"
#include "Method_Request_Dispatch_T.h"
#include "ProxySupplier.h"

/**
 * @class TAO_Notify_Method_Request_Dispatch
 *
 * @brief Dispatchs an event to a proxy supplier.
 *
 */

typedef TAO_Notify_Method_Request_Dispatch_T<const TAO_Notify_Event_var
                                         , TAO_Notify_ProxySupplier_Guard
                                         , const TAO_Notify_Event_var&
                                         , TAO_Notify_ProxySupplier*>  TAO_Notify_Method_Request_Dispatch_Base;

class TAO_Notify_Export TAO_Notify_Method_Request_Dispatch : public TAO_Notify_Method_Request_Dispatch_Base
                                                       , public TAO_Notify_Method_Request
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Dispatch (const TAO_Notify_Event_var& event, TAO_Notify_ProxySupplier* proxy_supplier, CORBA::Boolean filtering);

  /// Destructor
  ~TAO_Notify_Method_Request_Dispatch ();

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);
};

/*******************************************************************************************************/

/**
 * @class TAO_Notify_Method_Request_Dispatch_No_Copy
 *
 * @brief Dispatchs an event to a proxy supplier.
 *
 */

typedef TAO_Notify_Method_Request_Dispatch_T<const TAO_Notify_Event*
                                         , TAO_Notify_ProxySupplier*
                                         , const TAO_Notify_Event*
                                         , TAO_Notify_ProxySupplier*>  TAO_Notify_Method_Request_Dispatch_No_Copy_Base;

class TAO_Notify_Export TAO_Notify_Method_Request_Dispatch_No_Copy : public TAO_Notify_Method_Request_Dispatch_No_Copy_Base
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

typedef TAO_Notify_Method_Request_Dispatch_T<const TAO_Notify_Event_var&
                                         , TAO_Notify_ProxySupplier*
                                         , const TAO_Notify_Event_var&
                                         , TAO_Notify_ProxySupplier*>  TAO_Notify_Method_Request_Dispatch_No_Copy_Ex_Base;

class TAO_Notify_Export TAO_Notify_Method_Request_Dispatch_No_Copy_Ex : public TAO_Notify_Method_Request_Dispatch_No_Copy_Ex_Base
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
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Dispatch.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_DISPATCH_METHOD_REQUEST_H */
