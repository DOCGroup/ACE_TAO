// File   : CommandLineGrammar.hpp
// Author : Boris Kolpackov <boris@dre.vanderbilt.edu>
// $Id$

#ifndef COMMAND_LINE_GRAMMAR_H
#define COMMAND_LINE_GRAMMAR_H

#include <vector>
#include <string>

#include <boost/spirit.hpp>

#include "CCF/CodeGenerationKit/CommandLine.hpp"


using namespace std;
using namespace boost::spirit;

template <typename S>
struct string_directive : public unary<S, parser<string_directive<S> > >
{
  typedef
  string_directive
  self_t;

  string_directive (S const& a)
      : unary<S, parser<string_directive<S> > > (a)
  {
  }

  template <typename ScannerT>
  typename parser_result<self_t, ScannerT>::type
  parse(ScannerT const& scan) const
  {
    typedef
    typename ScannerT::iterator_t
    Iterator;
    
    typedef
    typename ScannerT::value_t
    Value;
    
    typedef
    typename Value::const_iterator
    ValueIterator;

    Iterator save = scan.first;

    int len = 0;

    if (!scan.at_end())
    {
      // Now go one level deeper

      Value const& v = *scan;

      ValueIterator first = v.begin ();
      ValueIterator last  = v.end ();

      scanner<ValueIterator, scanner_policies <> > sl_scan(first, last);

      match<nil_t> hit = this->subject ().parse(sl_scan);

      if (hit.length() == v.length ())
      {
        ++scan;

        return scan.create_match(1, nil_t(), save, scan.first);
      }
      else
      {
        return scan.no_match();
      }
    }
    else
    {
      return scan.no_match();
    }
  }
};


template <typename S>
string_directive<S>
string_d (parser<S> const& a)
{
  return string_directive<S>(a.derived());
}


struct CLineGrammar : public grammar<CLineGrammar>
{
  CommandLine& cl_;

  CLineGrammar (CommandLine& cl) throw () : cl_(cl) {}


  template <typename IteratorT, typename CodeT>
  struct SemanticAction
  {
    SemanticAction (CommandLine& cl) : cl_ (cl), code_ () {}

    void operator()(IteratorT first, IteratorT last) const
    {
      string str (first, last);
      code_.execute (cl_, str);
    }

  private:
    CommandLine& cl_;
    CodeT        code_;
  };


  struct SetCommandName
  {
    void execute (CommandLine& cl, string const& s) const throw ()
    {
      cl.command = s;
    }
  };


  template <typename IteratorT>
  struct SetOptionName
  {
    SetOptionName (CommandLine& cl,
                    CommandLine::Option::OptionType type)
        : cl_ (cl), type_ (type)
    {
    }

    void operator()(IteratorT first, IteratorT last) const
    {
      string s (first, last);
      cl_.options.push_back (CommandLine::Option(type_, s, ""));
    }

  private:
    CommandLine& cl_;
    CommandLine::Option::OptionType type_;
  };


  struct SetOptionValue
  {
    void execute (CommandLine& cl, string const& s) const throw ()
    {
      CommandLine::Options::reverse_iterator r = cl.options.rbegin ();

      if (r != cl.options.rend () && r->value_ == "")
      {
        r->value_ = s;
      }
      else
      {
        //@@
        //throw
      }
    }
  };

  struct SetArgument
  {
    void execute (CommandLine& cl, string const& s) const throw ()
    {
      cl.arguments.push_back (s);
    }
  };

  struct SetSeparator
  {
    void execute (CommandLine& cl, string const& s) const throw ()
    {
      cl.separator = true;
    }
  };


  template <typename ScannerT>
  struct definition
  {
    typedef
    typename ScannerT::value_t::const_iterator
    SLIterator;
    
    typedef
    scanner<SLIterator, scanner_policies <> >
    SLScanner;

    typedef
    rule<ScannerT>
    Rule;
    
    typedef
    rule<SLScanner>
    SLRule;


    Rule r_argument;
    Rule r_arg_list;

    Rule argument;
    Rule arg_list;

    SLRule option_prefix;
    SLRule e_name;   // equational name, i.e. can meet '='
    SLRule c_name;   // composite name, i.e. failes if meets '='
    SLRule e_value;  // equational value, i.e. can contain leading '-'
    SLRule c_value;  // composite value, i.e. cannot contain leading '-'

    Rule  composite;
    Rule  equational;

    Rule  option_list;
    Rule  option;

    SLRule command_name;

    Rule  command_line;

    // Semantic actions

    typedef
    SemanticAction<SLIterator, SetCommandName>
    CommandNameHandler;
    
    typedef
    SetOptionName<SLIterator>
    OptionNameHandler;
    
    typedef
    SemanticAction<SLIterator, SetOptionValue>
    OptionValueHandler;
    
    typedef
    SemanticAction<SLIterator, SetArgument>
    ArgumentHandler;
    
    typedef
    SemanticAction<SLIterator, SetSeparator>
    SeperatorHandler;

    /*

                 <epigraph>

                 As many people naively believe command line is
                 not a space separated array of words.

                                     -- unknown author.
                 </epigraph>



      NOTE:

       (1) This grammar is space-sensitive.

       (2) Anychar is assumend not to clash with <argv-delim>. Instead
           <argv-delim> is assumed to be a next-dimension entitiy.


     <command-line> ::= <command-name>
                        [
                          [ <argv-delim> <option-list>]
                          [
                            <option-prefix> [<argv-delim> <arg-list>] |
                            <argv-delim> <r-arg-list>
                          ]
                        ]

     <command-name> ::= anychar+

      <option-list> ::= <option> { <argv-delim> <option> }*
           <option> ::= <composite> | <equational>
        <composite> ::= <option-prefix> <name> [ <argv-delim><c-value> ]
       <equational> ::= <option-prefix> <name> '=' <e-value>
    <option-prefix> ::= '-' [ '-' ]
           <name>   ::= {anychar - '='}+
          <c-value> ::= anychar+ - {<option-prefix> anychar*}
          <e-valie> ::= anychar+

         <arg-list> ::= <argument> { <arg-delim> <argument> }*
       <r-arg-list> ::= <r-argument> { <arg-delim> <argument> }*

         <argument> ::= anychar+
       <r-argument> ::= anychar+ - {<option-prefix> anychar*}


    */


    definition(CLineGrammar const& self)
    {
      command_line =

        string_d (command_name)   >>
        !option_list              >>
        !(
          (
            string_d
            (
              option_prefix[ SeperatorHandler (self.cl_) ]
            ) >> !arg_list
          ) | r_arg_list
        );

      // command name

      command_name = (+anychar_p)[CommandNameHandler (self.cl_)];

      // options

      option_list  = *(option);

      option       = composite | equational;

      composite    =

        string_d
        (
          option_prefix >> c_name

        ) >> !string_d (c_value);

      equational   = string_d (option_prefix >> e_name >> '=' >> e_value);

      option_prefix   = lexeme_d[ch_p('-') >> !ch_p('-')];

      e_name       =

        lexeme_d
        [
          (
            +(
              anychar_p - ('=' | space_p)
            )

          )[ OptionNameHandler (self.cl_, CommandLine::Option::EQUATIONAL) ]
        ];

      c_name       =

        lexeme_d
        [
          (
            +(anychar_p - space_p) -

            (
              *(anychar_p - '=') >> '=' >> *anychar_p
            )

          )[ OptionNameHandler (self.cl_, CommandLine::Option::COMPOSITE) ]
        ];

      e_value      = (+anychar_p)[ OptionValueHandler (self.cl_) ];


      c_value      =
        (
          +anychar_p - ( option_prefix >> *anychar_p )

        )[ OptionValueHandler (self.cl_) ];



      // arguments

      arg_list     = *(argument);
      r_arg_list   = !(r_argument >> *argument);

      argument     = string_d ((+anychar_p)[ ArgumentHandler (self.cl_) ]);

      r_argument   = string_d
        (
          (
            +anychar_p - (option_prefix >> *anychar_p)

          )[ ArgumentHandler (self.cl_) ]
        );

    }

    rule<ScannerT> const& start() const
    {
      return command_line;
    }
  };
};


#endif  // COMMAND_LINE_GRAMMAR_H
