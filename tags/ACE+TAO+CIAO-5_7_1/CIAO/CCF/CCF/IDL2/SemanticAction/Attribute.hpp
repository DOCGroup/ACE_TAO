// file      : CCF/IDL2/SemanticAction/Attribute.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_ATTRIBUTE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_ATTRIBUTE_HPP

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      struct Attribute
      {
        virtual ~Attribute ()
        {
        }
        
        virtual void
        begin_ro () = 0;

        virtual void
        begin_rw () = 0;

        virtual void
        type (IdentifierPtr const& id) = 0;

        virtual void
        name (SimpleIdentifierPtr const& id) = 0;

        virtual void
        get_raises (IdentifierPtr const& id) = 0;

        virtual void
        set_raises (IdentifierPtr const& id) = 0;

        virtual void
        end () = 0;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_ATTRIBUTE_HPP
