// file      : CCF/Example/CIDL/LocalExecutorMapping/ExecutorMappingGenerator.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef EXECUTOR_MAPPING_GENERATOR_HPP
#define EXECUTOR_MAPPING_GENERATOR_HPP

#include "CCF/CodeGenerationKit/CommandLine.hpp"
#include "CCF/CodeGenerationKit/CommandLineDescriptor.hpp"

//@@ Don't really nee this. just forward declaration of TranslationUnit would
//   be enough.
#include "CCF/CIDL/SyntaxTree.hpp"

class ExecutorMappingGenerator
{
public:

  void
  options (CL::Description& d);

  void
  generate (CommandLine const& cl,
            CCF::CIDL::SyntaxTree::TranslationUnitPtr const&);
};

#endif // EXECUTOR_MAPPING_GENERATOR_HPP
