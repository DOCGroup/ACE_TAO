/* -*- C++ -*- */

/**
 * $Id$
 */

#ifndef LOAD_LIBRARY_HPP
#define LOAD_LIBRARY_HPP

#include "DLLEntry.hpp"
#include <string>

class OCML_Configurator_Library
{
public:
  OCML_Configurator_Library();
  ~OCML_Configurator_Library();

  char* call_function(std::string& values, const std::string& file_name);

private:
  HMODULE lib_;
  DLLFunctionPtr func_;
};

#endif // LOAD_LIBRARY_HPP
