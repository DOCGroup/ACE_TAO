// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO/tests/Cubit/TAO/DII_Cubit
//
// = FILENAME
//     client.h
//
// = DESCRIPTION
//      This class implements a Cubit client from non-IDL-generated code
//      to test DII functionality. It makes requests of a server which has the 
//      same operations as the one in IDL_Cubit
//
// = AUTHOR
//      Jeff Parsons <jp4@cs.wustl.edu>
// ============================================================================

#if !defined (_DII_C_CLIENT_H)
#define _DII_C_CLIENT_H

#include "tao/corba.h"
#include "ace/streams.h"
#include "ace/Profile_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Env_Value_T.h"
#include "ace/Read_Buffer.h"
#include "orbsvcs/CosNamingC.h"

// Since we don't yet have an interface repository or dynamic-Any, we
// just get the info from the IDL-generated files, since we're mainly
// interested in timing comparisons anyway.
#include "../IDL_Cubit/cubitC.h"

// # of timed tests in DI_Cubit::run.
const int NUMBER_OF_TESTS = 10;

class DII_Cubit_Client 
{
  // = TITLE
  //    Defines a class that encapsulates behaviour of a Cubit client
  //    that makes requests using DII rather than stubs.
  //
  // = DESCRIPTION
  //    This class declares an interface to run an example client for a
  //    Cubit CORBA server.  All the complexity for initializing the 
  //    client is hidden in the class.  Just the run() interface is needed.
public:
  // Constructor and destructor.
  DII_Cubit_Client (void);
  ~DII_Cubit_Client (void);

  int run (void);
  // Execute client example code.

  int init (int argc, char **argv);
  // Initialize the ORB and gets the Cubit objref from the Cubit factory.
private:
  int init_naming_service (void);
  // Gets objref through naming service

  int parse_args (void);
  // Parses the arguments passed on the command line.

  int read_ior (char *filename);
  // Function to read the cubit factory IOR from a file.

  void print_stats (const char *call_name,
                    ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time);
  // prints the time stats

  // DII versions of Cubit operations
	
  void cube_short_dii (void);

  void cube_long_dii (void);

  void cube_octet_dii (void);

  void cube_union_dii (void);

  void cube_struct_dii (void);

  void cube_octet_seq_dii (int length);

  void cube_long_seq_dii (int length);

  // Wrappers for cubing small and large sequences w/o args.

  void cube_small_long_seq (void);
  
  void cube_large_long_seq (void);

  void cube_small_octet_seq (void);

  void cube_large_octet_seq (void);

  void cube_mixin (void);
  // Wrapper for the mixin call, just to be neat.

  void (DII_Cubit_Client::*op_array_[NUMBER_OF_TESTS])(void);
  // Array of pointers to the operation functions.

  static char *stats_messages_[];
  // Array of labels for passing to print_stats.
  
  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  CORBA::ULong loop_count_;
  // # of calls in test loop.

  int shutdown_;
  // Flag to tell server to exit.

  int use_naming_service_;
  // Flag toggling use of naming service to get IOR.

  CORBA::Environment env_;
  // Environment variable.

  CORBA::ORB_var orb_var_;
  // Storage of the ORB reference.

  CORBA::Object_var factory_var_;
  // Storage of the Cubit_factory objref

  CORBA::Object_var obj_var_;
  // Storage of the Cubit objref.

  CORBA::ULong call_count_;
  // # of calls made to functions.

  CORBA::ULong error_count_;
  // # of errors incurred in the lifetime of the application.

  char *factory_IOR_;
  // IOR of the factory used to make a Cubit object.

  FILE *cubit_factory_ior_file_;
  // File from which to obtain the IOR.

  ACE_HANDLE f_handle_;
  // File handle to read the IOR.
};

#endif /* _DII_C_CLIENT_H */
