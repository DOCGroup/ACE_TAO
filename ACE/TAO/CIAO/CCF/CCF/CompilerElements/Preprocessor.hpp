// file      : CCF/CompilerElements/Preprocessor.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_COMPILER_ELEMENTS_PREPROCESSOR_HPP
#define CCF_COMPILER_ELEMENTS_PREPROCESSOR_HPP

#include <memory>
#include <string>
#include <set>

#include "CCF/CompilerElements/TokenStream.hpp"
#include "CCF/CompilerElements/PreprocessorToken.hpp"

namespace CCF
{
  namespace CompilerElements
  {
    namespace CPP
    {
      typedef
      std::set<std::string>
      Symbols;

      class Preprocessor : public TokenStream<Token>
      {
      public:
        virtual
        ~Preprocessor ();

        Preprocessor (TokenStream<char>& is, Symbols const&);

        virtual Token
        next ();

      private:
        class PreprocessorImpl;

        std::auto_ptr<PreprocessorImpl> impl_;
      };
    }
  }
}

#endif  // CCF_COMPILER_ELEMENTS_PREPROCESSOR_HPP
