// $Id$

#include "UDP_Client_i.h"

// This is the interface program that accesses the remote object

// Constructor.
UDP_Client_i::UDP_Client_i (CORBA::ORB_ptr orb,
                            UDP_ptr udp,
                            UDP_ptr udpHandler,
                            ACE_UINT32 msec,
                            ACE_UINT32 iterations)
: orb_ (CORBA::ORB::_duplicate (orb))
, udp_ (UDP::_duplicate (udp))
, udpHandler_ (UDP::_duplicate (udpHandler))
, delay_ (msec)
, iterations_ (iterations)
{

}

//Destructor.
UDP_Client_i::~UDP_Client_i (void)
{
  //no-op
}

int
UDP_Client_i::svc (void)
{
  ACE_CString client_name ("UDP");

  ACE_TCHAR pid[256];
  ACE_OS::sprintf (pid,
                   "%u",
                   ACE_static_cast (u_int, ACE_OS::getpid ()));
  client_name += "_";
  client_name += pid;

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::String_var corba_client_name =
        CORBA::string_dup (client_name.c_str ());

      for (ACE_UINT32 i = 0;
           i < iterations_;
           i++)
        {
          udp_->invoke (corba_client_name.in (),
                        udpHandler_.inout (),
                        i
                        ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG,
                      "invoked %s %d, going to wait %d ms\n",
                      corba_client_name.in (),
                      i,
                                                  delay_));
          ACE_Time_Value tv (0, delay_ * 1000);
          ACE_OS::sleep (tv);  // wait to not flood the server
        }

      // shut down remote ORB
      udp_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Time_Value tv (0, 500); // 50ms
      ACE_OS::sleep (tv);  // let the previous request go through

      // Shut down local ORB, trigger the end of the ORB event loop
          // in the main thread.
      orb_->shutdown ();

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "\tException");
      return -1;
    }
  ACE_ENDTRY;


  return 0;
}

