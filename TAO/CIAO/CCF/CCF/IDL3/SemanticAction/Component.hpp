// file      : CCF/IDL3/SemanticAction/Component.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_COMPONENT_HPP
#define CCF_IDL3_SEMANTIC_ACTION_COMPONENT_HPP

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
      class Component : public virtual Scope
      {
      public:

        virtual void
        begin (SimpleIdentifierPtr const& id) = 0;

        virtual void
        inherits (IdentifierPtr const& id) = 0;

        virtual void
        supports (IdentifierPtr const& id) = 0;

        virtual void
        end () = 0;
      };
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_COMPONENT_HPP
