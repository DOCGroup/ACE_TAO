// file      : CCF/CompilerElements/Preprocessor.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_COMPILER_ELEMENTS_PREPROCESSOR_HPP
#define CCF_COMPILER_ELEMENTS_PREPROCESSOR_HPP

#include <memory>

#include "CCF/CompilerElements/TokenStream.hpp"
#include "CCF/CompilerElements/PreprocessorToken.hpp"

namespace CCF
{
  namespace CompilerElements
  {
    namespace CPP
    {
      class Preprocessor : public TokenStream<Token>
      {
      public:
        virtual
        ~Preprocessor ();

        Preprocessor (TokenStream<char>& is);

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
