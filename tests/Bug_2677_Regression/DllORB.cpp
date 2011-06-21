// $Id$
#include "DllORB.h"
#include "ace/Arg_Shifter.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_NS_unistd.h"
#include "tao/TAO_Singleton_Manager.h"

DllORB::DllORB (void)
 : failPrePostInit_ (3),
   mv_orb_ (),
   mv_rootPOA_ ()
{
}

DllORB::~DllORB (void)
{
}

int
DllORB::init (int argc, ACE_TCHAR *argv[])
{
  try
    {
      if (failPrePostInit_ < 3)
        {
          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("Pre-ORB initialization ...\n")));
          // -----------------------------------------------------------------
          // Pre-ORB initialization steps necessary for proper DLL ORB
          // support.
          // -----------------------------------------------------------------
          // Make sure TAO's singleton manager is initialized, and set to not
          // register itself with the ACE_Object_Manager since it is under the
          // control of the Service Configurator.  If we register with the
          // ACE_Object_Manager, then the ACE_Object_Manager will still hold
          // (dangling) references to instances of objects created by this
          // module and destroyed by this object when it is dynamically
          // unloaded.
          int register_with_object_manager = 0;
          TAO_Singleton_Manager * p_tsm = TAO_Singleton_Manager::instance ();
          int result = p_tsm->init (register_with_object_manager);

          if (result == -1)
            {
              if (failPrePostInit_ == 0)
                {
                  ACE_DEBUG ((LM_ERROR,
                              ACE_TEXT ("Pre-ORB initialization failed.\n")));
                  return -1;
                }
              else if (failPrePostInit_ < 2)
                {
                  ACE_DEBUG ((LM_WARNING,
                              ACE_TEXT ("Pre-ORB initialization failed (ignored due to FailPrePostInit setting).\n")));
                }
              else
                {
                  ACE_DEBUG ((LM_INFO,
                              ACE_TEXT ("Pre-ORB initialization failed (ignored due to FailPrePostInit setting).\n")));
                }
            }
          else
            {
              ACE_DEBUG ((LM_INFO,
                          ACE_TEXT ("Pre-ORB initialization done.\n")));
            }
        }

      // Initialize the ORB
      mv_orb_ = CORBA::ORB_init (argc, argv);
      if (CORBA::is_nil (mv_orb_.in ()))
        {
          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("nil ORB\n")));
          return -1;
        }

      CORBA::Object_var v_poa =
        mv_orb_->resolve_initial_references ("RootPOA");

      mv_rootPOA_ = PortableServer::POA::_narrow (v_poa.in ());
      if (CORBA::is_nil (mv_rootPOA_.in ()))
        {
          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("nil RootPOA\n")));
          return -1;
        }

      mv_poaManager_ = mv_rootPOA_->the_POAManager ();
      if (CORBA::is_nil (mv_poaManager_.in ()))
        {
          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("nil POAManager\n")));
          return -1;
        }

      mv_poaManager_->activate ();
    }
  catch (...)
  {
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("ERROR: exception\n")));
    return -1;
  }

  return 0;
}


int
DllORB::fini (void)
{
  try
    {
      mv_poaManager_ = PortableServer::POAManager::_nil ();

      // attempt to protect against sporadic BAD_INV_ORDER exceptions
      ACE_OS::sleep (ACE_Time_Value (0, 500));

      mv_orb_->shutdown (1);
    }
  catch (...)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("ERROR: exception\n")));
      return -1;
    }

  try
    {
      mv_orb_->destroy ();
      mv_orb_ = CORBA::ORB::_nil ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  if (failPrePostInit_ < 3)
    {
      // -----------------------------------------------------------------
      // Post-ORB finalization steps necessary for proper DLL ORB
      // support.
      // -----------------------------------------------------------------
      // Explicitly clean up singletons created by TAO before
      // unloading this module.
      TAO_Singleton_Manager * p_tsm = TAO_Singleton_Manager::instance ();
      int result = p_tsm->fini ();
      if (result == -1)
        {
          if (failPrePostInit_ == 0)
            {
              ACE_DEBUG ((LM_ERROR,
                          ACE_TEXT ("Post-ORB finalization failed.\n")));
              return -1;
            }
          else if (failPrePostInit_ < 2)
            {
              ACE_DEBUG ((LM_WARNING,
                          ACE_TEXT ("Post-ORB finalization failed (ignored due to FailPrePostInit setting).\n")));
            }
          else
            {
              ACE_DEBUG ((LM_INFO,
                          ACE_TEXT ("Post-ORB finalization failed (ignored due to FailPrePostInit setting).\n")));
            }
        }
      else
        {
          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("Post-ORB finalization done.\n")));
        }
    }

  return 0;
}


ACE_FACTORY_DEFINE (bug_2677_regression, DllORB)
