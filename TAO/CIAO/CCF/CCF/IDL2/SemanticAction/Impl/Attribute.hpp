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
        class Attribute : public virtual SemanticAction::Attribute,
                          public Base
        {
        public:
          // @@ remove all these d-tors.
          //
          virtual
          ~Attribute () throw ();

          Attribute (Context& c);

          virtual void
          type (IdentifierPtr const& id);

          virtual void
          name (SimpleIdentifierPtr const& id);

        private:
          SemanticGraph::Type* type_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_ATTRIBUTE_HPP
