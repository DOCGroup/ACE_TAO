// file      : CCF/IDL3/SemanticAction/Uses.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_USES_HPP
#define CCF_IDL3_SEMANTIC_ACTION_USES_HPP

#include "CCF/IDL3/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      //
      //
      //
      class Uses
      {
      public:
        virtual
        ~Uses () throw () {}
        
        virtual void
        type (IdentifierPtr const& id) = 0;
        
        virtual void
        name (SimpleIdentifierPtr const& id) = 0;
      };
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_USES_HPP
