// file      : CCF/Example/CodeGenerationKit/Indentation/IDL/indent_idl.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CodeGenerationKit/IndentationIDL.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

#include <iostream>

using std::cout;
using std::endl;

//FUZZ: disable check_for_improper_main_declaration

int
main ()
{
  Indentation::Implanter<Indentation::IDL> guard (cout);

  cout << "const string s = \"Hello \\\"world;\";";
  cout << "const char c1 = \'\\\'\';";
  cout << "const char c2 = \';\';";

  cout << "interface I {"
       << "void foo ();"
       << "};";
}
