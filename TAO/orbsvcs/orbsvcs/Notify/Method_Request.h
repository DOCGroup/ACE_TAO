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

#include "ace/Method_Request.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "Event.h"
#include "Types.h"

/**
 * @class TAO_NS_Method_Request
 *
 * @brief Base class for NS method Requests
 *
 */
class TAO_Notify_Export TAO_NS_Method_Request : public ACE_Method_Request
{
public:
  /// Constuctor
  TAO_NS_Method_Request (TAO_NS_Event_var& event);

  /// Destructor
  virtual ~TAO_NS_Method_Request ();  

  /// Create a copy of this object.
  virtual TAO_NS_Method_Request* copy (void) = 0;

protected:
  TAO_NS_Event_var event_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_METHOD_REQUEST_H */
