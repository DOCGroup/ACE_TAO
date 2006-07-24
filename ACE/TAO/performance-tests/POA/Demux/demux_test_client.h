#ifndef TAO_DEMUX_TEST_CLIENT_H
#define TAO_DEMUX_TEST_CLIENT_H

// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Demux
//
// = FILENAME
//    demux_test_client.h
//
//    Definition of the client-side demux_test class
//
// = AUTHOR
//
//    Aniruddha Gokhale
//
// ============================================================================

// FUZZ: disable check_for_math_include

#include "ace/Get_Opt.h"
#include "demux_testC.h"
#include "demux_test_macros.h"
#include <math.h>

class Demux_Test_Client
{
public:

  // Request invocation strategies
  enum INVOKE_STRATEGY
  {
    LINEAR,
    RANDOM,
    BEST,
    WORST
  };

  typedef void (*OP_PTR) (Demux_Test_ptr ACE_ENV_ARG_DECL_NOT_USED);

  struct Operation_DB_Entry
  {
    OP_PTR op_;
  };

  Demux_Test_Client (void);
  // CTOR

  ~Demux_Test_Client (void);
  // DTOR

  int init (int argc, char *argv [] ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  // initialize the client test bed

  int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // run the tests

private:

  int parse_args (void);
  // parse command line arguments

  int init_operation_db (void);
  // initialize the operation database

  int run_linear_test (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // run linear strategy

  int run_random_test (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // run random strategy

  int run_best_test (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // run best strategy (w.r.t to linear)

  int run_worst_test (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // run worst strategy (w.r.t to linear)

  int print_results (void);
  // print results

  int argc_;
  // number of command line arguments

  char **argv_;
  // the actual command line arguments

  CORBA::ORB_var orb_;
  // The underlying ORB

  INVOKE_STRATEGY is_;
  // invocation strategy (default linear)

  CORBA::ULong num_POAs_;
  // number of POAs

  CORBA::ULong num_objs_;
  // number of objects

  CORBA::ULong num_ops_;
  // number of operations

  Demux_Test_var demux_test_[TAO_DEMUX_TEST_MAX_POAS][TAO_DEMUX_TEST_MAX_OBJS];
  // object references to the Demux_Test objects

  CORBA::ULong loop_count_;
  // number of times to invoke the request

  FILE *ior_fp_;
  // IOR database

  FILE *result_fp_;
  // temporary results file

  CORBA::ULong step_;
  // Step for the no. of objects to be skipped while making remote calls
  // on the given no. of objects.
  // If the step is 100, a call will be made to every 100th Object.

  Operation_DB_Entry op_db_[TAO_DEMUX_TEST_MAX_OPS];
  // database of operations

};


#endif /* TAO_DEMUX_TEST_CLIENT_H */
