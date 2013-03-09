
//=============================================================================
/**
 *  @file    server_A.h
 *
 *  $Id$
 *
 *    This class implements a simple server for the
 *    Nested Upcalls - Triangle test
 *
 *
 *  @author Michael Kircher
 */
//=============================================================================


#ifndef _TRIANGLE_TEST_OBJECT_A_SERVER_H
#define _TRIANGLE_TEST_OBJECT_A_SERVER_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Log_Msg.h"
#include "tao/Utils/ORB_Manager.h"
#include "Object_A_i.h"

/**
 * @class Object_A_Server
 *
 * @brief This is the server for the object A in the test.
 *
 * See the README file for more information.
 */
class Object_A_Server
{

public:

  /// Default constructor
  Object_A_Server (void);

  /// Destructor
  ~Object_A_Server (void);

  /// Initialize the NestedUpCalls_Server state - parsing arguments and ...
  int init (int argc,
            ACE_TCHAR **argv);

  /// Run the orb
  int run (void);

private:
  /// Parses the commandline arguments.
  int parse_args (void);

  /// File to output the IOR of the object A.
  FILE* ior_output_file_;

  /// The ORB manager
  TAO_ORB_Manager orb_manager_;

  /// Implementation object of the NestedUpCalls reactor.
  Object_A_i object_A_i_;

  /// Number of commandline arguments.
  int argc_;

  /// commandline arguments.
  ACE_TCHAR **argv_;
};

#endif /* _TRIANGLE_TEST_OBJECT_A_SERVER_H */
