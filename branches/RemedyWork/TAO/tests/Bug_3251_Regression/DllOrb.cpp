// $Id$

#include "DllOrb.h"
#include "ace/Arg_Shifter.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_NS_unistd.h"
#include "tao/TAO_Singleton_Manager.h"


DllOrb::DllOrb (void)
 :
   ma_barrier_(),
   mv_orb_ (),
   mv_rootPOA_ ()
{
}


DllOrb::~DllOrb (void)
{
}


int
DllOrb::init (int argc, ACE_TCHAR *argv[])
{
  int threadCnt = 1;

  try
  {
    ACE_Arg_Shifter as (argc, argv);
    const ACE_TCHAR *currentArg = 0;
    while (as.is_anything_left ())
    {
      if (0 != (currentArg = as.get_the_parameter (ACE_TEXT ("-NumThreads"))))
      {
        int num = ACE_OS::atoi (currentArg);
        if (num >= 1)
          threadCnt = num;
        as.consume_arg ();
      }
      else
        as.ignore_arg ();
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

  ACE_auto_ptr_reset (ma_barrier_, new ACE_Thread_Barrier (threadCnt + 1));

  this->activate(
    THR_NEW_LWP|THR_JOINABLE|THR_INHERIT_SCHED,
    threadCnt
  );
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("init mp_barrier->wait() ...\n")));
  ma_barrier_->wait();
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("init mp_barrier->wait() done\n")));

  return 0;
}


int
DllOrb::fini (void)
{
  try
  {
    mv_poaManager_->deactivate (1, 1);
    mv_poaManager_ = PortableServer::POAManager::_nil ();

    // attempt to protect against sporadic BAD_INV_ORDER exceptions
    ACE_OS::sleep (ACE_Time_Value (0, 500));

    mv_orb_->shutdown (1);

    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("wait() ...\n")));
    // wait for our threads to finish
    wait();
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("wait() done\n")));

    ACE_auto_ptr_reset (ma_barrier_, static_cast<ACE_Thread_Barrier *> (0));
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

  return 0;
}


int DllOrb::svc (void)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("svc mp_barrier->wait() ...\n")));
  ma_barrier_->wait();
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("svc mp_barrier->wait() done\n")));

  try
  {
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("mv_orb->run ...\n")));
    try
    {
      mv_orb_->run();
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("mv_orb->run returned\n")));
    }
    catch(CORBA::BAD_INV_ORDER const & rc_ex)
    {
      const CORBA::ULong VMCID = rc_ex.minor() & 0xFFFFF000U;
      const CORBA::ULong minorCode = rc_ex.minor() & 0xFFFU;
      if (VMCID == CORBA::OMGVMCID && minorCode == 4)
      {
        ACE_DEBUG ((LM_INFO, ACE_TEXT ("ignored 'CORBA::BAD_INV_ORDER: ORB has shutdown.'\n")));
      }
      else
      {
        throw;
      }
    }
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("mv_orb->run done\n")));
  }
  catch(...)
  {
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("Exception\n")));
    return -1;
  }

  return 0;
} /* end of DllOrb::svc ( ) */


ACE_FACTORY_DEFINE (bug_3251, DllOrb)
