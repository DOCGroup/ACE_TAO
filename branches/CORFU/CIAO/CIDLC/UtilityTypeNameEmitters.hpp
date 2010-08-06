// file      : CIDLC/UtilityTypeNameEmitters.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef UTILITYTYPENAMEEMITTERS_HPP
#define UTILITYTYPENAMEEMITTERS_HPP

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"
#include "EmitterBase.hpp"

using namespace std;
using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

// Emits type's full scoped name.
struct FullTypeNameEmitter : Traversal::Type,
                             EmitterBase
{
  FullTypeNameEmitter (Context&);

  virtual void
  traverse (SemanticGraph::Type&);
};

// Emits type's local name.
struct SimpleTypeNameEmitter : Traversal::Type,
                               EmitterBase
{
  SimpleTypeNameEmitter (Context&);

  virtual void
  traverse (SemanticGraph::Type&);
};

// Emits name of type's enclosing scope.
struct EnclosingTypeNameEmitter : Traversal::Type,
                                  EmitterBase
{
  EnclosingTypeNameEmitter (Context&);

  virtual void
  traverse (SemanticGraph::Type&);
};

// Emits scoped name minus the leading double colon.
struct StrippedTypeNameEmitter : Traversal::Type,
                                 EmitterBase
{
  StrippedTypeNameEmitter (Context&);

  virtual void
  traverse (SemanticGraph::Type&);
};

// Emits scoped name in flat form for use with a prefix.
struct FacetEnclosingTypeNameEmitter : Traversal::Type,
                                       EmitterBase
{
  FacetEnclosingTypeNameEmitter (Context&);

  virtual void
  traverse (SemanticGraph::Type&);
};

#endif  // UTILITYTYPENAMEEMITTERS_HPP  
