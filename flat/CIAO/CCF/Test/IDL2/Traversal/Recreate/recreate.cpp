// file      : Test/IDL2/Traversal/Recreate/recreate.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include <iostream>

#include "CCF/IDL2/SemanticGraph.hpp"

#include "../../SemanticGraph/HandBuilt/Builder.hpp"

#include "Generator.hpp"

//FUZZ: disable check_for_improper_main_declaration

int
main ()
{
  Builder b;
  CCF::IDL2::SemanticGraph::TranslationUnit& tu (*b.build ());

  IDL2::Generator g;

  g.generate (tu);

  delete &tu;
}
