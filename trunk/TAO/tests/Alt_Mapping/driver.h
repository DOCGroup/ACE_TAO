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
#include "alt_mappingC.h"

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

  int init (int argc, ACE_TCHAR **argv);
  // Initialize the driver object

  int run (void);
  // Execute client example code.

private:
  CORBA::ORB_var orb_;
  // underlying ORB (we do not own it)

  Alt_Mapping_var objref_;
  // object reference (we do not own it)
};

#endif /* !defined */
