// file      : CCF/IDL2/SemanticAction/Enum.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_ENUM_HPP
#define CCF_IDL2_SEMANTIC_ACTION_ENUM_HPP

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      struct Enum
      {
        virtual ~Enum ()
        {
        }
        
        virtual void
        begin (SimpleIdentifierPtr const& id) = 0;

        virtual void
        enumerator (SimpleIdentifierPtr const& id) = 0;

        virtual void
        end () = 0;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_ENUM_HPP
