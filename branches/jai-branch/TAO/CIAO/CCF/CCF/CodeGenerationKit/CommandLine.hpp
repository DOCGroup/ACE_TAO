// File   : CommandLine.hpp
// Author : Boris Kolpackov <boris@dre.vanderbilt.edu>
// $Id$

#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include "MSVC_Pragmas.hpp"

#include <algorithm>
#include <vector>
#include <string>

using namespace std;

//@@ this stuff needs proper reimplementation

class CommandLine
{
public:

  CommandLine () throw () : separator (false) {}


  // Option constrain checking
public:

  bool
  require (std::string option) const throw ()
  {
    return std::find_if (options.begin (),
                         options.end (),
                         OptionNamePredicat (option)) != options.end ();
  }

  bool
  depends (std::string dependant,
           std::string principal) const throw ()
  {
    Options::const_iterator begin = options.begin ();
    Options::const_iterator end   = options.end ();

    if (std::find_if (begin, end, OptionNamePredicat (dependant)) != end)
    {
      return std::find_if (begin, end, OptionNamePredicat (principal)) != end;
    }
    else
    {
      return true;
    }
  }

  bool
  inconsistent (std::string a,
                std::string b) const throw ()
  {
    Options::const_iterator begin = options.begin ();
    Options::const_iterator end   = options.end ();

    if (std::find_if (begin, end, OptionNamePredicat (a)) != end)
    {
      return std::find_if (begin, end, OptionNamePredicat (b)) == end;
    }
    else
    {
      return true;
    }
  }

  std::string
  get_value (std::string name, std::string const& not_found_value) const
  {
    Options::const_iterator i = std::find_if (
      options.begin (),
      options.end (),
      OptionNamePredicat (name));

    if (i != options.end () && !(i->value_.empty ()))
    {
      return i->value_;
    }
    else
    {
      return not_found_value;
    }
  }

  std::string
  get_value (std::string name, char const* not_found_value) const
  {
    Options::const_iterator i = std::find_if (
      options.begin (),
      options.end (),
      OptionNamePredicat (name));

    if (i != options.end () && !(i->value_.empty ()))
    {
      return i->value_;
    }
    else
    {
      return std::string (not_found_value);
    }
  }

  bool
  get_value (std::string name, bool not_found_value) const
  {
    Options::const_iterator i = std::find_if (
      options.begin (),
      options.end (),
      OptionNamePredicat (name));

    if (i != options.end ())
    {
      return true;
    }
    else
    {
      return not_found_value;
    }
  }

  // Arguments
  //
  //

  typedef
  std::vector<std::string>
  Arguments;

  typedef
  Arguments::const_iterator
  ArgumentIterator;

  ArgumentIterator
  argument_begin () const
  {
    return arguments.begin ();
  }


  ArgumentIterator
  argument_end () const
  {
    return arguments.end ();
  }


public:

  struct Option
  {
    enum OptionType
    {
      EQUATIONAL,
      COMPOSITE
    };

    Option (OptionType type,
            std::string const& name,
            std::string const& value) throw ()
        : type_ (type),
          name_ (name),
          value_ (value)
    {
    }

    OptionType  type_;
    std::string name_;
    std::string value_;
  };



  struct OptionNamePredicat
  {
    OptionNamePredicat (std::string const& name)
        : name_ (name)
    {
    }

    bool operator ()(Option const& option) throw ()
    {
      return name_ == option.name_;
    }

  private:
    std::string const name_;
  };

  typedef std::vector<Option> Options;
  typedef std::vector<std::string> Arguments;

  std::string command;
  Options     options;
  Arguments   arguments;

  bool        separator;
};

#endif  // COMMAND_LINE_H
