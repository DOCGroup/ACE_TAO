// file      : CCF/Example/CIDL/LocalExecutorMapping/ExecutorMappingGenerator.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef EXECUTOR_MAPPING_GENERATOR_HPP
#define EXECUTOR_MAPPING_GENERATOR_HPP

#include "CCF/CompilerElements/FileSystem.hpp"

#include "CCF/CodeGenerationKit/CommandLine.hpp"
#include "CCF/CodeGenerationKit/CommandLineDescriptor.hpp"

//@@ SemanticGraphFwd could be useful here.
//
#include "CCF/CIDL/SemanticGraph.hpp"

class ExecutorMappingGenerator
{
public:

  //@@ should be static?
  void
  options (CL::Description& d);

  //@@ maybe I should introduce constant and non-constant
  //   traversal.
  //
  void
  generate (CommandLine const& cl,
            CCF::CIDL::SemanticGraph::TranslationUnit&,
            fs::path const& file);
};

#endif // EXECUTOR_MAPPING_GENERATOR_HPP
