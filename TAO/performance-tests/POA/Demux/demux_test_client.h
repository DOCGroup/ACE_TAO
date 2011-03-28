#ifndef TAO_DEMUX_TEST_CLIENT_H
#define TAO_DEMUX_TEST_CLIENT_H

//=============================================================================
/**
 *  @file    demux_test_client.h
 *
 *  $Id$
 *
 *  @file    Definition of the client-side demux_test class
 *
 *  $Id$
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


// FUZZ: disable check_for_math_include

#include "ace/Get_Opt.h"
#include "ace/Vector_T.h"
#include "demux_testC.h"
#include "demux_test_macros.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"

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

  typedef void (*OP_PTR) (Demux_Test_ptr);

  struct Operation_DB_Entry
  {
    OP_PTR op_;
  };

  /// CTOR
  Demux_Test_Client (void);

  /// DTOR
  ~Demux_Test_Client (void);

  /// initialize the client test bed
  int init (int argc, ACE_TCHAR *argv []);

  /// run the tests
  int run (void);

private:

  /// parse command line arguments
  int parse_args (void);

  /// initialize the operation database
  int init_operation_db (void);

  /// run linear strategy
  int run_linear_test (void);

  /// run random strategy
  int run_random_test (void);

  /// run best strategy (w.r.t to linear)
  int run_best_test (void);

  /// run worst strategy (w.r.t to linear)
  int run_worst_test (void);

  /// print results
  int print_results (void);

  /// number of command line arguments
  int argc_;

  /// the actual command line arguments
  ACE_TCHAR **argv_;

  /// The underlying ORB
  CORBA::ORB_var orb_;

  /// invocation strategy (default linear)
  INVOKE_STRATEGY is_;

  /// number of POAs
  CORBA::ULong num_POAs_;

  /// number of objects
  CORBA::ULong num_objs_;

  /// number of operations
  CORBA::ULong num_ops_;

  /// object references to the Demux_Test objects
  typedef TAO_Intrusive_Ref_Count_Handle<Demux_Test> Demux_Test_Var;
  typedef ACE_Vector<Demux_Test_Var> Demux_Test_Container;
  Demux_Test_Container demux_test_;

  /// number of times to invoke the request
  CORBA::ULong loop_count_;

  /// IOR database
  FILE *ior_fp_;

  /// temporary results file
  FILE *result_fp_;

  /**
   * Step for the no. of objects to be skipped while making remote calls
   * on the given no. of objects.
   * If the step is 100, a call will be made to every 100th Object.
   */
  CORBA::ULong step_;

  /// database of operations
  Operation_DB_Entry op_db_[TAO_DEMUX_TEST_MAX_OPS];

};


#endif /* TAO_DEMUX_TEST_CLIENT_H */
