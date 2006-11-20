// file      : Test/IDL2/Traversal/Recreate/recreate.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include <iostream>

#include "CCF/IDL2/SemanticGraph.hpp"

#include "../../SemanticGraph/HandBuilt/Builder.hpp"

#include "Generator.hpp"

int
main ()
{
  Builder b;
  CCF::IDL2::SemanticGraph::TranslationUnit& tu (*b.build ());

  IDL2::Generator g;

  g.generate (tu);

  delete &tu;
}
