// file      : CCF/IDL2/SemanticAction/Member.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_MEMBER_HPP
#define CCF_IDL2_SEMANTIC_ACTION_MEMBER_HPP

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      class Member
      {
      public:
        virtual
        ~Member () throw () {}

        virtual void
        begin (IdentifierPtr const& id) = 0;

        virtual void
        declarator (SimpleIdentifierPtr const& id) = 0;

        virtual void
        end () = 0;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_MEMBER_HPP
