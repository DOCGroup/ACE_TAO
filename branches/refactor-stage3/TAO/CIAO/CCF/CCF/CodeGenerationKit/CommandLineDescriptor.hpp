// $Id$
#ifndef COMMAND_LINE_DESCRIPTOR_H
#define COMMAND_LINE_DESCRIPTOR_H

#include <string>
#include <vector>
#include <ostream>

//@@ temporary. should probably be changed to CommandLine
namespace CL
{
  class OptionDescription
  {
  public:
    OptionDescription (std::string name,
                       std::string description,
                       bool optional)
      : optional_ (optional),
        name_ (name),
        value_synopsis_ (),
        description_ (description)
    {
    }
    
    
    OptionDescription (std::string name,
                       std::string value_synopsis,
                       std::string description,
                       bool optional)
        : optional_ (optional),
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
    typedef
    std::vector<OptionDescription>
    OptionDescriptionList;

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

    void
    add_option (OptionDescription const& od)
    {
      options_.push_back (od);
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
    ArgumentDescriptionList arguments_;
  };
  
  inline void
  print_text (std::ostream& os, Description const& d)
  {
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
      os << (ob->optional () ? "[--" : "--")
         << ob->name ()
         << (ob->value_synopsis ().empty () ? "" : " ")
         << ob->value_synopsis ()
         << (ob->optional () ? "]" : "")
         << endl;

      os << "\t\t" << ob->description () << endl;
    }
    
  }


  inline void
  print_html (std::ostream& os, Description const& d)
  {
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
      os << "<dt>" << endl
         << "<code>" << endl
         << (ob->optional () ? "[--" : "--")
         << ob->name ()
         << (ob->value_synopsis ().empty () ? "" : " ")
         << ob->value_synopsis ()
         << (ob->optional () ? "]" : "") << endl
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
