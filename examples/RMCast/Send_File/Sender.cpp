// $Id$

#include "ace/RMCast/RMCast_UDP_Reliable_Sender.h"
#include "ace/INET_Addr.h"
#include "ace/FILE_IO.h"
#include "ace/Message_Block.h"
#include "ace/Reactor.h"

ACE_RCSID(tests, RMCast_Examples_Sender, "$Id$")

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc != 3)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Usage: %s <filename> <mcastgroup:port>\n",
                         argv[0]),
                        1);
    }

  const ACE_TCHAR *filename = argv[1];
  if (ACE_OS::access (filename, R_OK) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot read file <%s>\n", filename),
                        1);
    }

  ACE_INET_Addr mcast_group;
  if (mcast_group.set (argv[2]) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot read file <%s>\n", filename),
                        1);
    }


  ACE_HANDLE handle = ACE_OS::open (filename, O_RDONLY|O_BINARY);
  if (handle == ACE_INVALID_HANDLE)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open file <%s> %p\n", filename, ""),
                        1);
    }
  ACE_FILE_IO file_io;
  file_io.set_handle (handle);

  // We don't provide a module to receive the control messages, in
  // this example we simply ignore them.
  ACE_RMCast_UDP_Reliable_Sender sender (0);

  if (sender.init (mcast_group) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot init UDP I/O at <%s:%d> %p\n",
                         mcast_group.get_host_name (),
                         mcast_group.get_port_number (),
                         ""),
                        1);
    }

  // Use the Reactor to demultiplex all the messages
  ACE_Reactor *reactor = ACE_Reactor::instance ();

  sender.reactive_incoming_messages (reactor);
  {
    // Resend the messages every 20 milliseconds..
    ACE_Time_Value tv (2, 0);
    sender.reactive_resends (reactor, tv);
  }

  for (;;)
    {
      ACE_Message_Block payload (BUFSIZ + 1);

      ssize_t r = file_io.recv (payload.rd_ptr () + 1, BUFSIZ);
      if (r <= 0)
        break;

      payload.wr_ptr (r + 1);
      *(payload.rd_ptr ()) = 'N'; // Normal
      if (r < BUFSIZ)
        {
          *(payload.rd_ptr ()) = 'E'; // EOF
        }

      ACE_RMCast::Data data;
      data.payload = &payload;
      if (sender.data (data) != 0)
        break;

      if (r < BUFSIZ)
        {
          // Last buffer, terminate loop
          break;
        }

      // Handle incoming events, without blocking...
      ACE_Time_Value tv (4, 0);
      reactor->handle_events (&tv);
    }

  // Wait until all the messages are successfully delivered
  do
    {
      // Try for 50 milliseconds...
      ACE_Time_Value tv (5, 0);
      int r = reactor->handle_events (&tv);
      if (r == -1)
        break;
    }
  while (sender.has_data () || sender.has_members ());

  return 0;
}
