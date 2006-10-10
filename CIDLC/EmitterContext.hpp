// file      : CIDLC/EmitterContext.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef EMITTERCONTEXT_HPP
#define EMITTERCONTEXT_HPP

#include "CCF/CodeGenerationKit/CommandLine.hpp"

#include <ostream>
#include <string>

using namespace std;

class Context
{
public:
  Context (ostream& os,
           string export_macro,
           CommandLine const& cl);

  ostream& os (void);
  string export_macro (void);
  CommandLine const& cl (void);
  string composition_name (void);
  void composition_name (const string& name);

private:
  ostream& os_;
  string export_macro_;
  CommandLine const& cl_;
  string composition_name_;
};

#endif  // EMITTERCONTEXT_HPP
