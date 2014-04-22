// -*- C++ -*-

//=============================================================================
/**
 *  @file    source_i.h
 *
 *  $Id$
 *
 *  This class implements the source for a three tier system
 *  Adapted from TAO/examples/Simple/time
 *
 *
 *  @author Dale Wilson <wilsond@ociweb.com>
 */
//=============================================================================


#ifndef SOURCE_I_H
#define SOURCE_I_H

#include "ThreeTierC.h"

/**
 * @class Source_i
 *
 * @brief Source interface subclass.
 *
 * a source of data for the three tier test
 */
class Source_i
{
public:
  Source_i (ThreeTier_ptr target);

  ~Source_i ();

  bool parse_args(int argc, ACE_TCHAR *argv[]);

  /// Execute the methods
  int run ();

private:
  ThreeTier_var target_;

  // Number of messages to send (make this configurable)
  unsigned long counter_;

};


#endif /* SOURCE_I_H */
