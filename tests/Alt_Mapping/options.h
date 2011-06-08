// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    options.h
//
// = DESCRIPTION
//    Options for the Param_Test application
//
// = AUTHORS
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef OPTIONS_H
#define OPTIONS_H

#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/CORBA_String.h"
#include "tao/orbconf.h"

class Options
{
public:
  enum TEST_TYPE
  {
    NO_TEST,
    TEST_UB_STRING,
    TEST_UB_STRUCT_SEQUENCE,
    TEST_UB_STRING_SEQUENCE,
    TEST_UB_LONG_SEQUENCE,
    TEST_UB_OCTET_SEQUENCE,
    TEST_VAR_STRUCT,
    TEST_NESTED_STRUCT
  };

  enum INVOKE_TYPE
  {
    SII,
    DII
  };

  Options (void);
  // constructor

  ~Options (void);
  // destructor

  int parse_args (int argc, ACE_TCHAR **argv);
  // Parses the arguments passed on the command line.

  char const * param_test_ior (void) const;
  // return the IOR for the servant

  TEST_TYPE test_type (void);
  // what test to run

  INVOKE_TYPE invoke_type (void);
  // whether to use SII or DII

  CORBA::ULong loop_count (void);
  // number of times to run the test

  CORBA::Boolean debug (void) const;
  // whether debug option is on or not

  CORBA::Boolean shutdown (void) const;
  // If we should request the server to shutdown.

private:
  int read_ior (ACE_TCHAR *filename);
  // Function to read the servant IOR from a file.

  CORBA::String_var ior_;
  // IOR for the servant

  TEST_TYPE test_type_;
  // what test to run

  INVOKE_TYPE invoke_type_;
  // whether SII or DII

  CORBA::ULong loop_count_;
  // Number of times to do the "test_*" operations.

  CORBA::Boolean debug_;
  // debugging output values

  CORBA::Boolean shutdown_;
  // server shutdown flag.
};

typedef ACE_Singleton<Options, TAO_SYNCH_RECURSIVE_MUTEX> OPTIONS;

#endif /* OPTIONS_H */
