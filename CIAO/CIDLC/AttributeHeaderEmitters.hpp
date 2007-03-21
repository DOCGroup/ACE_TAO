// file      : CIDLC/AttributeHeaderEmitters.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef ATTRIBUTEHEADEREMITTERS_HPP
#define ATTRIBUTEHEADEREMITTERS_HPP

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include "CorbaTypeNameEmitters.hpp"
#include "UtilityTypeNameEmitters.hpp"
#include "EmitterBase.hpp"

using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

struct WriteAttributeHeaderEmitter : Traversal::ReadWriteAttribute,
                                     EmitterBase
{
  WriteAttributeHeaderEmitter (Context& c);

  virtual void
  pre (SemanticGraph::ReadWriteAttribute&);

  virtual void
  name (SemanticGraph::ReadWriteAttribute&);

  virtual void
  get_raises (SemanticGraph::ReadWriteAttribute&);

  virtual void
  set_raises (SemanticGraph::ReadWriteAttribute&);

  virtual void
  post (SemanticGraph::ReadWriteAttribute&);

  virtual void
  comma (SemanticGraph::ReadWriteAttribute&);

  private:
    INArgTypeNameEmitter write_type_name_emitter_;
    FullTypeNameEmitter type_name_emitter_;
    Traversal::Belongs write_belongs_;
    Traversal::SetRaises set_raises_;
};

struct AttributeHeaderEmitter : Traversal::ReadAttribute,
                                Traversal::ReadWriteAttribute,
                                EmitterBase
{
  AttributeHeaderEmitter (Context&);

  // ReadWriteAttribute
  //
  virtual void
  pre (SemanticGraph::ReadWriteAttribute&);

  virtual void
  name (SemanticGraph::ReadWriteAttribute&);

  virtual void
  get_raises_none (SemanticGraph::ReadWriteAttribute&);

  virtual void
  get_raises_pre (SemanticGraph::ReadWriteAttribute&);

  virtual void
  get_raises_post (SemanticGraph::ReadWriteAttribute&);

  virtual void
  set_raises (SemanticGraph::ReadWriteAttribute&);

  virtual void
  post (SemanticGraph::ReadWriteAttribute&);

  virtual void
  comma (SemanticGraph::ReadWriteAttribute&);

  // ReadAttribute
  //
  virtual void
  pre (SemanticGraph::ReadAttribute&);

  virtual void
  name (SemanticGraph::ReadAttribute&);

  virtual void
  get_raises_none (SemanticGraph::ReadAttribute&);

  virtual void
  get_raises_pre (SemanticGraph::ReadAttribute&);

  virtual void
  get_raises_post (SemanticGraph::ReadAttribute&);

  virtual void
  set_raises (SemanticGraph::ReadAttribute&);

  virtual void
  post (SemanticGraph::ReadAttribute&);

  virtual void
  comma (SemanticGraph::ReadAttribute&);

private:
  ReturnTypeNameEmitter read_type_name_emitter_;
  FullTypeNameEmitter type_name_emitter_;
  Traversal::Belongs read_belongs_;
  Traversal::GetRaises get_raises_;
};

#endif  // ATTRIBUTEHEADEREMITTERS_HPP
