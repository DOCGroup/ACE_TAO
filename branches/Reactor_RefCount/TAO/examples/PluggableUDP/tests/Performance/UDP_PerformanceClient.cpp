// $Id$

#include "ace/High_Res_Timer.h"

#include "UDP_PerformanceClient.h"

// This is the interface program that accesses the remote object

// Constructor.
UDP_PerformanceClient::UDP_PerformanceClient (CORBA::ORB_ptr orb,
                                              UDP_ptr udp,
                                              UDP_i *udpHandler,
                                              ACE_UINT32 burst_messages,
                                              ACE_UINT32 final_delta_micro_seconds)
: orb_ (CORBA::ORB::_duplicate (orb))
, udp_ (UDP::_duplicate (udp))
, udpHandler_ (udpHandler)
, last_wrong_messages_ (0)
, burst_messages_ (burst_messages)
, final_delta_micro_seconds_ (final_delta_micro_seconds)
{

}

//Destructor.
UDP_PerformanceClient::~UDP_PerformanceClient (void)
{
  //no-op
}

int
UDP_PerformanceClient::svc ()
{
  ACE_CString client_name ("UDP");

  ACE_TCHAR pid[256];
  ACE_OS::sprintf (pid,
                   "%u",
                   ACE_static_cast (u_int, ACE_OS::getpid ()));
  client_name += "_";
  client_name += pid;

  ACE_UINT32 micro_seconds = 0;
  ACE_UINT32 delta_micro_seconds = 1;

  ACE_TRY_NEW_ENV
    {
      CORBA::String_var corba_client_name =
        CORBA::string_dup (client_name.c_str ());

      UDP_var udpHandler_var = udpHandler_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      udp_->setResponseHandler (udpHandler_var.in ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_High_Res_Timer timer;
      while (1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\nTesting with %u us sleep (delta = %u us)\n",
                      micro_seconds,
                      delta_micro_seconds));

          ACE_Time_Value tv (0, micro_seconds);
          timer.start ();
          for (ACE_UINT32 j = 1;
               j <= burst_messages_;
               j++)
            {
              udp_->invoke (corba_client_name.in (),
                            j
                            ACE_ENV_ARG_PARAMETER);

              ACE_TRY_CHECK;

              if (micro_seconds)
                {
                  ACE_OS::sleep (tv);
                }
            }
           timer.stop ();

          ACE_Time_Value tv1 (1,0); // 1 s
          ACE_OS::sleep (tv1);

          ACE_UINT32 current_wrong_messages =
            udpHandler_->getWrongMessagesCount ();

          ACE_UINT32 current_message_count =
            udpHandler_->getMessagesCount ();

          // Reset expected request ID
          udp_->reset (corba_client_name.in ()
                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Give the reset a chance to propagate back to us
          ACE_OS::sleep (tv);


          if (current_wrong_messages == 0)
            {
              if (last_wrong_messages_ != 0)
                {
                  delta_micro_seconds /= 2;
                }

              if (micro_seconds >= delta_micro_seconds)
                micro_seconds -= delta_micro_seconds;
            }
          else  // current_wrong_messages != 0
            {
              if (last_wrong_messages_ != 0)
                {
                  if (delta_micro_seconds < ACE_INT32_MAX)
                    delta_micro_seconds *= 2;
                }

              if (micro_seconds < (ACE_UINT32_MAX - delta_micro_seconds))
                micro_seconds += delta_micro_seconds;
            }
          last_wrong_messages_ = current_wrong_messages;


          if (current_message_count == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "\nError: No callbacks received!\n\n"));
            }


            {
              ACE_Time_Value tv;
              timer.elapsed_time (tv);

              ACE_UINT32 calls_per_second = (1000L * burst_messages_) / tv.msec ();

              ACE_DEBUG ((LM_DEBUG,
                          "\n  Time needed %d s %d us  (%d ms) for %d messages"
                          "\n  Performance = %d asynch calls per second\n\n",
                          tv.sec (),
                          tv.usec (),
                          tv.msec (),
                          burst_messages_,
                          calls_per_second));
            }

          if (delta_micro_seconds <= final_delta_micro_seconds_
              && current_wrong_messages == 0)
            {
              break;
            }
        }

      // shut down remote ORB
      udp_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Time_Value tv (0, 50); // 50ms
      ACE_OS::sleep (tv);  // let the previous request go through

      // Shut down local ORB, trigger the end of the ORB event loop
            // in the main thread.
      orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "\tException");
      return -1;
    }
  ACE_ENDTRY;


  return 0;
}

