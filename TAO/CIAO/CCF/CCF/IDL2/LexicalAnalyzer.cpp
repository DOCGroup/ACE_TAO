// file      : CCF/IDL2/LexicalAnalyzer.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/LexicalAnalyzer.hpp"

#include <stdlib.h> // strtol
#include <iostream>

using std::cerr;
using std::endl;
using std::string;
using std::pair;
using std::size_t;

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
      keyword_table_.insert ("const"     );
      keyword_table_.insert ("exception" );
      keyword_table_.insert ("enum"      );
      keyword_table_.insert ("factory"   );
      keyword_table_.insert ("in"        );
      keyword_table_.insert ("include"   );
      keyword_table_.insert ("inout"     );
      keyword_table_.insert ("interface" );
      keyword_table_.insert ("local"     );
      keyword_table_.insert ("module"    );
      keyword_table_.insert ("out"       );
      keyword_table_.insert ("raises"    );
      keyword_table_.insert ("sequence"  );
      keyword_table_.insert ("sinclude"  );
      keyword_table_.insert ("struct"    );
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
      punctuation_table_.insert ("<");
      punctuation_table_.insert (">");
      punctuation_table_.insert (";");

      // operators
      //
      operator_table_.insert ("+");  // add
      operator_table_.insert ("&");  // and
      operator_table_.insert ("~");  // com
      operator_table_.insert ("/");  // div
      operator_table_.insert ("=");  // eq
      operator_table_.insert ("<<"); // lsh
      operator_table_.insert ("*");  // mul
      operator_table_.insert ("|");  // or
      operator_table_.insert ("%");  // rem
      operator_table_.insert ("-");  // sub
      operator_table_.insert (">>"); // rsh
      operator_table_.insert ("^");  // xor
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
      while (true) // Recovery loop.
      {
        Char c = skip_space (get ());

        if (c.is_eof ()) return TokenPtr (new EndOfStream (c.line ()));

        TokenPtr token;

        if (character_literal (c, token)) return token;

        if (string_literal (c, token)) return token;

        if (integer_literal (c, token)) return token;

        if (operator_ (c, token)) return token;

        if (punctuation (c, token)) return token;

        // Check for identifier last because it can be triggered by
        // wide string prefix (L"...").
        //
        if (c.is_alpha (loc_) || c == '_' || (c == ':' && peek () == ':'))
        {
          return identifier (c);
        }

        cerr << c.line () << ": error: unable to derive any token "
             << "from \'" << c.char_ () << "\'" << endl;


        //Do some primitive error recovery.
        //
        while (c != ';')
        {
          c = skip_space (get ());
          if (c.is_eof ()) return TokenPtr (new EndOfStream (c.line ()));
        }
      }
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
          cerr << "warning: no new line at the end of file"
               << endl;

          //@@ I wonder if it's ok to call get () again  after getting eof.
          //@@ no, it's not: istream throws exception (when enabled) on
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
          cerr << "error: end of file before C-style comment finished"
               << endl;
          return;

          //@@ I wonder if it's ok to call get () again  after getting eof.
          //   No, it is not.
        }

        //cerr << "lexer: c_comment: read character \'" << c << "\'"
        //          << endl;
      }
      while (c != '*' || peek () != '/');

      //cerr << "lexer: c_comment: finished C-comment \'" << c
      //          << "\',\'" << to_char_type (peek ())
      //          << "\'" << endl;

      get (); // get '/'
    }


    bool LexicalAnalyzer::
    read_simple_identifier (string& lexeme, CharBuffer& buf)
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
            cerr << "warning: no new line at the end of file"
                 << endl;
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
    traverse_identifier_tree (string& lexeme,
                              IdentifierTreeNode const& node)
    {
      if (node.map_.empty ()) return true;

      CharBuffer buf;
      string part;

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

      string lexeme;

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
          cerr << "error: " << c.line () << ": \':\' expected."
               << endl;

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
        cerr << "error: invalid identifier" << endl;
        return TokenPtr (new EndOfStream (c.line ()));
      }

      if (c.is_alpha (loc_) || c == '_')
      {
        lexeme += c.char_ ();
      }
      else
      {
        cerr << "error: invalid identifier" << endl;
        return TokenPtr (new EndOfStream (c.line ()));
      }

      while (true)
      {
        c = peek ();

        if (c.is_eof ())
        {
          cerr << "warning: no new line at the end of file"
               << endl;
          break;
        }

        // cerr << "lexer::identifier: peeking on \'" << c.char_ ()
        //          << "\'; current lexeme \'" << lexeme << "\'"
        //          << endl;

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

      //cerr << "lexer: found identifier with lexeme \'"
      //          << lexeme << "\'" << endl;

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

        // Check if it is a boolean literal.
        //
        if (lexeme == "TRUE" || lexeme == "FALSE")
        {
          return TokenPtr (new BooleanLiteral (lexeme, line_));
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

        if (i->size () == 2) // two-character punctuation
        {
          Char pc (peek ());

          if (!pc.is_eof () && (*i)[1] == pc)
          {
            get ();
          }
          else
          {
            // Move on to the next candidate.
            //
            ++i;
            continue;
          }
        }

        token = TokenPtr (new Punctuation (*i, c.line ()));;
        return true;
      }
    }

    bool LexicalAnalyzer::
    operator_ (Char c, TokenPtr& token)
    {
      OperatorTable::const_iterator i = operator_table_.begin ();

      while (true)
      {
        for (;i != operator_table_.end () && (*i)[0] != c; ++i);

        if (i == operator_table_.end ()) return false;

        if (i->size () == 2) // two-character operator
        {
          Char pc (peek ());

          if (!pc.is_eof () && (*i)[1] == pc)
          {
            get ();
          }
          else
          {
            // Move on to the next candidate.
            //
            ++i;
            continue;
          }
        }

        token = TokenPtr (new Operator (*i, c.line ()));;
        return true;
      }
    }

    pair<char, size_t> LexicalAnalyzer::
    scan_char (char const* s) throw (Format)
    {
      if (*s == '\0')
      {
        throw Format ();
      }
      else if (*s != '\\')
      {
        return pair<char, size_t> (*s, 1);
      }
      else
      {
        // Expected size is 2.
        //
        pair<char, size_t> r ('\0', 2);

        switch (*++s)
        {
        case 'n':
          r.first = '\n';
          break;

        case 't':
          r.first = '\t';
          break;

        case 'v':
          r.first = '\v';
          break;

        case 'b':
          r.first = '\b';
          break;

        case 'r':
          r.first = '\r';
          break;

        case 'f':
          r.first = '\f';
          break;

        case 'a':
          r.first = '\a';
          break;

        case '\\':
          r.first = '\\';
          break;

        case '?':
          r.first = '\?';
          break;

        case '\'':
          r.first = '\'';
          break;

        case '"':
          r.first = '\"';
          break;

        case 'x':
          {
            // hex

            char c = *++s;

            //@@ should use is_hex_digit
            //
            if((c >= '0' && c <= '9') ||
               (c >= 'a' && c <= 'f') ||
               (c >= 'A' && c <= 'F'))
            {
              // Maximum 2 digits.
              //
              string holder (s, 2);

              char* end;

              // Cannot fail. -1 < v < 256.
              //
              long v (strtol(holder.c_str (), &end, 16));

              r.first = static_cast<char> (v);
              r.second = 2 + end - holder.c_str ();
            }
            else
            {
              throw Format ();
            }
            break;
          }

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
          {
            // Maximum 3 digits.
            //
            string holder (s, 3);

            char* end;

            // Cannot fail.
            //
            long v (strtol(holder.c_str (), &end, 8));

            if (v < 0 || v > 255) throw Format ();

            r.first = static_cast<char> (v);
            r.second = 1 + end - holder.c_str ();

            break;
          }
        default:
          {
            throw Format ();
          }
        }

        return r;

      }
    }

    string LexicalAnalyzer::
    scan_string (string const& s) throw (Format)
    {
      string r;

      char const* p = s.c_str ();

      while (*p != '\0')
      {
        pair<char, size_t> c (scan_char (p));

        if (c.first == '\0') throw Format ();

        r += c.first;
        p += c.second;
      }

      return r;
    }


    bool LexicalAnalyzer::
    character_literal (Char c, TokenPtr& token)
    {
      if (c != '\'') return false;

      string lexeme;

      Char prev (c);

      while (true)
      {
        Char c = get ();

        if (c.is_eof ())
        {
          cerr << "error: end of file while reading character literal"
               << endl;
          break;
        }

        if (c == '\'' && prev != '\\') break;

        lexeme += c.char_ ();
        prev = c;
      }

      try
      {
        pair<char, size_t> r (scan_char (lexeme.c_str ()));
        if (r.second != lexeme.size ()) throw Format ();

        token = TokenPtr (new CharacterLiteral (r.first, lexeme, c.line ()));
        return true;
      }
      catch (Format const& e)
      {
        cerr << "error: invalid character literal format" << endl;
        return false;
      }
    }

    bool LexicalAnalyzer::
    string_literal (Char c, TokenPtr& token)
    {
      if (c != '\"') return false;

      string lexeme;
      string value;

      try
      {
        while (true)
        {
          string r (string_literal_trailer ());
          value += scan_string (r);
          lexeme += '\"' + r + '\"';

          // Check if there are more strings.
          //

          Char c = skip_space (get ());

          if (c != '\"')
          {
            ret (c); // put it back
            break;
          }


          // Add single space as string separator.
          //
          lexeme += " ";
        }

        // cerr << "string literal: <" << lexeme << ">/<" << value << ">"
        //      << endl;

        token = TokenPtr (new StringLiteral (value, lexeme, c.line ()));
        return true;
      }
      catch (Format const&)
      {
        cerr << "error: invalid string literal format" << endl;
        return false;
      }
    }

    string LexicalAnalyzer::
    string_literal_trailer ()
    {
      string r;

      Char prev ('\"', 0);

      while (true)
      {
        Char c = get ();

        if (c.is_eof ())
        {
          cerr << "error: end of file while reading string literal" << endl;
          break;
        }

        if (c == '\"' && prev != '\\') break;

        r += c.char_ ();
        prev = c;
      }

      return r;
    }

    unsigned long long LexicalAnalyzer::
    scan_integer (string const& s, unsigned short base)
      throw (Format, Boundary)
    {
      unsigned long long const max (~0ULL);

      unsigned long long bound (max / base);

      char const* p (s.c_str ());

      // Skip leading 0 if any.
      //
      while (*p != '\0' && *p == '0') ++p;

      unsigned long long result (0);

      while(*p != '\0')
      {
        unsigned short digit;

        //@@ I am using Char here just because I need
        //   manipulation functions. That's very bad.
        //
        Char c (Char (*p).to_upper (loc_));

        if (c.is_dec_digit (loc_))
        {
          digit = c.char_ () - '0';
        }
        else if (c.is_hex_digit (loc_))
        {
          digit = c.char_ () - 'A' + 10;
        }
        else
        {
          throw Format ();
        }

        if (digit > base) throw Format ();

        if (result > bound)
        {
          // cerr << "boundary: base: " << base << "; bound: " << std::hex
          //     << bound << "; result: " << std::hex << result << endl;

          throw Boundary ();
        }


        result *= base;
        result += digit;

        ++p;
      }

      return result;
    }

    //@@ need to return unparsed characters for recovery (like in
    //   integer_literal).
    //
    bool LexicalAnalyzer::
    integer_literal (Char first, TokenPtr& token)
    {
      try
      {
        Char c (first);

        if (!c.is_dec_digit (loc_)) return false;

        ret (c); // Temporary return the character.

        string lexeme, number;

        unsigned short base (10); // assume 10

        // Determine base and get rid of its identifications.
        //
        //
        if (c == '0')
        {
          lexeme += c.char_ ();

          get ();

          Char pc (peek ());

          if (!pc.is_eof ())
          {
            if (pc == 'x' || pc == 'X')
            {
              base = 16;
              lexeme += get ().char_ ();

              c = peek ();
            }
            else
            {
              base = 8;
              if (!pc.is_oct_digit (loc_))
              {
                number += c.char_ (); // this is needed to handle single 0
              }

              c = pc;
            }
          }
          else
          {
            number += c.char_ (); // this is needed to handle single 0
          }
        }

        while (true)
        {
          // Make sure c is a legal character.
          //

          if (c.is_eof ()) break;

          if (base == 8 && !c.is_oct_digit (loc_))
          {
            break;
          }
          else if (base == 10 && !c.is_dec_digit (loc_))
          {
            break;
          }
          else if (!c.is_hex_digit (loc_))
          {
            break;
          }

          get ();

          lexeme += c.char_ ();
          number += c.char_ ();

          c = peek ();
        }

        if (number.empty ()) throw Format ();

        unsigned long long value (scan_integer (number, base));

        //cerr << "integer literal: <" << lexeme << ">/<" << number << ">/<"
        //     << value << ">" << endl;

        token = TokenPtr (new IntegerLiteral (value, lexeme, first.line ()));
        return true;
      }
      catch (Format const&)
      {
        cerr << "error: invalid integer literal format" << endl;
        return false;
      }
      catch (Boundary const&)
      {
        cerr << "error: integer literal is too big" << endl;
        return false;
      }
    }
  }
}
