// file      : CCF/CodeGenerationKit/IndentationImplanter.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CODE_GENERATION_KIT_INDENTATION_IMPLANTER
#define CCF_CODE_GENERATION_KIT_INDENTATION_IMPLANTER

#include <ostream>

#include "CCF/CodeGenerationKit/IndentationBuffer.hpp"

namespace Indentation
{
  template <typename C>
  class ToStreamBufAdapter : public std::basic_streambuf<C>
  {
  public:
    typedef
    typename std::basic_streambuf<C>::traits_type
    traits_type;

    typedef
    typename std::basic_streambuf<C>::char_type
    char_type;

    typedef
    typename std::basic_streambuf<C>::int_type
    int_type;

  public:
    ToStreamBufAdapter (Buffer<C>& b)
        : buffer_ (b)
    {
    }

    virtual int_type
    overflow (int_type c)
    {
      return buffer_.put (traits_type::to_char_type (c));
    }

    virtual int
    sync ()
    {
      return 0;
    }

  private:
    Buffer<C>& buffer_;
  };

  template <typename C>
  class FromStreamBufAdapter : public Buffer<C>
  {
  public:
    typedef
    typename Buffer<C>::traits_type
    traits_type;

    typedef
    typename Buffer<C>::char_type
    char_type;

    typedef
    typename Buffer<C>::int_type
    int_type;

    typedef
    typename Buffer<C>::Exception
    Exception;

  public:
    FromStreamBufAdapter (std::basic_streambuf<C>& b)
        : buffer_ (b)
    {
    }

    virtual int_type
    put (char_type c)
    {
      return buffer_.sputc (c);
    }

    virtual void
    unbuffer ()
    {
      try
      {
        if (buffer_.pubsync () == 0) return;
      }
      catch (std::ios_base::failure const&)
      {
      }

      throw Exception ("underlying std::streambuf::sync failed");
    }

  private:
    std::basic_streambuf<C>& buffer_;
  };

  template <template <typename> class BufferType, typename C = char>
  class Implanter
  {
  public:
    Implanter (std::basic_ostream<C>& os)
        : os_ (os),
          prev_ (os_.rdbuf ()),
          from_adapter_ (*prev_),
          buffer_ (from_adapter_),
          to_adapter_ (buffer_)
    {
      os_.rdbuf (&to_adapter_);
    }

    template <typename Arg0>
    Implanter (std::basic_ostream<C>& os, Arg0 a0)
        : os_ (os),
          prev_ (os_.rdbuf ()),
          from_adapter_ (*prev_),
          buffer_ (from_adapter_, a0),
          to_adapter_ (buffer_)
    {
      os_.rdbuf (&to_adapter_);
    }

    ~Implanter ()
    {
      try
      {
        buffer_.unbuffer ();
      }
      catch (...)
      {
        // there is nothing I can do...
      }

      os_.rdbuf (prev_);
    }

  private:
    std::basic_ostream<C>& os_;
    std::basic_streambuf<C>* prev_;

    FromStreamBufAdapter<C> from_adapter_;

    BufferType<C> buffer_;

    ToStreamBufAdapter<C> to_adapter_;
  };
}

#endif // CCF_CODE_GENERATION_KIT_INDENTATION_IMPLANTER
