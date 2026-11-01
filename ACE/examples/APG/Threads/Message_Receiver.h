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

// Listing 1 code/ch12
struct DeviceCommandHeader
{
  int length_;
  int deviceId_;
};
// Listing 1

class HA_Device_Repository
{
public:
  HA_Device_Repository () = default;

  int update_device (int device_id, char *commands);

private:
  ACE_Task_Base *owner_ {};
};

int
HA_Device_Repository::update_device (int, char *)
{
  return 0;
}

class HA_CommandHandler : public ACE_Task<ACE_MT_SYNCH>
{
public:
  explicit HA_CommandHandler (HA_Device_Repository &rep) : rep_ (rep)
  { }

  int svc () override;

private:
  HA_Device_Repository &rep_;
};

class Message_Receiver :
  public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
public:
  Message_Receiver () : handler_ (nullptr)
  {
    ACE_ASSERT(0);
  }

  explicit Message_Receiver (HA_CommandHandler *ch) : handler_ (ch)
  { }

  ACE_Message_Block *shut_down_message ();

  int handle_input (ACE_HANDLE fd) override;

  int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK) override
  {
    this->peer ().close ();
    delete this;
    return 0;
  }

private:
  int read_header (DeviceCommandHeader *dch);
  int copy_payload (ACE_Message_Block *mb, int payload_length);

private:
  HA_CommandHandler *handler_;
};

#endif /* __MESSAGE_RECEIVER_H */
