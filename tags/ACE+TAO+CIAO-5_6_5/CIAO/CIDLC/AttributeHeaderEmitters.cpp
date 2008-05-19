// file      : CIDLC/AttributeHeaderEmitters.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "AttributeHeaderEmitters.hpp"
#include "Literals.hpp"

using namespace StringLiterals;

WriteAttributeHeaderEmitter::WriteAttributeHeaderEmitter (Context& c)
  : EmitterBase (c),
    write_type_name_emitter_ (c),
    type_name_emitter_ (c)
{
  write_belongs_.node_traverser (write_type_name_emitter_);

  edge_traverser (set_raises_);
  set_raises_.node_traverser (type_name_emitter_);
}

void
WriteAttributeHeaderEmitter::pre (SemanticGraph::ReadWriteAttribute&)
{
  os << "virtual void" << endl;
}

void
WriteAttributeHeaderEmitter::name (SemanticGraph::ReadWriteAttribute& a)
{
  os << a.name () << " (";

  Traversal::ReadWriteAttribute::belongs (a, write_belongs_);

  os << " " << a.name () << ")";
}

void
WriteAttributeHeaderEmitter::get_raises (SemanticGraph::ReadWriteAttribute&)
{
}

void
WriteAttributeHeaderEmitter::set_raises (SemanticGraph::ReadWriteAttribute&)
{
}

void
WriteAttributeHeaderEmitter::post (SemanticGraph::ReadWriteAttribute&)
{
  os << ";" << endl;
}

void
WriteAttributeHeaderEmitter::comma (SemanticGraph::ReadWriteAttribute&)
{
  os << "," << endl;
}

// ============================================================

AttributeHeaderEmitter::AttributeHeaderEmitter (Context& c)
  : EmitterBase (c),
    read_type_name_emitter_ (c),
    type_name_emitter_ (c)
{
  read_belongs_.node_traverser (read_type_name_emitter_);

  edge_traverser (get_raises_);
  get_raises_.node_traverser (type_name_emitter_);
}

// ReadWriteAttribute
//
void
AttributeHeaderEmitter::pre (SemanticGraph::ReadWriteAttribute& a)
{
  os << "virtual ";

  Traversal::ReadWriteAttribute::belongs (a, read_belongs_);

  os << endl;
}

void
AttributeHeaderEmitter::name (SemanticGraph::ReadWriteAttribute& a)
{
  os << a.name () << " ()";
}

void
AttributeHeaderEmitter::get_raises_none (SemanticGraph::ReadWriteAttribute&)
{
}

void
AttributeHeaderEmitter::get_raises_pre (SemanticGraph::ReadWriteAttribute&)
{
}

void
AttributeHeaderEmitter::get_raises_post (SemanticGraph::ReadWriteAttribute&)
{
}

void
AttributeHeaderEmitter::set_raises (SemanticGraph::ReadWriteAttribute&)
{
}

void
AttributeHeaderEmitter::post (SemanticGraph::ReadWriteAttribute& a)
{
  os << ";" << endl;

  WriteAttributeHeaderEmitter write_attribute_emitter (ctx);
  write_attribute_emitter.traverse (a);
}

void
AttributeHeaderEmitter::comma (SemanticGraph::ReadWriteAttribute&)
{
  os << "," << endl;
}

// ReadAttribute
//
void
AttributeHeaderEmitter::pre (SemanticGraph::ReadAttribute& a)
{
  os << "virtual ";

  Traversal::ReadAttribute::belongs (a, read_belongs_);

  os << endl;
}

void
AttributeHeaderEmitter::name (SemanticGraph::ReadAttribute& a)
{
  os << a.name () << " ()" << endl;
}

void
AttributeHeaderEmitter::get_raises_none (SemanticGraph::ReadAttribute&)
{
}

void
AttributeHeaderEmitter::get_raises_pre (SemanticGraph::ReadAttribute&)
{
}

void
AttributeHeaderEmitter::get_raises_post (SemanticGraph::ReadAttribute&)
{
}

void
AttributeHeaderEmitter::set_raises (SemanticGraph::ReadAttribute&)
{
}

void
AttributeHeaderEmitter::post (SemanticGraph::ReadAttribute&)
{
  os << ";" << endl;
}

void
AttributeHeaderEmitter::comma (SemanticGraph::ReadAttribute&)
{
  os << "," << endl;
}

