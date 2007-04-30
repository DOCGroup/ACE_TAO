// file      : CCF/IDL2/SemanticAction/Impl/Module.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_MODULE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_MODULE_HPP

#include "CCF/IDL2/SemanticGraph/Module.hpp"
#include "CCF/IDL2/SemanticAction/Module.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Module : SemanticAction::Module,
                        ScopeBase<SemanticGraph::Module>
        {
          Module (Context& c);

          virtual void
          begin (SimpleIdentifierPtr const& id);

          virtual void
          open_scope ();

          virtual void
          close_scope ();

          virtual void
          end ();
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_MODULE_HPP
