// $Id$

#include "ace/High_Res_Timer.h"
#include "ace/SString.h"
#include "ace/OS_NS_unistd.h"

#include "UDP_PerformanceClient.h"

// This is the interface program that accesses the remote object

// Constructor.
UDP_PerformanceClient::UDP_PerformanceClient (CORBA::ORB_ptr orb,
                                              UDP_ptr udp,
                                              UDP_i *udpHandler,
                                              ACE_UINT32 burst_messages)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , udp_ (UDP::_duplicate (udp))
  , udpHandler_ (udpHandler)
  , last_wrong_messages_ (0)
  , burst_messages_ (burst_messages)
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

  char pid[256];
  ACE_OS::sprintf (pid,
                   "%u",
                   static_cast<u_int> (ACE_OS::getpid ()));
  client_name += "_";
  client_name += pid;

  ACE_UINT32 micro_seconds = 0;
  ACE_UINT32 delta_micro_seconds = 1;

  try
    {
      CORBA::String_var corba_client_name =
        CORBA::string_dup (client_name.c_str ());

      UDP_var udpHandler_var = udpHandler_->_this ();

      udp_->setResponseHandler (udpHandler_var.in ());

      ACE_High_Res_Timer timer;
      while (1)
        {
          //FUZZ: disable check_for_lack_ACE_OS
          ACE_DEBUG ((LM_DEBUG,
                      "\nTesting with %u us sleep (delta = %u us)\n",
                      micro_seconds,
                      delta_micro_seconds));
          //FUZZ: enable check_for_lack_ACE_OS

          ACE_Time_Value tv (0, micro_seconds);
          timer.start ();
          for (ACE_UINT32 j = 1;
               j <= burst_messages_;
               j++)
            {
              udp_->invoke (corba_client_name.in (),
                            j);

              if (micro_seconds)
                {
                  ACE_OS::sleep (tv);
                }
            }
          timer.stop ();

          ACE_Time_Value tv1 (1, 0); // 1 s
          ACE_OS::sleep (tv1);

          ACE_UINT32 current_wrong_messages =
            udpHandler_->getWrongMessagesCount ();

          ACE_UINT32 current_message_count =
            udpHandler_->getMessagesCount ();

          // Reset expected request ID
          udp_->reset (corba_client_name.in ());

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

            time_t elapsed_sec = tv.sec ();
            suseconds_t elapsed_usec = tv.usec ();
            unsigned long elapsed_msec = tv.msec ();

            ACE_UINT32 calls_per_second = (1000L * burst_messages_) / tv.msec ();

            ACE_DEBUG ((LM_DEBUG,
                        "\n  Time needed %: s %d us  (%u ms) for %d messages"
                        "\n  Performance = %d asynch calls per second\n\n",
                        elapsed_sec,
                        elapsed_usec,
                        elapsed_msec,
                        burst_messages_,
                        calls_per_second));
          }

          if (current_wrong_messages == 0)
            break;
        }

      // shut down remote ORB
      for (int c = 0; c < 10; ++c)
        udp_->shutdown ();

      ACE_Time_Value tv (0, 50); // 50ms
      ACE_OS::sleep (tv);  // let the previous request go through

      // Shut down local ORB, trigger the end of the ORB event loop
      // in the main thread.
      orb_->shutdown (0);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("\tException");
      return -1;
    }

  return 0;
}

