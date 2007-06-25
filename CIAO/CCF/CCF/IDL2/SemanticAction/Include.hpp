// file      : CCF/IDL2/SemanticAction/Include.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_INCLUDE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_INCLUDE_HPP

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      struct Include
      {
        virtual ~Include ()
        {
        }
        
        virtual void
        quote (StringLiteralPtr const& sl) = 0;

        virtual void
        bracket (StringLiteralPtr const& sl) = 0;

        virtual void
        end () = 0;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_INCLUDE_HPP
