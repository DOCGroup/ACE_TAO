
//=============================================================================
/**
 *  @file     server.cpp
 *
 *  $Id$
 *
 *   This program tests that multiple calls to the Servant Locator
 *   can take place simultaneously.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "testS.h"
#include "tao/PortableServer/ServantLocatorC.h"
#include "tao/CDR.h"

int postCount = 0;
int errorCount = 0;

class test_i :
  public virtual POA_test
{
public:

  test_i (PortableServer::POA_ptr poa);

  void normal (void);

  void exceptional (void);

  void notexisting (void);

  PortableServer::POA_var poa_;
};

test_i::test_i (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
test_i::normal (void)
{
    ACE_DEBUG ((LM_DEBUG, "executing normal\n"));
}

void
test_i::exceptional (void)
{
    ACE_DEBUG ((LM_DEBUG, "executing exceptional\n"));
}

void
test_i::notexisting (void)
{
    ACE_DEBUG ((LM_DEBUG, "executing notexisting\n"));
}

class Servant_Locator :
  public PortableServer::ServantLocator
{
public:

  Servant_Locator (PortableServer::POA_ptr poa);

  ::PortableServer::Servant preinvoke (const PortableServer::ObjectId &,
                                       PortableServer::POA_ptr,
                                       const char *,
                                       PortableServer::ServantLocator::Cookie &);

  void postinvoke (const PortableServer::ObjectId &,
                   PortableServer::POA_ptr,
                   const char *,
                   PortableServer::ServantLocator::Cookie,
                   PortableServer::Servant);

  test_i servant_;
};

Servant_Locator::Servant_Locator (PortableServer::POA_ptr poa)
  : servant_ (poa)
{
}

::PortableServer::Servant
Servant_Locator::preinvoke (const PortableServer::ObjectId &oid,
                            PortableServer::POA_ptr,
                            const char *op,
                            PortableServer::ServantLocator::Cookie &)
{
  CORBA::String_var name =
    PortableServer::ObjectId_to_string (oid);

  ACE_DEBUG ((LM_DEBUG,
              "Servant_Locator::preinvoke for %C.%C ",
              name.in (), op ));

  if (ACE_OS::strcmp (op, "normal") == 0)
  {
    ACE_DEBUG ((LM_DEBUG, "returning servant\n"));
    return &this->servant_;
  }
  else if (ACE_OS::strcmp (op, "exceptional") == 0)
  {
    ACE_DEBUG ((LM_DEBUG, "throwing exception\n"));
      throw CORBA::INTERNAL();
  }
  else
  {
    ACE_DEBUG ((LM_DEBUG, "returning NULL\n"));
    return 0;
  }
}

void
Servant_Locator::postinvoke (const PortableServer::ObjectId &oid,
                             PortableServer::POA_ptr,
                             const char *op,
                             PortableServer::ServantLocator::Cookie,
                             PortableServer::Servant)
{
  ++postCount;
  CORBA::String_var name =
    PortableServer::ObjectId_to_string (oid);

  if (!op)
  {
    op = "NULL";
    ++errorCount;
  }

  ACE_DEBUG ((LM_DEBUG,
              "Servant_Locator::postinvoke for %C.%C\n",
              name.in (), op ));

}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int retval = 0;

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      CORBA::PolicyList policies;
      CORBA::ULong current_length = 0;

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_servant_retention_policy (PortableServer::NON_RETAIN);

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies);

      Servant_Locator servant_locator (child_poa.in ());
      child_poa->set_servant_manager (&servant_locator);

      PortableServer::ObjectId_var objectID =
        PortableServer::string_to_ObjectId ("object");

      CORBA::Object_var objectREF =
        child_poa->create_reference_with_id (objectID.in (),
                                             "IDL:test:1.0");

      test_var testObject =
        test::_narrow (objectREF.in ());

      testObject->normal();

      bool caught = false;
      try
      {
        testObject->exceptional();
      }
      catch (const CORBA::Exception&)
      {
        ACE_DEBUG ((LM_DEBUG, "exceptional() yielded exception\n"));
        caught = true;
      }
      if (!caught) ++errorCount;

      caught = false;
      try
      {
        testObject->notexisting();
      }
      catch (const CORBA::Exception&)
      {
        ACE_DEBUG ((LM_DEBUG, "notexisting() yielded exception\n"));
        caught = true;
      }
      if (!caught) ++errorCount;

      if (!errorCount)
      {
      ACE_DEBUG ((LM_DEBUG,"test successful\n"));
      }
      else
      {
      ACE_DEBUG ((LM_DEBUG,"unsuccessfull: %d errors\n", errorCount ));
      }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      retval = -1;
    }

  return errorCount;
}
