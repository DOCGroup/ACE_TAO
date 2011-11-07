
//=============================================================================
/**
 *  @file     ORB_destroy.cpp
 *
 *  $Id$
 *
 *   Simple ORB destruction test.
 *
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"

#include "ace/Log_Msg.h"

int
test_with_regular_poa_manager (int argc,
                               ACE_TCHAR **argv,
                               const char *orb_name,
                               int destroy_orb,
                               int destroy_poa)
{

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, orb_name);

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      if (destroy_poa)
        {
          root_poa->destroy (1, 1);
        }

      if (destroy_orb)
        {
          orb->destroy ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception raised");
    }

  return 0;
}

int
test_with_funky_poa_manager (int argc,
                             ACE_TCHAR **argv,
                             const char *orb_name,
                             int destroy_orb,
                             int destroy_poa,
                             int funky_poa_manager)
{

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, orb_name);

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      if (funky_poa_manager)
        {
          PortableServer::POAManager_var poa_manager =
            root_poa->the_POAManager ();

          poa_manager->activate ();
        }

      if (destroy_poa)
        {
          root_poa->destroy (1, 1);
        }

      if (destroy_orb)
        {
          orb->destroy ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception raised");
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;

  result = test_with_regular_poa_manager (argc, argv,
                                          "destroy_nothing_with_poa_manager",
                                          0, 0);
  ACE_ASSERT (result == 0);

  result = test_with_regular_poa_manager (argc, argv,
                                          "destroy_poa_with_poa_manager",
                                          0, 1);
  ACE_ASSERT (result == 0);

  result = test_with_regular_poa_manager (argc, argv,
                                          "destroy_orb_with_poa_manager",
                                          1, 0);
  ACE_ASSERT (result == 0);

  result = test_with_regular_poa_manager (argc, argv,
                                          "destroy_poa_and_orb_with_poa_manager",
                                          1, 1);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_nothing_without_poa_manager",
                                          0, 0, 0);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_poa_without_poa_manager",
                                          0, 1, 0);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_orb_without_poa_manager",
                                          1, 0, 0);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_poa_and_orb_without_poa_manager",
                                          1, 1, 0);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_nothing_with_funky_poa_manager",
                                          0, 0, 1);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_poa_with_funky_poa_manager",
                                          0, 1, 1);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_orb_with_funky_poa_manager",
                                          1, 0, 1);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_poa_and_orb_with_funky_poa_manager",
                                          1, 1, 1);
  ACE_ASSERT (result == 0);

  return result;
}
