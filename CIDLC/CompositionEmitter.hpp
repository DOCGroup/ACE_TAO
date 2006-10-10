// file      : CIDLC/CompositionEmitter.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef COMPOSITIONEMITTER_HPP
#define COMPOSITIONEMITTER_HPP

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include "EmitterBase.hpp"

using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

struct CompositionEmitter : Traversal::Composition,
                            EmitterBase
{
  CompositionEmitter (Context& c);

  virtual void pre (Type&);

  virtual void post (Type&);
};

#endif  // COMPOSITIONEMITTER_HPP
