// file      : CCF/CodeGenerationKit/CommandLineParser.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

#include "CCF/CodeGenerationKit/CommandLine.hpp"

bool parse (int argc, char* argv[], CommandLine& cl) throw ();

#endif  // COMMAND_LINE_PARSER_H
