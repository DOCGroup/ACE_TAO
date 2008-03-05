// file      : CCF/CodeGenerationKit/IndentationIDL.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CODE_GENERATION_KIT_INDENTATION_IDL_HPP
#define CCF_CODE_GENERATION_KIT_INDENTATION_IDL_HPP

#include <deque>

#include "CCF/CodeGenerationKit/IndentationBuffer.hpp"

namespace Indentation
{
  template <typename C>
  class IDL : public Buffer<C>
  {
  public:
    typedef
    typename Buffer<C>::traits_type
    traits_type;

    typedef
    typename Buffer<C>::char_type
    char_type;

    typedef
    typename Buffer<C>::int_type
    int_type;

    typedef
    typename Buffer<C>::EndOfStream
    EndOfStream;

  public:
    IDL (Buffer<C>& out)
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
    put (char_type c)
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
              typename Hold::reverse_iterator i = hold_.rbegin ();
              if (*i == '\n' && *(i + 1) == '\n') hold_.pop_back ();
              else break;
            }

            ensure_new_line ();
            output_indentation ();

            hold_.push_back (c);

            // result = write (c);

            //ensure_new_line ();

            // Add double newline after '}'.
            //
            hold_.push_back ('\n');
            hold_.push_back ('\n');


            break;
          }
        case ';':
          {
            // Handling '};' case.
            //

            bool brace (false);

            if (hold_.size () > 1 && hold_.back () == '\n')
            {
              bool pop_nl (false);

              for (typename Hold::reverse_iterator
                     i (hold_.rbegin ()), e (hold_.rend ()); i != e; ++i)
              {
                if (*i != '\n')
                {
                  if (*i == '}') brace = pop_nl = true;
                  break;
                }
              }

              if (pop_nl) while (hold_.back () == '\n') hold_.pop_back ();
            }

            output_indentation ();
            result = write (c);

            if (brace)
            {
              hold_.push_back ('\n');
              hold_.push_back ('\n');
            }

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
    unbuffer ()
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
    output_indentation ()
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
    write (char_type c)
    {
      hold_.push_back (c);

      int_type result (traits_type::eof ());

      while (!hold_.empty ())
      {
        result = out_.put (hold_.front ());

        if (result == traits_type::eof ()) throw Full ();

        hold_.pop_front ();
      }

      return result;
    }


  private:
    Buffer<C>& out_;
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
