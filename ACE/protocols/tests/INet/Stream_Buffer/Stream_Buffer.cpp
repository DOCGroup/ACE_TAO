/// @file Stream_Buffer.cpp

#include "ace/INet/BufferedStreamBuffer.h"
#include "ace/INet/BidirStreamBuffer.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include <ios>
#include <memory>
#include <stdexcept>

class Test_Handler
{
public:
  void add_reference () { ++references_; }
  void remove_reference () { --references_; }
  int read_from_stream (void *buffer, size_t length, size_t)
  {
    if (length == 0)
      {
        return 0;
      }
    *static_cast<char *> (buffer) = 'r';
    return 1;
  }
  int write_to_stream (void const *buffer, size_t length, size_t)
  {
    if (fail_write_)
      {
        return -1;
      }
    if (length > 0)
      {
        written_ = *static_cast<char const *> (buffer);
      }
    return static_cast<int> (length);
  }
  int references_ {};
  char written_ {};
  bool fail_write_ {};
};

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  Test_Handler handler;
  std::streamsize const invalid_sizes[] = {-1, 0, 1, 4};
  for (std::streamsize const size : invalid_sizes)
    {
      bool rejected {};
      try
        {
          ACE::IOS::BufferedStreamBuffer buffer (size, std::ios::in);
        }
      catch (std::invalid_argument const &)
        {
          rejected = true;
        }
      if (!rejected)
        {
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Invalid buffered stream size accepted\n")), 1);
        }

      rejected = false;
      try
        {
          ACE::IOS::BasicBidirStreamBuffer<char, Test_Handler> buffer (
            std::addressof (handler), size, std::ios::in | std::ios::out);
        }
      catch (std::invalid_argument const &)
        {
          rejected = true;
        }
      if (!rejected || handler.references_ != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Invalid bidirectional stream size accepted\n")), 1);
        }
    }

  {
    ACE::IOS::BufferedStreamBuffer input (5, std::ios::in);
    ACE::IOS::BufferedStreamBuffer output (5, std::ios::out);
    ACE::IOS::BasicBidirStreamBuffer<char, Test_Handler> bidirectional (
      std::addressof (handler), 5, std::ios::in | std::ios::out);
    if (handler.references_ != 1)
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Stream handler was not retained\n")), 1);
      }
    if (output.overflow (std::char_traits<char>::eof ()) == std::char_traits<char>::eof ()
        || bidirectional.sputc ('w') == std::char_traits<char>::eof ()
        || bidirectional.overflow (std::char_traits<char>::eof ()) == std::char_traits<char>::eof ()
        || handler.written_ != 'w'
        || bidirectional.sgetc () != 'r')
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Minimum-size stream buffer failed\n")), 1);
      }
    handler.fail_write_ = true;
    if (bidirectional.sputc ('x') == std::char_traits<char>::eof ()
        || bidirectional.overflow (std::char_traits<char>::eof ()) != std::char_traits<char>::eof ())
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Stream buffer failed to report a write error\n")), 1);
      }
  }
  if (handler.references_ != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Stream handler was not released\n")), 1);
    }
  return 0;
}
