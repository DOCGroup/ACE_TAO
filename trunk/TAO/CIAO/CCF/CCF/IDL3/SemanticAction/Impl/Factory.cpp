// file      : CCF/IDL3/SemanticAction/Impl/Factory.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SemanticAction/Impl/Factory.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {

        Factory::
        ~Factory () throw () {}

        Factory::
        Factory (CompilerElements::Context& context,
                 Diagnostic::Stream& dout,
                 SemanticGraph::TranslationUnit& tu)
            : IDL2::SemanticAction::Impl::Factory (context, dout, tu),

              component_ (ctx_),
              consumes_ (ctx_),
              emits_ (ctx_),
              event_type_ (ctx_),
              home_ (ctx_),
              home_factory_ (ctx_),
              include_ (ctx_, context, dout, *this),
              provides_ (ctx_),
              publishes_ (ctx_),
              system_include_ (ctx_, context, dout, *this),
              uses_ (ctx_)
        {
        }
      }
    }
  }
}
