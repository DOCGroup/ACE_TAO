// file      : CCF/CompilerElements/TokenStream.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_COMPILER_ELEMENTS_TOKEN_STREAM_HPP
#define CCF_COMPILER_ELEMENTS_TOKEN_STREAM_HPP

#include <string>
#include <istream>

namespace CCF
{
  namespace CompilerElements
  {
    template <typename Token>
    class TokenStream
    {
    public:
      // (JP 06-04-06) Not required by C++ spec, but it
      // eliminates buggy GCC warnings.
      virtual ~TokenStream () {}

      virtual Token
      next () = 0;
    };

    template <>
    class TokenStream<char>
    {
    public:
      typedef
      std::char_traits<char>
      traits;

      typedef
      traits::int_type
      int_type;

      typedef
      traits::char_type
      char_type;

    public:
      virtual int_type
      next () = 0;

      static char_type
      to_char_type (int_type i)
      {
        return traits::to_char_type (i);
      }

      static int_type
      eos ()
      {
        return traits::eof ();
      }
      
      virtual ~TokenStream ()
      { 
      }
    };

    class InputStreamAdapter : public TokenStream<char>
    {
    public:
      InputStreamAdapter (std::istream& is)
          : is_ (is)
      {
      }
      
      virtual ~InputStreamAdapter ()
      {
      }
      
    public:

      virtual int_type
      next ()
      {
        return is_.get ();
      }

    private:
      std::istream& is_;
    };
  }
}

#endif // CCF_COMPILER_ELEMENTS_TOKEN_STREAM_HPP
