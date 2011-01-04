// -*- C++ -*-

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

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Message_Block.h"
#include "ace/Bound_Ptr.h"
#include "ace/OS_NS_sys_time.h"

#include "orbsvcs/Notify/Event.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_Method_Request_Queueable;

/**
 * @class TAO_Notify_Method_Request
 *
 * @brief Base class for Method Requests
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request
{
public:
  /// Destructor.
  virtual ~TAO_Notify_Method_Request (void);

  /// Execute the Request
  virtual int execute (void) = 0;
  virtual TAO_Notify_Method_Request_Queueable* copy (void) = 0;
};

/***********************************************************************/

/**
 * @class TAO_Notify_Method_Request_Queueable
 *
 * @brief Interface for NS method Requests
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Queueable
  : public ACE_Message_Block
  , public TAO_Notify_Method_Request
{
public:
  enum {PRIORITY_BASE = 32768};

  TAO_Notify_Method_Request_Queueable();
  TAO_Notify_Method_Request_Queueable(const TAO_Notify_Event * event);

  virtual TAO_Notify_Method_Request_Queueable* copy (void);
  void init (const TAO_Notify_Event * event);

  /// The creation time of the event to which this request corresponds.
  const ACE_Time_Value& creation_time (void) const;

private:
  ACE_Time_Value time_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_Notify_METHOD_REQUEST_H */
