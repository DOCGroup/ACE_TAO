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

#include "Method_Request_Updates_T.h"
#include "Method_Request.h"
#include "EventTypeSeq.h"
#include "Proxy.h"

typedef TAO_NS_Method_Request_Updates_T<const TAO_NS_EventTypeSeq
                                        , TAO_NS_Proxy_Guard
                                        , const TAO_NS_EventTypeSeq&
                                        , TAO_NS_Proxy*
                                        > TAO_NS_Method_Request_Updates_Base;

/**
 * @class TAO_NS_Method_Request_Updates
 *
 * @brief Command Object to send updates to proxys.
 *
 */
class TAO_Notify_Export TAO_NS_Method_Request_Updates : public TAO_NS_Method_Request_Updates_Base
                                                        ,public TAO_NS_Method_Request
{
public:
  /// Constuctor
  TAO_NS_Method_Request_Updates (const TAO_NS_EventTypeSeq& added, const TAO_NS_EventTypeSeq& removed, TAO_NS_Proxy* proxy);

  /// Destructor
  ~TAO_NS_Method_Request_Updates ();

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);
};

/***********************************************************************************************************************/

typedef TAO_NS_Method_Request_Updates_T<const TAO_NS_EventTypeSeq&
                                        , TAO_NS_Proxy*
                                        , const TAO_NS_EventTypeSeq&
                                        , TAO_NS_Proxy*
                                        > TAO_NS_Method_Request_Updates_No_Copy_Base;

/**
 * @class TAO_NS_Method_Request_Updates_No_Copy
 *
 * @brief Command Object to send updates to proxys.
 *
 */
class TAO_Notify_Export TAO_NS_Method_Request_Updates_No_Copy : public TAO_NS_Method_Request_Updates_No_Copy_Base
                                                                ,public TAO_NS_Method_Request_No_Copy
{
public:
  /// Constuctor
  TAO_NS_Method_Request_Updates_No_Copy (const TAO_NS_EventTypeSeq& added, const TAO_NS_EventTypeSeq& removed, TAO_NS_Proxy* proxy);

  /// Destructor
  ~TAO_NS_Method_Request_Updates_No_Copy ();

  /// Create a copy of this object.
  virtual TAO_NS_Method_Request* copy (ACE_ENV_SINGLE_ARG_DECL);

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Updates.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_METHOD_REQUEST_UPDATES_H */
