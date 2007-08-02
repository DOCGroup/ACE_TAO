// file      : Test/IDL2/Traversal/Recreate/Generator.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "Generator.hpp"
#include "GeneratorImpl.hpp"

namespace IDL2
{
  Generator::
  ~Generator ()
  {
  }

  Generator::
  Generator ()
      : pimpl_ (new GeneratorImpl), impl_ (*pimpl_)
  {
  }

  Generator::
  Generator (GeneratorImpl& gi)
      : pimpl_ (), impl_ (gi)
  {
  }

  void Generator::
  generate (CCF::IDL2::SemanticGraph::TranslationUnit& tu)
  {
    impl_.generate (tu);
  }
}
