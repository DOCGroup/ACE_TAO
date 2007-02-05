// file      : CIDLC/DescriptorGenerator.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef DESCRIPTOR_GENERATOR_HPP
#define DESCRIPTOR_GENERATOR_HPP

#include "CCF/CodeGenerationKit/CommandLine.hpp"
#include "CCF/CodeGenerationKit/CommandLineDescriptor.hpp"

#include "CCF/CIDL/SemanticGraph.hpp"

class DescriptorGenerator
{
public:
  static void
  options (CL::Description& d);

  void
  generate (CommandLine const&,
            CCF::CIDL::SemanticGraph::TranslationUnit&);

private:
  fs::ofstream ofs_;
};


#endif // DESCRIPTOR_GENERATOR_HPP

/*
 * Local Variables:
 * mode: C++
 * c-basic-offset: 2
 * End:
 */
