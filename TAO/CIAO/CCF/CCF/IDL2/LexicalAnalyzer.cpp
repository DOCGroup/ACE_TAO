// file      : CCF/IDL2/LexicalAnalyzer.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/LexicalAnalyzer.hpp"

#include <iostream>

namespace CCF
{
  namespace IDL2
  {
    LexicalAnalyzer::
    LexicalAnalyzer (TokenStream<char>& is)
        : loc_ ("C"),
          is_ (is),
          after_nl (true),
          line_ (0)
    {
      // Keywords (alphabetic order).

      keyword_table_.insert ("abstract" );
      keyword_table_.insert ("attribute");
      keyword_table_.insert ("factory"  );
      keyword_table_.insert ("in"       );
      keyword_table_.insert ("include"  );
      keyword_table_.insert ("inout"    );
      keyword_table_.insert ("interface");
      keyword_table_.insert ("local"    );
      keyword_table_.insert ("module"   );
      keyword_table_.insert ("out"      );
      keyword_table_.insert ("sinclude" );
      keyword_table_.insert ("supports" );

      punctuation_table_.insert (":");
      punctuation_table_.insert (",");
      punctuation_table_.insert ("{");
      punctuation_table_.insert ("}");
      punctuation_table_.insert ("(");
      punctuation_table_.insert (")");
      punctuation_table_.insert (";");
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

        if (is_.eos(i))
        {
          return TokenPtr (new EndOfStream (line_));
        }

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

        //@@ I should return something special here. Perhaps error recovery
        //   should happen.
        return TokenPtr (new EndOfStream (line_));
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
          //@@ no, it's not: std::istream throws exception (when enabled) on
          //   second attempt.
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

      if (type == SIMPLE)
      {
        KeywordTable::const_iterator i = keyword_table_.find (lexeme);

        if (i != keyword_table_.end ())
        {
          return TokenPtr (new Keyword (*i, line_));
        }
        else
        {
          return TokenPtr (new SimpleIdentifier (lexeme, line_));
        }


        // otherwise deafult to Identifier
      }
      else if (type == SCOPED)
      {
        return TokenPtr (new ScopedIdentifier (lexeme, line_));
      }
      else //type == OTHER
      {
        return TokenPtr (new Identifier (lexeme, line_));
      }
    }

    bool LexicalAnalyzer::
    punctuation (char_type c, TokenPtr& token)
    {
      PunctuationTable::const_iterator i = punctuation_table_.begin ();

      while (true)
      {
        for (;i != punctuation_table_.end () && (*i)[0] != c; i++);

        if (i == punctuation_table_.end ()) return false;

        // if it's a two-character punctuation
        if (i->size () == 2)
        {
          if ((*i)[1] != peek ())
          {
            // move on to the next candidate
            i++;
            continue;
          }
        }

        token = TokenPtr (new Punctuation (*i, line_));;
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

      token = TokenPtr (new StringLiteral (lexeme, line_));
      return true;
    }
  }
}
