// file      : Example/IDL2/CxxMapping/Generator.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef EXAMPLE_IDL2_CXX_MAPPING_GENERATOR_HPP
#define EXAMPLE_IDL2_CXX_MAPPING_GENERATOR_HPP

#include <memory>

#include "CCF/IDL2/SemanticGraph.hpp"

namespace IDL2
{
  class GeneratorImpl;

  class Generator
  {
  public:
    ~Generator ();
    Generator ();

    void
    generate (CCF::IDL2::SemanticGraph::TranslationUnit& tu);

  protected:
    Generator (GeneratorImpl&);

  protected:
    std::auto_ptr<GeneratorImpl> pimpl_;
    GeneratorImpl& impl_;
  };
}

#endif  // EXAMPLE_IDL2_CXX_MAPPING_GENERATOR_HPP
