// file      : Test/CIDL/Compiler/Generator.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef TEST_CIDL_COMPILER_GENERATOR_HPP
#define TEST_CIDL_COMPILER_GENERATOR_HPP

#include <memory>

#include "CCF/CIDL/SemanticGraph.hpp"

namespace CIDL
{
  class GeneratorImpl;

  class Generator
  {
  public:
    ~Generator ();
    Generator ();

    void
    generate (CCF::CIDL::SemanticGraph::TranslationUnit& tu);

  protected:
    Generator (GeneratorImpl&);

  protected:
    std::auto_ptr<GeneratorImpl> pimpl_;
    GeneratorImpl& impl_;
  };
}

#endif  // TEST_CIDL_COMPILER_GENERATOR_HPP
