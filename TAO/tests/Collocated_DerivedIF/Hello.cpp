#include "Hello.h"
#include "tao/ORB_Core.h"

HelloAgain::HelloAgain (CORBA::ORB_ptr orb,
                        ACE_thread_t const thr_id)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , thr_id_ (thr_id)
{
}

char *
HelloAgain::get_string_again ()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Upcall get_string_again in process ..\n"));

  // Use portable thread IDs.
  ACE_Thread_ID const self_ID {};
  ACE_Thread_ID this_ID;
  this_ID.id (this->thr_id_);

  // A collocated servant runs in the calling thread.
  if (self_ID == this_ID)
    ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: A remote call has been made\n"));
  else if (this->orb_->orb_core ()->optimize_collocation_objects () &&
           this->orb_->orb_core ()->use_global_collocation ())
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) OK: An expected collocated call has been made\n"));

  return CORBA::string_dup ("Bye!");
}

char *
HelloAgain::get_string ()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) HelloAgain::get_string upcall in process ..\n"));

  // Use portable thread IDs.
  ACE_Thread_ID const self_ID {};
  ACE_Thread_ID this_ID;
  this_ID.id (this->thr_id_);

  // A collocated servant runs in the calling thread.
  if (self_ID == this_ID)
    ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: A remote call has been made\n"));
  else if (this->orb_->orb_core ()->optimize_collocation_objects () &&
           this->orb_->orb_core ()->use_global_collocation ())
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) OK: An expected collocated call has been made\n"));

  return CORBA::string_dup ("Hello there!");
}

void
HelloAgain::shutdown ()
{
  // Give the client time to return from this collocated call before
  // shutting down the ORB, avoiding BAD_INV_ORDER on fast systems.
  ACE_OS::sleep (1);
  this->orb_->shutdown (false);
}
