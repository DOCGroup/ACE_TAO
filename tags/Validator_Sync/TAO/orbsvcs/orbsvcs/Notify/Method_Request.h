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

#ifndef TAO_NS_METHOD_REQUEST_H
#define TAO_NS_METHOD_REQUEST_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Message_Block.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "Event.h"
#include "Types.h"
#include "Refcountable.h"

/**
 * @class TAO_NS_Method_Request
 *
 * @brief Interface for NS method Requests
 *
 */
class TAO_Notify_Export TAO_NS_Method_Request : public ACE_Message_Block
{
public:
  enum {PRIORITY_BASE = 32768};

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// Create a copy of this object.
  virtual TAO_NS_Method_Request* copy (void) = 0;
};

/***********************************************************************/

/**
 * @class TAO_NS_Method_Request_Event
 *
 * @brief Base class for NS method Requests on Events.
 *
 */
class TAO_Notify_Export TAO_NS_Method_Request_Event : public TAO_NS_Method_Request
{
public:
  /// Constuctor
  TAO_NS_Method_Request_Event (TAO_NS_Event_var& event);

  /// Destructor
  virtual ~TAO_NS_Method_Request_Event ();

protected:
  TAO_NS_Event_var event_;
};

/***********************************************************************/

/**
 * @class TAO_NS_Refcountable_Guard
 *
 * @brief Ref. Count Guard
 *
 */
class TAO_Notify_Export TAO_NS_Refcountable_Guard
{
public:
  TAO_NS_Refcountable_Guard (TAO_NS_Refcountable& refcountable);

  ~TAO_NS_Refcountable_Guard ();

protected:
  TAO_NS_Refcountable& refcountable_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_METHOD_REQUEST_H */
