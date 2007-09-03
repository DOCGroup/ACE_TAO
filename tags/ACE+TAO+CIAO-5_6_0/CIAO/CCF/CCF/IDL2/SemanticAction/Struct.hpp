// file      : CCF/IDL2/SemanticAction/Struct.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_STRUCT_HPP
#define CCF_IDL2_SEMANTIC_ACTION_STRUCT_HPP

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      struct Struct : Scope
      {
        virtual ~Struct ()
        {
        }
        
        virtual void
        begin_def (SimpleIdentifierPtr const& id) = 0;

        virtual void
        begin_fwd (SimpleIdentifierPtr const& id) = 0;

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

#endif  // CCF_IDL2_SEMANTIC_ACTION_STRUCT_HPP
