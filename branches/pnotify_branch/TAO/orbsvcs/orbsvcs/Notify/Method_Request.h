/* -*- C++ -*- */
/**
 *  @file Method_Request.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_METHOD_REQUEST_H
#define TAO_Notify_METHOD_REQUEST_H

#include /**/ "ace/pre.h"

#include "notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Message_Block.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/OS_NS_sys_time.h"

#include "Event.h"

class TAO_Notify_Method_Request;

/**
 * @class TAO_Notify_Method_Request_Base
 *
 * @brief Base class for Method Requests
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Base
{
public:
  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL) = 0;
  virtual TAO_Notify_Method_Request* copy (ACE_ENV_SINGLE_ARG_DECL) = 0;

};

/***********************************************************************/

/**
 * @class TAO_Notify_Method_Request_No_Copy
 *
 * @brief Base class for Method Requests that do not copy the event.
 * @@ TODO this class disappeared.  get rid of it!
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_No_Copy
  : public TAO_Notify_Method_Request_Base
{
public:
};

/***********************************************************************/

/**
 * @class TAO_Notify_Method_Request
 *
 * @brief Interface for NS method Requests
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request
  : public ACE_Message_Block
  , public TAO_Notify_Method_Request_Base
{
public:
  enum {PRIORITY_BASE = 32768};

  TAO_Notify_Method_Request();
  TAO_Notify_Method_Request(const TAO_Notify_Event * event);

  virtual TAO_Notify_Method_Request* copy (ACE_ENV_SINGLE_ARG_DECL);
  void init (const TAO_Notify_Event * event);
};

#if defined (__ACE_INLINE__)
#include "Method_Request.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_METHOD_REQUEST_H */
