/// @file Stream_Buffer.cpp

#include "ace/INet/BufferedStreamBuffer.h"
#include "ace/INet/BidirStreamBuffer.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include <ios>
#include <memory>
#include <stdexcept>
#include <string>

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
    if (length > max_write_)
      {
        length = max_write_;
      }
    if (length > 0)
      {
        written_ = *static_cast<char const *> (buffer);
        written_data_.append (static_cast<char const *> (buffer), length);
      }
    return static_cast<int> (length);
  }
  int references_ {};
  char written_ {};
  bool fail_write_ {};
  size_t max_write_ {1024};
  std::string written_data_;
};

class Test_Buffer : public ACE::IOS::BufferedStreamBuffer
{
public:
  explicit Test_Buffer (Test_Handler &handler)
    : ACE::IOS::BufferedStreamBuffer (8, std::ios::out), handler_ (handler)
  {
  }

  int write_to_stream (char const *buffer, std::streamsize length) override
  {
    return handler_.write_to_stream (buffer, static_cast<size_t> (length), 1);
  }

private:
  Test_Handler &handler_;
};

#if defined (ACE_HAS_WCHAR)
class Wide_Handler
{
public:
  void add_reference () { ++references_; }
  void remove_reference () { --references_; }
  int read_from_stream (void *, size_t, size_t) { return 0; }
  int write_to_stream (void const *buffer, size_t length, size_t char_size)
  {
    if (char_size != sizeof (wchar_t))
      {
        return -1;
      }
    if (length > 2)
      {
        length = 2;
      }
    written_.append (static_cast<wchar_t const *> (buffer), length);
    return static_cast<int> (length);
  }
  int references_ {};
  std::wstring written_;
};

class Wide_Buffer : public ACE::IOS::BasicBufferedStreamBuffer<wchar_t>
{
public:
  explicit Wide_Buffer (Wide_Handler &handler)
    : ACE::IOS::BasicBufferedStreamBuffer<wchar_t> (8, std::ios::out), handler_ (handler)
  {
  }

  int write_to_stream (wchar_t const *buffer, std::streamsize length) override
  {
    return handler_.write_to_stream (buffer, static_cast<size_t> (length), sizeof (wchar_t));
  }

private:
  Wide_Handler &handler_;
};
#endif

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

  handler.fail_write_ = false;
  handler.max_write_ = 2;
  handler.written_data_.clear ();
  {
    Test_Buffer output (handler);
    if (output.sputn ("abcd", 4) != 4 || output.pubsync () != -1
        || output.pubsync () != 0 || handler.written_data_ != "abcd")
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Buffered stream lost or repeated data after a short write\n")), 1);
      }
  }
  handler.written_data_.clear ();
  {
    ACE::IOS::BasicBidirStreamBuffer<char, Test_Handler> output (
      std::addressof (handler), 8, std::ios::out);
    if (output.sputn ("abcd", 4) != 4 || output.pubsync () != -1
        || output.pubsync () != 0 || handler.written_data_ != "abcd")
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Bidirectional stream lost or repeated data after a short write\n")), 1);
      }
  }
  handler.max_write_ = 1024;
  handler.written_data_.clear ();
  {
    Test_Buffer output (handler);
    if (output.sputn ("abcdefg", 7) != 7)
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Could not fill buffered stream\n")), 1);
      }
    handler.fail_write_ = true;
    if (output.overflow ('x') != std::char_traits<char>::eof ()
        || output.overflow ('y') != std::char_traits<char>::eof ())
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Buffered stream failed to report write errors\n")), 1);
      }
    handler.fail_write_ = false;
    if (output.pubsync () != 0 || handler.written_data_ != "abcdefgx")
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Buffered stream lost data after a failed flush\n")), 1);
      }
  }
  handler.written_data_.clear ();
  {
    ACE::IOS::BasicBidirStreamBuffer<char, Test_Handler> output (
      std::addressof (handler), 8, std::ios::out);
    if (output.sputn ("abcdefg", 7) != 7)
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Could not fill bidirectional stream\n")), 1);
      }
    handler.fail_write_ = true;
    if (output.overflow ('x') != std::char_traits<char>::eof ()
        || output.overflow ('y') != std::char_traits<char>::eof ())
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Bidirectional stream failed to report write errors\n")), 1);
      }
    handler.fail_write_ = false;
    if (output.pubsync () != 0 || handler.written_data_ != "abcdefgx")
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Bidirectional stream lost data after a failed flush\n")), 1);
      }
  }
#if defined (ACE_HAS_WCHAR)
  Wide_Handler wide_handler;
  {
    Wide_Buffer output (wide_handler);
    if (output.sputn (L"abcd", 4) != 4 || output.pubsync () != -1
        || output.pubsync () != 0 || wide_handler.written_ != L"abcd")
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Wide buffered stream repeated data after a short write\n")), 1);
      }
  }
  wide_handler.written_.clear ();
  {
    ACE::IOS::BasicBidirStreamBuffer<wchar_t, Wide_Handler> output (
      std::addressof (wide_handler), 8, std::ios::out);
    if (output.sputn (L"abcd", 4) != 4 || output.pubsync () != -1
        || output.pubsync () != 0 || wide_handler.written_ != L"abcd")
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Wide bidirectional stream repeated data after a short write\n")), 1);
      }
  }
  if (wide_handler.references_ != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Wide stream handler was not released\n")), 1);
    }
#endif
  return 0;
}
