
//=============================================================================
/**
 *  @file     POA_Destruction.cpp
 *
 *  $Id$
 *
 *   This program tests the destruction of a POA while there is an
 *   upcall in progress.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "testS.h"

class test_i : public POA_test
{
public:
  void destroy_poa (void);
};

void
test_i::destroy_poa (void)
{
  PortableServer::POA_var poa = this->_default_POA ();

  CORBA::Boolean etherealize_objects = 1;
  CORBA::Boolean wait_for_completion = 0;
  poa->destroy (etherealize_objects,
                wait_for_completion);
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      test_i servant;

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&servant);

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      test_var test_object = test::_narrow (object_act.in ());

      test_object->destroy_poa ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
