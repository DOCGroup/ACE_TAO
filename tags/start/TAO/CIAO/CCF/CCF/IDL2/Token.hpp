// file      : CCF/IDL2/Token.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL2_TOKEN_HPP
#define CCF_IDL2_TOKEN_HPP

#include <vector>
#include <string>
#include <ostream>

#include "CCF/CompilerElements/ReferenceCounting.hpp"

namespace CCF
{
  namespace IDL2
  {
    //
    //
    //
    class Token : public ReferenceCounting::DefaultImpl <>
    {
    protected:
      Token (std::string lexeme, unsigned long line)
          : lexeme_ (lexeme),
            line_ (line)
      {
      }

    public:
      virtual ~Token () throw () {}

    public:
      std::string
      lexeme () const
      {
        return lexeme_;
      }

      unsigned long
      line () const
      {
        return line_;
      }

    private:
      std::string lexeme_;
      unsigned long line_;
    };

    typedef
    ReferenceCounting::StrictPtr<Token>
    TokenPtr;

    typedef
    std::vector<TokenPtr>
    TokenList;

    inline std::ostream&
    operator<< (std::ostream& o, TokenPtr const& t)
    {
      if (t != 0) o << t->lexeme ();
      return o;
    }

    //
    //
    //
    class EndOfStream : public Token
    {
    public:
      EndOfStream (unsigned long line)
          : Token ("<end-of-stream>", line)
      {
      }
    };

    typedef
    ReferenceCounting::StrictPtr<EndOfStream>
    EndOfStreamPtr;

    //
    //
    //
    class Keyword : public Token
    {
    public:
      Keyword (std::string lexeme, unsigned long line)
          : Token (lexeme, line)
      {
      }
    };

    typedef
    ReferenceCounting::StrictPtr<Keyword>
    KeywordPtr;



    //
    //
    //
    class Punctuation : public Token
    {
    public:
      Punctuation (std::string lexeme, unsigned long line)
          : Token (lexeme, line)
      {
      }

      virtual
      ~Punctuation () throw () {}
    };

    typedef
    ReferenceCounting::StrictPtr<Punctuation>
    PunctuationPtr;


    //
    //
    //
    class Identifier : public Token
    {
    public:
      Identifier (std::string lexeme, unsigned long line)
          : Token (lexeme, line)
      {
      }
    };

    typedef
    ReferenceCounting::StrictPtr<Identifier>
    IdentifierPtr;


    //
    //
    //
    class SimpleIdentifier : public Identifier
    {
    public:
      SimpleIdentifier (std::string lexeme, unsigned long line)
          : Identifier (lexeme, line)
      {
      }
    };

    typedef
    ReferenceCounting::StrictPtr<SimpleIdentifier>
    SimpleIdentifierPtr;


    //
    //
    //
    class ScopedIdentifier : public Identifier
    {
    public:
      ScopedIdentifier (std::string lexeme, unsigned long line)
          : Identifier (lexeme, line)
      {
      }
    };

    typedef
    ReferenceCounting::StrictPtr<ScopedIdentifier>
    ScopedIdentifierPtr;


    //
    //
    //
    class StringLiteral : public Token
    {
    public:
      StringLiteral (std::string lexeme, unsigned long line)
          : Token (lexeme, line)
      {
      }
    };

    typedef
    ReferenceCounting::StrictPtr<StringLiteral>
    StringLiteralPtr;
  }
}

#endif  // CCF_IDL2_TOKEN_HPP
