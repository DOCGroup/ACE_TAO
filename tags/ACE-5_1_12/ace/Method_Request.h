/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Method_Request.h
 *
 *  $Id$
 *
 *  @author Andres Kruse <Andres.Kruse@cern.ch>
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_METHOD_REQUEST_H
#define ACE_METHOD_REQUEST_H
#include "ace/pre.h"

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Method_Request
 *
 * @brief Reifies a method into a request.  Subclasses provide
 * the necessary state and behavior.
 *
 * A <Method_Request> is inserted in the <Activation_Queue>,
 * where it is subsequently removed by a <Scheduler>, which
 * invokes the <call> method.
 */
class ACE_Export ACE_Method_Request
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  ACE_Method_Request (u_long priority = 0);

  /// Destructor.
  virtual ~ACE_Method_Request (void);

  // = Accessors.
  /// Get priority.
  u_long priority (void) const;

  /// Set priority.
  void priority (u_long);

  // = Invocation method (must be overridden by subclasses).
  /// Invoked when the <Method_Request> is scheduled to run.
  virtual int call (void) = 0;

protected:
  /// The priority of the request.
  u_long priority_;

private:
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Method_Request &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Method_Request (const ACE_Method_Request &))
};

#include "ace/post.h"
#endif /* ACE_METHOD_REQUEST_H */
