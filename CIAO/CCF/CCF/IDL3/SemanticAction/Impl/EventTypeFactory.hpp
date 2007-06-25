// file      : CCF/IDL3/SemanticAction/Impl/EventTypeFactory.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_EVENT_TYPE_FACTORY_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_EVENT_TYPE_FACTORY_HPP

#include "CCF/IDL3/SemanticGraph/EventType.hpp"
#include "CCF/IDL3/SemanticAction/EventTypeFactory.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct EventTypeFactory : SemanticAction::EventTypeFactory, Base
        {
          EventTypeFactory (Context& c);

          virtual void
          name (SimpleIdentifierPtr const& id);

          virtual void
          parameter (IdentifierPtr const& type_id,
                     SimpleIdentifierPtr const& name_id);

          virtual void
          raises (IdentifierPtr const& id);

        private:
          SemanticGraph::EventTypeFactory* f_;
        };
      }
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_EVENT_TYPE_FACTORY_HPP
