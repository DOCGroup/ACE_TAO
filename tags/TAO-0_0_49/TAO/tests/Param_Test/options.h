// $Id:

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
#if 0
    TEST_LONG,
    TEST_CHAR,
    TEST_OCTET,
    TEST_FLOAT,
    TEST_DOUBLE,
#endif
    TEST_UNBOUNDED_STRING
#if 0
    TEST_BOUNDED_STRING,
    TEST_TYPEDEFED_STRING
#endif
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

  char *param_test_key (void);
  // return the key

  char *hostname (void);
  // return the hostname

  CORBA::ULong portnum (void);
  // return the port number

  TEST_TYPE test_type (void);
  // what test to run

  INVOKE_TYPE invoke_type (void);
  // whether to use SII or DII

  CORBA::ULong loop_count (void);
  // number of times to run the test
private:
  char *param_test_key_;
  // Key of the obj ref to be retrieved

  char *hostname_;
  // Hostname of server.

  CORBA::ULong portnum_;
  // port number of server.

  TEST_TYPE test_type_;
  // what test to run

  INVOKE_TYPE invoke_type_;
  // whether SII or DII

  CORBA::ULong loop_count_;
  // Number of times to do the "test_*" operations.

};

typedef ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX> OPTIONS;

#endif /* OPTIONS_H */
