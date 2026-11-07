/* -*- C++ -*- */
#ifndef UTIL_H
#define UTIL_H

#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "MessageInfo.h"

#include <memory>
#include <new>

class Util
{
public:
  static MessageType identify_message (ACE_FILE_Addr &src)
  {
    // Determine the contents of the specified file
    return MessageType (MessageType::RAWTEXT, src);
  }

  static std::unique_ptr<ACE_FILE_IO> audio_to_text (ACE_FILE_Addr &,
                                                     ACE_FILE_Addr &dest)
  {
    ACE_FILE_Connector connector;
    std::unique_ptr<ACE_FILE_IO> file (new (std::nothrow) ACE_FILE_IO);
    if (!file)
      return {};
    if (connector.connect (*file, dest) == -1)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("create file")));
        return {};
      }

    // Convert audio data to printable text

    return file;
  }

  static std::unique_ptr<ACE_FILE_IO> video_to_text (ACE_FILE_Addr &,
                                                     ACE_FILE_Addr &dest)
  {
    ACE_FILE_Connector connector;
    std::unique_ptr<ACE_FILE_IO> file (new (std::nothrow) ACE_FILE_IO);
    if (!file)
      return {};
    if (connector.connect (*file, dest) == -1)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("create file")));
        return {};
      }

    // Extract audio data from video file and convert to printable text
    return file;
  }

  static int convert_to_unicode (ACE_FILE_Addr &src, ACE_FILE_Addr &dest)
  {
    ACE_FILE_Connector connector;
    ACE_FILE_IO input;
    if (connector.connect (input, src) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("read file")),
                        0);
    ACE_FILE_IO output;
    if (connector.connect (output, dest) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("create file")),
                        0);

    char rwbuf[512];
    ssize_t rwbytes;
    while ((rwbytes = input.recv (rwbuf, 512)) > 0)
      {
        output.send_n (rwbuf, rwbytes);
      }

    input.close ();
    output.close ();

    // Convert arbirary text to unicode
    return 0;
  }

  static int convert_to_mp3 (ACE_FILE_Addr &, ACE_FILE_Addr &)
  {
    // Convert arbitrary audio data to some standard mp3 format
    return 0;
  }

  static int convert_to_mpeg (ACE_FILE_Addr &, ACE_FILE_Addr &)
  {
    // Convert arbitrary vidio data to some standard mpeg format
    return 0;
  }
};

#endif /* UTIL_H */
