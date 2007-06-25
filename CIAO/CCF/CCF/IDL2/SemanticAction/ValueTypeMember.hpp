// file      : CCF/IDL2/SemanticAction/ValueTypeMember.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_VALUE_TYPE_MEMBER_HPP
#define CCF_IDL2_SEMANTIC_ACTION_VALUE_TYPE_MEMBER_HPP

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      struct ValueTypeMember
      {
        virtual ~ValueTypeMember ()
        {
        }
        
        virtual void
        begin_private () = 0;

        virtual void
        begin_public () = 0;

        virtual void
        type (IdentifierPtr const& id) = 0;

        virtual void
        name (SimpleIdentifierPtr const& id) = 0;

        virtual void
        end () = 0;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_MEMBER_HPP
