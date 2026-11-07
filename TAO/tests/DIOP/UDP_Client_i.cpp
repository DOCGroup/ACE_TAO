#include "UDP_Client_i.h"
#include "ace/OS_NS_unistd.h"
#include <string>

// This is the interface program that accesses the remote object

// Constructor.
UDP_Client_i::UDP_Client_i (CORBA::ORB_ptr orb,
                            UDP_ptr udp,
                            UDP_ptr udpHandler,
                            unsigned int msec,
                            std::size_t iterations)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , udp_ (UDP::_duplicate (udp))
  , udpHandler_ (UDP::_duplicate (udpHandler))
  , delay_ (msec)
  , iterations_ (iterations)
{
}

int
UDP_Client_i::svc ()
{
  std::string const client_name = "UDP_" + std::to_string (ACE_OS::getpid ());

  try
    {
      CORBA::String_var const corba_client_name =
        CORBA::string_dup (client_name.c_str ());

      for (std::size_t i = 0;
           i < iterations_;
           i++)
        {
          CORBA::Long const request_id = static_cast<CORBA::Long> (i);
          udp_->invoke (corba_client_name.in (),
                        udpHandler_.inout (),
                        request_id);

          ACE_DEBUG ((LM_DEBUG,
                      "invoked %C %d, going to wait %u ms\n",
                      corba_client_name.in (),
                      request_id,
                      delay_));
          ACE_Time_Value const timeout (0, delay_ * 1000);
          ACE_OS::sleep (timeout); // Avoid flooding the server.
        }

      // shut down remote ORB
      int constexpr shutdown_retries = 10;
      for (int c {}; c < shutdown_retries; ++c)
        udp_->shutdown ();

      ACE_Time_Value const timeout (0, 500); // 0.5 ms
      ACE_OS::sleep (timeout); // Let the previous request complete.

      // Shut down local ORB, trigger the end of the ORB event loop
          // in the main thread.
      orb_->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("\tException");
      return -1;
    }


  return 0;
}

