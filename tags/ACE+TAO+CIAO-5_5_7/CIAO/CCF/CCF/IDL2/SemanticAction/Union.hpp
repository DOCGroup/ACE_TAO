// file      : CCF/IDL2/SemanticAction/Union.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_UNION_HPP
#define CCF_IDL2_SEMANTIC_ACTION_UNION_HPP

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      struct Union : Scope
      {
        virtual ~Union ()
        {
        }
        
        virtual void
        begin_def (SimpleIdentifierPtr const& id) = 0;

        virtual void
        begin_fwd (SimpleIdentifierPtr const& id) = 0;

        virtual void
        type (IdentifierPtr const& id) = 0;

        //@@ Scope declares these functions. Why do I need to
        //   repeat it here?
        //
        virtual void
        open_scope () = 0;

        virtual void
        member_type (IdentifierPtr const& id) = 0;

        virtual void
        member_name (SimpleIdentifierPtr const& id) = 0;

        virtual void
        close_scope () = 0;

        virtual void
        end () = 0;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_UNION_HPP
