/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Method_Request.h
//
// = AUTHOR
//    Andres Kruse <Andres.Kruse@cern.ch> and Douglas C. Schmidt
//    <schmidt@cs.wustl.edu> 
// 
// ============================================================================

#ifndef ACE_METHOD_REQUEST_H
#define ACE_METHOD_REQUEST_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_Method_Request 
{
  // = TITLE
  //     Reifies a method into a request.  Subclasses provide
  //     the necessary state and behavior.
  //
  // = DESCRIPTION
  //     A <Method_Request> is inserted in the <Activation_Queue>,
  //     where it is subsequently removed by a <Scheduler>, which
  //     invokes the <call> method.
public:
  // = Initialization and termination methods.
  ACE_Method_Request (void);
  // Constructor.

  virtual ~ACE_Method_Request (void);
  // Destructor.

  // = Invocation method
  virtual int call (void) = 0;
  // Invoked when the <Method_Request> is scheduled to run.
};

#endif /* ACE_METHOD_REQUEST_H */
