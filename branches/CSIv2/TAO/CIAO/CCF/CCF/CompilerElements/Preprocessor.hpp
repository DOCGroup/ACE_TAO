// file      : CCF/CompilerElements/Preprocessor.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_PREPROCESSOR_HPP
#define CCF_PREPROCESSOR_HPP

#include <deque>

#include "CCF/CompilerElements/TokenStream.hpp"

namespace CCF
{
  //@@ this code is experimental and needs cleaning
  class Preprocessor : public TokenStream<char>
  {
  public:
    virtual
    ~Preprocessor () {}

    Preprocessor (TokenStream<char>& is)
        : state (PREPROCESSING),
          loc_ ("C"),
          is_ (is)
    {
    }

  public:
    virtual int_type
    next ()
    {
      return out_get ();
    }

  private:
    int_type
    underflow ()
    {
      int_type i = in_get ();

      if (eos (i))
      {
        return i;
      }

      char_type c = to_char_type (i);

      //std::cerr << "next_i: c = " << c <<  std::endl;

      switch (c)
      {
      case '\\': return handle_escape ();
      case '\"':
        {
          if (state == STRING_LITERAL) state = PREPROCESSING;
          else if (state == PREPROCESSING) state = STRING_LITERAL;
          else
          {
            //@@error
          }
          break;
        }
      case '\'':
        {
          if (state == CHAR_LITERAL) state = PREPROCESSING;
          else if (state == PREPROCESSING) state = CHAR_LITERAL;
          else
          {
            //@@error
          }
          break;
        }
      case '#':
        {
          if (state != PREPROCESSING) break;
          return handle_preprocessor_token ();
        }
      }

      return i;
    }

    int_type
    handle_escape ()
    {
      int_type i = in_get ();

      if (eos (i))
      {
        //@@ error
        return i;
      }

      char_type c = to_char_type (i);

      //@@ '\n' won't work on MShit
      switch (c)
      {
      case '\n': return underflow (); //skip it
      default:
        {
          out_buffer_.push_back (i);
          return '\\';
        }
      }
    }

    void
    skip_white_space ()
    {
      while (true)
      {
        int_type i = in_get ();
        if (!eos (i))
        {
          char_type c = to_char_type (i);
          if (std::isspace (c, loc_) && c != '\n') continue;
        }

        in_put (i);
        break;
      }
    }

    int_type
    handle_preprocessor_token ()
    {
      //std::cerr << "handle_preprocessor_token" << std::endl;

      skip_white_space ();

      std::string lexeme;

      int_type i;
      char_type c;

      while (true)
      {
        i = in_get ();

        if (eos (i))
        {
          //@@ error
          return i;
        }

        c = to_char_type (i);

        if (std::isalnum (c, loc_))
        {
          lexeme += c;
          continue;
        }
        break;
      }

      if (lexeme == "include") return handle_preprocessor_include ();
      else
      {
        // skip till newline
        while (true)
        {
          if (c == '\n') break;

          i = in_get ();

          if (eos (i))
          {
            //@@ error
            return i;
          }

          c = to_char_type (i);
        }
      }

      return underflow ();
    }


    int_type
    handle_preprocessor_include ()
    {
      //out_put (std::string ("include"));

      skip_white_space ();

      std::string file;

      int_type i = in_get ();

      if (eos (i))
      {
        //@@ error
        return i;
      }

      char_type c = to_char_type (i);

      char_type finilizer;

      if (c == '\"') finilizer = c;
      else if (c == '<') finilizer = '>';
      else
      {
        //@@error
      }

      while (true)
      {
        i = in_get ();

        if (eos (i))
        {
          //@@ error
          return i;
        }

        c = to_char_type (i);

        if (c == finilizer) break;

        file += c;
      }

      //@@ it's probably a good idea to skip until the newline
      //   and perhaps check that all the rest is whitespace.

      if (finilizer == '>') out_put (std::string("sinclude \""));
      else out_put (std::string("include \""));

      out_put (file);
      out_put ("\";");

      return out_get ();
    }


  private:

    // Buffer manipulation

    int_type
    in_get ()
    {
      if (in_buffer_.empty ())
      {
        return is_.next ();
      }
      else
      {
        int_type i = in_buffer_.front ();
        in_buffer_.pop_front ();
        return i;
      }
    }

    void
    in_put (int_type c)
    {
      in_buffer_.push_back (c);
    }

    int_type
    out_get ()
    {
      if (out_buffer_.empty ())
      {
        return underflow ();
      }
      else
      {
        int_type i = out_buffer_.front ();
        out_buffer_.pop_front ();
        return i;
      }
    }

    void
    out_put (std::string const& str)
    {
      for (std::string::const_iterator i = str.begin (); i != str.end (); i++)
      {
        out_buffer_.push_back (*i);
      }
    }

    void
    out_put (int_type c)
    {
      out_buffer_.push_back (c);
    }

  private:
    enum State
    {
      PREPROCESSING,
      STRING_LITERAL,
      CHAR_LITERAL
    } state;

    std::locale loc_;
    TokenStream<char>& is_;
    std::deque<int_type> in_buffer_;
    std::deque<int_type> out_buffer_;
  };
}

#endif //CCF_PREPROCESSOR_HPP
