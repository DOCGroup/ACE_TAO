// file      : CCF/Example/CodeGenerationKit/Indentation/IDL/indent_idl.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CodeGenerationKit/IndentationXML.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

#include <iostream>

using std::cout;
using std::endl;

//FUZZ: disable check_for_improper_main_declaration

int
main ()
{
  Indentation::Implanter<Indentation::XML> guard (cout);

  cout << "<?xml version=\"1.0\"?>" << endl;
  cout << "<!DOCTYPE corbacomponent SYSYEM \"corbacomponent.dtd\">" << endl << endl;

  cout << "<atag>whatever</atag>" << endl;

  cout << "<open>" << endl;
  cout << "<openother>" << endl;
  cout << "<openother \nlabel=\"<<<<<<<<<>><>\">" << endl;
  cout << "text" << endl;
  cout << "<taginitself/>" << endl;
  cout << "</openother>" << endl;
  cout << "</openother>" << endl;
  cout << "</open>" << endl;

}
