// $Id$
#ifndef EXECUTOR_MAPPING_GENERATOR_HPP
#define EXECUTOR_MAPPING_GENERATOR_HPP

#include "CCF/CodeGenerationKit/CommandLine.hpp"
#include "CCF/CodeGenerationKit/CommandLineDescriptor.hpp"

#include "CCF/CIDL/CIDL_SyntaxTreeFwd.hpp"

class ExecutorMappingGenerator
{
public:

  void
  options (CL::Description& d);

  void
  generate (CommandLine const& cl,
            CIDL::SyntaxTree::TranslationUnitPtr const&);
};

#endif // EXECUTOR_MAPPING_GENERATOR_HPP
