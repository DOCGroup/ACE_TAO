// file      : CCF/IDL2/SemanticAction/Impl/Include.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_INCLUDE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_INCLUDE_HPP


#include "CCF/IDL2/SemanticAction/Include.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Include : SemanticAction::Include
        {
          Include (Context& c);

          virtual void
          quote (StringLiteralPtr const& sl);

          virtual void
          bracket (StringLiteralPtr const& sl);

          virtual void
          end ();

        private:
          Context& ctx;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_INCLUDE_HPP
