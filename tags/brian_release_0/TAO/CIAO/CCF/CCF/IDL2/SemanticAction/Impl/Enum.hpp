// file      : CCF/IDL2/SemanticAction/Impl/Enum.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_ENUM_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_ENUM_HPP

#include "CCF/IDL2/SemanticGraph/Enum.hpp"
#include "CCF/IDL2/SemanticAction/Enum.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Enum : SemanticAction::Enum, Base
        {
          Enum (Context& c);

          virtual void
          begin (SimpleIdentifierPtr const& id);

          virtual void
          enumerator (SimpleIdentifierPtr const& id);

          virtual void
          end ();

        private:
          SemanticGraph::Enum* type_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_ENUM_HPP
