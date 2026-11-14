//=============================================================================
/**
 *  @file     wait_for_completion.cpp
 *
 *   This program tests the wait_for_completion feature of a POA.
 *
 *  @author  Irfan Pyarali
 */
#include "testS.h"

class test_i : public POA_test
{
public:
  test_i () = default;
  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  void destroy_poa () override;

  void test_poa (PortableServer::POA_ptr poa);

  PortableServer::POA_var poa_ {};
};

void
test_i::test_poa (PortableServer::POA_ptr poa)
{
  this->poa_ = PortableServer::POA::_duplicate (poa);
}

void
test_i::destroy_poa ()
{
  CORBA::Boolean const etherealize_objects = true;
  CORBA::Boolean const wait_for_completion = true;
  this->poa_->destroy (etherealize_objects,
                       wait_for_completion);
}

PortableServer::POA_ptr
init_orb (int argc,
          ACE_TCHAR **argv,
          char const *orb_name,
          CORBA::ORB_ptr &orb_ptr)
{
  // Initialize the ORB first.
  CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        argv,
                                        orb_name);

  // Obtain the RootPOA.
  CORBA::Object_var const obj =
    orb->resolve_initial_references ("RootPOA");

  // Get the POA_var object from Object_var.
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (obj.in ());

  // Get the POAManager of the RootPOA.
  PortableServer::POAManager_var const poa_manager =
    root_poa->the_POAManager ();

  poa_manager->activate ();

  orb_ptr = orb._retn ();
  return root_poa._retn ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb1;
      PortableServer::POA_var const first_poa =
        init_orb (argc,
                  argv,
                  "first ORB",
                  orb1.inout ());

      CORBA::ORB_var orb2;
      PortableServer::POA_var const second_poa =
        init_orb (argc,
                  argv,
                  "second ORB",
                  orb2.inout ());

      test_i servant;
      PortableServer::ObjectId_var const id =
        first_poa->activate_object (&servant);

      CORBA::Object_var const object_act = first_poa->id_to_reference (id.in ());

      test_var const test_object = test::_narrow (object_act.in ());

      bool expected_exception_raised {};

      try
        {
          servant.test_poa (first_poa.in ());

          test_object->destroy_poa ();
        }
      catch (CORBA::BAD_INV_ORDER const &)
        {
          // This is the correct exception! Ignore
          expected_exception_raised = true;
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception (
            "Exception caught of incorrect type");
          return -1;
        }

      // Make sure an exception was raised and it was of the correct
      // type.
      ACE_ASSERT (expected_exception_raised);

      // In non-debug compiles, asserts will disappear.
      ACE_UNUSED_ARG (expected_exception_raised);

      servant.test_poa (second_poa.in ());

      test_object->destroy_poa ();

      first_poa->destroy (true, true);

      orb1->destroy ();
      orb2->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Unexpected exception caught");
      return -1;
    }

  return 0;
}
