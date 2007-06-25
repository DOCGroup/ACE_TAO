// file      : CCF/IDL2/SemanticAction/Native.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_NATIVE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_NATIVE_HPP

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      struct Native
      {
        virtual ~Native ()
        {
        }
        
        virtual void
        name (SimpleIdentifierPtr const& id) = 0;

        virtual void
        end () = 0;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_NATIVE_HPP
