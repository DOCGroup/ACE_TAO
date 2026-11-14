
//=============================================================================
/**
 *  @file    server.h
 *
 *    This class implements a simple server for the
 *    Nested Upcalls - MT Client test
 *
 *  @author Michael Kircher
 */
//=============================================================================

#ifndef MT_CLIENT_TEST_MT_OBJECT_SERVER_H
#define MT_CLIENT_TEST_MT_OBJECT_SERVER_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Log_Msg.h"
#include "tao/Utils/ORB_Manager.h"
#include "MT_Object_i.h"
#include "IOR_File.h"

/**
 * @class MT_Object_Server
 *
 * @brief This is the server for the object A in the test.
 *
 * See the README file for more information.
 */
class MT_Object_Server
{
public:
  MT_Object_Server () = default;

  ~MT_Object_Server () = default;

  MT_Object_Server (MT_Object_Server const &) = delete;
  MT_Object_Server (MT_Object_Server &&) = delete;
  MT_Object_Server &operator= (MT_Object_Server const &) = delete;
  MT_Object_Server &operator= (MT_Object_Server &&) = delete;

  /// Initialize the NestedUpCalls_Server state - parsing arguments and ...
  int init (int argc,
            ACE_TCHAR **argv);

  /// Run the orb
  int run ();

private:
  /// Parses the commandline arguments.
  int parse_args ();

  /// File to output the IOR of the object A.
  MT_IOR_File ior_output_file_ { nullptr, &ACE_OS::fclose };

  /// The ORB manager
  TAO_ORB_Manager orb_manager_;

  /// Implementation object MT_OBject
  MT_Object_i mT_Object_i_;

  /// Number of commandline arguments.
  int argc_ {};

  /// Command-line arguments borrowed from ACE_TMAIN for this server's lifetime.
  ACE_TCHAR **argv_ {};
};

#endif /* MT_CLIENT_TEST_MT_OBJECT_SERVER_H */
