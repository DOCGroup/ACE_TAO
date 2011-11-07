//
// $Id$
//

#include "Server_Task.h"
#include "Sender_i.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Cache_Manager.h"
#include "ace/Manual_Event.h"

Server_Task::Server_Task (Sender_i *sender,
                          ACE_Manual_Event &event,
                          int no_iterations,
                          CORBA::ORB_ptr orb,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , sender_ (sender)
  , event_ (event)
  , no_iterations_ (no_iterations)
  , orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Server_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Wait for the Event\n"));

  /// Wait on the event
  /// Yes, wait on event twice. One for making sure that we have got
  /// all the clients  come and register with us.
  this->event_.wait ();



  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Start doing some work....\n"));


  try
    {
      // Before start
      CORBA::ULong connection_no =
        this->orb_->orb_core ()->lane_resources ().transport_cache ().current_size ();

      for (int i = 0; i != this->no_iterations_; ++i)
        {
          if (i%50 == 0)
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Sending message ....\n"));

          this->sender_->send_message ();

          // Sanity check for connections..
          if (connection_no <
              this->orb_->orb_core ()->lane_resources ().transport_cache ().current_size ())
            {
              // Get the latest and the greatest..
              connection_no =
                this->orb_->orb_core ()->lane_resources ().transport_cache ().current_size ();
              ACE_ERROR ((LM_DEBUG,
                          "(%P|%t) ERROR: New connections have been created\n"));
              ACE_ERROR ((LM_DEBUG,
                          "(%P|%t) ERROR: We will crash now...\n"));
              ACE_OS::abort ();
            }

        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception in iteration.\n");
    }

  return 0;
}
