// $Id$
#ifndef DESCRIPTOR_GENERATOR_HPP
#define DESCRIPTOR_GENERATOR_HPP

#include "CCF/CodeGenerationKit/CommandLine.hpp"
#include "CCF/CodeGenerationKit/CommandLineDescriptor.hpp"

#include "CCF/CIDL/SyntaxTree.hpp"

class DescriptorGenerator
{
public:
  void
  options (CL::Description& d);

  void
  generate (CommandLine const&,
            CCF::CIDL::SyntaxTree::TranslationUnitPtr const&);

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
