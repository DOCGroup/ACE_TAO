/**
 * $Id$
 *
 * Streams Listing 01
 *
 * An answering machine based on a one-way ACE_Stream
 */

#include "ace/OS_NS_string.h"
#include "ace/Stream.h"
#include "ace/Message_Block.h"
#include "ace/FILE_IO.h"

#include "MessageInfo.h"
#include "Message.h"
#include "BasicTask.h"
#include "EndTask.h"
#include "Util.h"
#include "RecordingDevice.h"

// Listing 21 code/ch18
class AnswerIncomingCall : public BasicTask
{
protected:
  virtual int process (Message *message)
  {
    ACE_TRACE ("AnswerIncomingCall::process()");

    if (message->recorder ()->answer_call () < 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("AnswerIncomingCall")),
                        -1);
      return 0;
  }
};
// Listing 21

// Listing 22 code/ch18
class GetCallerId : public BasicTask
{
protected:
  virtual int process (Message *message)
  {
    ACE_TRACE ("GetCallerId::process()");

    CallerId *id;
    id = message->recorder ()->retrieve_callerId ();
    if (!id)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("GetCallerId")),
                        -1);

    message->caller_id (id);
    return 0;
  }
};
// Listing 22

// Listing 23 code/ch18
class PlayOutgoingMessage : public BasicTask
{
protected:
  virtual int process (Message *message)
  {
    ACE_TRACE ("PlayOutgoingMessage::process()");

    ACE_FILE_Addr outgoing_message =
      this->get_outgoing_message (message);

    int pmrv =
      message->recorder ()->play_message (outgoing_message);
    if (pmrv < 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("PlayOutgoingMessage")),
                        -1);
    return 0;
  }

  ACE_FILE_Addr get_outgoing_message (Message *)
  {
    // Exclude 23
    return ACE_FILE_Addr (ACE_TEXT ("/tmp/outgoing_message"));
    // Exclude 23
  }
};
// Listing 23

// Listing 24 code/ch18
class RecordIncomingMessage : public BasicTask
{
protected:
  virtual int process (Message *message)
  {
    ACE_TRACE ("RecordIncomingMessage::process()");

    ACE_FILE_Addr incoming_message =
      this->get_incoming_message_queue ();

    MessageType *type =
      message->recorder ()->record_message (incoming_message);
    if (!type)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("RecordIncomingMessage")),
                        -1);
    message->incoming_message (incoming_message, type);
    return 0;
  }

  ACE_FILE_Addr get_incoming_message_queue (void)
  {
    // Exclude 24
    return ACE_FILE_Addr (ACE_TEXT ("/tmp/incoming_message"));
    // Exclude 24
  }
};
// Listing 24

// Listing 25 code/ch18
class ReleaseDevice : public BasicTask
{
protected:
  virtual int process (Message *message)
  {
    ACE_TRACE ("ReleaseDevice::process()");
    message->recorder ()->release ();
    return 0;
  }
};
// Listing 25

// Listing 26 code/ch18
class EncodeMessage : public BasicTask
{
protected:
  virtual int process (Message *message)
  {
    ACE_TRACE ("EncodeMessage::process()");

    ACE_FILE_Addr &incoming = message->addr ();
    ACE_FILE_Addr addr = this->get_message_destination (message);

    if (message->is_text ())
      Util::convert_to_unicode (incoming, addr);
    else if (message->is_audio ())
      Util::convert_to_mp3 (incoming, addr);
    else if (message->is_video ())
      Util::convert_to_mpeg (incoming, addr);

    message->addr (addr);
    return 0;
  }

  ACE_FILE_Addr get_message_destination (Message *)
  {
    // Exclude 26
    return ACE_FILE_Addr (ACE_TEXT ("/tmp/encoded_message"));
    // Exclude 26
  }
};
// Listing 26

// Listing 27 code/ch18
class SaveMetaData : public BasicTask
{
protected:
  virtual int process (Message *message)
  {
    ACE_TRACE ("SaveMetaData::process()");

    ACE_TString path (message->addr ().get_path_name ());
    path += ACE_TEXT (".xml");

    ACE_FILE_Connector connector;
    ACE_FILE_IO file;
    ACE_FILE_Addr addr (path.c_str ());
    if (connector.connect (file, addr) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("create meta-data file")),
                        0);

    file.truncate (0);
    this->write (file, "<Message>\n");
    // ...
    this->write (file, "</Message>\n");
    file.close ();
    return 0;
  }

private:
  //FUZZ: disable check_for_lack_ACE_OS
  int write (ACE_FILE_IO &file, const char *str)
  {
  //FUZZ: enable check_for_lack_ACE_OS
    return file.send (str, ACE_OS::strlen (str));
  }
};
// Listing 27

// Listing 28 code/ch18
class NotifySomeone : public BasicTask
{
protected:
  virtual int process (Message *message)
  {
    ACE_TRACE ("NotifySomeone::process()");

    // Format an email to tell someone about the
    // newly received message.
    // ...

    // Display message information in the logfile
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("New message from %s ")
                ACE_TEXT ("received and stored at %s\n"),
                message->caller_id ()->string (),
                message->addr ().get_path_name ()));
    return 0;
  }
};
// Listing 28

// Listing 10 code/ch18
class RecordingStream : public ACE_Stream<ACE_MT_SYNCH>
{
public:
  typedef ACE_Stream<ACE_MT_SYNCH> inherited;
  typedef ACE_Module<ACE_MT_SYNCH> Module;

  RecordingStream () : inherited()
  { }
// Listing 10

  //FUZZ: disable check_for_lack_ACE_OS
  // Listing 1000 code/ch18
  virtual int open (void *arg,
                    Module *head = 0, Module *tail = 0)
  {
  //FUZZ: enable check_for_lack_ACE_OS
    if (tail == 0)
      ACE_NEW_RETURN (tail,
                      Module (ACE_TEXT ("End Module"), new TheEndTask ()),
                      -1);
    this->inherited::open (arg, head, tail);
    // Listing 1000

    // Listing 1001 code/ch18
    Module *answerIncomingCallModule;
    ACE_NEW_RETURN (answerIncomingCallModule,
                    Module (ACE_TEXT ("Answer Incoming Call"),
                            new AnswerIncomingCall ()),
                    -1);

    // Listing 11 code/ch18
    Module *getCallerIdModule;
    ACE_NEW_RETURN (getCallerIdModule,
                    Module (ACE_TEXT ("Get Caller ID"), new GetCallerId ()),
                    -1);
    // Listing 11

    Module *playOGMModule;
    ACE_NEW_RETURN (playOGMModule,
                    Module (ACE_TEXT ("Play Outgoing Message"),
                            new PlayOutgoingMessage ()),
                    -1);

    Module *recordModule;
    ACE_NEW_RETURN (recordModule,
                    Module (ACE_TEXT ("Record Incoming Message"),
                            new RecordIncomingMessage ()),
                    -1);

    Module *releaseModule;
    ACE_NEW_RETURN (releaseModule,
                    Module (ACE_TEXT ("Release Device"),
                            new ReleaseDevice ()),
                    -1);

    Module *conversionModule;
    ACE_NEW_RETURN (conversionModule,
                    Module (ACE_TEXT ("Encode Message"),
                            new EncodeMessage ()),
                    -1);

    Module *saveMetaDataModule;
    ACE_NEW_RETURN (saveMetaDataModule,
                    Module (ACE_TEXT ("Save Meta-Data"),
                            new SaveMetaData ()),
                    -1);

    Module *notificationModule;
    ACE_NEW_RETURN (notificationModule,
                    Module (ACE_TEXT ("Notify Someone"),
                            new NotifySomeone ()),
                    -1);
    // Listing 1001

    // Listing 12 code/ch18
    if (this->push (notificationModule) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Failed to push %p\n"),
                         ACE_TEXT ("notificationModule")),
                        -1);
    if (this->push (saveMetaDataModule) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Failed to push %p\n"),
                         ACE_TEXT ("saveMetaDataModule")),
                        -1);
    if (this->push (conversionModule) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Failed to push %p\n"),
                         ACE_TEXT ("conversionModule")),
                        -1);
    if (this->push (releaseModule) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Failed to push %p\n"),
                         ACE_TEXT ("releaseModule")),
                        -1);
    if (this->push (recordModule) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Failed to push %p\n"),
                         ACE_TEXT ("recordModule")),
                        -1);
    if (this->push (playOGMModule) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Failed to push %p\n"),
                         ACE_TEXT ("playOGMModule")),
                        -1);
    if (this->push (getCallerIdModule) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Failed to push %p\n"),
                         ACE_TEXT ("getCallerIdModule")),
                        -1);
    if (this->push (answerIncomingCallModule) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Failed to push %p\n")
                         ACE_TEXT ("answerIncomingCallModule")),
                        -1);
    // Listing 12

    return 0;
  }

  // Listing 13 code/ch18
  int record (RecordingDevice *recorder)
  {
    ACE_Message_Block * mb = 0;
    ACE_NEW_RETURN (mb, ACE_Message_Block (sizeof(Message)), -1);

    Message *message = (Message *)mb->wr_ptr ();
    mb->wr_ptr (sizeof(Message));

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("RecordingStream::record() - ")
                ACE_TEXT ("message->recorder(recorder)\n")));
    message->recorder (recorder);

    int rval = this->put (mb);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("RecordingStream::record() - ")
                ACE_TEXT ("this->put() returns %d\n"),
                rval));
    return rval;
  }
  // Listing 13
};


// Listing 1 code/ch18
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  RecordingDevice *recorder =
    RecordingDeviceFactory::instantiate (argc, argv);
  // Listing 1

  // Listing 2 code/ch18
  RecordingStream *recording_stream;
  ACE_NEW_RETURN (recording_stream, RecordingStream, -1);

  if (recording_stream->open (0) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("RecordingStream->open()")),
                      0);
  // Listing 2

  // Listing 3 code/ch18
  for (;;)
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Waiting for incoming message\n")));
      RecordingDevice *activeRecorder =
        recorder->wait_for_activity ();

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Initiating recording process\n")));

      recording_stream->record (activeRecorder);
    }
  // Listing 3

  ACE_NOTREACHED (return 0;)
}
