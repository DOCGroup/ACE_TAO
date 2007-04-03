// file      : CIDLC/EmitterBase.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "EmitterBase.hpp"

EmitterBase::EmitterBase (Context& c)
  : ctx (c),
    os (ctx.os ())
{
}
