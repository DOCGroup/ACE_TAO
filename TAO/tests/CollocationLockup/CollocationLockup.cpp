// $Id$

/**
 * CollocationLockup.cpp
 * This is regression test against bug #2130.
 * It tests a deadlock between a thread making a collocated invocation
 * (which holds a lock on the ORB core and attempts to lock the POA) and a
 * thread activating a servant (which locks the POA and attempts to lock the
 * ORB core).
 *
 * This test was developed from a test submitted by Rick Marlborough for bug
 * #2297 and refined by Paul Calabrese.
 */

#include "ABS.h"
#include "SimpleNamingServiceC.h"

#include "ace/Thread_Manager.h"
#include "ace/SString.h"

namespace
{
#if defined (ACE_OPENVMS)
  // less threads on OpenVMS otherwise this test
  // (although working correctly) takes far too long.
  const size_t N_THREADS = 10;
#else
  const size_t N_THREADS = 20;
#endif
  const size_t N_ITERATIONS = 100;
}

class A_i : public virtual POA_A
{
};

class B_i : public virtual POA_C
{
public:
  B_i (PortableServer::POA_ptr poa)
    : poa_ (PortableServer::POA::_duplicate (poa))
  {}

  virtual A_ptr
  makeA ()
  {
    PortableServer::ServantBase_var servant = new A_i;
    PortableServer::ObjectId_var tmp = this->poa_->activate_object (servant.in());
    CORBA::Object_var obj = this->poa_->servant_to_reference (servant.in());
    return A::_narrow (obj.in ());
  }

private:
  PortableServer::POA_var poa_;
};

// Thread for ORB->run()
ACE_THR_FUNC_RETURN OrbRunThread (void*);

// Thread to run the test
ACE_THR_FUNC_RETURN TestThread (void*);

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Normal corba init
      CORBA::ORB_var Orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var pPoaObj =
        Orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var Poa = PortableServer::POA::_narrow (pPoaObj.in ());
      PortableServer::POAManager_var pMgr = Poa->the_POAManager ();
      pMgr->activate ();

      CORBA::Object_var pNSObj =
        Orb->resolve_initial_references ("SimpleNamingService");
      SimpleNamingService_var NameService =
        SimpleNamingService::_narrow (pNSObj.in ());

      if (CORBA::is_nil (NameService.in ()))
        {
          ACE_DEBUG ((LM_ERROR,
                      "ERROR: Could not locate the Simple Naming Service\n"));
          return 1;
        }

      ACE_Thread_Manager orb_thread;
      orb_thread.spawn (OrbRunThread, Orb.in (), THR_NEW_LWP | THR_DETACHED);

      // Setup
      PortableServer::ServantBase_var servant = new B_i (Poa.in ());
      PortableServer::ObjectId_var tmp = Poa->activate_object (servant.in());
      CORBA::Object_var b = Poa->servant_to_reference (servant.in());

      NameService->bind (b.in ());

      //Start threads
      ACE_Thread_Manager threads;
      threads.spawn_n (N_THREADS, TestThread, NameService.in ());
      ACE_DEBUG ((LM_INFO, "All threads spawned.\n"));

      threads.wait ();

      Orb->shutdown (0);
      orb_thread.wait();
      Orb->destroy ();
    } //destructor of ACE_Thread_Manager = implicit join
  catch (CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_ERROR, "Corba Exception: %s\n", ex._info ().c_str ()));
      return 1;
    }

  return 0;
}

ACE_THR_FUNC_RETURN
OrbRunThread (void *arg)
{
  CORBA::ORB_var Orb =
    CORBA::ORB::_duplicate (reinterpret_cast<CORBA::ORB_ptr> (arg));

  try
    {
      Orb->run ();
    }
  catch (CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_ERROR,
                  "In OrbRunThread: Corba Exception: %s\n",
                  ex._info ().c_str ()));
    }
  return 0;
}

ACE_THR_FUNC_RETURN
TestThread (void *arg)
{
  SimpleNamingService_var NameService =
    SimpleNamingService::_duplicate (reinterpret_cast<SimpleNamingService_ptr> (arg));

  try
    {
      for (size_t i (0); i < N_ITERATIONS; ++i)
        {
          CORBA::Object_var obj = NameService->resolve ();
          C_var b = C::_narrow (obj.in ());
          A_var tmp = b->makeA ();
          if (i % 50 == 0)
            ACE_DEBUG ((LM_INFO, "(%t) collocated call returned\n"));
        }
    }
  catch (CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_ERROR,
                  "In TestThread: Corba Exception: %s\n",
                  ex._info ().c_str ()));
    }
  return 0;
}
