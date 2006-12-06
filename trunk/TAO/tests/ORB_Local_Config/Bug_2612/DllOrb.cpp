/*
 * @author Iliyan jeliazkov <iliyan@ociweb.com>
 * @author Lothar Werzinger <lothar@tradescape.biz>
 *
 * $Id:$
 */


#include "ace/Arg_Shifter.h"
#include "ace/SString.h"
#include "ace/OS_NS_unistd.h"
#include "tao/corba.h"
#include "tao/TAO_Singleton_Manager.h"

#include "DllOrb.h"


DllOrb::DllOrb ( )
:
  m_failPrePostInit(0),
  mp_barrier(0),
  mv_orb(),
  mv_rootPOA()
{
}

DllOrb::~DllOrb ( )
  throw ()
{
  delete mp_barrier;
}


int DllOrb::init (int argc, char *argv[])
{
  int result = 0;
  int threadCnt = 1;

  try
  {
    ACE_Arg_Shifter as(argc, argv);
    const ACE_TCHAR *currentArg = 0;
    while(as.is_anything_left())
      {
        if((currentArg = as.get_the_parameter("-t")))
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
    mv_orb = CORBA::ORB_init(argc, argv, 0);
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
  catch(...)
  {
    return -1;
  }

  mp_barrier = new ACE_Thread_Barrier(threadCnt + 1);

  this->activate(
                 THR_NEW_LWP|THR_JOINABLE|THR_INHERIT_SCHED,
                 threadCnt
                 );
  mp_barrier->wait();

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
  catch(...)
    {
      return -1;
    }


  // wait for our threads to finish
  wait();

  delete mp_barrier;
  mp_barrier = 0;

  try
    {
      mv_orb->destroy();
    mv_orb = CORBA::ORB::_nil();
    }
  catch(CORBA::Exception const & rc_ex)
    {
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
        return -1;
    } /* end of if */

  return 0;
}


int DllOrb::svc (void)
{
  mp_barrier->wait();
  try
    {
      mv_orb->run();
    }
  catch(CORBA::BAD_INV_ORDER const & rc_ex)
    {
      const CORBA::ULong VMCID = rc_ex.minor() & 0xFFFFF000U;
      const CORBA::ULong minorCode = rc_ex.minor() & 0xFFFU;
      if (VMCID != CORBA::OMGVMCID || minorCode != 4)
        return -1;
    }
  catch(...)
    {
      return -1;
    }
  return 0;
}


ACE_FACTORY_DEFINE (DllOrb, DllOrb)
