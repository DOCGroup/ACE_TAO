// file      : CIDLC/OperationHeaderEmitters.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "OperationHeaderEmitters.hpp"
#include "Literals.hpp"

using namespace StringLiterals;

OperationEmitter::OperationEmitter (Context& c)
  : EmitterBase (c)
{
}

void
OperationEmitter::pre (Type&)
{
  os << "virtual ";
}

void
OperationEmitter::name (Type& o)
{
  os << endl << o.name ();
}

void
OperationEmitter::receives_pre (Type&)
{
  os << " (" << endl;
}

void
OperationEmitter::receives_none (Type&)
{
  os << " ();" << endl;
}

void
OperationEmitter::receives_post (Type&)
{
  os << ");" << endl;
}

void
OperationEmitter::raises (Type&)
{
}

void
OperationEmitter::comma (Type&)
{
  os << "," << endl;
}

// ==================================================

HomeOperationEmitter::HomeOperationEmitter (Context& c)
  : OperationEmitter (c)
{
}

void
HomeOperationEmitter::traverse (SemanticGraph::HomeFactory&)
{
}

void
HomeOperationEmitter::traverse (SemanticGraph::HomeFinder&)
{
}

// ==================================================

FactoryOperationEmitter::FactoryOperationEmitter (Context& c, bool for_exec)
  : EmitterBase (c),
    returns_emitter_ (c),
    for_exec_ (for_exec)
{
  returns_.node_traverser (returns_emitter_);
}

void
FactoryOperationEmitter::pre (SemanticGraph::HomeFactory&)
{
  os << "virtual ";
}

void
FactoryOperationEmitter::returns (SemanticGraph::HomeFactory& t)
{
  if (for_exec_)
    {
      os << STRS[COMP_EC] << "_ptr";
    }
  else
    {
      Traversal::HomeFactory::returns (t, returns_);
    }

  os << endl;
}

void
FactoryOperationEmitter::name (SemanticGraph::HomeFactory& t)
{
  os << t.name ();
}

void
FactoryOperationEmitter::receives_none (SemanticGraph::HomeFactory&)
{
  os << " ()" << endl;
}

void
FactoryOperationEmitter::receives_pre (SemanticGraph::HomeFactory&)
{
  os << " (" << endl;
}

void
FactoryOperationEmitter::receives_post (SemanticGraph::HomeFactory&)
{
  os << endl << ")" << endl;
}

void
FactoryOperationEmitter::raises (SemanticGraph::HomeFactory&)
{
}

void
FactoryOperationEmitter::post (SemanticGraph::HomeFactory&)
{
  os << ";" << endl;
}

void
FactoryOperationEmitter::comma (SemanticGraph::HomeFactory&)
{
  os << "," << endl;
}

// ==================================================

FinderOperationEmitter::FinderOperationEmitter (Context& c, bool for_exec)
  : EmitterBase (c),
    returns_emitter_ (c),
    for_exec_ (for_exec)
{
  returns_.node_traverser (returns_emitter_);
}

void
FinderOperationEmitter::pre (SemanticGraph::HomeFinder&)
{
  os << "virtual ";
}

void
FinderOperationEmitter::returns (SemanticGraph::HomeFinder& t)
{
  if (for_exec_)
    {
      os << STRS[COMP_EC] << "_ptr";
    }
  else
    {
      Traversal::HomeFinder::returns (t, returns_);
    }

  os << endl;
}

void
FinderOperationEmitter::name (SemanticGraph::HomeFinder& t)
{
  os << t.name ();
}

void
FinderOperationEmitter::receives_none (SemanticGraph::HomeFinder&)
{
  os << " ()" << endl;
}

void
FinderOperationEmitter::receives_pre (SemanticGraph::HomeFinder&)
{
  os << " (" << endl;
}

void
FinderOperationEmitter::receives_post (SemanticGraph::HomeFinder&)
{
  os << endl << ")" << endl;
}

void
FinderOperationEmitter::raises (SemanticGraph::HomeFinder&)
{
}

void
FinderOperationEmitter::post (SemanticGraph::HomeFinder&)
{
  os << ";" << endl;
}

void
FinderOperationEmitter::comma (SemanticGraph::HomeFinder&)
{
  os << "," << endl;
}

