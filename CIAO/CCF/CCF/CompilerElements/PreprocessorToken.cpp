// file      : CCF/CompilerElements/PreprocessorToken.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CompilerElements/PreprocessorToken.hpp"

namespace CCF
{
  namespace CompilerElements
  {
    namespace CPP
    {
      Token const Token::eos;

      Token::
      Token (char c, unsigned long line)
          : c_ (traits::to_int_type (c)), line_ (line)
      {
      }

      Token::
      Token ()
          : c_ (traits::eof ()), line_ (0)
      {
      }

      Token::
      operator char () const
      {
        if (*this == eos) throw EOS ();

        return traits::to_char_type (c_);
      }


      unsigned long Token::
      line () const 
      {
        if (*this == eos) throw EOS ();

        return line_;
      }

      bool
      operator== (Token const& a, Token const& b)
      {
        return a.c_ == b.c_;
      }

      bool
      operator!= (Token const& a, Token const& b)
      {
        return a.c_ != b.c_;
      }
    }
  }
}
