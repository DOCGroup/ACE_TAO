// file      : CCF/IDL3/SemanticAction/Impl/Provides.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_PROVIDES_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_PROVIDES_HPP

#include "CCF/IDL2/SemanticGraph/Interface.hpp"
#include "CCF/IDL3/SemanticAction/Provides.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Provides : SemanticAction::Provides, Base
        {
          Provides (Context& c);

          virtual void
          type (IdentifierPtr const& id);

          virtual void
          name (SimpleIdentifierPtr const& id);

        private:
          SemanticGraph::Interface* type_;
        };
      }
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_PROVIDES_HPP
