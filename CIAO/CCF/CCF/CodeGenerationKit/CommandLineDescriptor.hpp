// file      : CCF/CodeGenerationKit/CommandLineDescriptor.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef COMMAND_LINE_DESCRIPTOR_H
#define COMMAND_LINE_DESCRIPTOR_H

#include <map>
#include <list>
#include <vector>
#include <string>
#include <ostream>
#include <cassert>


namespace CL
{
  struct OptionType
  {
    enum Value
    {
      flag,
      value
    };

    OptionType (Value v)
        : v_ (v)
    {
    }

    operator Value () const
    {
      return v_;
    }

  private:
    Value v_;
  };


  class OptionDescription
  {
  public:
    OptionDescription (std::string name,
                       std::string description,
                       OptionType type,
                       bool optional = true)
        : optional_ (optional),
          type_ (type),
          name_ (name),
          value_synopsis_ (),
          description_ (description)
    {
      // There should be value_synopsis for non-flag options.
      //
      assert (type == OptionType::flag);
    }


    OptionDescription (std::string name,
                       std::string value_synopsis,
                       std::string description,
                       OptionType type,
                       bool optional = true)
        : optional_ (optional),
          type_ (type),
          name_ (name),
          value_synopsis_ (value_synopsis),
          description_ (description)
    {
    }

  public:
    bool
    optional () const
    {
      return optional_;
    }

    OptionType
    type () const
    {
      return type_;
    }

    std::string
    name () const
    {
      return name_;
    }

    std::string
    value_synopsis () const
    {
      return value_synopsis_;
    }

    std::string
    description () const
    {
      return description_;
    }


  private:
    bool optional_;
    OptionType type_;
    std::string name_;
    std::string value_synopsis_;
    std::string description_;
  };


  class Description
  {
  public:
    Description (std::string command)
        : command_ (command)
    {
    }

  private:
    Description (Description const&);

    Description&
    operator= (Description const&);

  private:
    // We are storing pointer to elements in this list in the map below.
    // To prevent element moving we will use list instead of vector.
    //
    typedef
    std::list<OptionDescription>
    OptionDescriptionList;

    typedef
    std::map<std::string, OptionDescription*>
    OptionDescriptionMap;

  public:

    std::string
    command () const
    {
      return command_;
    }


  public:
    typedef
    OptionDescriptionList::const_iterator
    OptionIterator;

    OptionIterator
    begin_option () const
    {
      return options_.begin ();
    }

    OptionIterator
    end_option () const
    {
      return options_.end ();
    }

    typedef
    OptionDescriptionMap::const_iterator
    OptionMapIterator;

    OptionDescription const*
    find_option (std::string const& option) const
    {
      OptionDescriptionMap::const_iterator i  (
        option_map_.find (option));

      return i == option_map_.end () ? 0 : i->second;
    }

    void
    add_option (OptionDescription const& od)
    {
      options_.push_back (od);
      option_map_[od.name ()] = &options_.back ();
    }

  private:
    typedef
    std::vector<std::string>
    ArgumentDescriptionList;

  public:
    typedef
    ArgumentDescriptionList::const_iterator
    ArgumentIterator;

    ArgumentIterator
    begin_argument () const
    {
      return arguments_.begin ();
    }

    ArgumentIterator
    end_argument () const
    {
      return arguments_.end ();
    }

    void
    add_argument (std::string arg)
    {
      arguments_.push_back (arg);
    }

  private:
    std::string command_;
    OptionDescriptionList options_;
    OptionDescriptionMap option_map_;
    ArgumentDescriptionList arguments_;
  };

  inline void
  print_text (std::ostream& os, Description const& d)
  {
    using std::endl;

    os << d.command ();

    Description::OptionIterator ob = d.begin_option ();
    Description::OptionIterator oe = d.end_option ();

    if (ob != oe)
    {
      os << " { OPTIONS }";
    }

    Description::ArgumentIterator ab = d.begin_argument ();
    Description::ArgumentIterator ae = d.end_argument ();

    for (; ab != ae; ab++)
    {
      os << " <" << *ab << ">";
    }

    os << endl << endl;

    for (; ob != oe; ob++)
    {
      bool flag (ob->type () == OptionType::flag);
      bool optional (ob->optional ());
      std::string prefix (ob->name ().length () == 1 ? "-" : "--");

      os << (optional ? "[" : "") << prefix << ob->name ()
         << (flag ? "" : " ") << ob->value_synopsis ()
         << (optional ? "]" : "")
         << endl;

      os << "\t\t" << ob->description () << endl << endl;
    }

  }


  inline void
  print_html (std::ostream& os, Description const& d)
  {
    using std::endl;

    os << "<html>" << endl
       << "<body>" << endl
       << "<div align=\"center\">" << endl
       << "<table width=\"700\" border=\"0\" cellspacing=\"0\" "
       << "cellpadding=\"0\">" << endl
       << "<tr>" << endl
       << "<td>" << endl;

    os << "<p>" << endl
       << "<code>" << endl;

    os << d.command ();

    Description::OptionIterator ob = d.begin_option ();
    Description::OptionIterator oe = d.end_option ();

    if (ob != oe)
    {
      os << " { OPTIONS }";
    }

    Description::ArgumentIterator ab = d.begin_argument ();
    Description::ArgumentIterator ae = d.end_argument ();

    for (; ab != ae; ab++)
    {
      os << " &lt;" << *ab << "&gt;";
    }

    os << endl
       << "</code>" << endl
       << "</p>" << endl;


    os << "<dl>" << endl;

    for (; ob != oe; ob++)
    {
      bool flag (ob->type () == OptionType::flag);
      bool optional (ob->optional ());
      std::string prefix (ob->name ().length () == 1 ? "-" : "--");

      os << "<dt>" << endl
         << "<code>" << endl
         << (optional ? "[" : "") << prefix << ob->name ()
         << (flag ? "" : " ") << ob->value_synopsis ()
         << (optional ? "]" : "") << endl
         << "</code>" << endl
         << "</dt>" << endl;

      os << "<dd>" << endl
         << "<p>" << endl
         << ob->description () << endl
         << "</p>" << endl
         << "</dd>" << endl;
    }

    os << "</dl>" << endl;

    os << "</td>" << endl
       << "</tr>" << endl
       << "</table>" << endl
       << "</div>" << endl
       << "</body>" << endl
       << "</html>" << endl;

  }
}


#endif  // COMMAND_LINE_DESCRIPTOR_H
