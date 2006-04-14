// file      : CCF/IDL2/SemanticAction/Impl/Native.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_NATIVE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_NATIVE_HPP

#include "CCF/IDL2/SemanticAction/Native.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Native : SemanticAction::Native, Base
        {
          Native (Context& c);

          virtual void
          name (SimpleIdentifierPtr const& id);

          virtual void
          end ();
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_NATIVE_HPP
