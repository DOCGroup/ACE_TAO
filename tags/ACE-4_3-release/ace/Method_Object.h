/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Method_Object.h
//
// = AUTHOR
//    Andres Kruse <Andres.Kruse@cern.ch> and Douglas C. Schmidt
//    <schmidt@cs.wustl.edu> 
// 
// ============================================================================

#if !defined (ACE_METHOD_OBJECT_H)
#define ACE_METHOD_OBJECT_H

#include "ace/OS.h"

class ACE_Export ACE_Method_Object 
  // = TITLE
  //     Reifies a method into an object.  Subclasses typically
  //     represent necessary state and behavior.
  //
  // = DESCRIPTION
  //     A <Method_Object> is inserted in the <Activation_Queue>, where
  //     it is subsequently removed by the <Scheduler> and invoked.
{
public:
  // = Initialization and termination methods.
  ACE_Method_Object (void);
  virtual ~ACE_Method_Object (void);

  // = Invocation method
  virtual int call (void) = 0;
  // Invoked when the <Method_Object> is scheduled to run.
};

#endif /* ACE_METHOD_OBJECT_H */
