// file      : CCF/IDL3/SemanticAction/EventType.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_EVENT_TYPE_HPP
#define CCF_IDL3_SEMANTIC_ACTION_EVENT_TYPE_HPP

#include "CCF/IDL3/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      struct EventType : Scope
      {
        virtual void
        begin_abstract_def (SimpleIdentifierPtr const& id) = 0;

        virtual void
        begin_abstract_fwd (SimpleIdentifierPtr const& id) = 0;

        virtual void
        begin_concrete_def (SimpleIdentifierPtr const& id) = 0;

        virtual void
        begin_concrete_fwd (SimpleIdentifierPtr const& id) = 0;

        virtual void
        inherits (IdentifierPtr const& id) = 0;

        virtual void
        supports (IdentifierPtr const& id) = 0;

        virtual void
        open_scope () = 0;

        virtual void
        close_scope () = 0;

        virtual void
        end () = 0;
      };
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_EVENT_TYPE_HPP
