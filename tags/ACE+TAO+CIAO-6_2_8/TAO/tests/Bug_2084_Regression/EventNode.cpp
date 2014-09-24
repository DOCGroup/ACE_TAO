//
// $Id$
//
#include "EventNode.h"
#include "tid_to_int.h"
#include "tao/ORB_Core.h"
#include "tao/ORB_Table.h"
#include "tao/ORB_Core_Auto_Ptr.h"

EventNode::EventNode (CORBA::ORB_ptr orb, ACE_thread_t thrid)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , thr_id_ (thrid)
{
}

void EventNode::registerHello ( ::Test::Hello_ptr h )
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) EventNode: registerHello will call get_string...\n"));

  if (ACE_Thread::self () == this->thr_id_)
    {
      if (this->orb_->orb_core ()->optimize_collocation_objects () &&
          this->orb_->orb_core ()->use_global_collocation ())
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: A remote call has been made "
                      " exiting ..\n"));
          ACE_OS::abort ();
        }
      else if (this->orb_->orb_core ()->optimize_collocation_objects () &&
               this->orb_->orb_core ()->use_global_collocation () == 0)
        {
          TAO::ORB_Table * const orb_table =
            TAO::ORB_Table::instance ();

          TAO_ORB_Core_Auto_Ptr tmp (orb_table->find ("server_orb"));
          if (tmp.get () == 0)
            {
              // We are running on a single ORB and this is an error.
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) ERROR: A remote call has been made "
                          " with a single ORB "
                          " exiting ..\n"));
              ACE_OS::abort ();
            }
        }
    }

  CORBA::String_var str = h->get_string(
    ACE_thread_t_to_integer< ::Test::ThreadId> (ACE_Thread::self ()));
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) - EventNode: string returned <%C>\n",
              str.in ()));
}

void
EventNode::shutdown (void)
{
  this->orb_->shutdown (0);
}
