// file      : CCF/IDL2/SemanticAction/Impl/Attribute.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_ATTRIBUTE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_ATTRIBUTE_HPP

#include "CCF/IDL2/SemanticGraph/Attribute.hpp"
#include "CCF/IDL2/SemanticAction/Attribute.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Attribute : SemanticAction::Attribute, Base
        {
          Attribute (Context& c);

          virtual void
          begin_ro ();

          virtual void
          begin_rw ();

          virtual void
          type (IdentifierPtr const& id);

          virtual void
          name (SimpleIdentifierPtr const& id);

          virtual void
          get_raises (IdentifierPtr const& id);

          virtual void
          set_raises (IdentifierPtr const& id);

          virtual void
          end ();

        private:
          bool readonly_;
          SemanticGraph::Type* type_;
          SemanticGraph::Attribute* a_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_ATTRIBUTE_HPP
