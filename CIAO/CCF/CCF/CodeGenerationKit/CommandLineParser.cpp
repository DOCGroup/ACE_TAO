// file      : CCF/CodeGenerationKit/CommandLineParser.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CodeGenerationKit/CommandLineParser.hpp"

#include <string>
#include <iostream>

using std::cerr;
using std::endl;

bool
parse (int argc, char* argv[], CL::Description const& cld, CommandLine& cl)
{
  cl.command = argv[0];

  bool seen_double_dash (false);

  for (int i (1); i < argc; ++i)
  {
    std::string arg (argv[i]);

    if (seen_double_dash)
    {
      cl.arguments.push_back (arg);
      continue;
    }

    if (arg == "--")
    {
      seen_double_dash = true;
      continue;
    }

    if (arg.length () > 0 && arg[0] == '-')
    {
      if (arg.length () > 1 && arg[1] == '-')
      {
        // Double-dash (long) option.
        //

        std::string op (arg, 2);

        if (CL::OptionDescription const* d = cld.find_option (op))
        {
          if (d->type () == CL::OptionType::value)
          {
            if (++i >= argc)
            {
              cerr << argv[0] << ": argument expected for option '--"
                   << op << "'" << endl;
              return false;
            }

            // cerr << "--" << op << ": " << argv[i] << endl;

            cl.options.push_back (CommandLine::Option (op, argv[i]));
          }
          else
            cl.options.push_back (CommandLine::Option (op));

          continue;
        }
      }
      else
      {
        // Single-dash (short) option. We support two formats: '-D foo' and
        // -Dfoo.
        //
        std::string op (arg, 1, 1);

        if (CL::OptionDescription const* d = cld.find_option (op))
        {
          if (d->type () == CL::OptionType::value)
          {
            std::string value;

            if (arg.length () > 2)
            {
              value.assign (arg, 2, arg.size () - 2);
            }
            else
            {
              if (++i >= argc)
              {
                cerr << argv[0] << ": argument expected for option '-"
                     << op << "'" << endl;
                return false;
              }

              value = argv[i];
            }

            // cerr << "-" << op << ": " << value << endl;

            cl.options.push_back (CommandLine::Option (op, value));
          }
          else
          {
            if (arg.length () > 2)
            {
              cerr << argv[0] << ": argument not expected for option '-"
                   << op << "' in '" << arg << "'" << endl;
              return false;
            }

            cl.options.push_back (CommandLine::Option (op));
          }

          continue;
        }
      }
    }

    cl.arguments.push_back (arg);
  }

  return true;
}
