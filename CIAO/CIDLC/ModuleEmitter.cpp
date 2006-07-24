// file      : CIDLC/ModuleEmitter.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "ModuleEmitter.hpp"

ModuleEmitter::ModuleEmitter (Context& c)
  : EmitterBase (c)
{
}

void
ModuleEmitter::pre (Type& t)
{
  os << "namespace " << t.name () << "{";
}

void
ModuleEmitter::post (Type&)
{
  os << "}";
}
