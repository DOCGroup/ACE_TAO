// -*- C++ -*-

//=============================================================================
/**
 *  @file    source_i.h
 *
 *  This class implements the source for a three tier system
 *  Adapted from TAO/examples/Simple/time
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
  explicit Source_i (ThreeTier_ptr target);
  Source_i (Source_i const &) = delete;
  Source_i &operator= (Source_i const &) = delete;
  Source_i (Source_i &&) = delete;
  Source_i &operator= (Source_i &&) = delete;

  /// Execute the methods
  int run ();

private:
  ThreeTier_var const target_;

  // Number of messages to send (make this configurable)
  unsigned long const counter_ {5};
};


#endif /* SOURCE_I_H */
