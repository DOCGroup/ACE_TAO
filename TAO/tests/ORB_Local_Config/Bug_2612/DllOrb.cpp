/// @author Iliyan Jeliazkov <iliyan@ociweb.com>
/// @author Lothar Werzinger <lothar@tradescape.biz>

#include "ace/Arg_Shifter.h"
#include "ace/OS_NS_unistd.h"

#include "tao/corba.h"
#include "tao/TAO_Singleton_Manager.h"

#include "DllOrb.h"

DllOrb::DllOrb (int nthreads)
  : m_nthreads_ (nthreads)
{
}

DllOrb::~DllOrb () noexcept = default;

int DllOrb::init (int argc, ACE_TCHAR *argv[])
{
  int thread_count = this->m_nthreads_;

  try
  {
    ACE_Arg_Shifter as (argc, argv);
    ACE_TCHAR const *current_arg {};
    while (as.is_anything_left ())
      {
        if ((current_arg = as.get_the_parameter (ACE_TEXT ("-t"))) != nullptr)
          {
            int const num = ACE_OS::atoi (current_arg);
            if (num >= 1)
              thread_count = num;
            as.consume_arg ();
          }
        else
          as.ignore_arg ();
      }

    ACE_DEBUG ((LM_DEBUG, "TEST (%P|%t) Pre-ORB initialization ...\n"));

    // Make sure TAO's singleton manager is initialized, and set to not
    // register itself with the ACE_Object_Manager since it is under the
    // control of the Service Configurator. If we register with the
    // ACE_Object_Manager, it will retain references to objects destroyed
    // when this module is dynamically unloaded.
    int const register_with_object_manager {};
    TAO_Singleton_Manager * const tsm = TAO_Singleton_Manager::instance ();
    int const result = tsm->init (register_with_object_manager);
    if (result == -1)
      return -1;

    // Initialize the ORB.
    this->mv_orb_ = CORBA::ORB_init (argc, argv);
    if (CORBA::is_nil (this->mv_orb_.in ()))
      return -1;

    CORBA::Object_var const poa_object (
      this->mv_orb_->resolve_initial_references ("RootPOA"));

    this->mv_root_poa_ = PortableServer::POA::_narrow (poa_object.in ());
    if (CORBA::is_nil (this->mv_root_poa_.in ()))
      return -1;

    this->mv_poa_manager_ = this->mv_root_poa_->the_POAManager ();
    if (CORBA::is_nil (this->mv_poa_manager_.in ()))
      return -1;

    this->mv_poa_manager_->activate ();
  }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("(%P|%t) init failed:"));
      return -1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) init failed\n")),
                        -1);
    }

#if defined (ACE_HAS_THREADS)
  this->mp_barrier_ = std::make_unique<ACE_Thread_Barrier> (thread_count + 1);

  this->activate (THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED,
                  thread_count);
  this->mp_barrier_->wait ();
#endif

  return 0;
}

int DllOrb::fini ()
{
  try
    {
#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) && !defined (TAO_HAS_MINIMUM_POA)
      this->mv_poa_manager_->deactivate (true, true);
#endif
      this->mv_poa_manager_ = PortableServer::POAManager::_nil ();

      // Attempt to protect against sporadic BAD_INV_ORDER exceptions.
      ACE_OS::sleep (ACE_Time_Value (0, 500));

      this->mv_orb_->shutdown (true);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("(%P|%t) fini failed:"));
      return -1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) fini failed to deactivate/shutdown\n")),
                        -1);
    }

  // Wait for our threads to finish.
  this->wait ();

#if defined (ACE_HAS_THREADS)
  this->mp_barrier_.reset ();
#endif

  try
    {
      this->mv_orb_->destroy ();
      this->mv_orb_ = CORBA::ORB::_nil ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("(%P|%t) init failed to destroy the orb:"));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, "TEST (%P|%t) Post-ORB finalization ...\n"));

  // Explicitly clean up singletons created by TAO before unloading this
  // module.
  TAO_Singleton_Manager * const tsm = TAO_Singleton_Manager::instance ();
  int const result = tsm->fini ();
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) fini failed to destroy TAO_Singleton_Manager\n")),
                      -1);

  return 0;
}

int DllOrb::svc ()
{
#if defined (ACE_HAS_THREADS)
  this->mp_barrier_->wait ();
#endif

  try
    {
      this->mv_orb_->run ();
    }
  catch (CORBA::BAD_INV_ORDER const &ex)
    {
      CORBA::ULong const vmcid = ex.minor () & 0xFFFFF000U;
      CORBA::ULong const minor_code = ex.minor () & 0xFFFU;
      if (vmcid != CORBA::OMGVMCID || minor_code != 4)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) svc exits (-1)\n")),
                          -1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("(%P|%t) svc - orb->run() failed:"));
      return -1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) svc got some exception\n")),
                        -1);
    }
  return 0;
}

ACE_FACTORY_DEFINE (DllOrb, DllOrb)
