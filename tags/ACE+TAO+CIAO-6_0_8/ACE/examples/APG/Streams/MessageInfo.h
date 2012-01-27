/* -*- C++ -*- */
// $Id$

#ifndef MESSAGE_INFO_H
#define MESSAGE_INFO_H

#include "ace/FILE_Addr.h"
#include "ace/SString.h"

/* Opaque class that represents a caller's ID */
class CallerId
{
public:
  CallerId () : id_ (ACE_TEXT ("UNKNOWN"))
  { }

  CallerId (ACE_TString id) : id_(id)
  { }

  const ACE_TCHAR * string(void)
  {
    return this->id_.c_str ();
  }

private:
  ACE_TString id_;
};

class MessageType
{
public:
  enum {
    // Known video codecs
    FIRST_VIDEO_CODEC = 1,

    DIVX,
    // ...
    LAST_VIDEO_CODEC,

    // Known audio codecs
    FIRST_AUDIO_CODEC,

    MP3,
    RAWPCM,
    // ...
    LAST_AUDIO_CODEC,

    // Known text codecs
    FIRST_TEXT_CODEC,

    RAWTEXT,
    XML,

    // ...
    LAST_TEXT_CODEC,

    LAST_CODEC
  };

  MessageType (int codec, const ACE_FILE_Addr& addr)
    : codec_(codec), addr_(addr)
  { }

  int get_codec (void)
  {
    return this->codec_;
  }

  ACE_FILE_Addr &get_addr (void)
  {
    return this->addr_;
  }

  int is_video (void)
  {
    return
      this->get_codec () > FIRST_VIDEO_CODEC &&
      this->get_codec () < LAST_VIDEO_CODEC;
  }

  int is_audio (void)
  {
    return
      this->get_codec () > FIRST_AUDIO_CODEC &&
      this->get_codec () < LAST_AUDIO_CODEC ;
  }

  int is_text (void)
  {
    return
      this->get_codec () > FIRST_TEXT_CODEC &&
      this->get_codec () < LAST_TEXT_CODEC ;
  }

private:
  int codec_;
  ACE_FILE_Addr addr_;
};

# endif /* MESSAGE_INFO_H */
