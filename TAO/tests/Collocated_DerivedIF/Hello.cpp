// $Id$

#include "Hello.h"
#include "tao/ORB_Core.h"
#include "tao/ORB_Table.h"
#include "tao/ORB_Core_Auto_Ptr.h"

HelloAgain::HelloAgain (CORBA::ORB_ptr orb,
             ACE_thread_t thrid)
 : orb_ (CORBA::ORB::_duplicate (orb))
   , thr_id_ (thrid)
{
}
char *
HelloAgain::get_string_again (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Upcall get_string_again in process ..\n"));

  /// Use portable thread IDs
  ACE_Thread_ID self_ID;
  // Thread ID from Server
  ACE_Thread_ID this_ID;
  this_ID.id(this->thr_id_);

  // Servant Thread ID same as Thread ID server, so a remote call,
  // in case of a collocation the servant runs in calling thread (Client)
  if (self_ID == this_ID)
    {
      ACE_ERROR ((LM_ERROR,"(%P|%t) ERROR: A remote call has been made \n"));
    }
  else
    {
      if (this->orb_->orb_core ()->optimize_collocation_objects () &&
            this->orb_->orb_core ()->use_global_collocation ())
        {
          ACE_DEBUG ((LM_DEBUG,
               "(%P|%t)OK: An expected collocated call has been made \n"));
        }
    }
  return CORBA::string_dup ("Bye!");
}

char *
HelloAgain::get_string (void)
{
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) HelloAgain::get_string Upcall get_string in process ..\n"));

    // Use portable thread IDs
    ACE_Thread_ID self_ID;
    // Thread ID from Server
    ACE_Thread_ID this_ID;
    this_ID.id(this->thr_id_);

    // Servant Thread ID same as Thread ID server, so a remote call,
    // in case of a collocation the servant runs in calling thread (Client)
    if (self_ID == this_ID)
      {
        ACE_ERROR ((LM_ERROR,"(%P|%t) ERROR: A remote call has been made \n"));
      }
    else
      {
        if (this->orb_->orb_core ()->optimize_collocation_objects () &&
              this->orb_->orb_core ()->use_global_collocation ())
          {
            ACE_DEBUG ((LM_DEBUG,
                 "(%P|%t) OK: An expected collocated call has been made \n"));
          }
      }
    return CORBA::string_dup ("Hello there!");
}
void
HelloAgain::shutdown (void)
{
  // Give the client thread time to return from the collocated
  // call to this method before shutting down the ORB.  We sleep
  // to avoid BAD_INV_ORDER exceptions on fast dual processor machines.
  ACE_OS::sleep (1);
  this->orb_->shutdown (0);
}

