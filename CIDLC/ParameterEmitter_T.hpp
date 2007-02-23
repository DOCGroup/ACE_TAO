// file      : CIDLC/ParameterEmitter_T.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef PARAMETEREMITTER_T_HPP
#define PARAMETEREMITTER_T_HPP

#include "EmitterBase.hpp"

// For generating parameter names.
template <typename T>
struct ParameterEmitter : T, EmitterBase
{
  ParameterEmitter (Context& c, bool for_exec_src = false)
    : EmitterBase (c),
      for_exec_src_ (for_exec_src)
  {
  }

  virtual void
  name (typename T::Type& p)
  {
    os << " "
       << (for_exec_src_ ? "/* " : "")
       << p.name ()
       << (for_exec_src_ ? " */" : "");
  }

private:
  bool for_exec_src_;
};

#endif  // PARAMETEREMITTER_T_HPP
