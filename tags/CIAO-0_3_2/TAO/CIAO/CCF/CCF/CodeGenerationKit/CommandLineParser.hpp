// File   : CommandLineParser.hpp
// Author : Boris Kolpackov <boris@dre.vanderbilt.edu>
// $Id$

#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

#include "MSVC_Pragmas.hpp"

#include "CCF/CodeGenerationKit/CommandLine.hpp"

bool parse (int argc, char* argv[], CommandLine& cl) throw ();

#endif  // COMMAND_LINE_PARSER_H
