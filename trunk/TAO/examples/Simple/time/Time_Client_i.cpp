//$Id$

#include "Time_Client_i.h"
#include "ace/OS_NS_time.h"

// This is the interface program that accesses the remote object

// Constructor.
Time_Client_i::Time_Client_i (void)
{
  //no-op
}

//Destructor.
Time_Client_i::~Time_Client_i (void)
{
  //no-op
}

int
Time_Client_i::run (const char *name,
                    int argc,
                    ACE_TCHAR *argv[])
{
  // Initialize the client.
  if (client_.init (name, argc, argv) == -1)
    return -1;

  try
    {
      // 64-bit OS's require pointers to be aligned on an
      // 8 byte boundary.  64-bit HP-UX requires a double to do this
      // while a long does it for 64-bit Solaris.
#if defined (HPUX)
      CORBA::Double padding = 0.0;
#else
      CORBA::Long padding = 0;
#endif /* HPUX */
      time_t timedate;

      ACE_UNUSED_ARG (padding);

      //Make the RMI.
      timedate = static_cast <time_t> (client_->current_time ());

      // Print out value
      // Use ACE_OS::ctime_r(), ctime() doesn't seem to work properly
      // under 64-bit solaris.
      ACE_TCHAR ascii_timedate[64] = ACE_TEXT ("");

      ACE_OS::ctime_r (&timedate, ascii_timedate, 64);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("string time is %s\n"),
                  ascii_timedate));

      if (client_.do_shutdown () == 1)
        client_->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("\tException");
      return -1;
    }

  return 0;
}
