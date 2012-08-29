// $Id$

#include "test_i.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"

// Constructor
test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa,
                ACE_thread_t thrid)
  : orb_ (CORBA::ORB::_duplicate (orb))
   ,poa_ (PortableServer::POA::_duplicate (poa))
   ,thr_id_ (thrid)
{
}

void
test_i::method (void)
{
}

char *
test_i::get_string (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Upcall in process ..\n"));
  // Use portable thread IDs
  ACE_Thread_ID self_ID;
  // Thread ID from Server
  ACE_Thread_ID this_ID;
  this_ID.id(this->thr_id_);

  // Servant Thread ID same as Thread ID server, so a remote call,
  // in case of a collocation the servant runs in calling thread (Client)
  if (self_ID == this_ID)
    {
      ACE_ERROR ((LM_ERROR,
                 "(%P|%t) ERROR: A remote call has been made "
                       " exiting ..\n"));
      ACE_OS::abort ();
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) OK: A collocated invocation has been made.\n"));
    }
  return CORBA::string_dup ("Hello there!");
}

