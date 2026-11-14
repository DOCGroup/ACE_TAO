/**
 * @file Coll_Tester.h
 * @brief Tests TAO's collocation mechanism.
 * @author Nanbor Wang
 */

#if !defined (TAO_COLLOCATION_TEST_H)
#define TAO_COLLOCATION_TEST_H

#include "Diamond_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Collocation_Test
{
public:
  Collocation_Test () = default;
  ~Collocation_Test ();
  Collocation_Test (Collocation_Test const &) = delete;
  Collocation_Test (Collocation_Test &&) = delete;
  Collocation_Test &operator= (Collocation_Test const &) = delete;
  Collocation_Test &operator= (Collocation_Test &&) = delete;

  /// Initialize the ORB and activate the servants.
  int init (int argc, ACE_TCHAR *argv[]);

  /// Narrow the object reference to the diamond interfaces and test calls.
  int test_narrow ();

  /// Run the collocation test.
  int run ();

private:
  CORBA::ORB_var orb_;

  PortableServer::POA_var root_poa_;

  PortableServer::POAManager_var poa_manager_;

  /// The object reference for the diamond servant.
  CORBA::Object_var diamond_obj_;

  /// Servants used by the basic narrowing test.
  Top_i top_servant_;
  Left_i left_servant_;
  Right_i right_servant_;
  Buttom_i diamond_servant_;
};

#endif /* TAO_COLLOCATION_TEST_H */
