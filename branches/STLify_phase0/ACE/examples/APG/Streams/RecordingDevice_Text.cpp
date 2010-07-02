/*
 * $Id$
 *
 * A RecordingDevice that listens to a socket and collects text.
 */

#include "MessageInfo.h"
#include "RecordingDevice.h"
#include "RecordingDevice_Text.h"
#include "Util.h"

TextListenerAcceptor::TextListenerAcceptor (void)
  : ACE_Event_Handler(), RecordingDevice()
{ }

// ACE_Event_Handler interface

int TextListenerAcceptor::open (ACE_INET_Addr &addr)
{
  if (this->acceptor_.open (addr, 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("acceptor open")),
                      -1);
  return 0;
}

ACE_HANDLE TextListenerAcceptor::get_handle (void) const
{
  return this->acceptor_.get_handle ();
}

int TextListenerAcceptor::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("TextListenerAcceptor - connection request\n" )));
  return 0;
}

int TextListenerAcceptor::accept (ACE_SOCK_Stream &peer)
{
  return this->acceptor_.accept (peer);
}

// RecordingDevice interface

// Open a listening socket on the port specified by argv.
int TextListenerAcceptor::init (int argc, ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG(argc);

  ACE_INET_Addr addr (argv[1]);

  if (this->open (addr) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("TextListener - open")),
                      -1);
  return 0;
}

ACE_Event_Handler *TextListenerAcceptor::get_handler (void) const
{
  return (ACE_Event_Handler *)this;
}

RecordingDevice *TextListenerAcceptor::wait_for_activity (void)
{
  this->RecordingDevice::wait_for_activity ();
  return new TextListener (this);
}

int TextListenerAcceptor::answer_call (void)
{
  return -1;
}

CallerId *TextListenerAcceptor::retrieve_callerId (void)
{
  return 0;
}

int TextListenerAcceptor::play_message (ACE_FILE_Addr &addr)
{
  ACE_UNUSED_ARG(addr);
  return 0;
}

MessageType *TextListenerAcceptor::record_message (ACE_FILE_Addr &addr)
{
  ACE_UNUSED_ARG(addr);
  return 0;
}


// Listing 01 code/ch18
TextListener::TextListener (TextListenerAcceptor *acceptor)
  : acceptor_(acceptor)
{
  ACE_TRACE ("TextListener ctor");

  ACE_NEW (this->command_stream_, CommandStream (&(this->peer_)));
  this->command_stream_->open (0);
}
// Listing 01

const ACE_TCHAR *TextListener::get_name (void) const
{
  return ACE_TEXT ("TextListener");
}

// Listing 02 code/ch18
int TextListener::answer_call (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TextListener::answer_call()\n")));

  Command *c = new Command ();
  c->command_ = Command::CMD_ANSWER_CALL;
  c->extra_data_ = this->acceptor_;

  c = this->command_stream_->execute (c);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TextListener::answer_call() ")
              ACE_TEXT ("result is %d\n"),
              c->numeric_result_));

  return c->numeric_result_;
}
// Listing 02

// Listing 03 code/ch18
CallerId *TextListener::retrieve_callerId (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TextListener::retrieve_callerId()\n")));

  Command *c = new Command ();
  c->command_ = Command::CMD_RETRIEVE_CALLER_ID;

  c = this->command_stream_->execute (c);

  CallerId *caller_id = new CallerId (c->result_);
  return caller_id;
}
// Listing 03

// Listing 04 code/ch18
int TextListener::play_message (ACE_FILE_Addr &addr)
{
  MessageType *type = Util::identify_message (addr);
  if (type->is_text ())
    {
      Command *c = new Command ();
      c->command_ = Command::CMD_PLAY_MESSAGE;
      c->extra_data_ = &addr;
      c = this->command_stream_->execute (c);
      return c->numeric_result_;
    }

  ACE_FILE_Addr temp (ACE_TEXT ("/tmp/outgoing_message.text"));
  ACE_FILE_IO *file;
  if (type->is_audio ())
    file = Util::audio_to_text (addr, temp);
  else if (type->is_video ())
    file = Util::video_to_text (addr, temp);
  else
    ACE_ERROR_RETURN
      ((LM_ERROR, ACE_TEXT ("Invalid message type %d\n"),
        type->get_codec ()), -1);
  int rval = this->play_message (temp);
  file->remove ();
  return rval;
}
// Listing 04

// Listing 05 code/ch18
MessageType *TextListener::record_message (ACE_FILE_Addr &addr)
{
  Command *c = new Command ();
  c->command_ = Command::CMD_RECORD_MESSAGE;
  c->extra_data_ = &addr;
  c = this->command_stream_->execute (c);
  if (c->numeric_result_ == -1)
    return 0;

  return new MessageType (MessageType::RAWTEXT, addr);
}
// Listing 05

// Listing 06 code/ch18
void TextListener::release (void)
{
  delete this;
}
// Listing 06
