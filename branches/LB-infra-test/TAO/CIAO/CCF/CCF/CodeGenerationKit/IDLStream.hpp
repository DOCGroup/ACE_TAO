// $Id$

#include "MSVC_Pragmas.hpp"

#include <streambuf>
#include <iostream>

//@@ would be nice to teach this beast to handle double newlines
//   the idea is to convert double newline into single newline when
//   the next character is '}'

class IDLFormattingBuffer : public std::streambuf
{
public:
  IDLFormattingBuffer (std::streambuf* next)
      : after_nl_ (true),
        state_ (DECL_COMPLETE),
        indentation_ (0),
        next_ (next)
  {
    setbuf (0, 0);
  }

  std::streambuf*
  next () const
  {
    return next_;
  }



protected:

  class Full {};

protected:

  virtual int_type
  overflow (int_type c)
  {
    int_type result;

    try
    {
      bool defaulting = false;
      switch (c)
      {
      case '\n':
        {
          result = write (c);
          after_nl_ = true;
          break;
        }
      case '{':
        {
          output_indentation ();
          result = write (c);
          indentation_++;
          break;
        }
      case '}':
        {
          if (indentation_ > 0) indentation_--;
          output_indentation ();
          result = write (c);
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

  void
  output_indentation () throw (Full)
  {
    if (after_nl_)
    {
      for (unsigned long i = 0; i < indentation_; i++)
      {
        write (' ');
        write (' ');
      }
      after_nl_ = false;
    }
  }

  int_type
  write (int_type c) throw (Full)
  {
    int_type result = next_->sputc (c);

    if (result == traits_type::eof ()) throw Full ();

    return result;
  }


private:

  enum State
  {
    DECL_COMPLETE,
  };

  bool after_nl_;
  State state_;
  unsigned long indentation_;
  std::streambuf* next_;
};
