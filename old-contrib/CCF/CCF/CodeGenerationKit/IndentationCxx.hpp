// file      : CCF/CodeGenerationKit/IndentationCxx.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CODE_GENERATION_KIT_INDENTATION_CXX_HPP
#define CCF_CODE_GENERATION_KIT_INDENTATION_CXX_HPP

#include <deque>
#include <stack>

#include "CCF/CodeGenerationKit/IndentationBuffer.hpp"

namespace Indentation
{
  /**
   * @class Cxx
   *
   * Indentation implementation for C++ files.
   */
  template <typename C>
  class Cxx : public Buffer <C>
  {
  public:
    /// Type definition of the traits type.
    typedef typename Buffer <C>::traits_type traits_type;

    /// Type definition of the char type.
    typedef typename Buffer <C>::char_type char_type;

    /// Type definition of the integer type
    typedef typename Buffer <C>::int_type int_type;

    /// Type definition of the end of stream.
    typedef typename Buffer <C>::EndOfStream EndOfStream;

  public:
    /**
     * Initializing constructor
     *
     * @param[in]           out       Target output buffer
     */
    Cxx (Buffer <C> & out)
    : out_ (out),
      position_ (0),
      paren_balance_ (0),
      spaces_ (2),
      construct_ (OTHER)
    {
      indentation_.push (0);
    }

    /// Destructor
    virtual ~Cxx (void)
    {
    }

  public:
    /**
     * Add a new character to the buffer. Based on the character, different
     * actions are taken. The newline always causes the buffer to be flushed.
     */
    virtual int_type put (char_type c)
    {
      int_type result = traits_type::to_int_type (c);

      try
      {
        bool defaulting = false;

        if (!this->hold_.empty () && this->hold_.back () == '(')
        {
          // We do not need the buffered elements anymore.
          this->unbuffer ();

          if (c == '\n')
            // Add another level to the indentation.
            this->indentation_.push (this->indentation_.top () + this->spaces_);
          else
            // Save the current position.
            this->indentation_.push (this->position_);
        }

        switch (c)
        {
        case '\n':
          // Save the eol character and start a new line.
          this->hold_.push_back (c);
          this->position_ = 0;

          if (this->construct_ == CXX_COMMENT)
            this->construct_ = OTHER;

          break;

        case '{':
          if (this->construct_ != CXX_COMMENT &&
              this->construct_ != STRING_LITERAL)
          {
            // Make sure there is a new line in the buffer. We then need to
            // move the correct position on the newline.
            this->ensure_new_line ();
            this->output_indentation ();
            result = this->write (c);

            // Make sure there is a newline after the brace.
            this->ensure_new_line ();

            // Increase the indentation by one.
            this->indentation_.push (this->indentation_.top () + this->spaces_);
          }
          else
            defaulting = true;

          break;

        case '}':
          if (!(this->construct_ == CXX_COMMENT || this->construct_ == STRING_LITERAL))
          {
            // Remove the current indentation value, if it exists.
            if (this->indentation_.size () > 1)
              this->indentation_.pop ();

            // Reduce multiple newlines to one.
            while (this->hold_.size () > 1)
            {
              typename Hold::reverse_iterator i = this->hold_.rbegin ();

              if (*i == '\n' && *(i + 1) == '\n')
                this->hold_.pop_back ();
              else
                break;
            }

            // Make sure we are on a newline and that we are at the
            // right position.
            this->ensure_new_line ();
            this->output_indentation ();

            // Now, we can add the new character to the newline.
            this->hold_.push_back (c);


            // Add double newline after '}'.
            this->hold_.push_back ('\n');
            this->hold_.push_back ('\n');

            // Set position to the start of the line.
            this->position_ = 0;
          }
          else
            defaulting = true;

          break;

        case ';':
          if (this->paren_balance_ != 0)
          {
            // We are inside for (;;) statement. Nothing to do here.
            defaulting = true;
          }
          else
          {
            // Handling the '};' case, i.e., complex type definitions.
            bool brace = false;

            if (this->hold_.size () > 1 && this->hold_.back () == '\n')
            {
              bool pop_nl = false;
              typedef typename Hold::reverse_iterator iterator;

              for (iterator i = this->hold_.rbegin (),
                   e = this->hold_.rend ();
                   i != e; ++ i)
              {
                if (*i != '\n')
                {
                  if (*i == '}')
                    brace = pop_nl = true;

                  break;
                }
              }

              if (pop_nl)
              {
                while (this->hold_.back () == '\n')
                  this->hold_.pop_back ();
              }
            }

            // Make sure we write the indentation.
            this->output_indentation ();
            result = this->write (c);

            // Update the position
            this->position_ ++;

            if (brace)
            {
              this->hold_.push_back ('\n');
              this->hold_.push_back ('\n');
            }

            if (this->construct_ != STRING_LITERAL && this->construct_ != CHAR_LITERAL)
              this->ensure_new_line ();
          }
          break;

        case '\\':
          if (this->construct_ != CXX_COMMENT)
          {
            this->output_indentation ();
            this->hold_.push_back (c);

            // Increment the position marker.
            ++ this->position_;
          }
          else
            defaulting = true;

          break;

        case '"':
          if (this->construct_ != CXX_COMMENT &&
             (this->hold_.empty () || this->hold_.back () != '\\'))
          {
            // not escape sequence
            if (this->construct_ == STRING_LITERAL)
              this->construct_ = OTHER;
            else
              this->construct_ = STRING_LITERAL;
          }

          defaulting = true;
          break;

        case '\'':
          if (this->construct_ != CXX_COMMENT &&
              this->construct_ != STRING_LITERAL &&
             (this->hold_.empty () || this->hold_.back () != '\\'))
          {
            // not escape sequence
            if (this->construct_ == CHAR_LITERAL)
              this->construct_ = OTHER;
            else
              this->construct_ = CHAR_LITERAL;
          }

          defaulting = true;
          break;

        case '(':
          if (this->construct_ == OTHER)
          {
            // Hold it so that we can see what's coming next.
            this->output_indentation ();
            this->hold_.push_back (c);

            // Update the position by one space to account for the
            // location of this parenthesis.
            this->position_ ++;

            // We must balance a new set of parenthesis.
            this->paren_balance_ ++;
          }
          else
            defaulting = true;

          break;

        case ')':
          if (this->construct_ == OTHER)
          {
            // We can remove the current indentation.
            if (this->indentation_.size () > 1)
              this->indentation_.pop ();

            // We have balanced one of the parenthesis.
            if (this->paren_balance_ > 0)
              this->paren_balance_ --;
          }

          defaulting = true;
          break;

        case '/':
          if (this->construct_ == OTHER)
          {
            if (!this->hold_.empty () && this->hold_.back () == '/')
            {
              // We are starting a C++ comment.
              this->construct_ = CXX_COMMENT;
              defaulting = true;
            }
            else
            {
              // This is just another character. We can just write it to
              // the holding buffer.
              this->output_indentation ();
              this->hold_.push_back (c);

              // Update the position.
              ++ this->position_;
            }
          }
          else
            defaulting = true;

          break;

        default:
          defaulting = true;
        }

        if (defaulting)
        {
          // Write the character to the holding buffer.
          this->output_indentation ();
          result = this->write (c);

          // Update the position.
          ++ this->position_;
        }
      }
      catch (const Full &)
      {
        result = traits_type::eof ();
      }

      return result;
    }

    /**
     * Unbuffer the elements. This will remove the elements on
     * the holding queue to the target output stream.
     */
    virtual void unbuffer (void)
    {
      int_type result;

      while (!this->hold_.empty ())
      {
        result = this->out_.put (this->hold_.front ());

        if (result == traits_type::eof ())
          throw EndOfStream ("unable to flush buffer");

        this->hold_.pop_front ();
      }
    }

  private:
    class Full {};

    /**
     * Helper method that makes sure a newline exists at the end
     * of the holding buffer.
     */
    void ensure_new_line (void)
    {
      if (this->hold_.empty () || hold_.back () != '\n')
      {
        // Insert a new line in the holding buffer.
        this->hold_.push_back ('\n');

        // Update the position to the start of the line.
        this->position_ = 0;
      }
    }

    /**
     * Helper method that writes the current indentation to the
     * output buffer.
     */
    void output_indentation (void)
    {
      if (!this->hold_.empty () && this->hold_.back () == '\n')
      {
        for (unsigned long i = 0; i < this->indentation_.top (); i++)
          this->write (' ');

        this->position_ += this->indentation_.top ();
      }
    }

    /**
     * Write a new character to the holding buffer.
     *
     * @param[in]       c       The new character.
     */
    int_type write (char_type c)
    {
      this->hold_.push_back (c);
      int_type result (traits_type::eof ());

      while (!this->hold_.empty ())
      {
        result = this->out_.put (this->hold_.front ());

        if (result == traits_type::eof ())
          throw Full ();

        this->hold_.pop_front ();
      }

      return result;
    }

  private:
    /// Target output buffer.
    Buffer <C> & out_;

    /// Current position on the line
    unsigned long position_;

    /// Balance the parenthesis
    unsigned long paren_balance_;

    /// Saved indentations based on state/scope.
    std::stack <size_t> indentation_;

    /// Number of space in a indentation.
    unsigned long spaces_;

    /// Suppress newline generation.
    bool suppress_nl_;

    enum Construct
    {
      OTHER,
      CXX_COMMENT,
      STRING_LITERAL,
      CHAR_LITERAL
    };

    /// The type of construct.
    Construct construct_;

    /// Type definition of the holding queue.
    typedef std::deque <int_type> Hold;

    /// Holding queue for elements.
    Hold hold_;
  };
}

#endif // CCF_CODE_GENERATION_KIT_INDENTATION_IDL_HPP
