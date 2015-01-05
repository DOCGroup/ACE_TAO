/*
** $Id$
**
** Example program from The ACE Programmer's Guide, Chapter 8.
** Copyright 2003 Addison-Wesley. All Rights Reserved.
*/

#include "HA_Proactive_Status.h"
#include "ace/Log_Msg.h"
#include "ace/Message_Block.h"
#include "ace/Proactor.h"
#include "ace/os_include/arpa/os_inet.h"

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)

// Listing 1 code/ch08
void
HA_Proactive_Service::open (ACE_HANDLE h, ACE_Message_Block&)
{
  this->handle (h);
  if (this->reader_.open (*this) != 0 ||
      this->writer_.open (*this) != 0   )
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
                  ACE_TEXT ("HA_Proactive_Service open")));
      delete this;
      return;
    }

  ACE_Message_Block *mb = 0;
  ACE_NEW_NORETURN (mb, ACE_Message_Block (1024));
  if (this->reader_.read (*mb, mb->space ()) != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
                  ACE_TEXT ("HA_Proactive_Service begin read")));
      mb->release ();
      delete this;
      return;
    }

  // mb is now controlled by Proactor framework.
  return;
}
// Listing 1

// Listing 2 code/ch08
void
HA_Proactive_Service::handle_read_stream
  (const ACE_Asynch_Read_Stream::Result &result)
{
  ACE_Message_Block &mb = result.message_block ();
  if (!result.success () || result.bytes_transferred () == 0)
    {
      mb.release ();
      delete this;
    }
  else
    {
      if (this->writer_.write (mb, mb.length ()) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("starting write")));
          mb.release ();
        }
      else
        {
          ACE_Message_Block *new_mb;
          ACE_NEW_NORETURN (new_mb, ACE_Message_Block (1024));
          this->reader_.read (*new_mb, new_mb->space ());
        }
    }
  return;
}
// Listing 2

// Listing 3 code/ch08
void
HA_Proactive_Service::handle_write_stream
(const ACE_Asynch_Write_Stream::Result &result)
{
  result.message_block ().release ();
  return;
}
// Listing 3

// The network address check only works for BSD-ish systems. This
// sort of network number accessor should be added to ACE_INET_Addr
// at some point...
#if defined (ACE_WIN32)
int
HA_Proactive_Acceptor::validate_connection
(const ACE_Asynch_Accept::Result&,
 const ACE_INET_Addr&,
 const ACE_INET_Addr&)
{
  return 0;
}
#else

// Listing 4 code/ch08
int
HA_Proactive_Acceptor::validate_connection (
   const ACE_Asynch_Accept::Result&,
   const ACE_INET_Addr& remote,
   const ACE_INET_Addr& local)
{
  struct in_addr *remote_addr =
    reinterpret_cast<struct in_addr*> (remote.get_addr ());
  struct in_addr *local_addr =
    reinterpret_cast<struct in_addr*> (local.get_addr ());
  if (inet_netof (*local_addr) == inet_netof (*remote_addr))
    return 0;

  return -1;
}
// Listing 4

#endif /* ACE_WIN32 */

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Listing 5 code/ch08
  ACE_INET_Addr listen_addr;     // Set up with listen port
  HA_Proactive_Acceptor aio_acceptor;
  if (0 != aio_acceptor.open (listen_addr,
                              0,     // bytes_to_read
                              0,     // pass_addresses
                              ACE_DEFAULT_ASYNCH_BACKLOG,
                              1,     // reuse_addr
                              0,     // proactor
                              1))    // validate_new_connection
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("acceptor open")), 1);
  // Listing 5

#if 0
  // Listing 6 code/ch08
  ACE_INET_Addr peer_addr;    // Set up peer addr
  ACE_Asynch_Connector<HA_Proactive_Service> aio_connect;
  aio_connect.connect (peer_addr);
  // Listing 6
#endif

  // Listing 7 code/ch08
  ACE_Proactor::instance ()->proactor_run_event_loop ();
  // Listing 7
  return 0;
}

#else

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This example requires asynchronous I/O support.\n")));
  return 1;
}

#endif /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */

