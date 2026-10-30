#include "Time_Client_i.h"
#include "ace/OS_NS_time.h"

// This is the interface program that accesses the remote object

int
Time_Client_i::run (char const *name,
                    int argc,
                    ACE_TCHAR *argv[])
{
  // Initialize the client.
  if (client_.init (name, argc, argv) == -1)
    return -1;

  try
    {
      // 64-bit OS's require pointers to be aligned on an
      // 8 byte boundary.
      CORBA::Long padding = 0;
      time_t timedate;

      ACE_UNUSED_ARG (padding);

      //Make the RMI.
      timedate = static_cast<time_t> (client_->current_time ());

      // Print out value
      ACE_TCHAR ascii_timedate[64] = ACE_TEXT ("");

      ACE_OS::ctime_r (&timedate, ascii_timedate, 64);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("string time is %s\n"),
                  ascii_timedate));

      if (client_.do_shutdown ())
        client_->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("\tException");
      return -1;
    }

  return 0;
}
