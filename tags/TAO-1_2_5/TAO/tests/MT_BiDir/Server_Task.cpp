//
// $Id$
//

#include "Server_Task.h"
#include "Sender_i.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Cache_Manager.h"

ACE_RCSID(Muxing, Server_Task, "Server_Task.cpp,v 1.1 2001/03/10 23:37:42 coryan Exp")

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

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Before start
      CORBA::ULong connection_no =
        this->orb_->orb_core ()->lane_resources ().transport_cache ().current_size ();

      for (int i = 0; i != this->no_iterations_; ++i)
        {
          if (i%50 == 0)
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Sending message ....\n"));

          this->sender_->send_message (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Sanity check for connections..
          if (connection_no <
              this->orb_->orb_core ()->lane_resources ().transport_cache ().current_size ())
            {
              // Get the latest and the greatest..
              connection_no =
                this->orb_->orb_core ()->lane_resources ().transport_cache ().current_size ();
              ACE_ERROR ((LM_DEBUG,
                          "(%P|%t) ERROR: New connections have been created \n"));
              ACE_ERROR ((LM_DEBUG,
                          "(%P|%t) ERROR: We will crash now... \n"));
              ACE_OS::abort ();
            }

        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception in  iteration \n");
    }
  ACE_ENDTRY;

  return 0;
}
