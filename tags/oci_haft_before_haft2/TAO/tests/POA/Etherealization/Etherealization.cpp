// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/Etherealization
//
// = FILENAME
//     Etherealization.cpp
//
// = DESCRIPTION
//     This program tests for deactivation and etherealization of
//     reference counted and non reference counted servants.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "testS.h"

class test_i : public POA_test
{
public:
  void method (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER*/)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

  ~test_i (void)
  {
    ACE_DEBUG ((LM_DEBUG, "~test_i called\n"));
  }
};

class test_i_with_reference_counting : public virtual PortableServer::RefCountServantBase,
                                       public virtual POA_test
{
public:
  void method (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

  ~test_i_with_reference_counting (void)
  {
    ACE_DEBUG ((LM_DEBUG, "~test_i_with_reference_counting called\n"));
  }
};

class Servant_Activator : public PortableServer::ServantActivator
{
public:
  PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
                                     PortableServer::POA_ptr poa
                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest));

  void etherealize (const PortableServer::ObjectId &oid,
                    PortableServer::POA_ptr adapter,
                    PortableServer::Servant servant,
                    CORBA::Boolean cleanup_in_progress,
                    CORBA::Boolean remaining_activations
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

PortableServer::Servant
Servant_Activator::incarnate (const PortableServer::ObjectId &id,
                              PortableServer::POA_ptr
                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::ForwardRequest))
{
  CORBA::String_var object_name =
    PortableServer::ObjectId_to_string (id);

  ACE_DEBUG ((LM_DEBUG,
              "\nIncarnate called with id = \"%s\"\n",
              object_name.in ()));

  if (ACE_OS::strcmp (object_name.in (),
                      "without reference counting") == 0)
    return new test_i;
  else
    return new test_i_with_reference_counting;
}


void
Servant_Activator::etherealize (const PortableServer::ObjectId &id,
                                PortableServer::POA_ptr ,
                                PortableServer::Servant servant,
                                CORBA::Boolean,
                                CORBA::Boolean
                                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::String_var object_name =
    PortableServer::ObjectId_to_string (id);

  ACE_DEBUG ((LM_DEBUG,
              "Etherealize called with id = \"%s\"\n",
              object_name.in ()));

  if (ACE_OS::strcmp (object_name.in (),
                      "without reference counting") == 0)
    delete servant;
  else
    servant->_remove_ref ();
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the RootPOA.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList policies (3);
      policies.length (3);

      // ID Assignment Policy
      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Lifespan Policy
      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Request Processing Policy
      policies[2] =
        root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create servant activator.
      PortableServer::ServantManager_var servant_manager =
        new Servant_Activator;

      // Set servant_activator as the servant_manager of child POA.
      child_poa->set_servant_manager (servant_manager.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      {
        // Create a reference with user created ID in child POA which
        // uses the Servant_Activator.
        PortableServer::ObjectId_var id =
          PortableServer::string_to_ObjectId ("without reference counting");

        object =
          child_poa->create_reference_with_id (id.in (),
                                               "IDL:test:1.0"
                                               ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        test_var test =
          test::_narrow (object.in ()
                         ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        test->method (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        child_poa->deactivate_object (id.in ()
                                      ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      {
        // Create a reference with user created ID in child POA which
        // uses the Servant_Activator.
        PortableServer::ObjectId_var id =
          PortableServer::string_to_ObjectId ("with reference counting");

        object =
          child_poa->create_reference_with_id (id.in (),
                                               "IDL:test:1.0"
                                               ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        test_var test =
          test::_narrow (object.in ()
                         ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        test->method (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        child_poa->deactivate_object (id.in ()
                                      ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      ACE_DEBUG ((LM_DEBUG,
                  "\nEnd of main()\n\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
