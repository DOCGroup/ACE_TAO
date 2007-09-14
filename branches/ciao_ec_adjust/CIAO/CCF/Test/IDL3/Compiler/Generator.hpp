// file      : Test/IDL3/Compiler/Generator.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef TEST_IDL3_COMPILER_GENERATOR_HPP
#define TEST_IDL3_COMPILER_GENERATOR_HPP

#include <memory>

#include "CCF/IDL3/SemanticGraph.hpp"

namespace IDL3
{
  class GeneratorImpl;

  class Generator
  {
  public:
    ~Generator ();
    Generator ();

    void
    generate (CCF::IDL3::SemanticGraph::TranslationUnit& tu);

  protected:
    Generator (GeneratorImpl&);

  protected:
    std::auto_ptr<GeneratorImpl> pimpl_;
    GeneratorImpl& impl_;
  };
}

#endif  // TEST_IDL3_COMPILER_GENERATOR_HPP
