// file      : CIDLC/CompositionEmitter.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "CompositionEmitter.hpp"
#include "Literals.hpp"

using namespace StringLiterals;

CompositionEmitter::CompositionEmitter (Context& c)
  : EmitterBase (c)
{
}

void
CompositionEmitter::pre (Type& t)
{
  ctx.composition_name (t.name ().unescaped_str ());

  os << STRS[CIDL_NS] << t.name () << "{";
}

void
CompositionEmitter::post (Type&)
{
  os << "}";
}
