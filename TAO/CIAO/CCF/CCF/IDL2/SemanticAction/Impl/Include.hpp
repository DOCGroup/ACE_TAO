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
        //
        //
        //
        class Include : public virtual SemanticAction::Include
        {
        public:
          virtual
          ~Include () throw ();

          Include (Context& c);

          virtual void
          begin (StringLiteralPtr const& sl);

          virtual void
          end ();

        private:
          Context& ctx;
        };

        //
        //
        //
        class SystemInclude : public virtual SemanticAction::SystemInclude
        {
        public:
          virtual
          ~SystemInclude () throw ();

          SystemInclude (Context& c);

          virtual void
          begin (StringLiteralPtr const& sl);

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
