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

#if !defined (OPTIONS_H)
#define OPTIONS_H

#include "ace/Singleton.h"
#include "tao/corba.h"

class Options
{
public:
  enum TEST_TYPE
  {
    NO_TEST,
    TEST_SHORT,
    TEST_UNBOUNDED_STRING,
    TEST_FIXED_STRUCT,
    TEST_STRING_SEQUENCE,
    TEST_BOUNDED_STRING_SEQUENCE,
    TEST_VAR_STRUCT,
    TEST_NESTED_STRUCT,
    TEST_STRUCT_SEQUENCE,
    TEST_BOUNDED_STRUCT_SEQUENCE,
    TEST_OBJREF,
    TEST_TYPECODE,
    TEST_ANY,
    TEST_OBJREF_SEQUENCE,
    TEST_ANYSEQ,
    TEST_SHORTSEQ,
    TEST_BOUNDED_SHORTSEQ,
    TEST_LONGSEQ,
    TEST_BOUNDED_LONGSEQ
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
