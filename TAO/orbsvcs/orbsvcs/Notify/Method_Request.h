/* -*- C++ -*- */
/**
 *  @file Method_Request.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_METHOD_REQUEST_H
#define TAO_Notify_METHOD_REQUEST_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Message_Block.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "Event.h"

class TAO_Notify_Method_Request;

/**
 * @class TAO_Notify_Method_Request_No_Copy
 *
 * @brief Base class for Method Requests that do not copy the event.
 *
 */
class TAO_Notify_Export TAO_Notify_Method_Request_No_Copy
{
public:
  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// Create a copy of this object.
  virtual TAO_Notify_Method_Request* copy (ACE_ENV_SINGLE_ARG_DECL) = 0;
};

/***********************************************************************/

/**
 * @class TAO_Notify_Method_Request
 *
 * @brief Interface for NS method Requests
 *
 */
class TAO_Notify_Export TAO_Notify_Method_Request : public ACE_Message_Block
{
public:
  enum {PRIORITY_BASE = 32768};

  void init (const TAO_Notify_Event_var& event);

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL) = 0;
};

#if defined (__ACE_INLINE__)
#include "Method_Request.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_Notify_METHOD_REQUEST_H */
