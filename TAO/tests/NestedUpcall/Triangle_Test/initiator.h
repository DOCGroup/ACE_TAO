
//=============================================================================
/**
 *  @file    initiator.h
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


#ifndef _TRIANGLE_TEST_INITITATOR_SERVER_H
#define _TRIANGLE_TEST_INITITATOR_SERVER_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Log_Msg.h"
#include "tao/Utils/ORB_Manager.h"
#include "Initiator_i.h"

/**
 * @class Initiator_Server
 *
 * @brief This is the server for the Initiator in the test.
 *
 * See the README file for more information.
 */
class Initiator_Server
{

public:

  /// Default constructor
  Initiator_Server (void);

  /// Destructor
  ~Initiator_Server (void);

  /// read in the IOR's for the two objects A and B
  int read_ior (ACE_TCHAR *filename, unsigned int A_B);

  /// Initialize the Initiator_Server state - parsing arguments and ...
  int init (int argc,
            ACE_TCHAR **argv);

  /// Run the orb
  int run (void);

private:
  /// Parses the commandline arguments.
  int parse_args (void);

  /// The IOR of object A
  char * object_A_key_;

  /// The IOR of object B
  char * object_B_key_;

  /// reference to object A
  Object_A_var object_A_var_;

  /// reference to object B
  Object_B_var object_B_var_;

  /// The ORB manager
  TAO_ORB_Manager orb_manager_;

  /// Implementation object of the Initiator
  Initiator_i *initiator_i_ptr_;

  /// Number of commandline arguments.
  int argc_;

  /// commandline arguments.
  ACE_TCHAR **argv_;

  /// IOR of my servant.
  CORBA::String_var str_;
};

#endif /* _TRIANGLE_TEST_INITITATOR_SERVER_H */
