// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    driver.h
//
// = DESCRIPTION
//    Header file for the driver program. The driver is a singleton.
//
// = AUTHORS
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef DRIVER_H
#define DRIVER_H

#include "options.h"

class Driver
{
  // = TITLE
  //    Driver
  //
  // = DESCRIPTION
  //    Driver program for the client
public:
  // = Constructor and destructor.
  Driver (void);
  ~Driver (void);

  int init (int argc, char **argv);
  // Initialize the driver object

  int run (void);
  // Execute client example code.

private:
  CORBA::ORB_var orb_;
  // underlying ORB (we do not own it)

  Param_Test_var objref_;
  // object reference (we do not own it)
};

#endif /* !defined */
