// file      : CCF/CodeGenerationKit/IndentationIDL.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CODE_GENERATION_KIT_INDENTATION_IDL_HPP
#define CCF_CODE_GENERATION_KIT_INDENTATION_IDL_HPP

#include <deque>

#include "CCF/CodeGenerationKit/IndentationBuffer.hpp"

namespace Indentation
{
  class IDL : public Buffer
  {
  public:
    IDL (Buffer& out)
        : out_ (out),
          indentation_ (0),
          spaces_ (2),
          construct_ (OTHER)
    {
    }

    virtual
    ~IDL () throw () {}

  public:
    virtual int_type
    put (char_type c) throw (ExH::System::Exception)
    {
      int_type result = traits_type::to_int_type (c);

      try
      {
        bool defaulting = false;
        switch (c)
        {
        case '\n':
          {
            hold_.push_back (c);
            break;
          }
        case '{':
          {
            ensure_new_line ();
            output_indentation ();
            result = write (c);
            ensure_new_line ();
            indentation_++;
            break;
          }
        case '}':
          {
            if (indentation_ > 0) indentation_--;

            // Reduce multiple newlines to one.
            while (hold_.size () > 1)
            {
              Hold::reverse_iterator i = hold_.rbegin ();
              if (*i == '\n' && *(i + 1) == '\n') hold_.pop_back ();
              else break;
            }

            ensure_new_line ();
            output_indentation ();
            result = write (c);
            break;
          }
        case ';':
          {
            output_indentation ();
            result = write (c);

            if (construct_ != STRING_LITERAL && construct_ != CHAR_LITERAL)
            {
              ensure_new_line ();
            }
            break;
          }
        case '\\':
          {
            hold_.push_back (c);
            break;
          }
        case '\"':
          {
            if (hold_.empty () || hold_.back () != '\\')
            {
              // not escape sequence
              if (construct_ == STRING_LITERAL) construct_ = OTHER;
              else construct_ = STRING_LITERAL;
            }

            defaulting = true;
            break;
          }
        case '\'':
          {
            if (hold_.empty () || hold_.back () != '\\')
            {
              // not escape sequence
              if (construct_ == CHAR_LITERAL) construct_ = OTHER;
              else construct_ = CHAR_LITERAL;
            }

            defaulting = true;
            break;
          }
        default:
          {
            defaulting = true;
            break;
          }
        }

        if (defaulting)
        {
          output_indentation ();
          result = write (c);
        }
      }
      catch (Full const&)
      {
        result = traits_type::eof ();
      }

      return result;
    }

    virtual void
    unbuffer () throw (EndOfStream, ExH::System::Exception)
    {
      int_type result;

      while (!hold_.empty ())
      {
        result = out_.put (hold_.front ());

        //@@ failed
        if (result == traits_type::eof ())
        {
          throw EndOfStream ("unable to flush buffer");
        }

        hold_.pop_front ();
      }
    }

  private:
    class Full {};

    void
    ensure_new_line ()
    {
      if (hold_.empty () || hold_.back () != '\n')
      {
        hold_.push_back ('\n');
      }
    }


    void
    output_indentation () throw (Full)
    {
      if (!hold_.empty () && hold_.back () == '\n')
      {
        for (unsigned long i = 0; i < indentation_ * spaces_; i++)
        {
          write (' ');
        }
      }
    }

    int_type
    write (char_type c) throw (Full)
    {
      hold_.push_back (c);

      int_type result;

      while (!hold_.empty ())
      {
        result = out_.put (hold_.front ());

        if (result == traits_type::eof ()) throw Full ();

        hold_.pop_front ();
      }

      return result;
    }


  private:
    Buffer& out_;
    unsigned long indentation_;
    unsigned long spaces_;

    bool suppress_nl_;

    enum Construct
    {
      OTHER,
      STRING_LITERAL,
      CHAR_LITERAL
    };

    Construct construct_;

    typedef
    std::deque<int_type>
    Hold;

    Hold hold_;
  };
}

#endif // CCF_CODE_GENERATION_KIT_INDENTATION_IDL_HPP
