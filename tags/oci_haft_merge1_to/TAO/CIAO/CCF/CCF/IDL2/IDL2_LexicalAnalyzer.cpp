// $Id$
#include "IDL2_LexicalAnalyzer.hpp"

#include <iostream>

namespace IDL2
{
  LexicalAnalyzer::
  LexicalAnalyzer (CCF::TokenStream<char>& is)
      : loc_ ("C"),
        is_ (is),
        after_nl (true),
        line_ (0),

        eos (new EndOfStream),

        abstract  (new Keyword),
        attribute (new Keyword),
        factory   (new Keyword),
        in        (new Keyword),
        include   (new Keyword),
        inout     (new Keyword),
        interface (new Keyword),
        local     (new Keyword),
        module    (new Keyword),
        out       (new Keyword),
        sinclude  (new Keyword),
        supports  (new Keyword),

        colon  (new Punctuation),
        comma  (new Punctuation),
        lbrace (new Punctuation),
        rbrace (new Punctuation),
        lparen (new Punctuation),
        rparen (new Punctuation),
        semi   (new Punctuation)
  {
    keyword_table_["abstract" ] = abstract;
    keyword_table_["attribute"] = attribute;
    keyword_table_["factory"  ] = factory;
    keyword_table_["in"       ] = in;
    keyword_table_["include"  ] = include;
    keyword_table_["inout"    ] = inout;
    keyword_table_["interface"] = interface;
    keyword_table_["local"    ] = local;
    keyword_table_["module"   ] = module;
    keyword_table_["out"      ] = out;
    keyword_table_["sinclude" ] = sinclude;
    keyword_table_["supports" ] = supports;

    punctuation_table_[":"] = colon;
    punctuation_table_[","] = comma;
    punctuation_table_["{"] = lbrace;
    punctuation_table_["}"] = rbrace;
    punctuation_table_["("] = lparen;
    punctuation_table_[")"] = rparen;
    punctuation_table_[";"] = semi;
  }


  LexicalAnalyzer::int_type LexicalAnalyzer::
  get ()
  {
    int_type i;

    if (buffer_.empty ())
    {
      i = is_.next ();
    }
    else
    {
      i = buffer_.front ();
      buffer_.pop_front ();
    }

    if (after_nl)
    {
      after_nl = false;
      line_++;
    }

    if (i == '\n')
    {
      after_nl = true;
    }

    return i;
  }

  LexicalAnalyzer::int_type LexicalAnalyzer::
  peek ()
  {
    int_type i;

    if (buffer_.empty ())
    {
      i = is_.next ();
      buffer_.push_back (i);
    }
    else
    {
      i = buffer_.front ();
    }

    return i;
  }

  LexicalAnalyzer::int_type LexicalAnalyzer::
  peek_more ()
  {
    int_type i;

    if (buffer_.size () < 2)
    {
      i = is_.next ();
      buffer_.push_back (i);
    }
    else
    {
      i = buffer_.at (1);
    }

    return i;
  }

  LexicalAnalyzer::char_type LexicalAnalyzer::
  to_char_type (int_type i)
  {
    return is_.to_char_type (i);
  }


  TokenPtr LexicalAnalyzer::
  next ()
  {
    while (true)
    {
      int_type i = get ();

      if (is_.eos(i)) return eos;

      char_type c = to_char_type (i);

      // Handling spaces
      if (std::isspace (c, loc_)) continue;

      // Handling C++ comments
      if (c == '/' && peek () == '/')
      {
        cxx_comment (c);
        continue;
      }

      // Handling C comments
      if (c == '/' && peek () == '*')
      {
        c_comment (c);
        continue;
      }

      if (std::isalpha (c, loc_) || c == '_' || (c == ':' && peek () == ':'))
      {
        return identifier (c);
      }

      TokenPtr token;

      if (string_literal (c, token)) return token;

      if (punctuation (c, token)) return token;


      std::cerr << line_ << ": error: unable to derive any token from \'"
                << c << "\'" << std::endl;

      //@@ I should return something special here.
      return eos;
    }
  }

  void LexicalAnalyzer::
  cxx_comment (char_type c)
  {
    while (c != '\n')
    {
      int_type i = get ();

      if (is_.eos (i))
      {
        std::cerr << "warning: no new line at the end of file" << std::endl;
        //@@ I wonder if it's ok to call get () again  after getting eof.
        break;
      }
      c = to_char_type (i);
    }
  }

  void LexicalAnalyzer::
  c_comment (char_type c)
  {
    get (); // get '*'

    do
    {
      int_type i = get ();

      if (is_.eos (i))
      {
        std::cerr << "error: end of file before C-style comment finished"
                  << std::endl;
        return;

        //@@ I wonder if it's ok to call get () again  after getting eof.
      }
      c = to_char_type (i);

      //std::cerr << "lexer: c_comment: read character \'" << c << "\'"
      //          << std::endl;
    }
    while (c != '*' || peek () != '/');

    //std::cerr << "lexer: c_comment: finished C-comment \'" << c
    //          << "\',\'" << to_char_type (peek ())
    //          << "\'" << std::endl;

    get (); // get '/'
  }

  TokenPtr LexicalAnalyzer::
  identifier (char_type c)
  {
    std::string lexeme;

    enum
      {
        SIMPLE,
        SCOPED,
        OTHER
      } type = SIMPLE;

    if (c == ':')
    {
      get ();
      lexeme = "::";
      type = SCOPED;
    }
    else
    {
      lexeme += c;
    }

    while (true)
    {
      int_type i = peek ();

      if (is_.eos (i))
      {
        std::cerr << "warning: no new line at the end of file" << std::endl;
        break;
      }

      c = to_char_type (i);

      //std::cerr << "lexer::identifier: peeking on \'" << c
      //          << "\'; current lexeme \'" << lexeme << "\'"
      //          << std::endl;

      if (std::isalnum (c, loc_) || c == '_')
      {
        get ();
        lexeme += c;
        continue;
      }

      if (c == ':' && peek_more () == ':')
      {
        get ();
        get ();
        lexeme += "::";
        if (type == SIMPLE) type = OTHER;
        continue;
      }

      break;
    }

    //std::cerr << "lexer: found identifier with lexeme \'"
    //          << lexeme << "\'" << std::endl;

    switch (type)
    {
    case SIMPLE:
      {
        KeywordTable::const_iterator i = keyword_table_.find (lexeme);

        if (i == keyword_table_.end ())
        {
          return TokenPtr (new SimpleIdentifier (lexeme, line_));
        }
        else
        {
          return i->second;
        }
      }
    case SCOPED:
      {
        return TokenPtr (new ScopedIdentifier (lexeme, line_));
      }
    case OTHER:
      {
        return TokenPtr (new Identifier (lexeme, line_));
      }
      //@@ without this line GCC issues warning and MSVC issues error
      // Is it required by standard?
    default:
      {
        throw InternalError ();
      }

    }
  }

  bool LexicalAnalyzer::
  punctuation (char_type c, TokenPtr& token)
  {
    PunctuationTable::const_iterator i = punctuation_table_.begin ();

    while (true)
    {
      for (;i != punctuation_table_.end () && i->first[0] != c; i++);

      if (i == punctuation_table_.end ()) return false;

      // if it's a two-character punctuation
      if (i->first.size () == 2)
      {
        if (i->first[1] != peek ())
        {
          // move on to the next candidate
          i++;
          continue;
        }
      }

      token = i->second;
      return true;
    }
  }

  bool LexicalAnalyzer::
  string_literal (char_type c, TokenPtr& token)
  {
    if (c != '\"') return false;

    std::string lexeme;

    while (true)
    {
      int_type i = get ();

      if (is_.eos (i))
      {
        std::cerr << "warning: end of file while reading string literal"
                  << std::endl;
        break;
      }

      c = to_char_type (i);

      if (c == '\"') break;
      else lexeme += c;
    }

    token = TokenPtr(new StringLiteral (lexeme, line_));
    return true;
  }
}
