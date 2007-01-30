// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/POA_Destruction
//
// = FILENAME
//     POA_Destruction.cpp
//
// = DESCRIPTION
//     This program tests the destruction of a POA while there is an
//     upcall in progress.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "testS.h"

class test_i : public POA_test
{
public:
  void destroy_poa (void)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

void
test_i::destroy_poa (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_var poa = this->_default_POA ();

  CORBA::Boolean etherealize_objects = 1;
  CORBA::Boolean wait_for_completion = 0;
  poa->destroy (etherealize_objects,
                wait_for_completion);
}

int
main (int argc, char **argv)
{

  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0);

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

      test_var test_object = servant._this ();

      test_object->destroy_poa ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
