// file      : CCF/CodeGenerationKit/IndentationXML.hpp
// author    : Diego Sevilla Ruiz <dsevilla@ditec.um.es>
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CODE_GENERATION_KIT_INDENTATION_XML_HPP
#define CCF_CODE_GENERATION_KIT_INDENTATION_XML_HPP

#include <deque>

#include "CCF/CodeGenerationKit/IndentationBuffer.hpp"

namespace Indentation
{
  template <typename C>
  class XML : public Buffer<C>
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
    XML (Buffer<C>& out)
        : out_ (out),
          indentation_ (0),
          base_indent_ (indentation_),
          spaces_ (2),
          construct_ (OTHER)
    {
    }

    virtual
    ~XML () throw () {}

  public:
    virtual int_type
    put (char_type c)
    {
      int_type result = traits_type::to_int_type (c);

      try
      {
        switch (c)
        {
        case '\n':
          {
            flush_buffer ();

            // If we are inside a tag, just output an artificial tab character
            if (construct_ == INSIDE_TAG)
            {
              hold_.push_back (c);
              c = traits_type::to_int_type (' ');

              // spaces_ - 1 because at the end the last space will be
              // put into the hold_ queue.
              for (unsigned long i = 0; i < spaces_ - 1; i++)
                hold_.push_back (c);
            }

            base_indent_ = indentation_;

            break;
          }

          // Case with <?xxx and <!DOCTYPE (for example).
          // We still don't handle XML comments
        case '?':
        case '!':
          {
            if (construct_ == INSIDE_TAG && hold_.back () == '<')
              indentation_--;

            break;
          }

        case '<':
          {
            //@@ This construct must not be INSIDE_TAG
            if (construct_ == OTHER)
            {
              construct_ = INSIDE_TAG;
              indentation_++;
            }

            break;
          }

        case '>':
          {
            if (construct_ == INSIDE_TAG)
              construct_ = OTHER;

            break;
          }

        case '/':
          {
            if (construct_ == INSIDE_TAG)
            {
              if (hold_.back () == '<')
                indentation_ -= 2;
              else
                indentation_--;

              if (indentation_ < 0)
                indentation_ = 0;
            }

            break;
          }

        case '\"':
          {
            if (construct_ == INSIDE_TAG)
              construct_ = STRING_LITERAL;
            else if (construct_ == STRING_LITERAL)
              construct_ = INSIDE_TAG;

            break;
          }

        }

        hold_.push_back (c);

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
      try
      {
        flush_buffer ();
      } catch (const Full&)
      {
        throw EndOfStream ("unable to flush buffer");
      }
    }

  private:
    class Full {};

    void
    flush_buffer ()
    {
      long delta = indentation_ - base_indent_;

      if (delta > 0)
        delta = 0;

      while (!hold_.empty ())
      {
        int_type c = hold_.front ();

        write (c);

        if (c == '\n')
          output_indentation (base_indent_ + delta);

        hold_.pop_front ();
      }
    }

    void
    output_indentation (unsigned long indentation)
    {
      for (unsigned long i = 0; i < indentation * spaces_; i++)
      {
        write (' ');
      }
    }

    int_type
    write (int_type c)
    {
      int_type result;

      result = out_.put (c);

      if (result == traits_type::eof ()) throw Full ();

      return result;
    }


  private:
    Buffer<C>& out_;
    long indentation_;
    unsigned long base_indent_;
    unsigned long spaces_;

    enum Construct
    {
      OTHER,
      /* Strings literals can only happen inside tags, like:
       * <tag whatever="xxx
       */
      STRING_LITERAL,
      INSIDE_TAG
    };

    Construct construct_;

    typedef
    std::deque<int_type>
    Hold;

    Hold hold_;
  };
}

#endif // CCF_CODE_GENERATION_KIT_INDENTATION_XML_HPP
