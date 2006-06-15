// file      : Test/IDL2/SemanticGraph/HandBuilt/hand_built.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "Builder.hpp"

using namespace CCF::IDL2;

int
main ()
{
  Builder b;
  SemanticGraph::TranslationUnit& tu (*b.build ());

  delete &tu;
}
