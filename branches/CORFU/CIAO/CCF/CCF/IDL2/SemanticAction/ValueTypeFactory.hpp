// file      : CCF/IDL2/SemanticAction/ValueTypeFactory.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_VALUE_TYPE_FACTORY_HPP
#define CCF_IDL2_SEMANTIC_ACTION_VALUE_TYPE_FACTORY_HPP

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      struct ValueTypeFactory
      {
        virtual ~ValueTypeFactory ()
        {
        }
        
        virtual void
        name (SimpleIdentifierPtr const& id) = 0;

        virtual void
        parameter (IdentifierPtr const& type,
                   SimpleIdentifierPtr const& name) = 0;

        virtual void
        raises (IdentifierPtr const& id) = 0;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_VALUE_TYPE_FACTORY_HPP
