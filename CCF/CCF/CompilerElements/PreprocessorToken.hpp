// file      : CCF/CompilerElements/PreprocessorToken.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_COMPILER_ELEMENTS_PREPROCESSOR_TOKEN_HPP
#define CCF_COMPILER_ELEMENTS_PREPROCESSOR_TOKEN_HPP

#include <string>

//@@ It is probably a good idea to move preprocessor into a
//   separate library.
//

namespace CCF
{
  namespace CompilerElements
  {
    namespace CPP
    {
      class Token
      {
      public:
        static Token const eos;

      public:
        Token (char c, unsigned long line);

        class EOS {};

        operator char () const;

        unsigned long
        line () const;

        friend bool
        operator== (Token const& a, Token const& b);

        friend bool
        operator!= (Token const& a, Token const& b);

      private:
        // Constructs eos token.
        //
        Token ();

      private:
        typedef
        std::char_traits<char>
        traits;

        traits::int_type c_;
        unsigned long line_;
      };
    }
  }
}

#endif  // CCF_COMPILER_ELEMENTS_PREPROCESSOR_TOKEN_HPP
