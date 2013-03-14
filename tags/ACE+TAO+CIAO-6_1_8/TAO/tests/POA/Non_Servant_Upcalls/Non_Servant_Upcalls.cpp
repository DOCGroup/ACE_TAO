
//=============================================================================
/**
 *  @file     Non_Servant_Upcalls.cpp
 *
 *  $Id$
 *
 *   This program tests the users ability to make calls on a POA
 *   during non-servant upcalls.  In this example, a servant which
 *   is being destroyed during because of a deactivate_object()
 *   call, tries to deactivate another object in its destructor.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "testS.h"

class test_i :
  public virtual POA_test
{
public:
  test_i (test_i *other);

  ~test_i (void);

  test_i *other_;
};

test_i::test_i (test_i *other)
  : other_ (other)
{
}

test_i::~test_i (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) test_i::~test_i\n"));

  if (this->other_)
    {
      try
        {
          PortableServer::POA_var poa = this->_default_POA ();

          PortableServer::ObjectId_var id = poa->servant_to_id (this->other_);

          ACE_DEBUG ((LM_DEBUG, "(%t) Deactivating other servant\n"));

          poa->deactivate_object (id.in ());
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("test_i::~test_i");
        }
    }
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

      test_i *servant1 = new test_i (0);
      test_i *servant2 = new test_i (servant1);

      PortableServer::ObjectId_var id1 =
        root_poa->activate_object (servant1);

      // Give ownership to POA.
      servant1->_remove_ref ();

      PortableServer::ObjectId_var id2 =
        root_poa->activate_object (servant2);

      // Give ownership to POA.
      servant2->_remove_ref ();

      root_poa->deactivate_object (id2.in ());

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
