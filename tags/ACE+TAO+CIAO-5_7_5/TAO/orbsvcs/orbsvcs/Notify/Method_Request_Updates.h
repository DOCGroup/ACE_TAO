// -*- C++ -*-

/**
 *  @file Method_Request_Updates.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_METHOD_REQUEST_UPDATES_H
#define TAO_Notify_METHOD_REQUEST_UPDATES_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Method_Request_Updates_T.h"
#include "orbsvcs/Notify/Method_Request.h"
#include "orbsvcs/Notify/EventTypeSeq.h"
#include "orbsvcs/Notify/Proxy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef TAO_Notify_Method_Request_Updates_T<const TAO_Notify_EventTypeSeq
                                        , TAO_Notify_Proxy::Ptr
                                        , const TAO_Notify_EventTypeSeq&
                                        , TAO_Notify_Proxy*
                                        > TAO_Notify_Method_Request_Updates_Base;

/**
 * @class TAO_Notify_Method_Request_Updates
 *
 * @brief Command Object to send updates to proxys.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Updates
  : public TAO_Notify_Method_Request_Updates_Base
  , public TAO_Notify_Method_Request_Queueable
{
public:
  /// Constructor
  TAO_Notify_Method_Request_Updates (const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed, TAO_Notify_Proxy* proxy);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Updates ();

  /// Execute the Request
  virtual int execute (void);
};

/*****************************************************************************/

typedef TAO_Notify_Method_Request_Updates_T<const TAO_Notify_EventTypeSeq&
                                        , TAO_Notify_Proxy*
                                        , const TAO_Notify_EventTypeSeq&
                                        , TAO_Notify_Proxy*
                                        > TAO_Notify_Method_Request_Updates_No_Copy_Base;

/**
 * @class TAO_Notify_Method_Request_Updates_No_Copy
 *
 * @brief Command Object to send updates to proxys.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Updates_No_Copy
  : public TAO_Notify_Method_Request_Updates_No_Copy_Base
  , public TAO_Notify_Method_Request
{
public:
  /// Constructor
  TAO_Notify_Method_Request_Updates_No_Copy (const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed, TAO_Notify_Proxy* proxy);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Updates_No_Copy ();

  /// Create a copy of this object.
  virtual TAO_Notify_Method_Request_Queueable* copy (void);

  /// Execute the Request
  virtual int execute (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_METHOD_REQUEST_UPDATES_H */
