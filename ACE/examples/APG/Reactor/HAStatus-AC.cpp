#include <memory>

#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/os_include/os_netdb.h"
#include "ClientService.h"

// Listing 1 code/ch07
#include "ace/Log_Msg.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor.h"
#include "ace/Acceptor.h"

static void
release_message_block (ACE_Message_Block *message_block)
{
  if (message_block != nullptr)
    message_block->release ();
}

using Message_Block_Ptr = std::unique_ptr<ACE_Message_Block,
                                          void (*) (ACE_Message_Block *)>;

using ClientAcceptor = ACE_Acceptor<ClientService, ACE_SOCK_ACCEPTOR>;
// Listing 1

// Listing 4 code/ch07
int
ClientService::open (void *p)
{
  if (super::open (p) == -1)
    return -1;

  ACE_TCHAR peer_name[MAXHOSTNAMELEN] {};
  ACE_INET_Addr peer_addr {};
  if (this->peer ().get_remote_addr (peer_addr) == 0 &&
      peer_addr.addr_to_string (peer_name, MAXHOSTNAMELEN) == 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) Connection from %s\n"),
                peer_name));
  return 0;
}
// Listing 4

// Listing 5 code/ch07
int
ClientService::handle_input (ACE_HANDLE)
{
  constexpr size_t INPUT_SIZE = 4096;
  char buffer[INPUT_SIZE] {};
  ssize_t recv_cnt {};
  ssize_t send_cnt {};

  recv_cnt = this->peer ().recv (buffer, sizeof(buffer));
  if (recv_cnt <= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Connection closed\n")));
      return -1;
    }

  send_cnt =
    this->peer ().send (buffer,
                        static_cast<size_t> (recv_cnt));
  if (send_cnt == recv_cnt)
    return 0;
  if (send_cnt == -1 && ACE_OS::last_error () != EWOULDBLOCK)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) %p\n"),
                       ACE_TEXT ("send")),
                      0);
  if (send_cnt == -1)
    send_cnt = 0;
  ACE_Message_Block *raw_message_block {};
  size_t const remaining =
    static_cast<size_t> ((recv_cnt - send_cnt));
  ACE_NEW_RETURN (raw_message_block, ACE_Message_Block (remaining), -1);
  Message_Block_Ptr message_block {raw_message_block, release_message_block};
  message_block->copy (&buffer[send_cnt], remaining);
  int const output_off = this->msg_queue ()->is_empty ();
  ACE_Time_Value nowait {ACE_OS::gettimeofday ()};
  if (this->putq (message_block.get (), &nowait) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p; discarding data\n"),
                  ACE_TEXT ("enqueue failed")));
      return 0;
    }
  message_block.release ();
  if (output_off)
    return this->reactor ()->register_handler
      (this, ACE_Event_Handler::WRITE_MASK);
  return 0;
}
// Listing 5

// Listing 6 code/ch07
int
ClientService::handle_output (ACE_HANDLE)
{
  ACE_Message_Block *raw_message_block {};
  ACE_Time_Value nowait {ACE_OS::gettimeofday ()};
  while (this->getq (raw_message_block, &nowait) != -1)
    {
      Message_Block_Ptr message_block {raw_message_block,
                                       release_message_block};
      ssize_t const send_cnt =
        this->peer ().send (message_block->rd_ptr (),
                            message_block->length ());
      if (send_cnt == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p\n"),
                    ACE_TEXT ("send")));
      else
        message_block->rd_ptr (static_cast<size_t> (send_cnt));
      if (message_block->length () > 0)
        {
          if (this->ungetq (message_block.get ()) == -1)
            return -1;
          message_block.release ();
          break;
        }
    }
  return (this->msg_queue ()->is_empty ()) ? -1 : 0;
}
// Listing 6

// Listing 7 code/ch07
int
ClientService::handle_close (ACE_HANDLE h, ACE_Reactor_Mask mask)
{
  if (mask == ACE_Event_Handler::WRITE_MASK)
    return 0;
  return super::handle_close (h, mask);
}
// Listing 7

// Listing 2 code/ch07
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_INET_Addr port_to_listen ("HAStatus");
  ClientAcceptor acceptor;
  if (acceptor.open (port_to_listen,
                     ACE_Reactor::instance (),
                     ACE_NONBLOCK) == -1)
    return 1;

  ACE_Reactor::instance ()->run_reactor_event_loop ();

  return (0);
}
// Listing 2

// Listing 8 code/ch07
// Listing 8
