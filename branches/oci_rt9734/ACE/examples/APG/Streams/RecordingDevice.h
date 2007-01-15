/* -*- C++ -*- */
// $Id$

#ifndef RECORDING_DEVICE_H
#define RECORDING_DEVICE_H

#include "ace/FILE_Addr.h"
#include "ace/Event_Handler.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "ace/Semaphore.h"

class CallerId;
class MessageType;

class RecordingDevice
{
public:
  RecordingDevice ()
  {
    // Initialize the semaphore so that we don't block on the
    // first call to wait_for_activity().
  }

  virtual ~RecordingDevice ()
  {
  }

  virtual const ACE_TCHAR *get_name (void) const
  {
    return ACE_TEXT ("UNKNOWN");
  }

  virtual int init (int, ACE_TCHAR *[])
  {
    return 0;
  }

  // Answer the incoming call
  virtual int answer_call (void) = 0;

  // Fetch some form of caller identification at the hardware level.
  virtual CallerId *retrieve_callerId (void) = 0;

  // Fetch the message at the location specified by 'addr' and play
  // it for the caller.
  virtual int play_message (ACE_FILE_Addr &addr) = 0;

  // Record data from our physical device into the file at the
  // specified address. Return the number of bytes recorded.
  virtual MessageType *record_message (ACE_FILE_Addr &addr) = 0;

  // Release the RecordingDevice to accept another incoming call
  virtual void release (void)
  {
    this->release_semaphore ();
  }

  // Get the handler of the device so that wait_for_activity() can
  // wait for data to arrive.
  virtual ACE_Event_Handler *get_handler (void) const
  {
    return 0;
  }

  virtual RecordingDevice *wait_for_activity (void)
  {
    // Block on a semaphore until it says we're ready to do
    // work.
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Waiting for semaphore\n")));
    this->acquire_semaphore ();

    // Use the reactor to wait for activity on our handle
    ACE_Reactor reactor;

    ACE_Event_Handler *handler = this->get_handler ();
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Handler is %@\n"),
                (void *)handler));

    reactor.register_handler (this->get_handler (),
                              ACE_Event_Handler::READ_MASK);

    reactor.handle_events ();
    // Error-check this...

    // Leave the semaphore locked so that we'll block until
    // recording_complete() is invoked.

    return this;
  }

protected:
  void acquire_semaphore (void)
  {
    this->semaphore_.acquire ();
  }

  void release_semaphore (void)
  {
    // Reset the semaphore so that wait_for_activity() will
    // unblock.
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Releasing semaphore\n")));
    this->semaphore_.release ();
  }

private:
  ACE_Semaphore semaphore_;
};

#include "RecordingDevice_Text.h"
#include "RecordingDevice_USRVM.h"
#include "RecordingDevice_QC.h"

#include "RecordingDeviceFactory.h"

#endif /* RECORDING_DEVICE_H */
