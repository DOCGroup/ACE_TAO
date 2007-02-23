// file      : CCF/IDL2/Token.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

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
      virtual
      ~Token () throw () {}

      virtual void
      print (std::ostream& o)
      {
        o << lexeme ();
      }

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
    class Operator : public Token
    {
    public:
      Operator (std::string lexeme, unsigned long line)
          : Token (lexeme, line)
      {
      }

      virtual
      ~Operator () throw () {}
    };

    typedef
    ReferenceCounting::StrictPtr<Operator>
    OperatorPtr;


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
    class Literal : public Token
    {
    public:
      Literal (std::string lexeme, unsigned long line)
          : Token (lexeme, line)
      {
      }
    };

    typedef
    ReferenceCounting::StrictPtr<Literal>
    LiteralPtr;


    //
    //
    //
    class BooleanLiteral : public Literal
    {
    public:
      BooleanLiteral (std::string lexeme, unsigned long line)
          : Literal (lexeme, line)
      {
      }

      bool
      value () const
      {
        return lexeme () == "TRUE";
      }

      virtual void
      print (std::ostream& o)
      {
        o << lexeme ();
      }
    };

    typedef
    ReferenceCounting::StrictPtr<BooleanLiteral>
    BooleanLiteralPtr;


    //
    //
    //
    class CharacterLiteral : public Literal
    {
    public:
      CharacterLiteral (char c, std::string lexeme, unsigned long line)
          : Literal (lexeme, line), c_ (c)
      {
      }

      char
      value () const
      {
        return c_;
      }

      virtual void
      print (std::ostream& o)
      {
        //@@ todo
        o << lexeme ();
      }

    private:
      char c_;
    };

    typedef
    ReferenceCounting::StrictPtr<CharacterLiteral>
    CharacterLiteralPtr;


    //
    //
    //
    class IntegerLiteral : public Literal
    {
    public:
      IntegerLiteral (unsigned long long value,
                      std::string lexeme,
                      unsigned long line)
          : Literal (lexeme, line), value_ (value)
      {
      }

      unsigned long long
      value () const
      {
        return value_;
      }

      virtual void
      print (std::ostream& o)
      {
        o << lexeme ();
      }

    private:
      unsigned long long value_;
    };

    typedef
    ReferenceCounting::StrictPtr<IntegerLiteral>
    IntegerLiteralPtr;


    //
    //
    //
    class StringLiteral : public Literal
    {
    public:
      virtual
      ~StringLiteral () throw () {}

      StringLiteral (std::string const& value,
                     std::string const& lexeme,
                     unsigned long line)
          : Literal (lexeme, line), value_ (value)
      {
      }

      std::string
      value () const
      {
        return value_;
      }

      virtual void
      print (std::ostream& o)
      {
        //@@ todo
        o << lexeme ();
      }

    private:
      std::string value_;
    };

    typedef
    ReferenceCounting::StrictPtr<StringLiteral>
    StringLiteralPtr;
  }
}

inline std::ostream&
operator<< (std::ostream& o, CCF::IDL2::TokenPtr const& t)
{
  if (t != 0) t->print (o);
  return o;
}

#endif  // CCF_IDL2_TOKEN_HPP
