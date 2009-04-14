// $Id$

//============================================================================
//
//  = FILENAME
//     Collocation_Test.h
//
//  = DESCRIPTION
//     Server class to perform testing of TAO's collocation mechanism.
//
//  = AUTHOR
//     Nanbor Wang
//
//=============================================================================

#if !defined (TAO_COLLOCATION_TEST_H)
#define TAO_COLLOCATION_TEST_H

#include "Diamond_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Collocation_Test
{
public:
  Collocation_Test (void);

  void shutdown (void);

  int init (int argc, ACE_TCHAR *argv[]);
  // Initializing the Collocation_Test object.

  int parse_args (int argc, ACE_TCHAR *argv[]);
  // Parse the test specific arguments.

  int test_narrow (void);
  // This test narrow an object reference to its base class and see
  // if it works correctly.

  int run (void);
  // Run the test.

private:
  CORBA::ORB_var orb_;

  PortableServer::POA_var root_poa_;

  PortableServer::POAManager_var poa_manager_;

  CORBA::Object_var diamond_obj_;
  // Our basic test object.

  Top_i top_servant_;
  Left_i left_servant_;
  Right_i right_servant_;
  Buttom_i diamond_servant_;
  // A collection of servant for basic narrowing test.
};

#endif /* TAO_COLLOCATION_TEST_H */
