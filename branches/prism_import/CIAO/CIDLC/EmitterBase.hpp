// file      : CIDLC/EmitterBase.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef EMITTERBASE_HPP
#define EMITTERBASE_HPP

#include "EmitterContext.hpp"

class EmitterBase
{
public:
  EmitterBase (Context&);

protected:
  Context& ctx;
  ostream& os;
};

#endif  // EMITTERBASE_HPP
