// file      : CIDLC/InterfaceEmitter.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef INTERFACEEMITTER_HPP
#define INTERFACEEMITTER_HPP

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include "EmitterBase.hpp"

using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

struct InterfaceEmitter : Traversal::UnconstrainedInterface,
                          EmitterBase
{
  InterfaceEmitter (Context& c);

  virtual void traverse (UnconstrainedInterface&);

private:
  bool add (UnconstrainedInterface&);

private:
  std::set<UnconstrainedInterface*> interfaces_;
};

#endif  // INTERFACEEMITTER_HPP
