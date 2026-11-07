//=============================================================================
/**
 *  @file     POAManagerFactory.cpp
 *
 *   This program tests Root_POA::the_POAManagerFactory and
 *   the POAManagerFactory interfaces such as create_POAManager(),
 *   list () and find ().
 *
 *  @author  Yan Dai
 */
//=============================================================================

#include <cstring>
#include "tao/ORB.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PI_Server/PI_Server.h"

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

static int fail {};
static int pretest {};
static bool verbose {};

#define VERIFY_CONDITION(Condition) \
{ \
    if (!(Condition)) \
    { \
        fail++; \
        if (!verbose) \
          ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t) - Failure at line %l\n"))); \
    } \
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("v"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'v':
        verbose = true;
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc,
                         argv,
                         "POAManagerFactoryTest");

      if (parse_args (argc, argv) != 0)
        return 1;

      // Obtain the RootPOA.
      CORBA::Object_var const obj =
        orb->resolve_initial_references ("RootPOA");

      // Narrow to POA.
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (obj.in ());

      if (verbose)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Obtain PoaManagerFactory reference: ")));
      pretest = fail;

      // Obtain the POAManagerFactory.
      PortableServer::POAManagerFactory_var const poa_manager_factory
        = root_poa->the_POAManagerFactory ();

      VERIFY_CONDITION (!CORBA::is_nil (poa_manager_factory.in ()));
      if (verbose)
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                    (pretest == fail) ? ACE_TEXT ("passed") : ACE_TEXT ("failed")));
      if (CORBA::is_nil (poa_manager_factory.in ()))
        return 1;

      CORBA::PolicyList const policies;

      if (verbose)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Create a POAManager: ")));
      pretest = fail;

      // Explicitly create a POAManager - "POAManager1" .
      PortableServer::POAManager_var const poa_manager_1
        = poa_manager_factory->create_POAManager ("POAManager1",
                                                  policies);

      VERIFY_CONDITION (!CORBA::is_nil (poa_manager_1.in ()));
      if (verbose)
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                    (pretest == fail) ? ACE_TEXT ("passed") : ACE_TEXT ("failed")));
      if (CORBA::is_nil (poa_manager_1.in ()))
        return 1;

      // Creating a POAManager with an exiting POAManager name raises exception.
      if (verbose)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Prevent duplicated POAManagers: ")));
      pretest = fail;
      CORBA::Boolean got_expected_exception {};
      try
        {
          PortableServer::POAManager_var poa_manager
            = poa_manager_factory->create_POAManager ("POAManager1",
                                                      policies);
        }
      catch (PortableServer::POAManagerFactory::ManagerAlreadyExists const &)
        {
          got_expected_exception = true;
        }

      VERIFY_CONDITION (got_expected_exception);
      if (verbose)
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                    (pretest == fail) ? ACE_TEXT ("passed") : ACE_TEXT ("failed")));

      if (!got_expected_exception)
        return 1;

      {
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("Implicitly create a POAManagers: ")));
        pretest = fail;
        // Implicitly create a POAManager instance which has an automatically
        // assigned name.
        PortableServer::POA_var const child_poa =
          root_poa->create_POA ("childPOA2",
                                PortableServer::POAManager::_nil (),
                                policies);

        PortableServer::POAManager_var const poa_manager_2
          = child_poa->the_POAManager ();

        VERIFY_CONDITION (!CORBA::is_nil (poa_manager_2.in ()));
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ? ACE_TEXT ("passed") : ACE_TEXT ("failed")));

        if (CORBA::is_nil (poa_manager_2.in ()))
          return 1;

        CORBA::String_var const poa_manager_2_name
          = poa_manager_2->get_id ();
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("Implicitly created POAManager's ID: [%C]\n"),
                      poa_manager_2_name.in()));

        if (std::strlen (poa_manager_2_name.in ()) == 0)
          return 1;

        if (verbose)
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("List POAManagers: ")));
        pretest = fail;

        PortableServer::POAManagerFactory::POAManagerSeq_var managers
          = poa_manager_factory->list ();

        VERIFY_CONDITION (managers->length () == 3);
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ? ACE_TEXT ("passed") : ACE_TEXT ("failed")));

        PortableServer::POAManager_var const root_poa_manager
          = root_poa->the_POAManager ();

        for (CORBA::ULong i {}; i < managers->length (); ++i)
          {
            CORBA::String_var const name = managers[i]->get_id ();
            if (verbose)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT("Validate listed POAManager [%C]: "),
                          name.in()));
            pretest = fail;

            PortableServer::POAManager_ptr const tmp_pm = managers[i];
            if ((std::strcmp (name.in (), "RootPOAManager") == 0
                 && tmp_pm == root_poa_manager.in ())
                || (std::strcmp (name.in (), "POAManager1") == 0
                    && tmp_pm == poa_manager_1.in ())
                || (std::strcmp (name.in (), poa_manager_2_name.in ()) == 0
                    && tmp_pm == poa_manager_2.in ()))
              {
                if (verbose)
                  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("passed\n")));
                continue;
              }

            if (verbose)
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT("failed\n")));
            fail++;
          }
      }

      {
        // Find a specific POAManager.
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("Find a POAManager: ")));
        pretest = fail;

        PortableServer::POAManager_var const manager
          = poa_manager_factory->find ("POAManager1");

        CORBA::String_var const name = manager->get_id ();

        VERIFY_CONDITION ((std::strcmp (name.in (), "POAManager1") == 0
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
      PortableServer::POA_var const child_poa =
        root_poa->create_POA ("childPOA",
                              poa_manager_1.in (),
                              policies);

      poa_manager_1->activate ();

      root_poa->destroy (true, true);

      orb->destroy ();

      if (verbose)
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("passed\n")));
    }
  catch (CORBA::Exception const &ex)
    {
      fail++;
      if (verbose)
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("failed\n")));
      ex._tao_print_exception ("Exception caught");
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("POAManagerFactory %s\n"),
              (pretest == fail) ? ACE_TEXT ("succeeded") : ACE_TEXT ("failed")));

  return 0;
}
