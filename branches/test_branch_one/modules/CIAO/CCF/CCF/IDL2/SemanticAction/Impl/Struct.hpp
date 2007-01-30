// file      : CCF/IDL2/SemanticAction/Impl/Struct.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_STRUCT_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_STRUCT_HPP

#include "CCF/IDL2/SemanticGraph/Struct.hpp"
#include "CCF/IDL2/SemanticAction/Struct.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Struct : SemanticAction::Struct,
                        ScopeBase<SemanticGraph::Struct>
        {
          Struct (Context& c);

          virtual void
          begin_def (SimpleIdentifierPtr const& id);

          virtual void
          begin_fwd (SimpleIdentifierPtr const& id);

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

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_STRUCT_HPP
