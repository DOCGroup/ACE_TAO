// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/Non_Servant_Upcalls
//
// = FILENAME
//     Non_Servant_Upcalls.cpp
//
// = DESCRIPTION
//     This program tests the users ability to make calls on a POA
//     during non-servant upcalls.  In this example, a servant which
//     is being destroyed during because of a deactivate_object()
//     call, tries to deactivate another object in its destructor.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "testS.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined (_MSC_VER)
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

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
      ACE_TRY_NEW_ENV
        {
          PortableServer::POA_var poa = this->_default_POA ();

          PortableServer::ObjectId_var id = poa->servant_to_id (this->other_
                                                                ACE_ENV_ARG_PARAMETER);

          ACE_DEBUG ((LM_DEBUG, "(%t) Deactivating other servant\n"));

          poa->deactivate_object (id.in ()
                                  ACE_ENV_ARG_PARAMETER);
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "test_i::~test_i");
        }
      ACE_ENDTRY;
    }
}

int
main (int argc, char **argv)
{
  ACE_TRY_NEW_ENV
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0
                                            ACE_ENV_ARG_PARAMETER);

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      test_i *servant1 = new test_i (0);
      test_i *servant2 = new test_i (servant1);

      PortableServer::ObjectId_var id1 =
        root_poa->activate_object (servant1
                                   ACE_ENV_ARG_PARAMETER);

      // Give ownership to POA.
      servant1->_remove_ref ();

      PortableServer::ObjectId_var id2 =
        root_poa->activate_object (servant2
                                   ACE_ENV_ARG_PARAMETER);

      // Give ownership to POA.
      servant2->_remove_ref ();

      root_poa->deactivate_object (id2.in ()
                                   ACE_ENV_ARG_PARAMETER);

      root_poa->destroy (1,
                         1
                         ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
