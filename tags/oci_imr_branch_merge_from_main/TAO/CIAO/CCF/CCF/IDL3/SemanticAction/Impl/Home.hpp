// file      : CCF/IDL3/SemanticAction/Impl/Home.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_HOME_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_HOME_HPP

#include "CCF/IDL3/SemanticGraph/Home.hpp"
#include "CCF/IDL3/SemanticAction/Home.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Home : SemanticAction::Home, ScopeBase<SemanticGraph::Home>
        {
          Home (Context& c);

          virtual void
          begin (SimpleIdentifierPtr const& id);

          virtual void
          inherits (IdentifierPtr const& id);

          virtual void
          supports (IdentifierPtr const& id);

          virtual void
          manages (IdentifierPtr const& id);

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

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_HOME_HPP
