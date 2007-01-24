// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/MT_Servant_Locator
//
// = FILENAME
//     server.cpp
//
// = DESCRIPTION
//     This program tests that multiple calls to the Servant Locator
//     can take place simultaneously.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "testS.h"
#include "ace/OS.h"
#include "tao/PortableServer/ServantLocatorC.h"
#include "tao/CDR.h"

int postCount = 0;
int errorCount = 0;

class test_i :
  public virtual POA_test
{
public:

  test_i (PortableServer::POA_ptr poa);

  void normal (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void exceptional (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void notexisting (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_var poa_;
};

test_i::test_i (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
test_i::normal (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
    ACE_DEBUG ((LM_DEBUG, "executing normal\n"));
}

void
test_i::exceptional (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
    ACE_DEBUG ((LM_DEBUG, "executing exceptional\n"));
}

void
test_i::notexisting (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
                                       PortableServer::ServantLocator::Cookie &
                                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest));

  void postinvoke (const PortableServer::ObjectId &,
                   PortableServer::POA_ptr,
                   const char *,
                   PortableServer::ServantLocator::Cookie,
                   PortableServer::Servant
                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

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
                            PortableServer::ServantLocator::Cookie &
                            ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::ForwardRequest))
{
  CORBA::String_var name =
    PortableServer::ObjectId_to_string (oid);

  ACE_DEBUG ((LM_DEBUG,
              "Servant_Locator::preinvoke for %s.%s ",
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
                             PortableServer::Servant
                             ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
              "Servant_Locator::postinvoke for %s.%s\n",
              name.in (), op ));

}

int
main (int argc, char **argv)
{
  int retval = 0;

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      CORBA::PolicyList policies;
      CORBA::ULong current_length = 0;

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER
                                                    ACE_ENV_ARG_PARAMETER);

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_servant_retention_policy (PortableServer::NON_RETAIN
                                                   ACE_ENV_ARG_PARAMETER);

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID
                                               ACE_ENV_ARG_PARAMETER);

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);

      Servant_Locator* servant_locator = new Servant_Locator(child_poa.in ()) ;
      child_poa->set_servant_manager (servant_locator
                                      ACE_ENV_ARG_PARAMETER);

      PortableServer::ObjectId_var objectID =
        PortableServer::string_to_ObjectId ("object");

      CORBA::Object_var objectREF =
        child_poa->create_reference_with_id (objectID.in (),
                                             "IDL:test:1.0"
                                             ACE_ENV_ARG_PARAMETER);

      test_var testObject =
        test::_narrow (objectREF.in ()
                       ACE_ENV_ARG_PARAMETER);

      testObject->normal();

      bool caught = false;
      ACE_TRY
      {
        testObject->exceptional();
      }
      ACE_CATCHANY
      {
        ACE_DEBUG ((LM_DEBUG, "exceptional() yielded exception\n"));
        caught = true;
      }
      ACE_ENDTRY;
      if (!caught) ++errorCount;

      caught = false;
      ACE_TRY
      {
        testObject->notexisting();
      }
      ACE_CATCHANY
      {
        ACE_DEBUG ((LM_DEBUG, "notexisting() yielded exception\n"));
        caught = true;
      }
      ACE_ENDTRY;
      if (!caught) ++errorCount;

      if (!errorCount)
      {
      ACE_DEBUG ((LM_DEBUG,"test successful\n"));
      }
      else
      {
      ACE_DEBUG ((LM_DEBUG,"unsuccessfull: %d errors\n", errorCount ));
      }

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      retval = -1;
    }
  ACE_ENDTRY;

  return retval;
}
