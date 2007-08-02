// file      : CCF/CompilerElements/Diagnostic.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_DIAGNOSTIC_HPP
#define CCF_DIAGNOSTIC_HPP

#include <string>
#include <sstream>
#include <iostream>

#include "CCF/CompilerElements/ExH.hpp"

//@@ Should be inside CCF namespace

namespace Diagnostic
{
  //
  //
  //
  class Record : public std::ostringstream
  {
  public:

    struct Type
    {
      enum Value
      {
        INFO,
        WARNING,
        ERROR
      };
    };

    Record (Type::Value type,
            std::string file,
            unsigned long line,
            std::string description = "")
        : std::ostringstream (description),
          type_ (type),
          file_ (file),
          line_ (line)
    {
    }

  public:

    Type::Value
    type () const
    {
      return type_;
    }

    std::string
    file () const
    {
      return file_;
    }

    unsigned long line () const
    {
      return line_;
    }

    std::string
    description () const
    {
      return str ();
    }

  private:
    Type::Value type_;
    std::string file_;
    unsigned long line_;
  };


  //
  //
  //
  struct Info : public Record
  {
    Info (std::string file,
          unsigned long line,
          std::string description = "")
        : Record (Type::INFO, file, line, description)
    {
    }
  };


  //
  //
  //
  struct Warning : public Record
  {
    Warning (std::string file,
             unsigned long line,
             std::string description = "")
        : Record (Type::WARNING, file, line, description)
    {
    }
  };


  //
  //
  //
  struct Error : public Record
  {
    Error (std::string file,
           unsigned long line,
           std::string description = "")
        : Record (Type::ERROR, file, line, description)
    {
    }
  };


  //
  //
  //
  class Stream
  {
  public:

    Stream ()
        : info_count_ (0),
          warning_count_ (0),
          error_count_ (0)
    {
    }


    Stream&
    operator<< (Record const& msg)
    {
      std::cerr << msg.file () << ":" << msg.line () << ": "
                << msg.description () << std::endl;

      switch (msg.type ())
      {
      case Record::Type::INFO:
        {
          info_count_++;
          break;
        }
      case Record::Type::WARNING:
        {
          warning_count_++;
        }
      case Record::Type::ERROR:
        {
          error_count_++;
        }
      }

      return *this;
    }

  public:
    unsigned long
    info_count ()
    {
      return info_count_;
    }

    unsigned long
    warning_coun ()
    {
      return warning_count_;
    }

    unsigned long
    error_count ()
    {
      return error_count_;
    }

  private:
    unsigned long info_count_;
    unsigned long warning_count_;
    unsigned long error_count_;

  private:
    // Copy semanic is not supported.
    Stream (Stream const&) throw ();
    Stream& operator= (Stream const&) throw ();
  };
}

#endif  // CCF_DIAGNOSTIC_HPP
