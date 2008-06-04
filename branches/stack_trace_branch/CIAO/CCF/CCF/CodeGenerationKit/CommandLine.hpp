// file      : CCF/CodeGenerationKit/CommandLine.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef COMMAND_LINE_HPP
#define COMMAND_LINE_HPP

#include <algorithm>
#include <vector>
#include <string>

//@@ this stuff needs proper reimplementation

class CommandLine
{
public:
  CommandLine ()
  {
  }

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

    if (i != options.end () && !(i->value ().empty ()))
    {
      return i->value ();
    }
    else
    {
      return not_found_value;
    }
  }

  // @@ the option should probably be searched in reverse order
  //

  std::string
  get_value (std::string name, char const* not_found_value) const
  {
    Options::const_iterator i = std::find_if (
      options.begin (),
      options.end (),
      OptionNamePredicat (name));

    if (i != options.end () && !(i->value ().empty ()))
    {
      return i->value ();
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
  
  bool 
  get_all_values (std::string name, std::vector<std::string> &values)
  {
    Options::iterator i (options.begin ());
    
    while ((i = std::find_if (i, 
                              options.end (), 
                              OptionNamePredicat (name))) != options.end ())
      {
        values.push_back (i->value ());
        ++i;
      }
    
    return values.size () != 0;
  }
  
  struct Option
  {
    Option (std::string const& name)
        : name_ (name)
    {
    }

    Option (std::string const& name,
            std::string const& value)
        : name_ (name),
          value_ (value)
    {
    }

    std::string const&
    name () const
    {
      return name_;
    }

    std::string const&
    value () const
    {
      return value_;
    }

  private:
    std::string name_;
    std::string value_;
  };

  typedef
  std::vector<Option>
  Options;

  typedef
  Options::const_iterator
  OptionsIterator;

  OptionsIterator
  options_begin () const
  {
    return options.begin ();
  }

  OptionsIterator
  options_end () const
  {
    return options.end ();
  }

  // Arguments
  //
  //

  typedef
  std::vector<std::string>
  Arguments;

  typedef
  Arguments::const_iterator
  ArgumentsIterator;

  ArgumentsIterator
  arguments_begin () const
  {
    return arguments.begin ();
  }

  ArgumentsIterator
  arguments_end () const
  {
    return arguments.end ();
  }

public:

  struct OptionNamePredicat
  {
    OptionNamePredicat (std::string const& name)
        : name_ (name)
    {
    }

    bool operator ()(Option const& option) throw ()
    {
      return name_ == option.name ();
    }

  private:
    std::string const name_;
  };

  std::string command;
  Options     options;
  Arguments   arguments;
};

#endif  // COMMAND_LINE_HPP
