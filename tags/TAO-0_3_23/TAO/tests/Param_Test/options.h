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

#include "tao/corba.h"

class Options
{
public:
  enum TEST_TYPE
  {
    NO_TEST,
    TEST_SHORT,
    TEST_ULONGLONG,
    TEST_UB_STRING,
    TEST_BD_STRING,
    TEST_FIXED_STRUCT,
    TEST_UB_STRING_SEQUENCE,
    TEST_BD_STRING_SEQUENCE,
    TEST_VAR_STRUCT,
    TEST_NESTED_STRUCT,
    TEST_OBJREF_STRUCT,
    TEST_UB_STRUCT_SEQUENCE,
    TEST_BD_STRUCT_SEQUENCE,
    TEST_OBJREF,
    TEST_TYPECODE,
    TEST_ANY,
    TEST_OBJREF_SEQUENCE,
    TEST_ANYSEQ,
    TEST_UB_SHORTSEQ,
    TEST_BD_SHORTSEQ,
    TEST_UB_LONGSEQ,
    TEST_BD_LONGSEQ,
    TEST_FIXED_ARRAY,
    TEST_VAR_ARRAY,
    TEST_MULTDIM_ARRAY,
    TEST_EXCEPTION,
    TEST_BIG_UNION,
    TEST_COMPLEX_ANY
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

  int parse_args (int argc, char **argv);
  // Parses the arguments passed on the command line.

  char *param_test_ior (void);
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
  int read_ior (char *filename);
  // Function to read the servant IOR from a file.

  char *ior_;
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

typedef ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX> OPTIONS;

#endif /* OPTIONS_H */
