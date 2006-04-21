// file      : CIDLC/SizeTypeCalculator.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CIDLC_SIZE_TYPE_CALCULATOR_HPP
#define CIDLC_SIZE_TYPE_CALCULATOR_HPP


//@@ Don't really need this. just forward declaration of
//   TranslationUnit would be enough.
//
#include "CCF/CIDL/SemanticGraph.hpp"

class SizeTypeCalculator
{
public:
  void
  calculate (CCF::CIDL::SemanticGraph::TranslationUnit&);
};

#endif  // CIDLC_SIZE_TYPE_CALCULATOR_HPP
