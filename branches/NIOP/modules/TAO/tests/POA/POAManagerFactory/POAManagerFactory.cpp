// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/POAManagerFactory
//
// = FILENAME
//     POAManagerFactory.cpp
//
// = DESCRIPTION
//     This program tests Root_POA::the_POAManagerFactory and
//     the POAManagerFactory interfaces such as create_POAManager(),
//     list () and find ().
//
// = AUTHOR
//     Yan Dai
//
//=========================================================================

#include "tao/ORB.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PI_Server/PI_Server.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/Get_Opt.h"

int fail = 0;
int pretest = 0;
int verbose = 0;

#define VERIFY(Condition) \
{ \
    if (!(Condition)) \
    { \
        fail++; \
        if (!verbose) \
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) - Failure at line %l\n"))); \
    } \
}


int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "v");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'v':
        verbose = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-v "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "POAManagerFactoryTest"
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow to POA.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (verbose)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Obtain PoaManagerFactory reference: ")));
      pretest = fail;

      // Obtain the POAManagerFactory.
      PortableServer::POAManagerFactory_var poa_manager_factory
        = root_poa->the_POAManagerFactory ();

      VERIFY (!CORBA::is_nil(poa_manager_factory.in()));
      if (verbose)
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                    (pretest == fail) ? ACE_TEXT ("passed") : ACE_TEXT ("failed")));
      if (CORBA::is_nil(poa_manager_factory.in()))
        return 1;

      CORBA::PolicyList policies (0);
      policies.length (0);

      if (verbose)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Create a POAManager: ")));
      pretest = fail;

      // Explicitly create a POAManager - "POAManager1" .
      PortableServer::POAManager_var poa_manager_1
        = poa_manager_factory->create_POAManager ("POAManager1",
                                                  policies
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      VERIFY (!CORBA::is_nil(poa_manager_1.in()));
      if (verbose)
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                    (pretest == fail) ? ACE_TEXT ("passed") : ACE_TEXT ("failed")));
      if (CORBA::is_nil(poa_manager_1.in()))
        return 1;

      // Creating a POAManager with an exiting POAManager name raises exception.
      if (verbose)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Prevent duplicated POAManagers: ")));
      pretest = fail;
      CORBA::Boolean got_expected_exception = false;
      ACE_TRY_EX (create)
      {
        PortableServer::POAManager_var poa_manager
         = poa_manager_factory->create_POAManager ("POAManager1",
                                                   policies
                                                   ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK_EX (create);
      }
      ACE_CATCH (PortableServer::POAManagerFactory::ManagerAlreadyExists, mae)
      {
        got_expected_exception = true;
      }
      ACE_ENDTRY;

      VERIFY (got_expected_exception);
      if (verbose)
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                    (pretest == fail) ? ACE_TEXT ("passed") : ACE_TEXT ("failed")));

      if (! got_expected_exception)
        return 1;

      {
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("Implicitly create a POAManagers: ")));
        pretest = fail;
        // Implicitly create a POAManager instance which has an automatically
        // assigned name.
        PortableServer::POA_var child_poa =
        root_poa->create_POA ("childPOA2",
                              PortableServer::POAManager::_nil (),
                              policies
                              ACE_ENV_ARG_PARAMETER);

        PortableServer::POAManager_var poa_manager_2
          = child_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        VERIFY (!CORBA::is_nil(poa_manager_2.in()));
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ? ACE_TEXT ("passed") : ACE_TEXT ("failed")));

        if (CORBA::is_nil(poa_manager_2.in()))
          return 1;

        CORBA::String_var poa_manager_2_name
          = poa_manager_2->get_id (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("Implicitly created POAManager's ID: [%s]\n"),
                      ACE_TEXT_CHAR_TO_TCHAR(poa_manager_2_name.in())));

        if (ACE_OS::strlen(poa_manager_2_name.in()) == 0)
          return 1;

        if (verbose)
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("List POAManagers: ")));
        pretest = fail;

        PortableServer::POAManagerFactory::POAManagerSeq_var managers
          = poa_manager_factory->list (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        VERIFY (managers->length () == 3);
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ? ACE_TEXT ("passed") : ACE_TEXT ("failed")));

        PortableServer::POAManager_var root_poa_manager
          = root_poa->the_POAManager(ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        for (CORBA::ULong i = 0; i < managers->length(); ++i)
        {
          CORBA::String_var name = managers[i]->get_id (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          if (verbose)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("Validate listed POAManager [%s]: "),
                        ACE_TEXT_CHAR_TO_TCHAR(name.in())));
          pretest = fail;

          const PortableServer::POAManager_ptr tmp_pm = managers[i];
          if ((ACE_OS::strcmp (name.in (), "RootPOAManager") == 0
            && tmp_pm == root_poa_manager.in ())
            || (ACE_OS::strcmp (name.in (), "POAManager1") == 0
            && tmp_pm == poa_manager_1.in ())
            || (ACE_OS::strcmp (name.in (), poa_manager_2_name.in ()) == 0
            && tmp_pm == poa_manager_2.in ()))
            {
              if (verbose)
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT("passed\n")));
              continue;
            }
          else
            {
              if (verbose)
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT("failed\n")));
              fail++;
            }
        }
      }

      {
        // Find a specific POAManager.
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("Find a POAManager: ")));
        pretest = fail;

        PortableServer::POAManager_var manager
          = poa_manager_factory->find ("POAManager1" ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        CORBA::String_var name = manager->get_id (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        VERIFY ((ACE_OS::strcmp (name.in (), "POAManager1") == 0
                 && manager.in () == poa_manager_1.in ()));

        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ? ACE_TEXT ("passed") : ACE_TEXT ("failed")));
      }

      // Create a child poa that associates with the explicitly
      // created POAManager.
      if (verbose)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Create A POA with explicit  POAManager: ")));
      pretest = fail;
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("childPOA",
                              poa_manager_1.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager_1->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      root_poa->destroy (1, 1);

      if (verbose)
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("passed\n")));
    }
  ACE_CATCHANY
    {
      fail++;
      if (verbose)
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("failed\n")));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("POAManagerFactory %s\n"),
              (pretest == fail) ? ACE_TEXT ("succeeded") : ACE_TEXT ("failed")));

  return 0;
}
