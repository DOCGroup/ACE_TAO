// file      : CCF/IDL2/SemanticAction/Interface.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_INTERFACE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_INTERFACE_HPP

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      //
      //
      //
      class Interface : public virtual Scope
      {
      public:
        virtual
        ~Interface () throw () {}

        virtual void
        begin_abstract (SimpleIdentifierPtr const& id) = 0;

        virtual void
        begin_local (SimpleIdentifierPtr const& id) = 0;

        virtual void
        begin_unconstrained (SimpleIdentifierPtr const& id) = 0;

        virtual void
        inherits (IdentifierPtr const& id) = 0;

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

#endif  // CCF_IDL2_SEMANTIC_ACTION_INTERFACE_HPP
