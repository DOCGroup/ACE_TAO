// file      : CCF/IDL2/SemanticAction/TypeId.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_TYPE_ID_HPP
#define CCF_IDL2_SEMANTIC_ACTION_TYPE_ID_HPP

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      struct TypeId
      {
        virtual ~TypeId ()
        {
        }
        
        virtual void
        begin (IdentifierPtr const& name, StringLiteralPtr const& id) = 0;

        virtual void
        end () = 0;
      };


      struct TypePrefix
      {
        virtual ~TypePrefix ()
        {
        }
        
        virtual void
        begin (IdentifierPtr const& name, StringLiteralPtr const& prefix) = 0;

        virtual void
        end () = 0;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_TYPE_ID_HPP
