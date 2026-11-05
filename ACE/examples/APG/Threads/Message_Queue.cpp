#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "Message_Receiver.h"
#include <cstring>

// Listing 5 code/ch12
int
HA_CommandHandler::svc ()
{
  while (true)
    {
      ACE_Message_Block *raw_message_block {};
      if (this->getq (raw_message_block) == -1)
        break;
      ACE_Message_Block_Unique_Ptr message_block {raw_message_block};
      if (message_block->msg_type () == ACE_Message_Block::MB_HANGUP)
        break;

      DeviceCommandHeader header {};
      std::memcpy (&header, message_block->rd_ptr (), sizeof (header));
      message_block->rd_ptr (sizeof (header));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Message for device #%d with ")
                  ACE_TEXT ("command payload of:\n%s"),
                  header.deviceId_, message_block->rd_ptr ()));
      this->rep_.update_device (header.deviceId_, message_block->rd_ptr ());
    }

  ACE_Reactor::instance ()->end_reactor_event_loop ();

  return 0;
}
// Listing 5

// Listing 4 code/ch12
ACE_Message_Block_Unique_Ptr
Message_Receiver::shut_down_message ()
{
  ACE_Message_Block *raw_message_block {};
  ACE_NEW_RETURN
    (raw_message_block,
     ACE_Message_Block (0, ACE_Message_Block::MB_HANGUP), nullptr);
  return ACE_Message_Block_Unique_Ptr {raw_message_block};
}
// Listing 4

int
Message_Receiver::read_header (DeviceCommandHeader &header)
{
  ssize_t const result = this->peer ().recv_n (&header, sizeof (header));
  if (result <= 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Receive failure")),
                      -1);
  return 0;
}
// Listing 3 code/ch12
int
Message_Receiver::copy_payload (ACE_Message_Block &message_block,
                                int const payload_length)
{
  ssize_t const result =
    this->peer ().recv_n (message_block.wr_ptr (), payload_length);

  if (result <= 0)
    return -1;

  message_block.wr_ptr (payload_length);
  return 0;
}
// Listing 3
// Listing 2 code/ch12
int
Message_Receiver::handle_input (ACE_HANDLE)
{
  DeviceCommandHeader dch {};
  if (this->read_header (dch) < 0)
    return -1;

  if (dch.deviceId_ < 0)
    {
      // Handle shutdown.
      ACE_Message_Block_Unique_Ptr shutdown_message {this->shut_down_message ()};
      if (!shutdown_message
          || this->handler_->putq (shutdown_message.get ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Failed to enqueue shutdown\n")),
                          -1);
      shutdown_message.release ();
      return -1;
    }

  ACE_Message_Block *raw_message_block {};
  ACE_NEW_RETURN
    (raw_message_block, ACE_Message_Block (dch.length_ + sizeof dch), -1);
  ACE_Message_Block_Unique_Ptr message_block {raw_message_block};
  // Copy the header.
  message_block->copy (reinterpret_cast<char const *> (&dch), sizeof dch);
  // Copy the payload.
  if (this->copy_payload (*message_block, dch.length_) < 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Receive failure")), -1);
  // Pass it off to the handler thread.
  if (this->handler_->putq (message_block.get ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Failed to enqueue message\n")), -1);
  message_block.release ();
  return 0;
}
// Listing 2

static void report_usage (int const argc, ACE_TCHAR *argv[])
{
  if (argc < 2)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Usage: %s port\n"), argv[0]));
      ACE_OS::exit (-1);
    }
}


class Acceptor : public ACE_Acceptor<Message_Receiver, ACE_SOCK_ACCEPTOR>
{
public:
  explicit Acceptor (HA_CommandHandler *handler) : handler_ (handler) {}
  Acceptor (Acceptor const &) = delete;
  Acceptor (Acceptor &&) = delete;
  Acceptor &operator= (Acceptor const &) = delete;
  Acceptor &operator= (Acceptor &&) = delete;

protected:
  int make_svc_handler (Message_Receiver *&mr) override
  {
    ACE_NEW_RETURN (mr, Message_Receiver (handler_), -1);
    return 0;
  }

private:
  // The command handler outlives the acceptor and its service handlers.
  HA_CommandHandler *handler_ {};
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  report_usage (argc, argv);

  u_short const port = static_cast<u_short> (ACE_OS::atoi (argv[1]));

  HA_Device_Repository rep;
  HA_CommandHandler handler (rep);
  ACE_ASSERT (handler.activate () == 0);
  //start up the handler.

  Acceptor acceptor (&handler);
  ACE_INET_Addr addr (port);
  if (acceptor.open (addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Failed to open connection")), -1);

  ACE_Reactor::instance ()->run_reactor_event_loop ();
  //run the reactive event loop

  handler.wait ();
  //reap the handler before exiting.

  return 0;
}

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */
