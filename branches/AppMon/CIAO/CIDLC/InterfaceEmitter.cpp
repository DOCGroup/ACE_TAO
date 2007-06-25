// file      : CIDLC/InterfaceEmitter.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "InterfaceEmitter.hpp"

InterfaceEmitter::InterfaceEmitter (Context& c)
  : EmitterBase (c)
{
}

void
InterfaceEmitter::traverse (UnconstrainedInterface& i)
{
  if (add (i))
  {
    Traversal::UnconstrainedInterface::traverse (i);
  }
}

bool
InterfaceEmitter::add (UnconstrainedInterface& i)
{
  return interfaces_.insert (&i).second;
}

