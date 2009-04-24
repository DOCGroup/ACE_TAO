// file      : CIDLC/ModuleEmitter.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef MODULEEMITTER_HPP
#define MODULEEMITTER_HPP

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include "EmitterBase.hpp"

using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

struct ModuleEmitter : Traversal::Module, EmitterBase
{
  ModuleEmitter (Context& c);

  virtual void pre (Type&);
  virtual void post (Type&);
};

#endif  // MODULEEMITTER_HPP
