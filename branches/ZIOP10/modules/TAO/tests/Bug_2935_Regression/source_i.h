// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Simple/ThreeTier
//
// = FILENAME
//    source_i.h
//
// = DESCRIPTION
//    This class implements the source for a three tier system
//    Adapted from TAO/examples/Simple/time
//
// = AUTHOR
//   Dale Wilson <wilsond@ociweb.com>
//
// ============================================================================

#ifndef SOURCE_I_H
#define SOURCE_I_H

#include "ThreeTierC.h"

class Source_i
{
  // = TITLE
  //   Source interface subclass.
  //
  // = DESCRIPTION
  // a source of data for the three tier test
public:
  Source_i (ThreeTier_ptr target);

  ~Source_i ();

  bool parse_args(int argc, char *argv[]);

  int run ();
  // Execute the methods

private:
  ThreeTier_var target_;

  // Number of messages to send (make this configurable)
  unsigned long counter_;

};


#endif /* SOURCE_I_H */
