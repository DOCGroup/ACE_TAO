#ifndef TAO_DEMUX_TEST_CLIENT_H
#define TAO_DEMUX_TEST_CLIENT_H

//=============================================================================
/**
 *  @file    demux_test_client.h
 *
 *  Definition of the client-side demux_test class
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


// FUZZ: disable check_for_math_include

#include "ace/Get_Opt.h"
#include "demux_testC.h"
#include "demux_test_file.h"
#include "demux_test_macros.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"
#include <vector>

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

  using OP_PTR = void (*) (Demux_Test_ptr);

  struct Operation_DB_Entry
  {
    OP_PTR op_;
  };

  /// CTOR
  Demux_Test_Client ();

  /// DTOR
  ~Demux_Test_Client () = default;

  Demux_Test_Client (Demux_Test_Client const &) = delete;
  Demux_Test_Client &operator= (Demux_Test_Client const &) = delete;
  Demux_Test_Client (Demux_Test_Client &&) = delete;
  Demux_Test_Client &operator= (Demux_Test_Client &&) = delete;

  /// initialize the client test bed
  int init (int argc, ACE_TCHAR *argv []);

  /// run the tests
  int run ();

private:
  /// parse command line arguments
  int parse_args ();

  /// initialize the operation database
  int init_operation_db ();

  /// run linear strategy
  int run_linear_test ();

  /// run random strategy
  int run_random_test ();

  /// run best strategy (w.r.t to linear)
  int run_best_test ();

  /// run worst strategy (w.r.t to linear)
  int run_worst_test ();

  /// print results
  int print_results ();

  /// number of command line arguments
  int argc_;

  /// the actual command line arguments
  ACE_TCHAR **argv_ {};

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
  using Demux_Test_Var = TAO_Intrusive_Ref_Count_Handle<Demux_Test>;
  using Demux_Test_Container = std::vector<Demux_Test_Var>;
  Demux_Test_Container demux_test_;

  /// number of times to invoke the request
  CORBA::ULong loop_count_;

  /// IOR database
  Demux_Test_Utils::File_Handle ior_fp_;

  /// temporary results file
  Demux_Test_Utils::File_Handle result_fp_;

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
