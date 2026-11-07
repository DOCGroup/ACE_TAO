
//=============================================================================
/**
 *  @file     server.cpp
 *
 *   This program tests that multiple calls to the Servant Locator
 *   can take place simultaneously.
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================
#include "testS.h"
#include "tao/PortableServer/ServantLocatorC.h"
#include <cstring>

static int errorCount {};

class test_i :
  public virtual POA_test
{
public:
  explicit test_i (PortableServer::POA_ptr poa);
  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  void normal () override;

  void exceptional () override;

  void notexisting () override;

  PortableServer::POA_var poa_ {};
};

test_i::test_i (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
test_i::normal ()
{
  ACE_DEBUG ((LM_DEBUG, "executing normal\n"));
}

void
test_i::exceptional ()
{
  ACE_DEBUG ((LM_DEBUG, "executing exceptional\n"));
}

void
test_i::notexisting ()
{
  ACE_DEBUG ((LM_DEBUG, "executing notexisting\n"));
}

class Servant_Locator :
  public PortableServer::ServantLocator
{
public:
  explicit Servant_Locator (PortableServer::POA_ptr poa);
  Servant_Locator (Servant_Locator const &) = delete;
  Servant_Locator (Servant_Locator &&) = delete;
  Servant_Locator &operator= (Servant_Locator const &) = delete;
  Servant_Locator &operator= (Servant_Locator &&) = delete;

  ::PortableServer::Servant preinvoke (PortableServer::ObjectId const &,
                                       PortableServer::POA_ptr,
                                       char const *,
                                       PortableServer::ServantLocator::Cookie &) override;

  void postinvoke (PortableServer::ObjectId const &,
                   PortableServer::POA_ptr,
                   char const *,
                   PortableServer::ServantLocator::Cookie,
                   PortableServer::Servant) override;

  test_i servant_;
};

Servant_Locator::Servant_Locator (PortableServer::POA_ptr poa)
  : servant_ (poa)
{
}

::PortableServer::Servant
Servant_Locator::preinvoke (PortableServer::ObjectId const &oid,
                            PortableServer::POA_ptr,
                            char const *op,
                            PortableServer::ServantLocator::Cookie &)
{
  CORBA::String_var name =
    PortableServer::ObjectId_to_string (oid);

  ACE_DEBUG ((LM_DEBUG,
              "Servant_Locator::preinvoke for %C.%C ",
              name.in (), op ));

  if (std::strcmp (op, "normal") == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "returning servant\n"));
      return &this->servant_;
    }
  else if (std::strcmp (op, "exceptional") == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "throwing exception\n"));
      throw CORBA::INTERNAL ();
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "returning NULL\n"));
      return nullptr;
    }
}

void
Servant_Locator::postinvoke (PortableServer::ObjectId const &oid,
                             PortableServer::POA_ptr,
                             char const *op,
                             PortableServer::ServantLocator::Cookie,
                             PortableServer::Servant)
{
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      CORBA::PolicyList policies;
      CORBA::ULong current_length {};

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_servant_retention_policy (PortableServer::NON_RETAIN);

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      PortableServer::POA_var const child_poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies);

      Servant_Locator servant_locator (child_poa.in ());
      child_poa->set_servant_manager (&servant_locator);

      PortableServer::ObjectId_var const objectID =
        PortableServer::string_to_ObjectId ("object");

      CORBA::Object_var const objectREF =
        child_poa->create_reference_with_id (objectID.in (),
                                             "IDL:test:1.0");

      test_var const testObject =
        test::_narrow (objectREF.in ());

      testObject->normal();

      bool caught {};
      try
        {
          testObject->exceptional();
        }
      catch (CORBA::Exception const &)
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
      catch (CORBA::Exception const &)
        {
          ACE_DEBUG ((LM_DEBUG, "notexisting() yielded exception\n"));
          caught = true;
        }
      if (!caught) ++errorCount;

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      ++errorCount;
    }

  if (!errorCount)
    {
      ACE_DEBUG ((LM_DEBUG,"test successful\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,"unsuccessfull: %d errors\n", errorCount ));
    }

  return errorCount;
}
