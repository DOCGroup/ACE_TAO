// file      : CCF/IDL2/SemanticAction/Impl/Exception.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_EXCEPTION_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_EXCEPTION_HPP

#include "CCF/IDL2/SemanticGraph/Exception.hpp"
#include "CCF/IDL2/SemanticAction/Exception.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        class Exception : public virtual SemanticAction::Exception,
                          public virtual ScopeBase<SemanticGraph::Exception>
        {
        public:
          virtual
          ~Exception () throw ();
          
          Exception (Context& c);

          virtual void
          begin (SimpleIdentifierPtr const& id);

          virtual void
          open_scope ();

          virtual void
          close_scope ();

          virtual void
          end ();
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_EXCEPTION_HPP
