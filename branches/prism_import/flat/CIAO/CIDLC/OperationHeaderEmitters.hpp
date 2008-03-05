// file      : CIDLC/OperationHeaderEmitters.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef OPERATIONHEADEREMITTERS_HPP
#define OPERATIONHEADEREMITTERS_HPP

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include "EmitterBase.hpp"
#include "CorbaTypeNameEmitters.hpp"

using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

struct OperationEmitter : Traversal::Operation,
                          EmitterBase
{
  OperationEmitter (Context& c);

  virtual void pre (Type&);

  virtual void name (Type&);

  virtual void receives_pre (Type&);

  virtual void receives_none (Type&);

  virtual void receives_post (Type&);
  
  virtual void raises (Type&);
  
  virtual void comma (Type&);
};

// HomeFactory and HomeFinder are tied to Operation in
// the front end. Since we want to treat them differently
// than regular operations in a home (we want to generate
// base class factory operations returning the base component,
// for example), we use this class for regular home operations
// that overrides HomeFactory and HomeFinder traversals
// to do nothing.
struct HomeOperationEmitter : OperationEmitter,
                              Traversal::HomeFactory,
                              Traversal::HomeFinder
{
  HomeOperationEmitter (Context& c);

  virtual void
  traverse (SemanticGraph::HomeFactory&);

  virtual void
  traverse (SemanticGraph::HomeFinder&);
};

struct FactoryOperationEmitter : Traversal::HomeFactory,
                                 EmitterBase
{
  FactoryOperationEmitter (Context& c, bool for_exec = false);

  virtual void
  pre (SemanticGraph::HomeFactory&);

  virtual void
  returns (SemanticGraph::HomeFactory&);

  virtual void
  name (SemanticGraph::HomeFactory&);

  virtual void
  receives_none (SemanticGraph::HomeFactory&);

  virtual void
  receives_pre (SemanticGraph::HomeFactory&);

  virtual void
  receives_post (SemanticGraph::HomeFactory&);

  virtual void
  raises (SemanticGraph::HomeFactory&);

  virtual void
  post (SemanticGraph::HomeFactory&);

  virtual void
  comma (SemanticGraph::HomeFactory&);

private:
  ReturnTypeNameEmitter returns_emitter_;
  Traversal::Returns returns_;
  bool for_exec_;
};

struct FinderOperationEmitter : Traversal::HomeFinder,
                                EmitterBase
{
  FinderOperationEmitter (Context& c, bool for_exec = false);

  virtual void pre (SemanticGraph::HomeFinder&);

  virtual void returns (SemanticGraph::HomeFinder&);

  virtual void name (SemanticGraph::HomeFinder&);

  virtual void receives_none (SemanticGraph::HomeFinder&);

  virtual void receives_pre (SemanticGraph::HomeFinder&);

  virtual void receives_post (SemanticGraph::HomeFinder&);

  virtual void raises (SemanticGraph::HomeFinder&);

  virtual void post (SemanticGraph::HomeFinder&);

  virtual void comma (SemanticGraph::HomeFinder&);

private:
  ReturnTypeNameEmitter returns_emitter_;
  Traversal::Returns returns_;
  bool for_exec_;
};

#endif  // OPERATIONHEADEREMITTERS_HPP
