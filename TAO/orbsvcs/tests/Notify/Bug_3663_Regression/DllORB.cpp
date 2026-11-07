#include "DllORB.h"
#include "ace/Barrier.h"
#include "ace/Arg_Shifter.h"
#include "ace/OS_NS_unistd.h"
#include "tao/TAO_Singleton_Manager.h"

DllORB::DllORB () = default;

DllORB::~DllORB () = default;

int
DllORB::init (int argc, ACE_TCHAR* argv[])
{
  int thread_count { 1 };

  try
    {
      ACE_Arg_Shifter arg_shifter (argc, argv);
      ACE_TCHAR const* current_arg {};
      while (arg_shifter.is_anything_left ())
        {
          if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT ("-NumThreads"))) != nullptr)
            {
              int const count = ACE_OS::atoi (current_arg);
              if (count >= 1)
                thread_count = count;
              arg_shifter.consume_arg ();
            }
          else
            arg_shifter.ignore_arg ();
        }

      if (this->fail_pre_post_init_ < 3)
        {
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("Pre-ORB initialization ...\n")));
          // Make sure TAO's singleton manager is initialized, and set to not
          // register itself with the ACE_Object_Manager. The Service Configurator
          // controls its lifetime, so ACE_Object_Manager must not retain dangling
          // references after this module is unloaded.
          int const register_with_object_manager = 0;
          TAO_Singleton_Manager* const singleton_manager = TAO_Singleton_Manager::instance ();
          int const result = singleton_manager->init (register_with_object_manager);

          if (result == -1)
            {
              if (this->fail_pre_post_init_ == 0)
                {
                  ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Pre-ORB initialization failed.\n")));
                  return -1;
                }
              else if (this->fail_pre_post_init_ < 2)
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
            ACE_DEBUG ((LM_INFO, ACE_TEXT ("Pre-ORB initialization done.\n")));
        }

      this->orb_ = CORBA::ORB_init (argc, argv);
      if (CORBA::is_nil (this->orb_.in ()))
        {
          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("nil ORB\n")));
          return -1;
        }

      CORBA::Object_var const poa_object =
        this->orb_->resolve_initial_references ("RootPOA");
      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ());
      if (CORBA::is_nil (this->root_poa_.in ()))
        {
          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("nil RootPOA\n")));
          return -1;
        }

      this->poa_manager_ = this->root_poa_->the_POAManager ();
      if (CORBA::is_nil (this->poa_manager_.in ()))
        {
          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("nil POAManager\n")));
          return -1;
        }

      this->poa_manager_->activate ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("DllORB::init: ");
      return -1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("ERROR: exception\n")));
      return -1;
    }

  this->barrier_ = std::make_unique<ACE_Thread_Barrier> (thread_count + 1);
  this->activate (THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED, thread_count);
  this->barrier_->wait ();
  return 0;
}

int
DllORB::fini ()
{
  try
    {
      this->poa_manager_->deactivate (1, 1);
      this->poa_manager_ = PortableServer::POAManager::_nil ();
      ACE_OS::sleep (ACE_Time_Value (0, 500));
      this->orb_->shutdown (true);
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("DllORB::fini: ");
      return -1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("ERROR: exception\n")));
      return -1;
    }

  this->wait ();
  this->barrier_.reset ();

  try
    {
      this->orb_->destroy ();
      this->orb_ = CORBA::ORB::_nil ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  if (this->fail_pre_post_init_ < 3)
    {
      TAO_Singleton_Manager* const singleton_manager = TAO_Singleton_Manager::instance ();
      int const result = singleton_manager->fini ();
      if (result == -1)
        {
          if (this->fail_pre_post_init_ == 0)
            {
              ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Post-ORB finalization failed.\n")));
              return -1;
            }
          else if (this->fail_pre_post_init_ < 2)
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
        ACE_DEBUG ((LM_INFO, ACE_TEXT ("Post-ORB finalization done.\n")));
    }

  return 0;
}

int
DllORB::svc ()
{
  this->barrier_->wait ();
  int result {};

  try
    {
      this->orb_->run ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("DllORB::svc: ");
      result = 1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("ERROR: exception\n")));
      result = 1;
    }

  return result;
}

ACE_FACTORY_DEFINE (bug3663, DllORB)
