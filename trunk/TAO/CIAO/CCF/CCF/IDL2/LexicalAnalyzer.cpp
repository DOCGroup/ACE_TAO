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
      //

      keyword_table_.insert ("abstract"  );
      keyword_table_.insert ("attribute" );
      keyword_table_.insert ("factory"   );
      keyword_table_.insert ("in"        );
      keyword_table_.insert ("include"   );
      keyword_table_.insert ("inout"     );
      keyword_table_.insert ("interface" );
      keyword_table_.insert ("local"     );
      keyword_table_.insert ("module"    );
      keyword_table_.insert ("out"       );
      keyword_table_.insert ("sinclude"  );
      keyword_table_.insert ("supports"  );
      keyword_table_.insert ("typedef"   );
      keyword_table_.insert ("typeid"    );
      keyword_table_.insert ("typeprefix");

      // Identifiers (alphabetic order).
      //
      // Note: if you are planning to hack something up
      // in the code below, first make sure you understand
      // how everything works!
      //

      IdentifierTreeNode end;

      identifier_tree_["Object"   ] = end;
      identifier_tree_["ValueBase"] = end;
      identifier_tree_["any"      ] = end;
      identifier_tree_["boolean"  ] = end;
      identifier_tree_["char"     ] = end;
      identifier_tree_["double"   ] = end;
      identifier_tree_["float"    ] = end;

      IdentifierTreeNode long_;
      long_[""      ] = end;
      long_["double"] = end;
      long_["long"  ] = end;

      identifier_tree_["long"     ] = long_;
      identifier_tree_["octet"    ] = end;
      identifier_tree_["short"    ] = end;
      identifier_tree_["string"   ] = end;

      IdentifierTreeNode unsigned_long_;
      unsigned_long_[""    ] = end;
      unsigned_long_["long"] = end;

      IdentifierTreeNode unsigned_;
      unsigned_["long"  ] = unsigned_long_;
      unsigned_["short" ] = end;

      identifier_tree_["unsigned" ] = unsigned_;
      identifier_tree_["void"     ] = end;
      identifier_tree_["wchar"    ] = end;
      identifier_tree_["wstring"  ] = end;

      // punctuation (pair-matched).
      //
      punctuation_table_.insert (":");
      punctuation_table_.insert (",");
      punctuation_table_.insert ("{");
      punctuation_table_.insert ("}");
      punctuation_table_.insert ("(");
      punctuation_table_.insert (")");
      punctuation_table_.insert (";");
    }

    LexicalAnalyzer::Char LexicalAnalyzer::
    get_from_stream ()
    {
      int_type i = is_.next ();

      if (after_nl)
      {
        after_nl = false;
        line_++;
      }

      if (i == '\n') after_nl = true;

      return Char (i, line_);
    }


    LexicalAnalyzer::Char LexicalAnalyzer::
    get ()
    {
      if (!buffer_.empty ())
      {
        Char c = buffer_.front ();
        buffer_.pop_front ();
        return c;
      }
      else
      {
        return get_from_stream ();
      }
    }

    LexicalAnalyzer::Char LexicalAnalyzer::
    peek ()
    {
      if (buffer_.empty ())
      {
        buffer_.push_back (get_from_stream ());
      }

      return buffer_.front ();
    }

    LexicalAnalyzer::Char LexicalAnalyzer::
    peek_more ()
    {
      if (buffer_.size () < 2)
      {
        buffer_.push_back (get_from_stream ());
      }

      return buffer_.at (1);
    }

    void LexicalAnalyzer::
    ret (Char const& c)
    {
      buffer_.push_front (c);
    }

    TokenPtr LexicalAnalyzer::
    next ()
    {
      Char c = skip_space (get ());

      if (c.is_eof ()) return TokenPtr (new EndOfStream (c.line ()));

      if (c.is_alpha (loc_)
          || c == '_'
          || (c == ':' && peek () == ':')) return identifier (c);

      TokenPtr token;

      if (string_literal (c, token)) return token;

      if (punctuation (c, token)) return token;


      std::cerr << c.line () << ": error: unable to derive any token "
                << "from \'" << c.char_ () << "\'" << std::endl;

      //@@ I should return something special here. Perhaps error recovery
      //   should happen.
      return TokenPtr (new EndOfStream (c.line ()));
    }

    LexicalAnalyzer::Char LexicalAnalyzer::
    skip_space (Char c)
    {
      bool first (true);

      while (true)
      {
        if (!first) c = get ();
        else first = false;

        if (c.is_eof ()) return c;

        // Handling spaces
        if (c.is_space (loc_)) continue;

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

        return c;
      }
    }


    void LexicalAnalyzer::
    cxx_comment (Char c)
    {
      while (c != '\n')
      {
        c = get ();

        if (c.is_eof ())
        {
          std::cerr << "warning: no new line at the end of file"
                    << std::endl;

          //@@ I wonder if it's ok to call get () again  after getting eof.
          //@@ no, it's not: std::istream throws exception (when enabled) on
          //   second attempt.
          break;
        }
      }
    }

    void LexicalAnalyzer::
    c_comment (Char c)
    {
      get (); // get '*'

      do
      {
        c = get ();

        if (c.is_eof ())
        {
          std::cerr << "error: end of file before C-style comment finished"
                    << std::endl;
          return;

          //@@ I wonder if it's ok to call get () again  after getting eof.
          //   No, it is not.
        }

        //std::cerr << "lexer: c_comment: read character \'" << c << "\'"
        //          << std::endl;
      }
      while (c != '*' || peek () != '/');

      //std::cerr << "lexer: c_comment: finished C-comment \'" << c
      //          << "\',\'" << to_char_type (peek ())
      //          << "\'" << std::endl;

      get (); // get '/'
    }

    bool LexicalAnalyzer::
    read_simple_identifier (std::string& lexeme, CharBuffer& buf)
    {
      Char c = skip_space (get ());

      buf.push_back (c);

      if (c.is_eof ()) return false;

      if (c.is_alpha (loc_) || c == '_')
      {
        lexeme += c.char_ ();

        while (true)
        {
          c = peek ();

          if (c.is_eof ())
          {
            std::cerr << "warning: no new line at the end of file"
                      << std::endl;
            break;
          }

          if (c.is_alnum (loc_) || c == '_')
          {
            buf.push_back (get ());
            lexeme += c.char_ ();
            continue;
          }

          break;
        }

        return true;
      }

      return false;
    }

    bool LexicalAnalyzer::
    traverse_identifier_tree (std::string& lexeme,
                              IdentifierTreeNode const& node)
    {
      if (node.map_.empty ()) return true;

      CharBuffer buf;
      std::string part;

      if (read_simple_identifier (part, buf))
      {
        IdentifierTreeNode::PrefixMap::const_iterator i (
          node.map_.find (part));

        if (i != node.map_.end ())
        {
          if (traverse_identifier_tree (part, i->second))
          {
            lexeme += " " + part;
            return true;
          }
        }
      }

      // Return characters to the buffer in case we couldn't
      // match anything.

      for(;!buf.empty (); buf.pop_back ()) ret (buf.back ());

      // Check is the node.map_ contains empty key which indicates
      // that what we've got is good enough.
      //
      return node.map_.find ("") != node.map_.end ();
    }


    TokenPtr LexicalAnalyzer::
    identifier (Char first)
    {
      Char c (first);

      std::string lexeme;

      enum
      {
        SIMPLE,
        SCOPED,
        OTHER
      } type = SIMPLE;

      if (c == ':')
      {
        if((c = get ()) != ':')
        {
          std::cerr << "error: " << c.line () << ": \':\' expected."
                    << std::endl;

          return TokenPtr (new EndOfStream (c.line ()));
          //@@ error handling is lame for lexical analyzer.
        }

        lexeme = "::";
        type = SCOPED;
        c = get ();
      }

      // First caracter of an identifier.
      //
      if (c.is_eof ())
      {
        std::cerr << "error: invalid identifier" << std::endl;
        return TokenPtr (new EndOfStream (c.line ()));
      }

      if (c.is_alpha (loc_) || c == '_')
      {
        lexeme += c.char_ ();
      }
      else
      {
        std::cerr << "error: invalid identifier" << std::endl;
        return TokenPtr (new EndOfStream (c.line ()));
      }

      while (true)
      {
        c = peek ();

        if (c.is_eof ())
        {
          std::cerr << "warning: no new line at the end of file"
                    << std::endl;
          break;
        }

        // std::cerr << "lexer::identifier: peeking on \'" << c.char_ ()
        //          << "\'; current lexeme \'" << lexeme << "\'"
        //          << std::endl;

        if (c.is_alnum (loc_) || c == '_')
        {
          get ();
          lexeme += c.char_ ();
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
        // Check if it's a keyword.
        {
          KeywordTable::const_iterator i (keyword_table_.find (lexeme));

          if (i != keyword_table_.end ())
          {
            return TokenPtr (new Keyword (*i, first.line ()));
          }
        }

        // Check if it is a reserved identifier.
        //

        {
          IdentifierTreeNode::PrefixMap::const_iterator i (
            identifier_tree_.map_.find (lexeme));

          if (i != identifier_tree_.map_.end ())
          {
            if (traverse_identifier_tree (lexeme, i->second))
            {
              return TokenPtr (
                new SimpleIdentifier (lexeme, first.line ()));
            }
            else
            {
              //@@ error
            }
          }
        }

        // Default to SimpleIdentifier.
        //
        return TokenPtr (new SimpleIdentifier (lexeme, line_));
      }
      else if (type == SCOPED)
      {
        return TokenPtr (new ScopedIdentifier (lexeme, first.line ()));
      }
      else //type == OTHER
      {
        return TokenPtr (new Identifier (lexeme, first.line ()));
      }
    }

    bool LexicalAnalyzer::
    punctuation (Char c, TokenPtr& token)
    {
      PunctuationTable::const_iterator i = punctuation_table_.begin ();

      while (true)
      {
        for (;i != punctuation_table_.end () && (*i)[0] != c; ++i);

        if (i == punctuation_table_.end ()) return false;

        // if it's a two-character punctuation
        if (i->size () == 2)
        {
          //@@ this code is broken.
          abort ();

          if ((*i)[1] != peek ())
          {
            // move on to the next candidate
            ++i;
            continue;
          }
        }

        token = TokenPtr (new Punctuation (*i, c.line ()));;
        return true;
      }
    }

    bool LexicalAnalyzer::
    string_literal (Char c, TokenPtr& token)
    {
      if (c != '\"') return false;

      std::string lexeme;

      while (true)
      {
        Char c = get ();

        if (c.is_eof ())
        {
          std::cerr << "warning: end of file while reading string literal"
                    << std::endl;
          break;
        }

        if (c == '\"') break;
        else lexeme += c.char_ ();
      }

      token = TokenPtr (new StringLiteral (lexeme, c.line ()));
      return true;
    }
  }
}
