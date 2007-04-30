// file      : CIDLC/UtilityTypeNameEmitters.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "UtilityTypeNameEmitters.hpp"
#include "CCF/CodeGenerationKit/Regex.hpp"

#include <sstream>

FullTypeNameEmitter::FullTypeNameEmitter (Context& c)
  : EmitterBase (c)
{
}

void
FullTypeNameEmitter::traverse (SemanticGraph::Type& t)
{
  os << t.scoped_name ();
}

// ===================================================

SimpleTypeNameEmitter::SimpleTypeNameEmitter (Context& c)
  : EmitterBase (c)
{
}

void
SimpleTypeNameEmitter::traverse (SemanticGraph::Type& t)
{
  os << t.name ();
}

// ===================================================

EnclosingTypeNameEmitter::EnclosingTypeNameEmitter (Context& c)
  : EmitterBase (c)
{
}

void
EnclosingTypeNameEmitter::traverse (SemanticGraph::Type& t)
{
  os << t.scoped_name ().scope_name ();
}

// ===================================================

StrippedTypeNameEmitter::StrippedTypeNameEmitter (Context& c)
  : EmitterBase (c)
{
}

void
StrippedTypeNameEmitter::traverse (SemanticGraph::Type& t)
{
  ScopedName scoped (t.scoped_name ());
  os << Name (scoped.begin () + 1, scoped.end ());
}

// ===================================================

FacetEnclosingTypeNameEmitter::FacetEnclosingTypeNameEmitter (Context& c)
  : EmitterBase (c)
{
}

void
FacetEnclosingTypeNameEmitter::traverse (SemanticGraph::Type& t)
{
  // We need to escape C++ keywords before flattening the name.
  //
  std::ostringstream ostr;
  ostr.pword (name_printer_index) = os.pword (name_printer_index);
  ostr << t.scoped_name ().scope_name ();
  os << regex::perl_s (ostr.str (), "/::/_/");
}

