/**
 *  @file    Codecs_Test.cpp
 *
 *  Checks the functionality of the ACE Codecs class.
 *
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 */

#include "test_config.h"
#include "ace/Codecs.h"
#include "ace/ACE.h"
#include <cerrno>
#include <limits>

#if defined (ACE_HAS_THREADS)
# include <atomic>
# include <cstring>
# include <thread>
#endif

// Don't change the strings thinking that they are typos

const ACE_Byte normal_stream[] = "This is a sample test stream, to test simple Base64 encoding";

const ACE_Byte one_padded_stream[] = "This stream is different from the above in that, it results in one padding character to be adde";

const ACE_Byte two_padded_stream[] = "This stream is different from the above in that, it results in two padding characters to be addedddd";

int
encode_decode_stream (ACE_Byte const * stream, size_t length)
{
  size_t encode_len = 0;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Input stream = %C\n"),
              stream));

  ACE_Byte* encodeBuf = ACE_Base64::encode (stream, length,
                                            &encode_len);
  if (encodeBuf == nullptr)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error in encoding stream\n")));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Base64 encoded stream = %C\n"),
              encodeBuf));

  ACE_Base64::Buffer const cleanup_encodeBuf (encodeBuf);

  size_t decode_len = 0;
  ACE_Byte* decodeBuf = ACE_Base64::decode (encodeBuf, &decode_len);

  if (decodeBuf == nullptr)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Error in decoding encoded stream\n")));
      return -1;
    }

  ACE_Base64::Buffer const cleanup_decodeBuf (decodeBuf);

  if (decode_len != length)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Decoded stream has an unexpected length\n")));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Decoded Base64 encoded stream = %C\n"),
              decodeBuf));

  for (size_t i = 0; i < length; ++i)
    {
      if (decodeBuf[i] != stream[i])
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Encoded->Decoded stream differs from original stream\n")));
          return -1;
        }
    }
  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST (ACE_TEXT ("Codecs_Test"));
  int status = 0;

  size_t overflow_length {};
  ACE_Byte const input[] = "x";
  errno = 0;
  ACE_Base64::Buffer const overflow (ACE_Base64::encode (
    input, (std::numeric_limits<size_t>::max) (), std::addressof (overflow_length)));
  if (overflow || errno != ENOMEM)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Base64 input length overflow was not rejected\n")));
      ACE_END_TEST;
      return 1;
    }
  errno = 0;
  size_t const large_input = ((std::numeric_limits<size_t>::max) () / 4) * 3;
  ACE_Base64::Buffer const line_overflow (ACE_Base64::encode (
    input, large_input, std::addressof (overflow_length)));
  if (line_overflow || errno != ENOMEM)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Base64 line length overflow was not rejected\n")));
      ACE_END_TEST;
      return 1;
    }

#if defined (ACE_HAS_THREADS)
  std::atomic<bool> start {false};
  std::atomic<bool> failed {false};
  std::thread workers[8];
  for (std::thread &worker : workers)
    {
      worker = std::thread ([&start, &failed] ()
        {
          while (!start.load ())
            {
              std::this_thread::yield ();
            }
          ACE_Byte const input[] = "YWJj";
          for (int i {}; i < 100; ++i)
            {
              size_t length {};
              ACE_Base64::Buffer decoded (ACE_Base64::decode (input, std::addressof (length)));
              if (!decoded || length != 3 || std::memcmp (decoded.get (), "abc", 3) != 0)
                {
                  failed = true;
                  break;
                }
            }
        });
    }
  start = true;
  for (std::thread &worker : workers)
    {
      worker.join ();
    }
  if (failed)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Concurrent Base64 decoding failed\n")));
      ACE_END_TEST;
      return 1;
    }
#endif

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This is ACE Version %u.%u.%u\n\n"),
              ACE::major_version (),
              ACE::minor_version(),
              ACE::micro_version()));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing ACE Base64 - normal stream\n\n")));

  status = encode_decode_stream (normal_stream, sizeof (normal_stream) - 1);

  if (status == 0) {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Testing ACE Base64 - one padded stream\n\n")));
    status = encode_decode_stream (one_padded_stream,
                                   sizeof (one_padded_stream) - 1);
  }
  if (status == 0) {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Testing ACE Base64 - two padded stream\n\n")));
    status = encode_decode_stream (two_padded_stream,
                                   sizeof (two_padded_stream) - 1);
  }
  ACE_END_TEST;
  return status;
}

