// file      : CCF/IDL3/SemanticAction/EventType.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_EVENT_TYPE_HPP
#define CCF_IDL3_SEMANTIC_ACTION_EVENT_TYPE_HPP

#include "CCF/IDL3/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      //
      //
      //
      class EventType : public virtual Scope
      {
      public:
        
        virtual void
        begin (SimpleIdentifierPtr const& id) = 0;
        
        virtual void
        inherits (IdentifierPtr const& id) = 0;
        
        virtual void
        end () = 0;
      };
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_EVENT_TYPE_HPP
