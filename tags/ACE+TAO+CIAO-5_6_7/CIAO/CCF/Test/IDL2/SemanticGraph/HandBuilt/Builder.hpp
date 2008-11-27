// file      : Test/IDL2/SemanticGraph/HandBuilt/Builder.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef TEST_IDL2_SEMANTIC_GRAPH_BUILDER_HPP
#define TEST_IDL2_SEMANTIC_GRAPH_BUILDER_HPP

#include "CCF/IDL2/SemanticGraph.hpp"

class Builder
{
public:
  CCF::IDL2::SemanticGraph::TranslationUnit*
  build ();
};

#endif  // TEST_IDL2_SEMANTIC_GRAPH_BUILDER_HPP
