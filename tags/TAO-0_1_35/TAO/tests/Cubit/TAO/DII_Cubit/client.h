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
//     This class implements a Cubit client from non-IDL-generated
//     code to test DII functionality. It makes requests of a server
//     which has the same operations as the one in IDL_Cubit
//
// = AUTHORS
//      Jeff Parsons <jp4@cs.wustl.edu>
// ============================================================================

#if !defined (_DII_C_CLIENT_H)
#define _DII_C_CLIENT_H

#include "tao/corba.h"
#include "ace/streams.h"
#include "ace/Profile_Timer.h"
#include "ace/Get_Opt.h"

// Since we don't yet have an interface repository or dynamic-Any, we
// just get the info from the IDL-generated files, since we're mainly
// interested in timing comparisons anyway.
#include "../IDL_Cubit/cubitC.h"

class DII_Cubit_Client 
{
  // = TITLE
  //    Defines a class that encapsulates behaviour of a Cubit client
  //    that makes requests using DII rather than stubs.
  //
  // = DESCRIPTION
  //    This class declares an interface to run the example client for
  //    a Cubit CORBA server.  All the complexity for initializing the
  //    server is hidden in the class.  Just the run() interface is
  //    needed.
public:
  // Constructor and destructor.
  DII_Cubit_Client (void);
  ~DII_Cubit_Client (void);

  int run (void);
  // Execute client example code.

  int init (int argc, char **argv);
  // Initialize the client communication endpoint with server.

private:
  int parse_args (void);
  // Parses the arguments passed on the command line.

  void print_stats (const char *call_name,
                    ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time);
  // prints the time stats

  // DII versions of Cubit operations
	
  void cube_short_dii (void);
  void cube_long_dii (void);
  void cube_octet_dii (void);
  void cube_union_dii (void);
  void cube_struct_dii (void);
  void cube_octet_seq_dii (void);
  void cube_long_seq_dii (void);

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  CORBA::ULong loop_count_;
  // Number of times to do the cube operations.

  int exit_later_;
  // Flag to tell server to not exit immediately

  CORBA::Environment env_;
  // Environment variable

  CORBA::ORB_var orb_var_;
  // Storage of the ORB reference

  CORBA::Object_var factory_var_;
  // Storage of the Cubit_factory objref

  CORBA::Object_var obj_var_;
  // Storage of the Cubit objref
  
  CORBA::ULong call_count_;
  // # of calls made to functions

  CORBA::ULong error_count_;
  // # of errors incurred in the lifetime of the application.

  //	const char *TAO_arg_ior_;
  // Pointer to object reference string.

  char *factory_IOR_;
  // IOR of the factory used to make a Cubit object.

  char *factory_key_;
  // Key of server factory.

  char *hostname_;
  // Hostname of server.

  CORBA::ULong portnum_;
  // Default port number of server.
};

#endif /* _DII_C_CLIENT_H */
