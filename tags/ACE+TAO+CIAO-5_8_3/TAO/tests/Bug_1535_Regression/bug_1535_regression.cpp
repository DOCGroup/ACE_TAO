// $Id$

#include "Test.h"
#include "tao/Utils/ORB_Destroyer.h"
#include "tao/Utils/RIR_Narrow.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "tao/Utils/Servant_Var.h"

PortableServer::POA_ptr
create_persistent_POA (PortableServer::POA_ptr parent,
                       char const * name)
{
  TAO::Utils::PolicyList_Destroyer plist (3);
  plist.length(3);

  plist[0] =
    parent->create_lifespan_policy (PortableServer::PERSISTENT);

  plist[1] =
    parent->create_id_assignment_policy (PortableServer::USER_ID);

  plist[2] =
    parent->create_implicit_activation_policy (
      PortableServer::NO_IMPLICIT_ACTIVATION);

  PortableServer::POAManager_var mgr =
    parent->the_POAManager ();

  return parent->create_POA (name,
                             mgr.in(),
                             plist);
}

void
test_create_object_before_servant_reactivation (
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr root_poa)
{
  // Create a persistent POA and then create a reference in it...
  PortableServer::POA_var persistent_poa =
    create_persistent_POA(root_poa,
                          "T1");


  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId ("TestServant");

  char const * id = _tc_Test->id ();

  CORBA::Object_var object =
    persistent_poa->create_reference_with_id (
      oid.in (),
      id);

  if (CORBA::is_nil (object.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) nil reference in create_reference_with_id\n"));

      return;
    }

  CORBA::String_var ior =
    orb->object_to_string (object.in ());

  // Now destroy the POA...
  persistent_poa->destroy (true,
                           true);

  // Now create the POA again...
  persistent_poa =
    create_persistent_POA (root_poa,
                           "T1");

  // And try to create the object again...
  object =
    orb->string_to_object (ior.in ());

  if(CORBA::is_nil (object.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) nil reference in string_to_object (servant reactivation)\n"));
      return;
    }

  persistent_poa->destroy (true,
                           true);
}

void
test_create_object_before_POA_reactivation(
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr root_poa)
{
  // Create a persistent POA and then create a reference in it...
  PortableServer::POA_var persistent_poa =
    create_persistent_POA (root_poa,
                           "T2");

  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId ("TestServant");

  char const * id = _tc_Test->id ();

  CORBA::Object_var object =
    persistent_poa->create_reference_with_id (oid.in (),
                                              id);

  if (CORBA::is_nil (object.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) nil reference in create_reference_with_id\n"));
      return;
    }

  CORBA::String_var ior =
    orb->object_to_string (object.in ());

  // Now destroy the POA...
  persistent_poa->destroy (true,
                           true);

  // And try to create the object again...
  object = orb->string_to_object (ior.in ());

  if (CORBA::is_nil (object.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) nil reference in string_to_object (POA reactivation)\n"));

      return;
    }

  persistent_poa->destroy (true,
                           true);
}

void
test_no_implicit_activation (
  PortableServer::POA_ptr root_poa)
{
  // Create a persistent POA and then create a reference in it...
  PortableServer::POA_var persistent_poa =
    create_persistent_POA (root_poa,
                           "T3");

  Hello myhello (persistent_poa.in ());

  bool succeed = false;
  try
    {
      // Implicit activation should fail
      Test_var myservant =
          myhello._this ();

    }
  catch (const PortableServer::POA::WrongPolicy& )
    {
      succeed = true;
    }
  catch (const PortableServer::POA::ServantNotActive& )
    {
      // This is now the case when looking at the corba spec, raised
      // an issue 10522 about this
      succeed = true;
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, Implicit activation failed with invalid exception\n"));
  }

  // Now destroy the POA...
  persistent_poa->destroy (true,
                           true);
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv);

      TAO::Utils::ORB_Destroyer orb_destroyer (orb.in());

      PortableServer::POA_var root_poa =
        TAO::Utils::RIR_Narrow<PortableServer::POA>::narrow (orb.in (),
                                                             "RootPOA");

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      test_create_object_before_POA_reactivation (orb.in(),
                                                  root_poa.in ());

      test_create_object_before_servant_reactivation (orb.in (),
                                                      root_poa.in ());

      test_no_implicit_activation (root_poa.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught a CORBA exception\n");
      return 1;
    }
  catch (...)
    {
      return 1;
    }

  return 0;
}
