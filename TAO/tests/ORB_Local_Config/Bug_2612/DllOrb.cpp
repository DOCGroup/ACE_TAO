/*
 * @author Iliyan jeliazkov <iliyan@ociweb.com>
 * @author Lothar Werzinger <lothar@tradescape.biz>
 *
 * $Id$
 */


#include "ace/Arg_Shifter.h"
#include "ace/SString.h"
#include "ace/OS_NS_unistd.h"

#include "tao/corba.h"
#include "tao/TAO_Singleton_Manager.h"

#include "DllOrb.h"


DllOrb::DllOrb (int nthreads)
:
  m_nthreads_ (nthreads),
  m_failPrePostInit(0),
#if defined (ACE_HAS_THREADS)
  mp_barrier(0),
#endif
  mv_orb(),
  mv_rootPOA()
{
}

DllOrb::~DllOrb ( )
  throw ()
{
#if defined (ACE_HAS_THREADS)
  delete mp_barrier;
#endif
}


int DllOrb::init (int argc, char *argv[])
{
  int result = 0;
  int threadCnt = this->m_nthreads_;

  try
  {
    ACE_Arg_Shifter as(argc, argv);
    const ACE_TCHAR *currentArg = 0;
    while(as.is_anything_left())
      {
        if(0 != (currentArg = as.get_the_parameter("-t")))
          {
            int num = ACE_OS::atoi(currentArg);
            if(num >= 1)
              threadCnt = num;
            as.consume_arg();
          }
        else
          as.ignore_arg();
      }

    if (m_failPrePostInit < 3)
      {
        ACE_DEBUG((LM_DEBUG, "TEST (%P|%t) Pre-ORB initialization ...\n"));

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
        TAO_Singleton_Manager * p_tsm = TAO_Singleton_Manager::instance();
        result = p_tsm->init(register_with_object_manager);

        if (result == -1 && m_failPrePostInit == 0)
          return -1;
      }

    // Initialize the ORB
    mv_orb = CORBA::ORB_init(argc, argv);
    if (CORBA::is_nil(mv_orb.in()))
      return -1;

    CORBA::Object_var v_poa = mv_orb->resolve_initial_references("RootPOA");

    mv_rootPOA = PortableServer::POA::_narrow(v_poa.in ());
    if (CORBA::is_nil(mv_rootPOA.in()))
      return -1;

    mv_poaManager = mv_rootPOA->the_POAManager();
    if (CORBA::is_nil(mv_poaManager.in()))
      return -1;

    mv_poaManager->activate();
  }
  catch(CORBA::Exception& ex)
  {
    ex._tao_print_exception (ACE_TEXT ("(%P|%t) init failed:"));
    return -1;
  }
  catch(...)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) init failed\n")),
                      -1);
  }

#if defined (ACE_HAS_THREADS)
  mp_barrier = new ACE_Thread_Barrier(threadCnt + 1);

  this->activate(
                 THR_NEW_LWP|THR_JOINABLE|THR_INHERIT_SCHED,
                 threadCnt
                 );
  mp_barrier->wait();
#endif

  return 0;
}


int DllOrb::fini (void)
{
  int result;

  try
    {
#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) && !defined (TAO_HAS_MINIMUM_POA)
      mv_poaManager->deactivate(1, 1);
#endif
      mv_poaManager = PortableServer::POAManager::_nil();

      // attempt to protect against sporadic BAD_INV_ORDER exceptions
      ACE_OS::sleep(ACE_Time_Value(0, 500));

      mv_orb->shutdown(1);
    }
  catch(CORBA::Exception& ex)
  {
    ex._tao_print_exception (ACE_TEXT ("(%P|%t) fini failed:"));
    return -1;
  }
  catch(...)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) fini failed to deactivate/shutdown\n")),
                        -1);
    }


  // wait for our threads to finish
  this->wait();

#if defined (ACE_HAS_THREADS)
  delete mp_barrier;
  mp_barrier = 0;
#endif

  try
    {
      mv_orb->destroy();
      mv_orb = CORBA::ORB::_nil();
    }
  catch(CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT (
        "(%P|%t) init failed to destroy the orb:"));
    return -1;
  }

  if (m_failPrePostInit < 3)
    {
      ACE_DEBUG((LM_DEBUG, "TEST (%P|%t) Post-ORB finalization ...\n"));

      // -----------------------------------------------------------------
      // Post-ORB finalization steps necessary for proper DLL ORB
      // support.
      // -----------------------------------------------------------------
      // Explicitly clean up singletons created by TAO before
      // unloading this module.
      TAO_Singleton_Manager * p_tsm = TAO_Singleton_Manager::instance();
      result = p_tsm->fini();
      if (result == -1 && m_failPrePostInit == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) fini failed to destroy TAO_Singleton_Manager\n")),
                          -1);
    } /* end of if */

  return 0;
}


int DllOrb::svc (void)
{
#if defined (ACE_HAS_THREADS)
  mp_barrier->wait();
#endif

  try
    {
      mv_orb->run();
    }
  catch(CORBA::BAD_INV_ORDER const & rc_ex)
    {
      const CORBA::ULong VMCID = rc_ex.minor() & 0xFFFFF000U;
      const CORBA::ULong minorCode = rc_ex.minor() & 0xFFFU;
      if (VMCID != CORBA::OMGVMCID || minorCode != 4)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) svc exits (-1)\n")),
                          -1);
    }
  catch(CORBA::Exception& ex)
  {
    ex._tao_print_exception (ACE_TEXT ("(%P|%t) svc - orb->run() failed:"));
    return -1;
  }
  catch(...)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) svc got some exception\n")),
                        -1);
    }
  return 0;
}


ACE_FACTORY_DEFINE (DllOrb, DllOrb)
