/**
 * Sample code from The ACE Programmer's Guide,
 * copyright 2003 Addison-Wesley. All Rights Reserved.
 */

#ifndef __MESSAGE_RECEIVER_H_
#define __MESSAGE_RECEIVER_H_

#include "ace/Log_Msg.h"
#include "ace/Message_Block.h"
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/Task.h"
#include <memory>

struct Message_Block_Deleter
{
  void operator() (ACE_Message_Block *message_block) const
  {
    message_block->release ();
  }
};

using Message_Block_Owner = std::unique_ptr<ACE_Message_Block, Message_Block_Deleter>;

// Listing 1 code/ch12
struct DeviceCommandHeader
{
  int length_ {};
  int deviceId_ {};
};
// Listing 1

class HA_Device_Repository
{
public:
  HA_Device_Repository () = default;
  HA_Device_Repository (HA_Device_Repository const &) = delete;
  HA_Device_Repository (HA_Device_Repository &&) = delete;
  HA_Device_Repository &operator= (HA_Device_Repository const &) = delete;
  HA_Device_Repository &operator= (HA_Device_Repository &&) = delete;

  int update_device (int device_id, char const *commands);
};

int
HA_Device_Repository::update_device (int, char const *)
{
  return 0;
}

class HA_CommandHandler : public ACE_Task<ACE_MT_SYNCH>
{
public:
  explicit HA_CommandHandler (HA_Device_Repository &rep) : rep_ (rep) {}
  HA_CommandHandler (HA_CommandHandler const &) = delete;
  HA_CommandHandler (HA_CommandHandler &&) = delete;
  HA_CommandHandler &operator= (HA_CommandHandler const &) = delete;
  HA_CommandHandler &operator= (HA_CommandHandler &&) = delete;

  int svc () override;

private:
  HA_Device_Repository &rep_;
};

class Message_Receiver :
  public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
public:
  // ACE_Acceptor requires a default constructor for its fallback factory path.
  Message_Receiver () = default;

  explicit Message_Receiver (HA_CommandHandler *handler) : handler_ (handler) {}
  Message_Receiver (Message_Receiver const &) = delete;
  Message_Receiver (Message_Receiver &&) = delete;
  Message_Receiver &operator= (Message_Receiver const &) = delete;
  Message_Receiver &operator= (Message_Receiver &&) = delete;

  Message_Block_Owner shut_down_message ();

  int handle_input (ACE_HANDLE) override;

  int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK) override
  {
    this->peer ().close ();
    // The ACE service-handler lifecycle owns this object and calls handle_close.
    delete this;
    return 0;
  }

private:
  int read_header (DeviceCommandHeader &header);
  int copy_payload (ACE_Message_Block &message_block, int payload_length);

private:
  // The command handler outlives the acceptor and its service handlers.
  HA_CommandHandler *handler_ {};
};

#endif /* __MESSAGE_RECEIVER_H */
