// file      : CCF/IDL2/SemanticAction/Module.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_MODULE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_MODULE_HPP

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      struct Module : Scope
      {
        virtual ~Module ()
        {
        }
        
        virtual void
        begin (SimpleIdentifierPtr const& id) = 0;

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

#endif  // CCF_IDL2_SEMANTIC_ACTION_MODULE_HPP
