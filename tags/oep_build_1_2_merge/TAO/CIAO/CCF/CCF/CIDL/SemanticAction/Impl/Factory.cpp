// file      : CCF/CIDL/SemanticAction/Impl/Factory.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Elements.hpp"
#include "CCF/IDL2/SemanticGraph/Module.hpp"
#include "CCF/IDL2/SemanticGraph/Interface.hpp"

#include "CCF/CIDL/SemanticAction/Impl/Factory.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        using namespace SemanticGraph;

        Factory::
        Factory (CompilerElements::Context& context,
                   Diagnostic::Stream& dout,
                   SemanticGraph::TranslationUnit& tu)
            : IDL2::SemanticAction::Impl::Factory (context, dout, tu),
              IDL3::SemanticAction::Impl::Factory (context, dout, tu),

              composition_ (ctx_),
              home_executor_ (ctx_)
        {
        }
      }
    }
  }
}
