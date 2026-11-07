//=============================================================================
/**
 *  @file     POA_Destruction.cpp
 *
 *   This program tests the destruction of a POA while there is an
 *   upcall in progress.
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
};

void
test_i::destroy_poa ()
{
  PortableServer::POA_var const poa = this->_default_POA ();

  CORBA::Boolean const etherealize_objects = true;
  CORBA::Boolean const wait_for_completion {};
  poa->destroy (etherealize_objects,
                wait_for_completion);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      CORBA::Object_var const obj =
        orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      test_i servant;

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&servant);

      CORBA::Object_var const object_act = root_poa->id_to_reference (id.in ());

      test_var const test_object = test::_narrow (object_act.in ());

      test_object->destroy_poa ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
