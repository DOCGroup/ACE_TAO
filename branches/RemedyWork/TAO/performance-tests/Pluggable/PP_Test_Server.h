
//=============================================================================
/**
 *  @file    PP_Test_Server.h
 *
 *  $Id$
 *
 *    This class implements simple oneway and twoway no-op requests
 *    to time latency and overhead of the pluggable protocol functionality.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef _PP_TEST_SERVER_H
#define _PP_TEST_SERVER_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Utils/ORB_Manager.h"
#include "PP_Test_i.h"

/**
 * @class PP_Test_Server
 *
 * @brief Defines a server class that implements the functionality
 * of a server process as an object.
 *
 * The interface is quite simple. A server program has to call
 * init to initialize the server's state and then call run
 * to run the orb.
 */
class PP_Test_Server
{
public:
  // = Initialization and termination methods.
  /// Default constructor
  PP_Test_Server (void);

  /// Destructor
  ~PP_Test_Server (void);

  /// Initialize the server state - parsing arguments, etc.
  int init (int argc,
            ACE_TCHAR **argv);

  /// Run the orb
  int run (void);

private:
  /// Parses the commandline arguments.
  int parse_args (void);

  /// File to output the cubit factory IOR.
  FILE* ior_output_file_;

  /// The ORB manager.
  TAO_ORB_Manager orb_manager_;

  /// Implementation object of the Pluggable Test factory.
  Pluggable_Test_Factory_i *factory_impl_;

  /// ID of the factory.
  CORBA::String_var factory_id_;

  /// Number of commandline arguments.
  int argc_;

  /// commandline arguments.
  ACE_TCHAR **argv_;
};

#endif /* _PP_TEST_SERVER_H */
