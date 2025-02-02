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

  ACE_HANDLE get_handle () const;

  int handle_input (ACE_HANDLE);

  int accept (ACE_SOCK_Stream &peer);

  // RecordingDevice interface

  // Open a listening socket on the port specified by argv.
  int init (int argc, ACE_TCHAR *argv[]);

  ACE_Event_Handler *get_handler () const;

  virtual RecordingDevice *wait_for_activity ();

  virtual int answer_call ();

  virtual CallerId *retrieve_callerId ();

  virtual int play_message (ACE_FILE_Addr &addr);

  virtual MessageType *record_message (ACE_FILE_Addr &addr);

private:
  ACE_SOCK_Acceptor acceptor_;
};

// Listing 01 code/ch18
class TextListener : public RecordingDevice
{
public:
  TextListener (TextListenerAcceptor *acceptor);

  virtual const ACE_TCHAR *get_name () const;

  int answer_call ();

  CallerId *retrieve_callerId ();

  int play_message (ACE_FILE_Addr &addr);

  MessageType *record_message (ACE_FILE_Addr &addr);

  virtual void release ();
  // Listing 01
  // Listing 02 code/ch18
private:
  CommandStream *command_stream_;
  TextListenerAcceptor *acceptor_;
  ACE_SOCK_Stream peer_;
};
// Listing 02

#endif /* RECORDING_DEVICE_TEXT_H */
