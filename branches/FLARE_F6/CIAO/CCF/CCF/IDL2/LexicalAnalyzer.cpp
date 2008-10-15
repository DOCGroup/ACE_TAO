// file      : CCF/IDL2/LexicalAnalyzer.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

// This file has no dependency on ACE, thus we turn 
// all ACE_OS checks off
// FUZZ: disable check_for_lack_ACE_OS

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
    LexicalAnalyzer (CompilerElements::TokenStream<Char>& is)
        : loc_ ("C"), is_ (is)
    {
      // Keywords (alphabetic order).
      //

      keyword_table_.insert ("abstract"   );
      keyword_table_.insert ("attribute"  );
      keyword_table_.insert ("__binclude" );
      keyword_table_.insert ("case"       );
      keyword_table_.insert ("const"      );
      keyword_table_.insert ("custom"     );
      keyword_table_.insert ("default"    );
      keyword_table_.insert ("exception"  );
      keyword_table_.insert ("enum"       );
      keyword_table_.insert ("factory"    );
      keyword_table_.insert ("getraises"  );
      keyword_table_.insert ("in"         );
      keyword_table_.insert ("inout"      );
      keyword_table_.insert ("interface"  );
      keyword_table_.insert ("__qinclude" );
      keyword_table_.insert ("local"      );
      keyword_table_.insert ("module"     );
      keyword_table_.insert ("native"     );
      keyword_table_.insert ("oneway"     );
      keyword_table_.insert ("out"        );
      keyword_table_.insert ("private"    );
      keyword_table_.insert ("public"     );
      keyword_table_.insert ("raises"     );
      keyword_table_.insert ("readonly"   );
      keyword_table_.insert ("sequence"   );
      keyword_table_.insert ("setraises"  );
      keyword_table_.insert ("struct"     );
      keyword_table_.insert ("supports"   );
      keyword_table_.insert ("switch"     );
      keyword_table_.insert ("truncatable");
      keyword_table_.insert ("typedef"    );
      keyword_table_.insert ("typeid"     );
      keyword_table_.insert ("typeprefix" );
      keyword_table_.insert ("union"      );
      keyword_table_.insert ("valuetype"  );

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
      punctuation_table_.insert ("[");
      punctuation_table_.insert ("]");
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
    get ()
    {
      if (!ibuffer_.empty ())
      {
        Char c = ibuffer_.front ();
        ibuffer_.pop_front ();
        return c;
      }
      else
      {
        return is_.next ();
      }
    }

    LexicalAnalyzer::Char LexicalAnalyzer::
    peek ()
    {
      if (ibuffer_.empty ())
      {
        ibuffer_.push_back (is_.next ());
      }

      return ibuffer_.front ();
    }

    LexicalAnalyzer::Char LexicalAnalyzer::
    peek_more ()
    {
      while (ibuffer_.size () < 2)
      {
        ibuffer_.push_back (is_.next ());
      }

      return ibuffer_.at (1);
    }

    void LexicalAnalyzer::
    ret (Char const& c)
    {
      ibuffer_.push_front (c);
    }


    TokenPtr LexicalAnalyzer::
    next ()
    {
      while (true) // Recovery loop.
      {
        Char c = skip_space (get ());

        if (is_eos (c)) return TokenPtr (new EndOfStream (0));

        TokenPtr token;

        if (character_literal (c, token)) return token;

        if (string_literal (c, token)) return token;

        if (integer_literal (c, token)) return token;

        // Check for identifier after literals  because it can be
        // triggered by wide string prefix (L"...").
        //
        if (is_alpha (c) || c == '_' || (c == ':' && peek () == ':'))
        {
          return identifier (c);
        }

        // Check for punctuation after identifier because ':' in
        // scoped name will trigger.
        //
        if (operator_ (c, token)) return token;

        if (punctuation (c, token)) return token;

        cerr << c.line () << ": error: unable to derive any token "
             << "from \'" << c << "\'" << endl;


        //Do some primitive error recovery.
        //
        while (c != ';')
        {
          c = skip_space (get ());
          if (is_eos (c)) return TokenPtr (new EndOfStream (0));
        }
      }
    }

    LexicalAnalyzer::Char LexicalAnalyzer::
    skip_space (Char c)
    {
      while (!is_eos (c) && is_space (c)) c = get ();

      return c;
    }

    bool LexicalAnalyzer::
    read_simple_identifier (string& lexeme, CharBuffer& buf)
    {
      Char c = skip_space (get ());

      buf.push_back (c);

      if (is_eos (c)) return false;

      if (is_alpha (c) || c == '_')
      {
        lexeme += c;

        while (true)
        {
          c = peek ();

          if (is_eos (c))
          {
            cerr << "warning: no new line at the end of file" << endl;
            break;
          }

          if (is_alnum (c) || c == '_')
          {
            get ();
            buf.push_back (c);
            lexeme += c;
            continue;
          }

          break;
        }

        return true;
      }

      return false;
    }


    bool LexicalAnalyzer::
    traverse_identifier_tree (string& lexeme, IdentifierTreeNode const& node)
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
    identifier (Char c)
    {
      unsigned long line (c.line ());

      string lexeme;

      enum
      {
        simple,
        scoped,
        other
      } type = simple;

      if (c == ':')
      {
        //@@ not checking for eos here
        if ((c = get ()) != ':')
        {
          cerr << "error: " << c.line () << ": \':\' expected."
               << endl;

          return TokenPtr (new EndOfStream (0));
          //@@ error handling is lame for lexical analyzer.
        }

        lexeme = "::";
        type = scoped;
        c = get ();
      }

      // First caracter of an identifier.
      //
      if (is_eos (c))
      {
        cerr << "error: invalid identifier" << endl;
        return TokenPtr (new EndOfStream (0));
      }

      if (is_alpha (c) || c == '_')
      {
        lexeme += c;
      }
      else
      {
        cerr << "error: invalid identifier" << endl;
        return TokenPtr (new EndOfStream (0));
      }

      while (true)
      {
        c = peek ();

        if (is_eos (c))
        {
          cerr << "warning: no new line at the end of file" << endl;
          break;
        }

        // cerr << "lexer::identifier: peeking on \'" << c.char_ ()
        //          << "\'; current lexeme \'" << lexeme << "\'"
        //          << endl;

        if (is_alnum (c) || c == '_')
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
          if (type == simple) type = other;
          continue;
        }

        break;
      }

      //cerr << "lexer: found identifier with lexeme \'"
      //          << lexeme << "\'" << endl;

      if (type == simple)
      {
        // Check if it's a keyword.
        {
          KeywordTable::const_iterator i (keyword_table_.find (lexeme));

          if (i != keyword_table_.end ())
          {
            return TokenPtr (new Keyword (*i, line));
          }

          // This part is tricky. If it's after 6pm then come back
          // in the morning. In essence I want the same name
          // ('string' and 'wstring') to be recognized as a keyword
          // in one case and as an identifier in the other. When
          // we see 'string' followed by '<' we want it to be a
          // keyword. If it's just all by itself then we want to treat
          // it as an identifier (since it is a complete construct
          // by itself). So here we are going to check for that.
          //

          if (lexeme == "string" || lexeme == "wstring")
          {
            Char c = skip_space (get ());
            ret (c);

            if (c == '<')
            {
              return TokenPtr (new Keyword (lexeme, line));
            }
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
                new SimpleIdentifier (lexeme, line));
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
          return TokenPtr (new BooleanLiteral (lexeme, line));
        }

        // Default to SimpleIdentifier.
        //
        return TokenPtr (new SimpleIdentifier (lexeme, line));
      }
      else if (type == scoped)
      {
        return TokenPtr (new ScopedIdentifier (lexeme, line));
      }
      else //type == other
      {
        return TokenPtr (new Identifier (lexeme, line));
      }
    }

    bool LexicalAnalyzer::
    punctuation (Char c, TokenPtr& token)
    {
      unsigned long line (c.line ());

      PunctuationTable::const_iterator i = punctuation_table_.begin ();

      while (true)
      {
        for (;i != punctuation_table_.end () && (*i)[0] != c; ++i);

        if (i == punctuation_table_.end ()) return false;

        if (i->size () == 2) // two-character punctuation
        {
          Char pc (peek ());

          if (!is_eos (pc) && (*i)[1] == pc)
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

        token = TokenPtr (new Punctuation (*i, line));
        return true;
      }
    }

    bool LexicalAnalyzer::
    operator_ (Char c, TokenPtr& token)
    {
      unsigned long line (c.line ());

      OperatorTable::const_iterator i = operator_table_.begin ();

      while (true)
      {
        for (;i != operator_table_.end () && (*i)[0] != c; ++i);

        if (i == operator_table_.end ()) return false;

        if (i->size () == 2) // two-character operator
        {
          Char pc (peek ());

          if (!is_eos (pc) && (*i)[1] == pc)
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

        token = TokenPtr (new Operator (*i, line));
        return true;
      }
    }

    pair<char, size_t> LexicalAnalyzer::scan_char (char const* s)
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

            char c (*++s);

            if(is_hex_digit (c))
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

    string LexicalAnalyzer::scan_string (string const& s)
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

      unsigned long line (c.line ());
      string lexeme;

      Char prev (c);

      while (true)
      {
        c = get ();

        if (is_eos (c))
        {
          cerr << "error: end of file while reading character literal"
               << endl;
          break;
        }

        if (c == '\'' && prev != '\\') break;

        lexeme += c;
        prev = c;
      }

      try
      {
        pair<char, size_t> r (scan_char (lexeme.c_str ()));
        if (r.second != lexeme.size ()) throw Format ();

        token = TokenPtr (new CharacterLiteral (r.first, lexeme, line));
        return true;
      }
      catch (Format const&)
      {
        cerr << "error: invalid character literal format" << endl;
        return false;
      }
    }

    bool LexicalAnalyzer::
    string_literal (Char c, TokenPtr& token)
    {
      if (c != '\"') return false;

      unsigned long line (c.line ());
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

          c = skip_space (get ());

          if (c != '\"')
          {
            ret (c); // put it back
            break;
          }


          // Add single space as a string separator.
          //
          lexeme += " ";
        }

        // cerr << "string literal: <" << lexeme << ">/<" << value << ">"
        //      << endl;

        token = TokenPtr (new StringLiteral (value, lexeme, line));
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

        if (is_eos (c))
        {
          cerr << "error: end of file while reading string literal" << endl;
          break;
        }

        if (c == '\"' && prev != '\\') break;

        r += c;
        prev = c;
      }

      return r;
    }

    unsigned long long LexicalAnalyzer::
    scan_integer (string const& s, unsigned short base)
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

        char c (to_upper (*p));

        if (is_dec_digit (c))
        {
          digit = c - '0';
        }
        else if (is_hex_digit (c))
        {
          digit = c - 'A' + 10;
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
    integer_literal (Char c, TokenPtr& token)
    {
      try
      {
        if (!is_dec_digit (c)) return false;

        unsigned long line (c.line ());

        ret (c); // Temporarily return the character.

        string lexeme, number;

        unsigned short base (10); // assume 10

        // Determine base and get rid of its identifications.
        //
        //
        if (c == '0')
        {
          lexeme += c;

          get ();

          Char pc (peek ());

          if (!is_eos (pc))
          {
            if (pc == 'x' || pc == 'X')
            {
              get ();
              base = 16;
              lexeme += pc;

              c = peek ();
            }
            else
            {
              base = 8;
              if (!is_oct_digit (pc))
              {
                number += c; // this is needed to handle single 0
              }

              c = pc;
            }
          }
          else
          {
            number += c; // this is needed to handle single 0
          }
        }

        while (true)
        {
          // Make sure c is a legal character.
          //

          if (is_eos (c)) break;

          if (base == 8 && !is_oct_digit (c))
          {
            break;
          }
          else if (base == 10 && !is_dec_digit (c))
          {
            break;
          }
          else if (!is_hex_digit (c))
          {
            break;
          }

          get ();

          lexeme += c;
          number += c;

          c = peek ();
        }

        if (number.empty ()) throw Format ();

        unsigned long long value (scan_integer (number, base));

        //cerr << "integer literal: <" << lexeme << ">/<" << number << ">/<"
        //     << value << ">" << endl;

        token = TokenPtr (new IntegerLiteral (value, lexeme, line));
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
