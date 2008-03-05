// file      : CIDLC/EmitterContext.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "EmitterContext.hpp"

Context::Context (ostream& os,
                  string export_macro,
                  CommandLine const& cl)
  : os_ (os),
    export_macro_ (export_macro),
    cl_ (cl)
{
}

ostream&
Context::os (void)
{
  return os_;
}

string
Context::export_macro (void)
{
  return export_macro_;
}

CommandLine const&
Context::cl (void)
{
  return cl_;
}

string
Context::composition_name (void)
{
  return composition_name_;
}

void
Context::composition_name (const string& name)
{
  composition_name_ = name;
}

