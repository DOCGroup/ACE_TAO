//=============================================================================
/**
 *  @file    server_A.h
 *
 *    This class implements a simple server for the
 *    Nested Upcalls - Triangle test
 *
 *  @author Michael Kircher
 */
//=============================================================================
#ifndef _TRIANGLE_TEST_OBJECT_A_SERVER_H
#define _TRIANGLE_TEST_OBJECT_A_SERVER_H

#include "ace/Get_Opt.h"
#include "IOR_File.h"

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
  Object_A_Server () = default;

  ~Object_A_Server () = default;

  Object_A_Server (Object_A_Server const &) = delete;
  Object_A_Server (Object_A_Server &&) = delete;
  Object_A_Server &operator= (Object_A_Server const &) = delete;
  Object_A_Server &operator= (Object_A_Server &&) = delete;

  /// Initialize the NestedUpCalls_Server state - parsing arguments and ...
  int init (int argc,
            ACE_TCHAR **argv);

  /// Run the orb
  int run ();

private:
  /// Parses the commandline arguments.
  int parse_args ();

  /// File to output the IOR of the object A.
  IOR_File ior_output_file_ { nullptr, &ACE_OS::fclose };

  /// The ORB manager
  TAO_ORB_Manager orb_manager_;

  /// Implementation object of the NestedUpCalls reactor.
  Object_A_i object_A_i_;

  /// Number of commandline arguments.
  int argc_ {};

  /// commandline arguments.
  ACE_TCHAR **argv_ {};
};

#endif /* _TRIANGLE_TEST_OBJECT_A_SERVER_H */
