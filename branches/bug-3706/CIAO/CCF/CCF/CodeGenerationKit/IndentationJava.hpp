// file      : CCF/CodeGenerationKit/IndentationJava.hpp
// author    : James H. Hill <hillj@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CODE_GENERATION_KIT_INDENTATION_JAVA_HPP
#define CCF_CODE_GENERATION_KIT_INDENTATION_JAVA_HPP

#include <deque>
#include <stack>

#include "CCF/CodeGenerationKit/IndentationBuffer.hpp"

namespace Indentation
{
  template <typename C>
  class Java : public Buffer<C>
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
    Java (Buffer<C>& out)
        : out_ (out),
          position_ (0),
          paren_balance_ (0),
          spaces_ (2),
          construct_ (OTHER)
    {
      indentation_.push (0);
    }

    virtual
    ~Java () throw () {}

  public:
    virtual int_type
    put (char_type c)
    {
      int_type result = traits_type::to_int_type (c);

      try
      {
        bool defaulting = false;

        if (!hold_.empty () && hold_.back () == '(')
        {
          // We don't need to hold it any more.
          unbuffer ();

          if (c == '\n')
            indentation_.push (indentation_.top () + spaces_);
          else
            indentation_.push (position_);
        }

        switch (c)
        {
        case '\n':
          {
            hold_.push_back (c);
            position_ = 0; // Starting a new line.

            if (construct_ == JAVA_COMMENT)
            {
              //std::cerr << "end comment" << endl;
              construct_ = OTHER;
            }

            break;
          }
        case '{':
          {
            ensure_new_line ();
            output_indentation ();
            result = write (c);
            ensure_new_line ();

            indentation_.push (indentation_.top () + spaces_);

            break;
          }
        case '}':
          {
            if (indentation_.size () > 1)
              indentation_.pop ();

            // Reduce multiple newlines to one.
            while (hold_.size () > 1)
            {
              typename Hold::reverse_iterator i = hold_.rbegin ();

              if (*i == '\n' && *(i + 1) == '\n')
                hold_.pop_back ();
              else
                break;
            }

            ensure_new_line ();
            output_indentation ();

            hold_.push_back (c);

            // Add newline after '}'.
            //
            ensure_new_line ();
            break;
          }
        case ';':
          {
            if (paren_balance_ != 0)
            {
              // We are inside for (;;) statement. Nothing to do here.
              //
              defaulting = true;
            }
            else if (construct_ != STRING_LITERAL && construct_ != CHAR_LITERAL)
            {
              output_indentation ();
              result = write (c);
              ensure_new_line ();
            }

            break;
          }
        case '\\':
          {
            if (construct_ != JAVA_COMMENT)
            {
              output_indentation ();
              hold_.push_back (c);
              position_++;
            }
            else
              defaulting = true;

            break;
          }
        case '\"':
          {
            if (construct_ != JAVA_COMMENT &&
                (hold_.empty () || hold_.back () != '\\'))
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
            if (construct_ != JAVA_COMMENT &&
                (hold_.empty () || hold_.back () != '\\'))
            {
              // not escape sequence
              if (construct_ == CHAR_LITERAL) construct_ = OTHER;
              else
                {
                  //std::cerr << "char literal" << endl;
                  construct_ = CHAR_LITERAL;
                }

            }

            defaulting = true;
            break;
          }
        case '(':
          {
            if (construct_ == OTHER)
            {
              // Hold it so that we can see what's coming next.
              //
              output_indentation ();
              hold_.push_back (c);
              position_++;
              paren_balance_++;
            }
            else
              defaulting = true;

            break;
          }
        case ')':
          {
            if (construct_ == OTHER)
            {
              if (indentation_.size () > 1)
                indentation_.pop ();

              if (paren_balance_ > 0)
                paren_balance_--;
            }

            defaulting = true;
            break;
          }
        case '/':
          {
            if (construct_ == OTHER)
            {
              if (!hold_.empty () && hold_.back () == '/')
              {
                construct_ = JAVA_COMMENT;
                //std::cerr << "start comment" << endl;
                defaulting = true;
              }
              else
              {
                output_indentation ();
                hold_.push_back (c);
                position_++;
              }
            }
            else
            {
              defaulting = true;
            }

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
          position_++;
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
        position_ = 0; // Starting a new line.
      }
    }


    void
    output_indentation ()
    {
      if (!hold_.empty () && hold_.back () == '\n')
      {
        unsigned long indent = indentation_.top ();

        for (unsigned long i = 0; i < indent; i ++)
          write (' ');

        position_ += indentation_.top ();
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

        if (result == traits_type::eof ())
          throw Full ();

        hold_.pop_front ();
      }

      return result;
    }


  private:
    Buffer<C>& out_;
    unsigned long position_; // Current position on the line.
    unsigned long paren_balance_; // ( ) balance.
    std::stack<unsigned long> indentation_;
    unsigned long spaces_;

    bool suppress_nl_;

    enum Construct
    {
      OTHER,
      JAVA_COMMENT,
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
