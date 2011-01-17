// $Id$
#include "DllORB.h"
#include "ace/Barrier.h"
#include "ace/Arg_Shifter.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_NS_unistd.h"
#include "tao/TAO_Singleton_Manager.h"


DllORB::DllORB ()
 : failPrePostInit_ (3),
   mp_barrier_ (0),
   mv_orb_ (),
   mv_rootPOA_ ()
{
}

DllORB::~DllORB ()
{
  delete mp_barrier_;
}

int
DllORB::init (int argc, ACE_TCHAR *argv[])
{
  int threadCnt = 1;

  try
    {
      ACE_Arg_Shifter as (argc, argv);
      const ACE_TCHAR *currentArg = 0;
      while (as.is_anything_left ())
        {
          if ((currentArg = as.get_the_parameter (ACE_TEXT ("-NumThreads"))))
            {
              int num = ACE_OS::atoi (currentArg);
              if (num >= 1)
                threadCnt = num;
              as.consume_arg ();
            }
        else
          as.ignore_arg ();
        }

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

  mp_barrier_ = new ACE_Thread_Barrier (threadCnt + 1);

  this->activate (THR_NEW_LWP|THR_JOINABLE|THR_INHERIT_SCHED, threadCnt);
  mp_barrier_->wait ();

  return 0;
}


int
DllORB::fini ()
{
  try
    {
      mv_poaManager_->deactivate (1, 1);
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

  // wait for our threads to finish
  this->wait ();

  delete mp_barrier_;
  mp_barrier_ = 0;

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


int DllORB::svc ()
{
  mp_barrier_->wait ();

  int result = 0;

  try
    {
      mv_orb_->run ();
    }
  catch (...)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("ERROR: exception\n")));
      result = 1;
    }

  return result;
}


ACE_FACTORY_DEFINE (bug3646d, DllORB)
