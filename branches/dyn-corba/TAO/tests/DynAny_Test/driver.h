// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    driver.h
//
// = DESCRIPTION
//    Header file for the driver program.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#if !defined (DRIVER_H)
#define DRIVER_H

#include "tao/corba.h"

class Driver
{
public:
  // = Constructor and destructor.
  Driver (void);
  ~Driver (void);

enum TEST_TYPE
  {
    NO_TEST,
    TEST_DYNANY,
    TEST_DYNARRAY,
    TEST_DYNENUM,
    TEST_DYNSEQUENCE,
    TEST_DYNSTRUCT,
    TEST_DYNUNION
  };
  
  int init (int argc, char* argv[]);
  // Initialize the driver object.

  int parse_args (int argc, char* argv[]);
  // Parse command line arguments.

  int run (void);
  // Execute test code.

private:
  CORBA::ORB_var orb_;
  // underlying ORB (we do not own it)

  TEST_TYPE test_type_;
};

#endif /* DRIVER_H */
