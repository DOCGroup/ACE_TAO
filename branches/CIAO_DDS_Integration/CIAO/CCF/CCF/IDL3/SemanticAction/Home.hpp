// file      : CCF/IDL3/SemanticAction/Home.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_HOME_HPP
#define CCF_IDL3_SEMANTIC_ACTION_HOME_HPP

#include "CCF/IDL3/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      struct Home : Scope
      {
        virtual void
        begin (SimpleIdentifierPtr const& id) = 0;

        virtual void
        inherits (IdentifierPtr const& id) = 0;

        virtual void
        supports (IdentifierPtr const& id) = 0;

        virtual void
        manages (IdentifierPtr const& id) = 0;

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

#endif  // CCF_IDL3_SEMANTIC_ACTION_HOME_HPP
