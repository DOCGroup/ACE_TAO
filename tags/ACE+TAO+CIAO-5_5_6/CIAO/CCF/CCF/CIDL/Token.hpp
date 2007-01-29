// file      : CCF/CIDL/Token.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CIDL_TOKEN_HPP
#define CCF_CIDL_TOKEN_HPP

#include "CCF/IDL3/Token.hpp"

namespace CCF
{
  namespace CIDL
  {
    using IDL3::Token;
    using IDL3::TokenPtr;
    using IDL3::TokenList;
    using IDL3::EndOfStream;
    using IDL3::EndOfStreamPtr;
    using IDL3::Keyword;
    using IDL3::KeywordPtr;
    using IDL3::Punctuation;
    using IDL3::PunctuationPtr;
    using IDL3::Identifier;
    using IDL3::IdentifierPtr;
    using IDL3::SimpleIdentifier;
    using IDL3::SimpleIdentifierPtr;
    using IDL3::ScopedIdentifier;
    using IDL3::ScopedIdentifierPtr;
    using IDL3::StringLiteral;
    using IDL3::StringLiteralPtr;
  }
}

#endif  // CCF_CIDL_TOKEN_HPP
