/* -*- C++ -*- */
/*
 * A RecordingDevice that listens to a socket and collects text.
 */

#ifndef RECORDING_DEVICE_TEXT_H
#define RECORDING_DEVICE_TEXT_H

#include "ace/FILE_IO.h"
#include "ace/FILE_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

#include "CommandStream.h"
#include "MessageInfo.h"
#include "RecordingDevice.h"

class TextListenerAcceptor :
      public ACE_Event_Handler,
      public RecordingDevice
{
public:
  TextListenerAcceptor ();

  // ACE_Event_Handler interface

  int open (ACE_INET_Addr &addr);

  ACE_HANDLE get_handle () const override;

  int handle_input (ACE_HANDLE) override;

  int accept (ACE_SOCK_Stream &peer);

  // RecordingDevice interface

  // Open a listening socket on the port specified by argv.
  int init (int argc, ACE_TCHAR *argv[]);

  ACE_Event_Handler *get_handler () const override;

  RecordingDevice *wait_for_activity () override;

  int answer_call () override;

  CallerId *retrieve_callerId () override;

  int play_message (ACE_FILE_Addr &addr) override;

  MessageType *record_message (ACE_FILE_Addr &addr) override;

private:
  ACE_SOCK_Acceptor acceptor_;
};

// Listing 01 code/ch18
class TextListener : public RecordingDevice
{
public:
  TextListener (TextListenerAcceptor *acceptor);

  const ACE_TCHAR *get_name () const override;

  int answer_call () override;

  CallerId *retrieve_callerId () override;

  int play_message (ACE_FILE_Addr &addr) override;

  MessageType *record_message (ACE_FILE_Addr &addr) override;

  void release () override;
  // Listing 01
  // Listing 02 code/ch18
private:
  CommandStream *command_stream_;
  TextListenerAcceptor *acceptor_;
  ACE_SOCK_Stream peer_;
};
// Listing 02

#endif /* RECORDING_DEVICE_TEXT_H */
