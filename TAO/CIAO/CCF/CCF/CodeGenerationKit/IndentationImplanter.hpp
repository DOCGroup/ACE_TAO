// file      : CCF/CodeGenerationKit/IndentationImplanter.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CODE_GENERATION_KIT_INDENTATION_IMPLANTER
#define CCF_CODE_GENERATION_KIT_INDENTATION_IMPLANTER

#include <ostream>

#include "CCF/CodeGenerationKit/IndentationBuffer.hpp"

namespace Indentation
{
  class ToStreamBufAdapter : public std::streambuf
  {
  public:
    ToStreamBufAdapter (Buffer& buffer)
        : buffer_ (buffer)
    {
    }

    virtual int_type
    overflow (int_type c)
    {
      return buffer_.put (Buffer::traits_type::to_char_type (c));
    }

    virtual int
    sync ()
    {
      return 0;
    }

  private:
    Buffer& buffer_;
  };

  class FromStreamBufAdapter : public Buffer
  {
  public:
    FromStreamBufAdapter (std::streambuf& buffer)
        : buffer_ (buffer)
    {
    }

    virtual int_type
    put (char_type c) throw (Exception, ExH::System::Exception)
    {
      return buffer_.sputc (traits_type::to_int_type (c));
    }

    virtual void
    unbuffer () throw (ExH::System::Exception)
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
    std::streambuf& buffer_;
  };

  template <typename Buffer>
  class Implanter
  {
  public:
    Implanter (std::ostream& os)
        : os_ (os),
          prev_ (os_.rdbuf ()),
          from_adapter_ (*prev_),
          buffer_ (from_adapter_),
          to_adapter_ (buffer_)
    {
      os_.rdbuf (&to_adapter_);
    }

    template <typename Arg0>
    Implanter (std::ostream& os, Arg0 a0)
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
    std::ostream& os_;
    std::streambuf* prev_;

    FromStreamBufAdapter from_adapter_;

    Buffer buffer_;

    ToStreamBufAdapter to_adapter_;
  };
}

#endif // CCF_CODE_GENERATION_KIT_INDENTATION_IMPLANTER
