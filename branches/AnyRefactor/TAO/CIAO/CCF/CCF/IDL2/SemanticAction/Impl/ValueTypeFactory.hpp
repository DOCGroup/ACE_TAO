// file      : CCF/IDL2/SemanticAction/Impl/ValueTypeFactory.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_VALUE_TYPE_FACTORY_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_VALUE_TYPE_FACTORY_HPP

#include "CCF/IDL2/SemanticGraph/ValueType.hpp"
#include "CCF/IDL2/SemanticAction/ValueTypeFactory.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct ValueTypeFactory : SemanticAction::ValueTypeFactory, Base
        {
          ValueTypeFactory (Context& c);

          virtual void
          name (SimpleIdentifierPtr const& id);

          virtual void
          parameter (IdentifierPtr const& type_id,
                     SimpleIdentifierPtr const& name_id);

          virtual void
          raises (IdentifierPtr const& id);

        private:
          SemanticGraph::ValueTypeFactory* f_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_VALUE_TYPE_FACTORY_HPP
