// file      : CCF/IDL3/Token.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_TOKEN_HPP
#define CCF_IDL3_TOKEN_HPP

#include "CCF/IDL2/Token.hpp"

namespace CCF
{
  namespace IDL3
  {
    using IDL2::Token;
    using IDL2::TokenPtr;
    using IDL2::TokenList;
    using IDL2::EndOfStream;
    using IDL2::EndOfStreamPtr;
    using IDL2::Keyword;
    using IDL2::KeywordPtr;
    using IDL2::Punctuation;
    using IDL2::PunctuationPtr;
    using IDL2::Identifier;
    using IDL2::IdentifierPtr;
    using IDL2::SimpleIdentifier;
    using IDL2::SimpleIdentifierPtr;
    using IDL2::ScopedIdentifier;
    using IDL2::ScopedIdentifierPtr;
    using IDL2::StringLiteral;
    using IDL2::StringLiteralPtr;
  }
}

#endif  // CCF_IDL3_TOKEN_HPP
