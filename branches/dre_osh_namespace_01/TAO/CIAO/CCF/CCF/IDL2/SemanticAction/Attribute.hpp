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
      //
      //
      //
      class Attribute
      {
      public:
        virtual
        ~Attribute () throw () {}

        virtual void
        type (IdentifierPtr const& id) = 0;

        virtual void
        name (SimpleIdentifierPtr const& id) = 0;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_ATTRIBUTE_HPP
