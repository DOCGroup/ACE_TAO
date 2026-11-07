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
#include <cstddef>

namespace
{
  constexpr std::size_t N_THREADS = 20;
  constexpr std::size_t N_ITERATIONS = 100;
}

class A_i : public virtual POA_A
{
};

class B_i : public virtual POA_C
{
public:
  explicit B_i (PortableServer::POA_ptr poa)
    : poa_ (PortableServer::POA::_duplicate (poa))
  {}

  B_i (B_i const &) = delete;
  B_i (B_i &&) = delete;
  B_i &operator= (B_i const &) = delete;
  B_i &operator= (B_i &&) = delete;

  A_ptr makeA () override
  {
    PortableServer::ServantBase_var servant = new A_i;
    PortableServer::ObjectId_var const tmp = this->poa_->activate_object (servant.in ());
    CORBA::Object_var const obj = this->poa_->servant_to_reference (servant.in ());
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
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object {orb->resolve_initial_references ("RootPOA")};
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();
      poa_manager->activate ();

      CORBA::Object_var const naming_service_object {
        orb->resolve_initial_references ("SimpleNamingService")};
      SimpleNamingService_var const naming_service {
        SimpleNamingService::_narrow (naming_service_object.in ())};

      if (CORBA::is_nil (naming_service.in ()))
        {
          ACE_DEBUG ((LM_ERROR,
                      "ERROR: Could not locate the Simple Naming Service\n"));
          return 1;
        }

      ACE_Thread_Manager orb_thread;
      orb_thread.spawn (OrbRunThread, orb.in (), THR_NEW_LWP | THR_DETACHED);

      // Setup
      PortableServer::ServantBase_var const servant {new B_i (poa.in ())};
      PortableServer::ObjectId_var const id {poa->activate_object (servant.in ())};
      CORBA::Object_var const b {poa->servant_to_reference (servant.in ())};

      naming_service->bind (b.in ());

      //Start threads
      ACE_Thread_Manager threads;
      threads.spawn_n (N_THREADS, TestThread, naming_service.in ());
      ACE_DEBUG ((LM_INFO, "All threads spawned.\n"));

      threads.wait ();

      orb->shutdown (false);
      orb_thread.wait ();
      orb->destroy ();
    } //destructor of ACE_Thread_Manager = implicit join
  catch (CORBA::Exception const &ex)
    {
      ACE_DEBUG ((LM_ERROR, "Corba Exception: %s\n", ex._info ().c_str ()));
      return 1;
    }

  return 0;
}

ACE_THR_FUNC_RETURN
OrbRunThread (void *arg)
{
  CORBA::ORB_var const orb {
    CORBA::ORB::_duplicate (reinterpret_cast<CORBA::ORB_ptr> (arg))};

  try
    {
      orb->run ();
    }
  catch (CORBA::Exception const &ex)
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
  SimpleNamingService_var const naming_service {
    SimpleNamingService::_duplicate (reinterpret_cast<SimpleNamingService_ptr> (arg))};

  try
    {
      for (std::size_t i {}; i < N_ITERATIONS; ++i)
        {
          CORBA::Object_var const obj {naming_service->resolve ()};
          C_var const b {C::_narrow (obj.in ())};
          A_var const a {b->makeA ()};
          if (i % 50 == 0)
            ACE_DEBUG ((LM_INFO, "(%t) collocated call returned\n"));
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_DEBUG ((LM_ERROR,
                  "In TestThread: Corba Exception: %s\n",
                  ex._info ().c_str ()));
    }
  return 0;
}
