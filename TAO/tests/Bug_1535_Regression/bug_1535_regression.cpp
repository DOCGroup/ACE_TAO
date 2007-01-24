// $Id$

#include "Test.h"
#include "tao/Utils/ORB_Destroyer.h"
#include "tao/Utils/RIR_Narrow.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "tao/Utils/Servant_Var.h"

ACE_RCSID (Bug_1535_Regression,
           bug_1535_regression,
           "$Id$")

PortableServer::POA_ptr
create_persistent_POA (PortableServer::POA_ptr parent,
                       char const * name
                       ACE_ENV_ARG_DECL)
{
  TAO::Utils::PolicyList_Destroyer plist (3);
  plist.length(3);

  plist[0] =
    parent->create_lifespan_policy (PortableServer::PERSISTENT
                                    ACE_ENV_ARG_PARAMETER);

  plist[1] =
    parent->create_id_assignment_policy (PortableServer::USER_ID
                                         ACE_ENV_ARG_PARAMETER);

  plist[2] =
    parent->create_implicit_activation_policy (
      PortableServer::NO_IMPLICIT_ACTIVATION
      ACE_ENV_ARG_PARAMETER);

  PortableServer::POAManager_var mgr =
    parent->the_POAManager ();

  return parent->create_POA (name,
                             mgr.in(),
                             plist
                             ACE_ENV_ARG_PARAMETER);
}

void
test_create_object_before_servant_reactivation (
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr root_poa
  ACE_ENV_ARG_DECL)
{
  // Create a persistent POA and then create a reference in it...
  PortableServer::POA_var persistent_poa =
    create_persistent_POA(root_poa,
                          "T1"
                          ACE_ENV_ARG_PARAMETER);


  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId ("TestServant");

  char const * id = _tc_Test->id ();

  CORBA::Object_var object =
    persistent_poa->create_reference_with_id (
      oid.in (),
      id
      ACE_ENV_ARG_PARAMETER);

  if (CORBA::is_nil (object.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) nil reference in create_reference_with_id \n"));

      return;
    }

  CORBA::String_var ior =
    orb->object_to_string (object.in ()
                           ACE_ENV_ARG_PARAMETER);

  // Now destroy the POA...
  persistent_poa->destroy (true,
                           true);

  // Now create the POA again...
  persistent_poa =
    create_persistent_POA (root_poa,
                           "T1"
                           ACE_ENV_ARG_PARAMETER);

  // And try to create the object again...
  object =
    orb->string_to_object (ior.in ()
                           ACE_ENV_ARG_PARAMETER);

  if(CORBA::is_nil (object.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) nil reference in string_to_object (servant reactivation) \n"));
      return;
    }

  persistent_poa->destroy (true,
                           true
                           ACE_ENV_ARG_PARAMETER);
}

void
test_create_object_before_POA_reactivation(
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr root_poa
  ACE_ENV_ARG_DECL)
{
  // Create a persistent POA and then create a reference in it...
  PortableServer::POA_var persistent_poa =
    create_persistent_POA (root_poa,
                           "T2"
                           ACE_ENV_ARG_PARAMETER);

  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId ("TestServant");

  char const * id = _tc_Test->id ();

  CORBA::Object_var object =
    persistent_poa->create_reference_with_id (oid.in (),
                                              id
                                              ACE_ENV_ARG_PARAMETER);

  if (CORBA::is_nil (object.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) nil reference in create_reference_with_id \n"));
      return;
    }

  CORBA::String_var ior =
    orb->object_to_string (object.in ()
                           ACE_ENV_ARG_PARAMETER);

  // Now destroy the POA...
  persistent_poa->destroy (true,
                           true
                           ACE_ENV_ARG_PARAMETER);

  // And try to create the object again...
  object = orb->string_to_object (ior.in ()
                                  ACE_ENV_ARG_PARAMETER);

  if (CORBA::is_nil (object.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) nil reference in string_to_object (POA reactivation) \n"));

      return;
    }

  persistent_poa->destroy (true,
                           true
                           ACE_ENV_ARG_PARAMETER);
}

void
test_no_implicit_activation (
  PortableServer::POA_ptr root_poa
  ACE_ENV_ARG_DECL)
{
  // Create a persistent POA and then create a reference in it...
  PortableServer::POA_var persistent_poa =
    create_persistent_POA (root_poa,
                           "T3"
                           ACE_ENV_ARG_PARAMETER);

  Hello myhello (persistent_poa.in ());

  bool succeed = false;
  ACE_TRY_NEW_ENV
    {
      // Implicit activation should fail
      Test_var myservant =
          myhello._this ();

    }
  ACE_CATCH (PortableServer::POA::WrongPolicy, ex)
    {
      succeed = true;
    }
  ACE_CATCH (PortableServer::POA::ServantNotActive, ex)
    {
      // This is now the case when looking at the corba spec, raised
      // an issue 10522 about this
      succeed = true;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, Implicit activation failed with invalid exception\n"));
  }

  // Now destroy the POA...
  persistent_poa->destroy (true,
                           true
                           ACE_ENV_ARG_PARAMETER);
}

int
main (int argc, char *argv[])
{
  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         ""
                         ACE_ENV_ARG_PARAMETER);

      TAO::Utils::ORB_Destroyer orb_destroyer (orb.in());

      PortableServer::POA_var root_poa =
        TAO::Utils::RIR_Narrow<PortableServer::POA>::narrow (orb.in (),
                                                             "RootPOA"
                                                             ACE_ENV_ARG_PARAMETER);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      test_create_object_before_POA_reactivation (orb.in(),
                                                  root_poa.in ()
                                                  ACE_ENV_ARG_PARAMETER);

      test_create_object_before_servant_reactivation (orb.in (),
                                                      root_poa.in ()
                                                      ACE_ENV_ARG_PARAMETER);

      test_no_implicit_activation (root_poa.in ()
                                   ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught a CORBA exception \n");
      return 1;
    }
  ACE_CATCHALL
    {
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
