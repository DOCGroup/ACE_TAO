// file      : Test/IDL2/Traversal/Recreate/Generator.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef TEST_IDL2_TRAVERSAL_RECREATE_GENERATOR_HPP
#define TEST_IDL2_TRAVERSAL_RECREATE_GENERATOR_HPP

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

#endif  // TEST_IDL2_TRAVERSAL_RECREATE_GENERATOR_HPP
