// File   : CommandLineParser.cpp
// Author : Boris Kolpackov <boris@dre.vanderbilt.edu>
// $Id$

#include "CCF/CodeGenerationKit/CommandLineParser.hpp"
#include "CCF/CodeGenerationKit/CommandLineGrammar.hpp"

#include <iostream>

using std::cerr;
using std::endl;

bool parse (int argc, char* argv[], CommandLine& cl) throw ()
{
  typedef vector<string> Argv;

  Argv v;

  for (int i = 0; i < argc; i++)
  {
    v.push_back (argv[i]);
  }

  Argv::iterator first = v.begin ();
  Argv::iterator last  = v.end ();

  scanner<Argv::iterator, scanner_policies <> > scan(first, last);

  CLineGrammar g (cl);

  match<nil_t> hit = g.parse(scan);

  bool result = hit.length() == v.size ();

  // some semantic analisys
  if (!cl.separator)
  {
    CommandLine::Options::reverse_iterator r = cl.options.rbegin ();

    if (r != cl.options.rend () &&
        r->value_ != "" &&
        r->type_ == CommandLine::Option::COMPOSITE)
    {

      cerr << "command line: assuming <" << r->value_
           << "> to be a value of option <" << r->name_
           << "> and not the first argument" << endl;

      cerr  << "command line: write ... --" << r->name_ << " -- "
            << r->value_ << " ... to indicate otherwise" << endl;
    }
  }

  return result;
}
