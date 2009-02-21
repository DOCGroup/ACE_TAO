// file      : CCF/IDL2/SemanticAction/ValueType.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_VALUE_TYPE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_VALUE_TYPE_HPP

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      struct ValueType : Scope
      {
        virtual ~ValueType ()
        {
        }
        
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

#endif  // CCF_IDL2_SEMANTIC_ACTION_VALUE_TYPE_HPP
