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

  CORBA::ORB_var g_pOrb;
  PortableServer::POA_var g_pPoa;
  SimpleNamingService_var g_pNameService;
}

class A_i : public virtual POA_A
{
};

class B_i : public virtual POA_C
{
public:
  virtual A_ptr
  makeA ()
  {
    PortableServer::ServantBase_var servant = new A_i;
    g_pPoa->activate_object (servant.in());
    CORBA::Object_var obj = g_pPoa->servant_to_reference (servant.in());
    return A::_narrow (obj.in ());
  }
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
      ACE_Thread_Manager threads;

      // Normal corba init
      g_pOrb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var pPoaObj =
        g_pOrb->resolve_initial_references ("RootPOA");
      g_pPoa = PortableServer::POA::_narrow (pPoaObj.in ());
      PortableServer::POAManager_var pMgr = g_pPoa->the_POAManager ();
      pMgr->activate ();

      CORBA::Object_var pNSObj =
        g_pOrb->resolve_initial_references ("SimpleNamingService");
      g_pNameService = SimpleNamingService::_narrow (pNSObj.in ());

      if (CORBA::is_nil (g_pNameService.in ()))
        {
          ACE_DEBUG ((LM_ERROR,
                      "ERROR: Could not locate the Simple Naming Service\n"));
          return 1;
        }

      ACE_Thread::spawn (OrbRunThread, 0, THR_NEW_LWP | THR_DETACHED);

      // Setup
      PortableServer::ServantBase_var servant = new B_i;
      g_pPoa->activate_object (servant.in());
      CORBA::Object_var b = g_pPoa->servant_to_reference (servant.in());

      g_pNameService->bind (b.in ());

      //Start threads
      threads.spawn_n (N_THREADS, TestThread);
      ACE_DEBUG ((LM_INFO, "All threads spawned.\n"));

      threads.wait ();

    } //destructor of ACE_Thread_Manager = implicit join
  catch (CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_ERROR, "Corba Exception: %s\n", ex._info ().c_str ()));
      return 1;
    }

  g_pOrb->shutdown (0);
  g_pOrb->destroy ();

  return 0;
}

ACE_THR_FUNC_RETURN
OrbRunThread (void*)
{
  try
    {
      g_pOrb->run ();
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
TestThread (void*)
{
  try
    {
      for (size_t i (0); i < N_ITERATIONS; ++i)
        {
          CORBA::Object_var obj = g_pNameService->resolve ();
          C_var b = C::_narrow (obj.in ());
          b->makeA ();
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
